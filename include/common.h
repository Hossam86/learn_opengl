#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// simple shader source
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "} ";

// callback function on the window that gets called each time the window is resized.
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    glViewport(0, 0, width, height);
}

// glfw: create,initialization and configue GLFW window
GLFWwindow *create_glfw_window(const char *title, int width, int height)
{
    // glfw:initialization and configuration
    //-------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: window creation
    //-------------------------------------------------------
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    return window;
}

// initialize GLAD before we call any OpenGL function:
bool initialize_opengl_context(GLFWwindow *window)
{
    // Before you can use the OpenGL API, you must have a current OpenGL context.
    glfwMakeContextCurrent(window);

    // glad: load all opengl function pointers (We should initialize GLAD before we call any OpenGL function)
    //-------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

// We also want to have some form of input control in GLFW and we can achieve this with several of GLFW's input
//  functions. We'll be using GLFW's glfwGetKey function that takes the window as input together with a key.
//  The function returns whether this key is currently being pressed. We're creating a processInput function
// to keep all input code organized

void processInput(GLFWwindow *window)
{
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
