//
// Created by alex on 19.09.15.
//

#include <string>
#include <ctime>

#include "Game.h"

#define  LOG_TAG "Game"

namespace {

    std::string gVertexShader =
        "precision mediump float;\n"
        "attribute vec4 vPosition;\n"
        "uniform mat4 pMatrix;\n"
        "uniform vec2 vOffset;\n"
        "uniform vec4 vColor;\n"
        "varying vec4 fragColor;\n"
        "void main() {\n"
        "  vec4 point = vPosition;\n"
        "  point.xy += vOffset;\n"
        "  gl_Position = pMatrix * point;\n"
        "  fragColor = vColor;\n"
        "}\n";

    std::string gFragmentShader =
        "precision mediump float;\n"
        "varying vec4 fragColor;\n"
        "void main() {\n"
        "  gl_FragColor = fragColor;\n"
        "}\n";

    double getTimeNsec() {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return double(now.tv_sec) + double(now.tv_nsec) / 1000000000LL;
    }

} // namespace

namespace ZB {

    Game::Game()
        : initialized(false), world(), proj_matrix(), top_offset(), real_width(0), real_height(0),
          last_frame_time(0), score(0), state(State::INIT)
    { }

    Game::~Game() { }

    Game &Game::instance() {
        static Game g;
        if (!g.initialized) {
            g.init();
        }
        return g;
    }

    void Game::setViewport(int width, int height) {
        if (!initialized) return;

        LOGI("setViewport(%d, %d)", width, height);
        glViewport(0, 0, width, height);
        Utils::checkGlError("glViewport");

        real_width = width;
        real_height = height;

        float proportional_height = world.width() / (real_width / real_height);
        top_offset =  (proportional_height - world.height()) / 2.0f;

        Utils::ortho(0, world.width(), proportional_height, 0, -1, 1, proj_matrix);

        Utils::translate(proj_matrix, 0, top_offset);

        world.reset();
    }

    void Game::render() {
        if (!initialized) return;

        static float grey = 0.8f;
        glClearColor(grey, grey, grey, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        world.render();
    }

    void Game::update() {
        if (!initialized) return;

        double now = getTimeNsec();

        if (state == State::PLAYING) {
            world.update(now - last_frame_time);
        }

        last_frame_time = now;
    }

    void Game::touch(float x, float y) {
        if (!initialized) return;

        if (state == State::WAIT) {
            state = State::PLAYING;
            LOGI("State::PLAYING");
        } else if (state == State::PLAYING) {
            world.kickTheBird();
        } else if (state == State::OVER) {
            world.reset();
            score = 0;
            state = State::WAIT;
            LOGI("State::WAIT");
        }

        LOGI("Touch in (%f, %f)", x, y);
    }

    void Game::gameOver() {
        state = State::OVER;
        LOGI("State::OVER");
    }

    void Game::increaseScore() {
        score++;
        LOGI("Score %d", score);
    }

    const Utils::Matrix& Game::projMatrix() {
        return proj_matrix;
    }

    float Game::getTopOffset() {
        return top_offset;
    }


    void Game::init() {
        Utils::printGLString("Version", GL_VERSION);
        Utils::printGLString("Vendor", GL_VENDOR);
        Utils::printGLString("Renderer", GL_RENDERER);
        Utils::printGLString("Extensions", GL_EXTENSIONS);

        std::shared_ptr<Shader> shader(new Shader());

        if (!shader->setSource(gVertexShader, gFragmentShader)) {
            LOGE("Can't init shader");
            return;
        }

        world.setShader(shader);

        last_frame_time = getTimeNsec();

        LOGI("Game Initiated");
        initialized = true;
        state = State::WAIT;
        LOGI("State::WAIT");
        score = 0;
    }

} // namespace ZB