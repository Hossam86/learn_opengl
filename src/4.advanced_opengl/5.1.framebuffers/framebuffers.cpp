
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <iostream>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// callback funcs
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_scroll(GLFWwindow *window, double xoffset, double yoffset);
void process_input(GLFWwindow *window);

int main() {

  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);
#endif

  // glfw: window creation
  // ------------------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_WIDTH,
                                        "learn_opengl:framebuffer", NULL, NULL);
  if (window == NULL) {
    std::cout << "glfw: failed to create window!\n";
  }

  // Makes the context of the specified window current for the calling thread.
  // ------------------------------------------
  glfwMakeContextCurrent(window);

  // set mouse and window resizing callback funcs
  // ------------------------------------------
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, mouse_scroll);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // make sure that glad initilzed opengl funcs
  // ------------------------------------------
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout <<"glad: failed to initialize opengl funcs\n";
  }

  // configure global opengl state
  // ------------------------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile shaders 
  // ------------------------------------------
  Shader shader("5.1.framebuffers.vs", "5.1.framebuffers.fs");
  Shader screen_shader("5.1.framebuffers_screen.vs", "5.1.framebuffers_screen.fs");
  


}