//
// Created by alex on 19.09.15.
//

#include <GLES2/gl2.h>

#include "Shader.h"
#include "Utils.h"

#define  LOG_TAG "Shader"

//--------------------------------------------------------------
// helper functions
//--------------------------------------------------------------
namespace {
    inline std::string shaderTypeToString(GLenum type)
    {
        switch (type) {
            case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
            case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
        }
        return "UNKNOWN_SHADER";
    }

    GLuint createProgram(GLuint vert, GLuint frag)
    {
        GLuint program = glCreateProgram();
        if (program) {
            glAttachShader(program, vert);
            Utils::checkGlError("glAttachShader");
            glAttachShader(program, frag);
            Utils::checkGlError("glAttachShader");
            glLinkProgram(program);

            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE) {
                GLint log_len = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
                if (log_len) {
                    std::unique_ptr<char[]> log(new char[log_len]);
                    glGetProgramInfoLog(program, log_len, nullptr, log.get());
                    LOGE("Could not link program:\n%s\n", log.get());
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }

    GLuint createShader(const std::string& src, GLenum shaderType)
    {
        GLuint shader = glCreateShader(shaderType);
        if (shader) {
            char const *c_str = src.c_str();
            glShaderSource(shader, 1, &c_str, nullptr);
            glCompileShader(shader);

            GLint compiled = GL_FALSE;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled != GL_TRUE) {
                int log_len = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
                std::unique_ptr<char[]> log(new char[log_len]);
                glGetShaderInfoLog(shader, log_len, nullptr, log.get());
                std::string type_str = shaderTypeToString(shaderType);
                LOGE("Could not compile shader %s:\n%s\n", type_str.c_str(), log.get());
            }
        }
        return shader;
    }
}

//--------------------------------------------------------------
// Shader class
//--------------------------------------------------------------
namespace ZB {

    Shader::Shader() : program(0), vertex_index(0), fragment_index(0), cache()
    {}

    Shader::~Shader()
    {
        glDeleteProgram(program);
    }

    bool Shader::setSource(const std::string& vertex_src, const std::string& fragment_src)
    {
        vertex_index = createShader(vertex_src, GL_VERTEX_SHADER);
        fragment_index = createShader(fragment_src, GL_FRAGMENT_SHADER);
        program = createProgram(vertex_index, fragment_index);

        cache.clear();
        glDeleteShader(vertex_index);
        glDeleteShader(fragment_index);

        return program != 0;
    }

    GLint Shader::attrib(const std::string& attrib)
    {
        if (cache.count(attrib)) {
            return cache[attrib];
        }

        GLint attr_index = glGetAttribLocation(program, attrib.c_str());
        Utils::checkGlError("glGetAttribLocation");
        cache[attrib] = attr_index;
        return attr_index;
    }

    GLint Shader::uniform(const std::string& uniform)
    {
        if (cache.count(uniform)) {
            return cache[uniform];
        }

        GLint unif_index = glGetUniformLocation(program, uniform.c_str());
        Utils::checkGlError("glGetUniformLocation");
        cache[uniform] = unif_index;
        return unif_index;
    }

    void Shader::use()
    {
        glUseProgram(program);
    }

} // namespace ZB

