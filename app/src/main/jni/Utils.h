//
// Created by alex on 19.09.15.
//

#ifndef ZEPTOBIRD_UTILS_H
#define ZEPTOBIRD_UTILS_H

#include <android/log.h>
#include <GLES2/gl2.h>

#define  LOG_TAG "Utils"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace Utils {

    const long double TAU = 6.283185307179586476925286766559005768394338798750211641949889;

    inline void printGLString(const char *name, GLenum s) {
        const char *v = (const char *) glGetString(s);
        LOGI("GL %s = %s\n", name, v);
    }

    inline void checkGlError(const char *op) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    struct Matrix {
        float data[16];
    };

    inline void ortho(float left, float right, float bottom, float top, float zNear, float zFar,
                      Matrix &matrix) {
        for (int i = 0; i < 16; ++i)
            matrix.data[i] = 0;

        matrix.data[0] = 2.0f / (right - left);
        matrix.data[5] = 2.0f / (top - bottom);
        matrix.data[10] = -2.0f / (zFar - zNear);
        matrix.data[12] = -(right + left) / (right - left);
        matrix.data[13] = -(top + bottom) / (top - bottom);
        matrix.data[14] = -(zFar + zNear) / (zFar - zNear);
        matrix.data[15] = 1;
    }

    inline void translate(Matrix &matrix, float x, float y) {
        matrix.data[12] += matrix.data[0] * x + matrix.data[4] * y;
        matrix.data[13] += matrix.data[1] * x + matrix.data[5] * y;
    }

    struct color {
        class _color_value {
        public:
            constexpr _color_value() : value(0)
            {}

            explicit _color_value(float v) : value(clip_value(v))
            {}

            float operator= (float v)
            {
                return (value = clip_value(v));
            }
            bool operator== (const _color_value &rhs)
            {
                return value == rhs.value;
            }
            operator float() const { return value; }
        private:
            float value;
            static constexpr float clip_value (float const v)
            {
                return (v > 1.0f ? 1.0f : (v < 0 ? 0.0f : v));
            }
        };

        color() : r(), g(), b(), a()
        {}

        color(int rgb)
                : r(clip_byte((rgb >> 16) & 0xFF)), g(clip_byte((rgb >> 8) & 0xFF)), b(clip_byte(rgb & 0xFF)), a(1.0f)
        {}

        color(int cr, int cg, int cb, int ca = 255)
                : r(clip_byte(cr)), g(clip_byte(cg)), b(clip_byte(cb)), a(clip_byte(ca))
        {}

        color(float fr, float fg, float fb, float fa = 1.0f)
                : r(fr), g(fg), b(fb), a(fa)
        {}

        _color_value r, g, b, a;

        _color_value const & operator[] (int i) const
        {
            return *(&r + i);
        }

        _color_value & operator[] (int i)
        {
            return *(&r + i);
        }

    private:
        static constexpr float clip_byte (int const b)
        {
            return (b > 255 ? 255 : (b < 0 ? 0 : b)) / 255.0f;
        }
    };


} // namespace Utils

#endif //ZEPTOBIRD_UTILS_H
