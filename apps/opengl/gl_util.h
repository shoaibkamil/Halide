//
//  gl_util.h
//

#include <GL/glew.h> //glew.h is needed to make GL extensions work
//glew.h includes gl.h as well

bool check_opengl_error();

bool check_framebuffer_status();
    
GLuint create_texture(GLsizei width,
                    GLsizei height,
                    const void *pixel_data);
