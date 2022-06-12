#pragma once

#include <GL/glew.h>
#include <string>

GLuint CompileShader(GLuint type, const std::string& shader, int* success);

GLuint CreateComputeProgram(const std::string& shader);
GLuint CreateShaderProgram(const std::string& vertex, const std::string& fragment);

void DebugCallbackARB(GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const GLvoid* userParam);

void DebugCallbackAMD(GLuint id,
    GLenum category,
    GLenum severity,
    GLsizei length,
    const GLchar* msg,
    GLvoid* userParam);