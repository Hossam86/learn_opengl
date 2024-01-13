#include <common.h>
const char *TITLE = "learn_opengl -- glfw_window";

int main() {
  // glfw:initialization and configuration
  //-------------------------------------------------------
  GLFWwindow *window = create_glfw_window(TITLE, SCR_WIDTH, SCR_HEIGHT);

  // we want to initialize GLAD before we call any OpenGL function:
  //-------------------------------------------------------
  if (!initialize_opengl_context(window))
    return -1;

  glViewport(0, 0, 800, 600);

  // We do have to tell GLFW we want to call this function on every window
  // resize by registering it: register call back function to change view port
  // when the window size changed
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // We don't want the application to draw a single image and then immediately
  // quit and close the window.
  //  We want the application to keep drawing images and handling user input
  //  until the program has been
  // explicitly told to stop. For this reason we have to create a while loop,
  // that we now call the render
  //  loop, that keeps on running until we tell GLFW to stop.
  // render loop
  while (!glfwWindowShouldClose(window)) {
    // process input
    processInput(window);

    // render commands,
    //....

    // As you may recall from the OpenGL chapter, the glClearColor function is a
    // state-setting function and glClear is a state-using function in that it
    // uses the current state to retrieve the clearing color from.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // check and call events and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // as soon as we exit the render loop we would like to properly clean/delete
  // all of GLFW's resources that
  //  were allocated. We can do this via the glfwTerminate function that we call
  //  at the end of the main function.
  glfwTerminate();
  return 0;
}