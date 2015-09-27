//
// Created by alex on 19.09.15.
//

#ifndef ZEPTOBIRD_SHADER_H
#define ZEPTOBIRD_SHADER_H

#include <string>
#include <map>
#include <GLES2/gl2.h>

namespace ZB {

    class Shader {
    public:
        Shader();

        Shader(const Shader&) = delete;
        Shader &operator=(const Shader&) = delete;

        ~Shader();

        bool setSource(const std::string& vertex_src, const std::string& fragment_src);

        GLint attrib(const std::string& attrib_name);

        GLint uniform(const std::string& uniform_name);

        void use();

    private:
        GLuint program, vertex_index, fragment_index;
        std::map<std::string, GLint> cache;
    };

} // namespace ZB

#endif //ZEPTOBIRD_SHADER_H
