// Depth testing
// In the coordinate systems chapter we've rendered a 3D container and made use
// of a depth buffer to prevent triangles rendering in the front while they're
// supposed to be behind other triangles. In this chapter we're going to
// elaborate a bit more on those depth values the depth buffer (or z-buffer)
// stores and how it actually determines if a fragment is in front.

// The depth-buffer is a buffer that, just like the color buffer (that stores
// all the fragment colors: the visual output), stores information per fragment
// and has the same width and height as the color buffer. The depth buffer is
// automatically created by the windowing system and stores its depth values as
// 16, 24 or 32 bit floats. In most systems you'll see a depth buffer with a
// precision of 24 bits.

// When depth testing is enabled, OpenGL tests the depth value of a fragment
// against the content of the depth buffer. OpenGL performs a depth test and if
// this test passes, the fragment is rendered and the depth buffer is updated
// with the new depth value. If the depth test fails, the fragment is discarded.

// Depth testing is done in screen space after the fragment shader has run (and
// after the stencil test which we'll get to in the next chapter). The screen
// space coordinates relate directly to the viewport defined by OpenGL's
// glViewport function and can be accessed via GLSL's built-in gl_FragCoord
// variable in the fragment shader. The x and y components of gl_FragCoord
// represent the fragment's screen-space coordinates (with (0,0) being the
// bottom-left corner). The gl_FragCoord variable also contains a z-component
// which contains the depth value of the fragment. This z value is the value
// that is compared to the depth buffer's content.

// Today most GPUs support a hardware feature called early depth testing. Early
// depth testing allows the depth test to run before the fragment shader runs.
// Whenever it is clear a fragment isn't going to be visible (it is behind other
// objects) we can prematurely discard the fragment.

// Fragment shaders are usually quite expensive so wherever we can avoid running
// them we should. A restriction on the fragment shader for early depth testing
// is that you shouldn't write to the fragment's depth value. If a fragment
// shader would write to its depth value, early depth testing is impossible;
// OpenGL won't be able to figure out the depth value beforehand.
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glad/glad.h>
#include <shader.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_click_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void process_input(GLFWwindow *window);
unsigned int load_texture(const char *path);

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GL_TRUE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWORD_COMPACT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LEARN_OPENGL", NULL, NULL);

      if (window==NULL)
      {
        std::cout << "glfw: failed to create window!\n";
        glfwTerminate();
        return -1;
      }
      
}