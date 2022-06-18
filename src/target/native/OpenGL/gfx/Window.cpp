#include "Window.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#ifdef WITH_GLEW
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "imgui.h"

namespace gfx {

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Window::Window(const Config& config):
      m_config(config),
      m_window(NULL),
      m_position(),
      m_horizontal_angle_rad(),
      m_vertical_angle_rad(),
      m_fov_deg(),
      m_scroll_speed(1.0f),
      m_mouse_speed(0.001f),
      m_view_matrix(),
      m_projection_matrix(),
      m_last_time(0.0),
      m_control_mouse(false),
      m_shift_pressed(false),
      m_should_close(false)
{
   // Initialise GLFW
   glfwSetErrorCallback(glfw_error_callback);
   if(not glfwInit())
   {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      exit(EXIT_FAILURE);
   }

   selectView(0);
     
   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // Open a window and create its OpenGL context
   m_window = glfwCreateWindow(m_config.width(), m_config.height(), "FastLED OpenGL", NULL, NULL);
   if(m_window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
      glfwTerminate();
      exit(EXIT_FAILURE);
   }
   glfwMakeContextCurrent(m_window);
   glEnable(GL_MULTISAMPLE);
   glfwSetWindowUserPointer(m_window, this);
   glfwSetKeyCallback(m_window, &Window::doKeyCallback);
   glfwSetScrollCallback(m_window, &Window::doScrollCallback);

#ifndef __APPLE__
   // Initialize GLEW
   glewExperimental = true; // Needed for core profile
   if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      glfwTerminate();
      exit(EXIT_FAILURE);
   }
#endif
      
   // Ensure we can capture the escape key being pressed below
   glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

   if (m_control_mouse)
   {
      // Hide the mouse and enable unlimited mouvement
      glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      
      // Set the mouse at the center of the screen
      glfwPollEvents();
      glfwSetCursorPos(m_window, m_config.width()/2, m_config.height()/2);
   }

   // black background
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

Window::~Window()
{      
   // Close OpenGL window and terminate GLFW
   glfwTerminate();   
}

void Window::processInputs()
{
   if (m_last_time == 0.0)
   {
      m_last_time = glfwGetTime();
   }

   // Compute time difference between current and last frame
   double currentTime = glfwGetTime();
   float deltaTime = float(currentTime - m_last_time);

   int width, height;
   glfwGetWindowSize(m_window, &width, &height);

   if (m_control_mouse)
   {
      double xpos, ypos;
      glfwGetCursorPos(m_window, &xpos, &ypos);

      // Reset mouse position for next frame
      glfwSetCursorPos(m_window, width/2, height/2);

      // Compute new orientation
      m_horizontal_angle_rad += m_mouse_speed * float(width/2 - xpos );
      m_vertical_angle_rad   += m_mouse_speed * float(height/2 - ypos );
   }

   glm::vec3 direction(getDirection());
   
   // Right vector
   glm::vec3 right = glm::vec3(
                        sin(m_horizontal_angle_rad - 3.14f/2.0f), 
			0,
			cos(m_horizontal_angle_rad - 3.14f/2.0f));
   
   // Up vector
   glm::vec3 up = glm::cross( right, direction );

   // Move up 
   if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS){
     m_position += up * deltaTime * m_config.strafingSpeed();
   }
   // Move down
   if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS){
     m_position -= up * deltaTime * m_config.strafingSpeed();
   }
   // Strafe right
   if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS){
     m_position += right * deltaTime * m_config.strafingSpeed();
   }
   // Strafe left
   if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS){
     m_position -= right * deltaTime * m_config.strafingSpeed();
   }

   // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
   m_projection_matrix = glm::perspective(glm::radians(m_fov_deg), float(width)/float(height), 0.1f, 1000.0f);
   // Camera matrix
   m_view_matrix = glm::lookAt(m_position,           // Camera is here
			       m_position+direction, // and looks here : at the same position, plus "direction"
			       up                    // Head is up (set to 0,-1,0 to look upside-down)
			       );

   // For the next frame, the "last time" will be "now"
   m_last_time = currentTime;
}

GLFWwindow* Window::get()
{
   return m_window;
}

const glm::mat4& Window::getViewMatrix() const
{
   return m_view_matrix;
}

const glm::mat4& Window::getProjectionMatrix() const
{
   return m_projection_matrix;
}

const glm::vec3& Window::getCameraPosition() const
{
   return m_position;
}

glm::vec3 Window::getDirection() const
{
  // Direction : Spherical coordinates to Cartesian coordinates conversion
  return glm::vec3(cos(m_vertical_angle_rad) * sin(m_horizontal_angle_rad), 
		   sin(m_vertical_angle_rad),
		   cos(m_vertical_angle_rad) * cos(m_horizontal_angle_rad)
		   );
}

  void Window::selectView(unsigned index)
{
  if (index < m_config.views().size())
  {
    const Config::View& view(m_config.views()[index]);
    m_position = glm::vec3(view.x, view.y, view.z);
    m_fov_deg = view.fov_deg;
    m_horizontal_angle_rad = M_PI * (view.horizontal_angle_deg + 180.f) / 180.f;
    m_vertical_angle_rad = M_PI * view.vertical_angle_deg / 180.f;
  }
}
  
void Window::swapBuffers()
{
   glfwSwapBuffers(m_window);
   glfwPollEvents();
}

bool Window::shouldClose() const
{
   return m_should_close or
          glfwWindowShouldClose(m_window);
}

void Window::doKeyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
   static_cast<Window*>(glfwGetWindowUserPointer(window))->keyCallback(key, scan_code, action, mods);
}

void Window::keyCallback(int key, int scan_code, int action, int mods)
{
   if (key == GLFW_KEY_RIGHT_SHIFT or key == GLFW_KEY_LEFT_SHIFT)
   {
      m_shift_pressed = (action == GLFW_PRESS);
      return;
   }
   
   if (action != GLFW_PRESS) return;
   
   if (m_shift_pressed)
   {
      switch(key) {
      case GLFW_KEY_M:
         // Toggle mouse control
         m_control_mouse = not m_control_mouse;
         
         if (m_control_mouse)
         {
            // Hide the mouse and enable unlimited mouvement
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            
            // Set the mouse at the center of the screen
            glfwPollEvents();
            int width, height;
            glfwGetWindowSize(m_window, &width, &height);
            glfwSetCursorPos(m_window, width/2, height/2);
         }
         else
         {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         }
         break;
      case GLFW_KEY_0:
      case GLFW_KEY_1:
      case GLFW_KEY_2:
      case GLFW_KEY_3:
      case GLFW_KEY_4:
      case GLFW_KEY_5:
      case GLFW_KEY_6:
      case GLFW_KEY_7:
      case GLFW_KEY_8:
      case GLFW_KEY_9:
         selectView(key - GLFW_KEY_0);
         break;
      case GLFW_KEY_Q:
         m_should_close = true;
         break;
      default:
         if (isprint(key))
         {
            m_config.keyboardHandler().handleKey(key);
         }
         break;
      }
   }
   else if (isprint(key))
   {
      m_config.keyboardHandler().handleKey(key);
   }
   else if (key == GLFW_KEY_ENTER)
   {
      m_config.keyboardHandler().handleKey('\n');
   }
}

void Window::doScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
   static_cast<Window*>(glfwGetWindowUserPointer(window))->scrollCallback(xoffset, yoffset);
}

void Window::scrollCallback(float xoffset, float yoffset)
{
   // don't scroll if we're in the menu
   if (ImGui::GetIO().WantCaptureMouse) return;
   m_position += getDirection() * yoffset * m_scroll_speed;
}

}
