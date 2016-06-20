//
//  gl_util.cpp
//

#include <cstdio>
#include <string>
#include "gl_util.h"

using namespace std;

string opengl_error_string(GLenum error_code)
{
    switch (error_code)
    {
        case GL_NO_ERROR:
            return string("No error");
        case GL_INVALID_ENUM:
            return string("Invalid enum");
        case GL_INVALID_OPERATION:
            return string("Invalid operation");
        case GL_INVALID_VALUE:
            return string("Invalid value");
        case GL_OUT_OF_MEMORY:
            return string("Out of memory");
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return string("Invalid framebuffer operation");
        default:
            return string("Unknown");
    }
}

bool check_opengl_error()
{
#ifdef DEBUG
    GLenum error_code = glGetError(); 
    if (error_code != GL_NO_ERROR)
    {
        printf("GL ERROR: %s\n", opengl_error_string(error_code).c_str());
        return true;
    }
#endif
    return false;
}

bool check_framebuffer_status()
{
#ifdef DEBUG
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        switch (status) {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                printf("Framebuffer incomplete attachment\n");
                break;
                
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                printf("Framebuffer missing attachment\n");
                break;
                
            case GL_FRAMEBUFFER_UNSUPPORTED:
                printf("Framebuffer incomplete dimensions\n");
                break;

            case GL_FRAMEBUFFER_UNDEFINED:
                printf("Framebuffer undefined\n");
                break;
                
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                printf("Framebuffer incomplete draw buffer\n");
                break;
                
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                printf("Framebuffer incomplete read buffer");
                break;
                
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                printf("Framebuffer incomplete multisample\n");
                break;
                
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                printf("Framebuffer incomplete layer targets\n");
                break;

            default:
                printf("Framebuffer incomplete, unknown status: %u\n", status);
                break;
        }
        return false;
    }
#endif
    return true;
}

    
GLuint create_texture(GLsizei width,
                    GLsizei height,
                    const void *texels)
{
    GLuint texture_id;
    
    glGenTextures(1, &texture_id);
    
    if (!texture_id) {
        printf("glGenTextures error");
        return 0;
    }
    if(check_opengl_error())
        printf("\n%s error check failed at 107\n", __PRETTY_FUNCTION__);
    GLenum minification_filter = GL_NEAREST;
    GLenum magnification_filter = GL_NEAREST;
    GLenum target = GL_TEXTURE_2D;
    GLenum internal_format = GL_RGBA;
    GLenum wrap_s = GL_CLAMP_TO_EDGE;
    GLenum wrap_t = GL_CLAMP_TO_EDGE;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;

    glBindTexture(target, texture_id);
    
    // closely packed data with byte boundaries at 1 byte
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(target, 0, internal_format,
                 width, height,
                 0, format, type, texels);
    if(check_opengl_error())
        printf("\n%s error check failed at 124\n", __PRETTY_FUNCTION__);
    
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minification_filter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magnification_filter);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
    if(check_opengl_error())
        printf("\n%s error check failed at 131\n", __PRETTY_FUNCTION__);
    
    // Unbind the texture
    glBindTexture(target, 0);
    if(check_opengl_error())
        printf("\n%s error check failed at 137\n", __PRETTY_FUNCTION__);
    
    return texture_id;
}
