//
//  gl_context.h
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdbool.h>
#include <GL/glew.h> //glew.h is needed to make GL extensions work
//glew.h includes gl.h as well
#include <GLFW/glfw3.h>
    
    //creates a OpenGL 4.3 context and a GLFWwindow using GLFW
    bool create_gl_context_window(GLFWwindow** window);
    
    //deletes the OpenGL context and cleans up
    bool delete_gl_context();

#ifdef __cplusplus
}
#endif
