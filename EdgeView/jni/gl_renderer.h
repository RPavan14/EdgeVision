#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <GLES2/gl2.h>
#include <cstdint>

class GLRenderer {
public:
    GLRenderer();
    ~GLRenderer();
    
    bool initialize();
    void render(const uint8_t* data, int width, int height);
    void cleanup();
    
private:
    GLuint compileShader(GLenum type, const char* source);
    
    GLuint shaderProgram;
    GLuint textureId;
    GLint positionLocation;
    GLint texCoordLocation;
    GLint textureLocation;
    
    static const char* vertexShaderSource;
    static const char* fragmentShaderSource;
};

#endif // GL_RENDERER_H
