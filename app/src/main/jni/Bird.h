//
// Created by alex on 19.09.15.
//

#ifndef ZEPTOBIRD_BIRD_H
#define ZEPTOBIRD_BIRD_H

#include <vector>

#include <GLES2/gl2.h>

#include "Vec2.h"
#include "Shader.h"

namespace ZB {

    class Bird{
    public:
        Bird();
        ~Bird();

        void render();
        void setShader(std::shared_ptr<Shader>);

        Vec2f getPosition();
        Vec2f getVelocity();
        float radius();

        void kick();

        void update(double time_diff);

        void setPosition(const Vec2f& pos);

        void reset();

    private:
        std::vector<Vec2f> points;
        Vec2f position;

        Vec2f velocity;
        Vec2f acceleration;

        std::shared_ptr<Shader> shader;
    };

} // namespace ZB


#endif //ZEPTOBIRD_BIRD_H
