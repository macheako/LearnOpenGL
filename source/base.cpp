//
//  main.cpp
//  Hello Triangle
//
//  Created by Crunchy on 5/21/20.
//  Copyright © 2020 AnOrganization. All rights reserved.
//

#include <iostream>
#include <cmath>

#include "Shader.h"
#include <glad/3.3/glad.h>
#include <GLFW/glfw3.h>

// Function Declarations
/*---------------------------------*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

bool check_shader_compilation(unsigned int shader);
bool check_program_link(unsigned int program);

// Global Variables
/*---------------------------------*/
char errlog[512];
float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

// Shader Source
/*---------------------------------*/
const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\0";

// START APPLICATION
/*----------------------------------------------------------------*/
int main(int argc, const char * argv[])
{
    try
    {
        // Initialize GLFW
        /*---------------------------------*/
        glfwInit();

        // Define version and compatibility settings
        /*---------------------------------*/
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
            
        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // Create OpenGL window and context
        /*---------------------------------*/
        GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
        if (!window)
            throw new std::runtime_error("[glfw] Unable to create window");
        
        // Register context with OpenGL
        /*---------------------------------*/
        glfwMakeContextCurrent(window);
        
        // Register window resize callback
        /*---------------------------------*/
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Initialize glad
        /*---------------------------------*/
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw new std::runtime_error("[glad] Failed to initialize");
        
        // Create Shader Object
        /*---------------------------------*/
        
        
        // Create & Bind New Vertex Buffer Object(s)
        /*---------------------------------*/
        unsigned int VBO, VAO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);
        
        // Bind Buffer Array
        /*---------------------------------*/
        glBindVertexArray(VAO);
        
        // Bind & Set Vertex Buffer(s)
        /*---------------------------------*/
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // Configure Vertex Attributes
        /*---------------------------------*/
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
        // attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored
        // in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this
        // rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally
        // don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        
        // uncomment this call to draw in wireframe polygons.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        Shader myShader("base.vert", "base.frag");
        
        // Run Loop
        /*---------------------------------*/
        while (!glfwWindowShouldClose(window))
        {
            // Capture Input
            processInput(window);
            
            // Clear Screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            myShader.use();
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            
            // glfw: swap buffers and poll IO events
            // (keys pressed/released, mouse moved etc.)
            /*---------------------------------*/
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    catch (std::exception e)
    {
        std::cerr << "Exception Thrown! " << e.what() << std::endl;
    }
    
    // Terminate GLFW
    /*---------------------------------*/
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys
// are pressed/released this frame and react accordingly
/*----------------------------------------------------*/
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize)
// this callback function executes
/*----------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool check_shader_compilation(unsigned int shader)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, errlog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << errlog << std::endl;
    }
    return success;
}

bool check_program_link(unsigned int program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, errlog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << errlog << std::endl;
    }
    return success;
}
