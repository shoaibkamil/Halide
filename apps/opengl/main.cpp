// use apply_xray to apply xray

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "pixel_operation.h"
#include "HalideRuntime.h"
#include "HalideRuntimeOpenGL.h"
#include "benchmark.h"
#include "gl_context.h"
#include "gl_util.h"

class Image {
public:
    enum Layout {
        Interleaved, Planar
    };

    buffer_t buf;

    Image(int w, int h, int c, int elem_size, Layout layout = Interleaved) {
        memset(&buf, 0, sizeof(buffer_t));
        buf.extent[0] = w;
        buf.extent[1] = h;
        buf.extent[2] = c;
        buf.elem_size = elem_size;

        if (layout == Interleaved) {
            buf.stride[0] = buf.extent[2];
            buf.stride[1] = buf.extent[0] * buf.stride[0];
            buf.stride[2] = 1;
        } else {
            buf.stride[0] = 1;
            buf.stride[1] = buf.extent[0] * buf.stride[0];
            buf.stride[2] = buf.extent[1] * buf.stride[1];
        }
        size_t size = w * h * c * elem_size;
        buf.host = (uint8_t*)malloc(size);
        memset(buf.host, 0, size);
        buf.host_dirty = true;
    }
    ~Image() {
        free(buf.host);
    }
};

int main(int argc, const char *argv[])
{
    int width = 4096, height = 2160;

    const int channels = 4;

    Image input(width, height, channels, sizeof(uint8_t), Image::Interleaved);
    Image image_x(width, height, channels, sizeof(uint8_t), Image::Interleaved);
    Image output(width, height, channels, sizeof(uint8_t), Image::Interleaved);

    for (int i = 0; i < width * height * channels; i += channels)
    {
        image_x.buf.host[i] = 0;
        image_x.buf.host[i + 1] = 255;
        image_x.buf.host[i + 2] = 0;
        image_x.buf.host[i + 3] = 255;

        input.buf.host[i] = 255;
        input.buf.host[i + 1] = 255;
        input.buf.host[i + 2] = 255;
        input.buf.host[i + 3] = 255;
    }

    unsigned char *outputPixelsRGBA = (unsigned char *)malloc(sizeof(unsigned char) * width * height * channels);
    output.buf.host = &outputPixelsRGBA[0];

    GLFWwindow* window;
    if(! create_gl_context_window(&window))
        printf("\nOpenGL context creation failed\n");
    glewExperimental = GL_TRUE;
    glewInit();
    if(check_opengl_error())
        printf("Error check did not really fail. It's an issue with glewExperimental\n");
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

    // glfwSetWindowShouldClose(window, GL_TRUE);

    GLint curFBO;
    GLuint tempFBO;
    glGenFramebuffers(1, &tempFBO);
    if(check_opengl_error())
        printf("\n%s OpenGL error check failed\n", __PRETTY_FUNCTION__);
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, tempFBO);

    GLuint output_tex_id = create_texture(width,height,nullptr);
    // Attach output texture to FBO so we can read it with glReadPixels.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output_tex_id, 0.0);
    output.buf.dev = 0;
    int status = halide_opengl_wrap_render_target(NULL, &output.buf);
    pixel_operation(&input.buf, &image_x.buf, &output.buf);
        
    if(!check_framebuffer_status()) // TO DO: SHOULD PROBABLY THROW
    {
        printf("\nFramebuffer status check failed");
    }
    
    glReadPixels(0, 0, width, height,
                 GL_RGBA, GL_UNSIGNED_BYTE, output.buf.host);
    
    // Rebind original framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, curFBO);
    
    glDeleteFramebuffers(1, &tempFBO);

    if(!delete_gl_context())
        printf("\nOpenGL context cleanup failed");

    return 0;
}
