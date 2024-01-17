#pragma once
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum Camera_Movment {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

// Default Camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
  // camera Attributes
public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldup;

  // euler angles
  float yaw;
  float pitch;

  // camera options
  float movement_speed;
  float mouse_seneitivty;
  float zoom;

  // constructor with vectors
  Camera(glm::vec3 _position = glm::vec3(0.0, 0.0, 0.0),
         glm::vec3 _up = glm::vec3(0.0, 1.0, 0.0), float _yaw = YAW,
         float _pitch = PITCH)
      : front(glm::vec3(0.0, 0.0, -1.0)), movement_speed(SPEED),
        mouse_seneitivty(SENSITIVITY), zoom(ZOOM) {
    position = _position;
    worldup = _up;
    yaw = _yaw;
    pitch = _pitch;
    update_camera_vectors();
  }
  // constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw_, float pitch_)
      : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED),
        mouse_seneitivty(SENSITIVITY), zoom(ZOOM) {
    position = glm::vec3(posX, posY, posZ);
    worldup = glm::vec3(upX, upY, upZ);
    yaw = yaw_;
    pitch = pitch_;
    update_camera_vectors();
  }

  glm::mat4 get_view_matrix() {
    return glm::lookAt(position, position + front, up);
  }

  // processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void process_keyboard(Camera_Movment direction, float delta_time) {
    float velocity = movement_speed * delta_time;
    if (direction == FORWARD)
      position += front * velocity;
    if (direction == BACKWARD)
      position -= front * velocity;
    if (direction == LEFT)
      position -= right * velocity;
    if (direction == RIGHT)
      position += right * velocity;
  }

  // processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void process_mouse_scroll(float xoffset, float yoffset,
                            GLboolean contrain_pitch = true) {
    xoffset *= mouse_seneitivty;
    yoffset *= mouse_seneitivty;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (contrain_pitch) {
      if (pitch > 89.0f)
        pitch = 89.0f;
      if (pitch < -89.0f)
        pitch = -89.0f;
    }
    // update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors();
  }

  // processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset) {
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
      zoom = 1.0f;
    if (zoom > 45.0f)
      zoom = 45.0f;
  }

private:
  // calculates the front vector from the Camera's (updated) Euler Angles
  void update_camera_vectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    // normalize the vectors, because their length gets closer to 0 the more you
    // look up or down which results in slower movement.
    right = glm::normalize(glm::cross(front, worldup));
    up = glm::normalize(glm::cross(right, front));
  }
};