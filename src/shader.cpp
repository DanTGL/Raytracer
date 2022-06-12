#include "shader.h"

#include <iostream>

GLuint CompileShader(GLuint type, const std::string& shaderCode, int* success) {
    GLuint shader = glCreateShader(type);
    const char* code = shaderCode.c_str();
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, success);

    return shader;
}

GLuint CreateShaderProgram(const std::string& vertex, const std::string& fragment) {
    int success;
    char infoLog[512];

    GLuint vertShader = CompileShader(GL_VERTEX_SHADER, vertex, &success);

    if (!success) {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragShader = CompileShader(GL_FRAGMENT_SHADER, fragment, &success);

    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create and link shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    // Check for linking errors and validate program
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shader
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

GLuint CreateComputeProgram(const std::string& shaderCode) {
    int success;
    char infoLog[512];

    GLuint shader = CompileShader(GL_COMPUTE_SHADER, shaderCode, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create and link shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);

    // Check for linking errors and validate program
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shader
    glDeleteShader(shader);

    return program;
}

void DebugCallbackARB(GLenum source,
    GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* msg, const GLvoid* userParam) {
    const char* _source;
    const char* _type;
    const char* _severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    // ignore notification severity (you can add your own ignores)
    // + Adds __debugbreak if _DEBUG is defined (automatic in visual studio)
    // note: __debugbreak is specific for MSVC, won't work with gcc/clang
    // -> in that case remove it and manually set breakpoints
    if (_severity != "NOTIFICATION") {
        printf("OpenGL error [%d]: %s of %s severity, raised from %s: %s\n",
            id, _type, _severity, _source, msg);
#ifdef _DEBUG
        __debugbreak();
#endif
    }
}

void DebugCallbackAMD(GLuint id,
    GLenum category,
    GLenum severity,
    GLsizei length,
    const GLchar* msg,
    GLvoid* userParam) {
    const char* _source;
    const char* _type;
    const char* _severity;

    std::cout << msg << std::endl;
}