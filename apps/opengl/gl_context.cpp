//
//  gl_context.cpp
//

#include <stdio.h>
#include <stdlib.h>
#include "gl_context.h"

// This is the callback we'll be registering with GLFW for errors.
// It'll just print out the error to the STDERR stream.
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

bool create_gl_context_window(GLFWwindow** window)
{
    // Initialize GLFW, and if it fails to initialize for any reason, print it out to STDERR.
    if (!glfwInit()) {
        fprintf(stderr, "Failed initialize GLFW.");
        exit(0);
        return false;
    }
    // Set the error callback, as mentioned above.
    glfwSetErrorCallback(error_callback);
    
    // Set up OpenGL options.
    // Use OpenGL verion 4.1,
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // GLFW_OPENGL_FORWARD_COMPAT specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed.
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Indicate we only want the newest core profile, rather than using backwards compatible and deprecated features.
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Make the window resize-able.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
    
    // Create a window to put our stuff in.
    *window = glfwCreateWindow(2048, 1024, "OpenGL", NULL, NULL);
    // glfwHideWindow(window);
    
    // If the window fails to be created, print out the error, clean up GLFW and exit the program.
    if(!*window) {
        fprintf(stderr, "Failed to create GLFW window.");
        delete_gl_context(*window);
        exit(0);
        return false;
    }
    
    // Use the window as the current context (everything that's drawn will be place in this window).
    glfwMakeContextCurrent(*window);
    
    // Set the keyboard callback so that when we press ESC, it knows what to do.
    glfwSetKeyCallback(*window, key_callback);
    
    return true;
}

bool delete_gl_context(GLFWwindow* window)
{
    while (!glfwWindowShouldClose(window))
      glfwPollEvents();

    // Release the current context
    glfwMakeContextCurrent(NULL);
    
    // Destroy all windows and cursors.
    //Note: No window's context may be current on another thread when glfwTerminate function is called.
    glfwTerminate();
    return true;
}
