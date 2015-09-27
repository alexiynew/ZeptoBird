//
// Created by alex on 19.09.15.
//

#include "World.h"
#include "Utils.h"
#include "Game.h"
#include <cstdlib>
#include <ctime>

#define  LOG_TAG "World"

namespace ZB {

    const float world_width = 20.0f; // the size of the world measured in parrots
    const float world_height = 24.0f;

    const float world_speed = 6.0f;

    const float pipe_width = 2.5f;
    const float pipe_height = world_height;
    const int   pipes_steps = 10;  // the number of positions for the holes in the pipe

    const Utils::color pipe_color(0x00BF32);
    const Utils::color back_color(0x04819E);
    const Utils::color ground_color(0xBF7730);

    World::World()
        : bird(), background(), ceil(), floor(),  pipes(), point_lines(), pipe_points()
    {
        background.push_back(Vec2f(0.0f, 0.0f));
        background.push_back(Vec2f(world_width, 0.0f));
        background.push_back(Vec2f(world_width, world_height));
        background.push_back(Vec2f(0.0f, world_height));

        pipe_points.push_back(Vec2f(0.0f, 0.0f));
        pipe_points.push_back(Vec2f(pipe_width, 0.0f));
        pipe_points.push_back(Vec2f(pipe_width, world_height));
        pipe_points.push_back(Vec2f(0.0f, world_height));
    }

    World::~World() { }

    void World::reset() {
        bird.setPosition(Vec2f(world_width / 5.0f, world_height / 2.0f));
        bird.reset();

        pipes.clear();
        point_lines.clear();

        createPipesPair();

        ceil.clear();
        floor.clear();

        float top_offset = Game::instance().getTopOffset();

        // setup ground vertices
        ceil.push_back(Vec2f(0.0f, -top_offset));
        ceil.push_back(Vec2f(world_width, -top_offset));
        ceil.push_back(Vec2f(world_width, 0));
        ceil.push_back(Vec2f(0.0f, 0));

        floor.push_back(Vec2f(0.0f, world_height));
        floor.push_back(Vec2f(world_width, world_height));
        floor.push_back(Vec2f(world_width, world_height + top_offset));
        floor.push_back(Vec2f(0.0f, world_height + top_offset));

        LOGI("reset");
    }

    float World::width() {
        return world_width;
    }

    float World::height() {
        return world_height;
    }

    void World::setShader(std::shared_ptr<Shader> shdr) {
        shader = shdr;
        bird.setShader(shader);
    }

    void World::render() {
        shader->use();

        // get shader attributes and uniform indexes
        GLint gvPositionHandle = shader->attrib("vPosition");
        GLint pMatrixHandle = shader->uniform("pMatrix");
        GLint offsetHandle = shader->uniform("vOffset");
        GLint colorHandle = shader->uniform("vColor");

        // world position
        glUniform2f(offsetHandle, 0, 0);
        glUniformMatrix4fv(pMatrixHandle, 1, GL_FALSE, Game::instance().projMatrix().data);

        // draw background
        glUniform4f(colorHandle, back_color.r, back_color.g, back_color.b, 1.0f);
        glVertexAttribPointer((GLuint) gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, &background[0]);
        glEnableVertexAttribArray((GLuint) gvPositionHandle);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)background.size());

        Utils::checkGlError("draw background");

        bird.render();

        // setup pipes params
        glUniform4f(colorHandle, pipe_color.r, pipe_color.g, pipe_color.b, 1.0f);
        glVertexAttribPointer((GLuint) gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, &pipe_points[0]);
        glEnableVertexAttribArray((GLuint) gvPositionHandle);
        Utils::checkGlError("setup pipes");

        // draw pipes
        for (const Vec2f& pos : pipes) {
            glUniform2f(offsetHandle, pos.x, pos.y);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)pipe_points.size());
        }
        Utils::checkGlError("draw pipes");

        glUniform2f(offsetHandle, 0, 0);
        Utils::checkGlError("set ground pos");

        // draw ground
        glUniform4f(colorHandle, ground_color.r, ground_color.g, ground_color.b, 1.0f);

        glVertexAttribPointer((GLuint) gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, &ceil[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)ceil.size());

        glVertexAttribPointer((GLuint) gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, &floor[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)floor.size());

        Utils::checkGlError("draw ground");
    }

    void World::update(double time_diff) {
        // update all
        bird.update(time_diff);

        for (Vec2f& pos : pipes) {
            pos.x -= world_speed * time_diff;
        }

        for (Vec2f& pos : point_lines) {
            pos.x -= world_speed * time_diff;
        }

        if (pipes.back().x < world_width / 2.0f) {
            createPipesPair();
        }

        // delete redundant pipes
        while (pipes.front().x < -pipe_width) {
            pipes.pop_front();
        }

        // check collisions
        if (borderCollide() || pipeCollide()) {
            Game::instance().gameOver();
        }

        // count score
        while (point_lines.front().x < bird.getPosition().x) {
            point_lines.pop_front();
            Game::instance().increaseScore();
        }
    }

    /// true if bird hit pipe
    bool World::pipeCollide() {
        Vec2f pos = bird.getPosition();
        float r = bird.radius();

        auto squareDist = [](float p, float min, float max){
            float out = 0;

            if (p < min) {
                double val = min - p;
                out += val * val;
            }

            if (p > max) {
                double val = p - max;
                out += val * val;
            }

            return out;
        };

        for (const auto& pipe : pipes) {
            float dist = squareDist(pos.x, pipe.x, pipe.x + pipe_width) +
                         squareDist(pos.y, pipe.y, pipe.y + pipe_height);
            if (dist <= r * r) {
                // fix bird position
                Vec2f inv_velocity = normalize(bird.getVelocity() * -1.0f);
                bird.setPosition(bird.getPosition() + inv_velocity * (float)fabs(dist - r*r));
                return true;
            }
        }

        return false;
    }

    /// true if bird hit the ground
    bool World::borderCollide() {
        Vec2f pos = bird.getPosition();
        float r = bird.radius();

        if (pos.y + r > height()) {
            bird.setPosition(Vec2f(pos.x, height() - r));
            return true;
        }
        if (pos.y - r < 0) {
            bird.setPosition(Vec2f(pos.x, r));
            return true;
        }

        return false;
    }

    void World::kickTheBird() {
        bird.kick();
    }

    void World::createPipesPair() {
        int hole = 3 + rand() % 3;
        int pos = rand() % 8;
        if (hole + pos > pipes_steps) {
            pos = pipes_steps - hole;
        }

        float part = (pipe_height - 2.0f) / float(pipes_steps);

        pipes.push_back(Vec2f(world_width, -pipe_height + pos * part + 1.0f));
        pipes.push_back(Vec2f(world_width, (pos + hole) * part - 1.0f));

        point_lines.push_back(Vec2f(world_width + (pipe_width / 2.0f), 0));
    }

} // namespace ZB

