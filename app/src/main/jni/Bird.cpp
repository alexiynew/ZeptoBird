//
// Created by alex on 19.09.15.
//

#include "Utils.h"
#include "Bird.h"
#include "Game.h"

#define  LOG_TAG "Bird"

namespace ZB {

    const float bird_radius = 0.5; // bird size is 1
    const int circle_segments = 20;

    const float gravity = 9.8;
    const float max_speed = 12.0f;

    const float kick_force = -6.0f;

    const Utils::color bird_color(0xFF8373);

    Bird::Bird()
        : points(), position(0,0), velocity(0,0), acceleration(0, gravity)
    {
        // make circle
        for(int i = 0; i < circle_segments; ++i) {
            float theta = float(Utils::TAU) * float(i) / float(circle_segments);
            float x = bird_radius * cosf(theta);
            float y = bird_radius * sinf(theta);
            points.push_back(Vec2f(x, y));
        }
    }

    Bird::~Bird() { }

    void Bird::setPosition(const Vec2f& pos) {
        position = pos;
    }

    float Bird::radius() {
        return bird_radius;
    }

    void Bird::kick() {
        if (velocity.y > 0) {
            acceleration.y = kick_force;
            velocity.y = kick_force;
        } else {
            // let fly higher
            velocity.y += kick_force;
        }
    }

    void Bird::update(double time_diff) {
        // apply acceleration
        velocity += acceleration * (float)time_diff;

        acceleration.y += gravity;
        if (acceleration.y > gravity) {
            acceleration.y = gravity;
        }

        if (velocity.y < -max_speed) {
            velocity.y = -max_speed;
        } else if (velocity.y > max_speed) {
            velocity.y = max_speed;
        }

        // new position
        position += velocity * (float)time_diff;
    }

    void Bird::reset() {
        velocity = Vec2f(0,0);
        acceleration = Vec2f(0, gravity);
    }

    Vec2f Bird::getPosition() {
        return position;
    }

    Vec2f Bird::getVelocity() {
        return velocity;
    }

    void Bird::setShader(std::shared_ptr<Shader> shdr) {
        shader = shdr;
    }

    void Bird::render() {
        shader->use();

        // get shader attributes and uniform indexes
        GLint gvPositionHandle = shader->attrib("vPosition");
        GLint pMatrixHandle = shader->uniform("pMatrix");
        GLint offsetHandle = shader->uniform("vOffset");
        GLint colorHandle = shader->uniform("vColor");

        // bird position
        glUniform2f(offsetHandle, position.x, position.y);
        glUniformMatrix4fv(pMatrixHandle, 1, GL_FALSE, Game::instance().projMatrix().data);

        // draw bird
        glUniform4f(colorHandle, bird_color.r, bird_color.g, bird_color.b, 1.0f);
        glVertexAttribPointer((GLuint) gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, &points[0]);
        glEnableVertexAttribArray((GLuint) gvPositionHandle);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circle_segments);
        Utils::checkGlError("draw bird");
    }



} // namespace ZB
