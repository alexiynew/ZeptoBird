//
// Created by alex on 19.09.15.
//

#ifndef ZEPTOBIRD_GAME_H
#define ZEPTOBIRD_GAME_H

#include "World.h"
#include "Utils.h"

#include <GLES2/gl2.h>

namespace ZB {

    class Game {
    public:
        static Game &instance();

        Game(const Game &) = delete;

        Game &operator=(const Game &) = delete;

        ~Game();

        void setViewport(int width, int height);

        void touch(float x, float y);

        void update();

        void render();

        void gameOver();

        void increaseScore();

        const Utils::Matrix& projMatrix();

        float getTopOffset();

    private:

        enum class State { INIT, WAIT, PLAYING, OVER };

        Game();

        void init();

        bool initialized;

        World world;

        Utils::Matrix proj_matrix;
        float top_offset;


        float real_width;
        float real_height;

        double last_frame_time;

        int score;

        State state;
    };

} // namespace ZB

#endif //ZEPTOBIRD_GAME_H
