//
// Created by alex on 19.09.15.
//

#ifndef ZEPTOBIRD_WORLD_H
#define ZEPTOBIRD_WORLD_H

#include <memory>
#include <list>

#include "Shader.h"
#include "Bird.h"

namespace ZB {

    class World{
    public:
        World();
        ~World();

        void reset();

        void setShader(std::shared_ptr<Shader>);

        void render();

        void update(double time_diff);

        float width();

        float height();

        void kickTheBird();


    private:

        Bird bird;

        std::shared_ptr<Shader> shader;

        std::vector<Vec2f> background;
        std::vector<Vec2f> ceil;
        std::vector<Vec2f> floor;

        std::list<Vec2f> pipes;
        std::list<Vec2f> point_lines;

        std::vector<Vec2f> pipe_points;

        void createPipesPair();

        /// true if bird hit the ground
        bool borderCollide();

        /// true if bird hit pipe
        bool pipeCollide();
    };

} // namespace ZB

#endif //ZEPTOBIRD_WORLD_H
