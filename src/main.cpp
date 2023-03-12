#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

    // initialization code
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create Window
    GLFWwindow *window = glfwCreateWindow(800, 600, "learn_Opengl", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GlFW Window! " << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // We should initialize GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // before we have start renderring we shoukd tell opengl the size of the window
    glViewport(0, 0, 600, 800);

    // register call back function to change view port when the window size changed
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //clean and free glfw resources
    glfwTerminate();


    return 0;

}
