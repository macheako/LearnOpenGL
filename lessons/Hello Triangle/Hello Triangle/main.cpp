//
//  main.cpp
//  Hello Triangle
//
//  Created by Crunchy on 5/21/20.
//  Copyright © 2020 AnOrganization. All rights reserved.
//

#include <iostream>
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
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

// Shader Source
/*---------------------------------*/
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
        
        // Create & Bind New Vertex Buffer Object
        /*---------------------------------*/
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        // Populate Buffer Object
        /*---------------------------------*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // Create Vertex Shader
        /*---------------------------------*/
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        
        // Attach And Compile Vertex Shader
        /*---------------------------------*/
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        
        if (!check_shader_compilation(vertexShader))
            throw new std::runtime_error("Unable To Compile Vertex Shader");
        
        // Create Fragment Shader
        /*---------------------------------*/
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        // Attach And Compile Fragment Shader
        /*---------------------------------*/
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        
        if (!check_shader_compilation(fragmentShader))
            throw new std::runtime_error("Unable To Link Shader Program");
        
        // Create Shader Program
        /*---------------------------------*/
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        
        // Link Vertex Attributes
        /*---------------------------------*/
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);        
        
        // Attach Shaders And Link Program
        /*---------------------------------*/
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        glUseProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        // Run Loop
        /*---------------------------------*/
        while (!glfwWindowShouldClose(window))
        {
            // Input
            processInput(window);
            
            // Render
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // glfw: swap buffers and poll IO events
            // (keys pressed/released, mouse moved etc.)
            /*---------------------------------*/
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
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
