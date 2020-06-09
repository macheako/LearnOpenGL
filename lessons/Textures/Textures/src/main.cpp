//
//  main.cpp
//  Hello Triangle
//
//  Created by Crunchy on 5/21/20.
//  Copyright © 2020 AnOrganization. All rights reserved.
//

#include <iostream>
#include <cmath>

#include "stb_image.h"
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
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

const char * VertexShaderPath   = "shaders/vertex/base.vs";
const char * FragmentShaderPath = "shaders/fragment/blend.texture2.fs";

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
        
        // OpenGL window and context
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
        
        // Create Shader(s)
        /*---------------------------------*/
        Shader shader(VertexShaderPath, FragmentShaderPath);
        
        // Create Object Buffer(s)
        /*---------------------------------*/
        unsigned int VBO, VAO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);
        
        // Create Texture Buffer(s)
        /*---------------------------------*/
        unsigned int texture;
        glGenTextures(1, &texture);
        
        // Bind Vertex Array
        /*---------------------------------*/
        glBindVertexArray(VAO);
        
        // Set Object Buffer(s)
        /*---------------------------------*/
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                        
        // Configure Vertex Attributes
        /*---------------------------------*/
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);  
        
        // Set Element Buffer(s)
        /*---------------------------------*/
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
        // attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this
        // rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally
        // don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        
        // uncomment this call to draw in wireframe polygons.
//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        // Generate Texture Object(s)
        /*---------------------------------*/
        unsigned int texture1, texture2;
        glGenTextures(1, &texture1);
        
        // Bind Texture 1
        /*---------------------------------*/
        glBindTexture(GL_TEXTURE_2D, texture1);
        
        // Configure Texture Parameters
        /*---------------------------------*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Load Texture 1
        /*---------------------------------*/
        int width, height, nrChannels;
        std::string imagePath = "images/wood.jpg";
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);
        if (!data)
        {
            throw new std::runtime_error("Failed to load image: " + imagePath);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
        
        // Create Texture 2
        /*---------------------------------*/
        imagePath = "images/brick.jpg";
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        // Configure Texture Parameters
        /*---------------------------------*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Load Texture 2 Image
        /*---------------------------------*/
        data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);
        if (!data)
        {
            throw new std::runtime_error("Failed to load image: " + imagePath);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
        
        // Set Texture Uniforms
        /*---------------------------------*/
        shader.use();
        shader.setInt("uTexture1", 0);
        shader.setInt("uTexture2", 1);
        shader.setFloat("uBlend", 0.5);
        
        // Run Loop
        /*---------------------------------*/
        while (!glfwWindowShouldClose(window))
        {
            // Capture Input
            processInput(window);
            
            // Clear Screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
                                    
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            
            shader.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
            // glfw: swap buffers and poll IO events
            // (keys pressed/released, mouse moved etc.)
            /*---------------------------------*/
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
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
