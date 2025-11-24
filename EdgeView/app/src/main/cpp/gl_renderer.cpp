#include "gl_renderer.h"
#include <android/log.h>

#define LOG_TAG "GLRenderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

const char* GLRenderer::vertexShaderSource = R"(
    attribute vec4 aPosition;
    attribute vec2 aTexCoord;
    varying vec2 vTexCoord;
    
    void main() {
        gl_Position = aPosition;
        vTexCoord = aTexCoord;
    }
)";

const char* GLRenderer::fragmentShaderSource = R"(
    precision mediump float;
    uniform sampler2D uTexture;
    varying vec2 vTexCoord;
    
    void main() {
        gl_FragColor = texture2D(uTexture, vTexCoord);
    }
)";

GLRenderer::GLRenderer() : shaderProgram(0), textureId(0) {
    LOGI("GL Renderer created");
}

GLRenderer::~GLRenderer() {
    cleanup();
    LOGI("GL Renderer destroyed");
}

bool GLRenderer::initialize() {
    // Create and compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    if (vertexShader == 0) {
        LOGE("Failed to compile vertex shader");
        return false;
    }
    
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (fragmentShader == 0) {
        LOGE("Failed to compile fragment shader");
        glDeleteShader(vertexShader);
        return false;
    }
    
    // Create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check linking status
    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        LOGE("Shader program linking failed: %s", infoLog);
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        return false;
    }
    
    // Clean up individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Get attribute and uniform locations
    positionLocation = glGetAttribLocation(shaderProgram, "aPosition");
    texCoordLocation = glGetAttribLocation(shaderProgram, "aTexCoord");
    textureLocation = glGetUniformLocation(shaderProgram, "uTexture");
    
    // Generate texture
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    LOGI("GL Renderer initialized successfully");
    return true;
}

void GLRenderer::render(const uint8_t* data, int width, int height) {
    if (!data || shaderProgram == 0) {
        return;
    }
    
    // Update texture with new data
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    // Use shader program
    glUseProgram(shaderProgram);
    
    // Set up vertex data
    const float vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,  // Bottom left
         1.0f, -1.0f, 1.0f, 1.0f,  // Bottom right
        -1.0f,  1.0f, 0.0f, 0.0f,  // Top left
         1.0f,  1.0f, 1.0f, 0.0f   // Top right
    };
    
    // Enable vertex attributes
    glEnableVertexAttribArray(positionLocation);
    glEnableVertexAttribArray(texCoordLocation);
    
    // Set vertex attribute pointers
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices);
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices + 2);
    
    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureLocation, 0);
    
    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Disable vertex attributes
    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(texCoordLocation);
}

GLuint GLRenderer::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        LOGE("Shader compilation failed: %s", infoLog);
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

void GLRenderer::cleanup() {
    if (textureId != 0) {
        glDeleteTextures(1, &textureId);
        textureId = 0;
    }
    
    if (shaderProgram != 0) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
}
