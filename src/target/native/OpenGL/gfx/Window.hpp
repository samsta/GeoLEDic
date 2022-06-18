#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/glm.hpp>
#include "Config.hpp"

struct GLFWwindow;

namespace gfx {

class Config;
  
class Window
{
public:
   Window(const Config& config);
   ~Window();

   void processInputs();
   
   const glm::mat4& getViewMatrix() const;
   const glm::mat4& getProjectionMatrix() const;
   const glm::vec3& getCameraPosition() const;

   void swapBuffers();
   
   bool shouldClose() const;

   GLFWwindow* get();
   glm::vec3 getDirection() const;

private:
   
   static void doKeyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods);
   void keyCallback(int key, int scan_code, int action, int mods);

   static void doScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
   void scrollCallback(float xoffset, float yoffset);

   void selectView(unsigned index);

   const Config& m_config;
   GLFWwindow*   m_window;
   glm::vec3     m_position;
   float         m_horizontal_angle_rad;
   float         m_vertical_angle_rad;
   float         m_fov_deg;
   float         m_scroll_speed;
   float         m_mouse_speed;

   glm::mat4     m_view_matrix;
   glm::mat4     m_projection_matrix;
   double        m_last_time;

   bool          m_control_mouse;
   bool          m_shift_pressed;
   bool          m_should_close;
};

}
#endif
