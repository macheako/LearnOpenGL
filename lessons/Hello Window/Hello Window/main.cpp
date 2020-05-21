//
//  main.cpp
//  TestApp
//
//  Created by Crunchy on 5/20/20.
//  Copyright © 2020 Crunchy. All rights reserved.
//

#include <glad/3.3/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, const char * argv[]) {
    // Initialize GLFW
    glfwInit();

    // Define version and compatibility settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    //    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create OpenGL window and context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window)
    {
        std::cerr << "[glfw] Unable to create window" << std::endl;
        
        // Terminate GLFW
        glfwTerminate();
        return -1;
    }
    
    // Register context with OpenGL
    glfwMakeContextCurrent(window);
    
    // Register window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW
    //    glewExperimental = GL_TRUE;
    //    glewInit();
    
    // Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);
        
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Terminate GLFW
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
