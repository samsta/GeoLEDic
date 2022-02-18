#ifndef GFX_CONFIG_HPP
#define GFX_CONFIG_HPP

#include <vector>
#include <map>
#include <string>

namespace gfx {
  
class Config
{
public:
   
   friend class Menu;
   
   struct View
   {
      View(float x, float y, float z, float fov_deg = 45.f, float horizontal_angle_deg = 0.f, float vertical_angle_deg = 0.f):
         x(x),
         y(y),
         z(z),
         fov_deg(fov_deg),
         horizontal_angle_deg(horizontal_angle_deg),
         vertical_angle_deg(vertical_angle_deg)
      {}
      float x, y, z;
      float fov_deg, horizontal_angle_deg, vertical_angle_deg;
   };
   
   struct Label3D 
   {
      float x;
      float y;
      float z;
      std::string line1;
      std::string line2;
   };

   class KeyboardHandler
   {
   public:
      virtual ~KeyboardHandler(){}
      virtual void handleKey(char c) = 0;
   };

   class MenuPresenter
   {
   public:
      virtual ~MenuPresenter(){};
      virtual void drawMenu() = 0;
   };
   
   Config():
      m_width(1024),
      m_height(768),
      m_fps(30),
      m_cutoff_distance(5.5),
      m_attenuation_constant(0.5),
      m_attenuation_linear(10),
      m_attenuation_square(10),
      m_strafing_speed(20),
      m_views(1, View(0, 8, 5)),
      m_keyboard_handler(&m_null_handler),
      m_menu_presenter(),
      m_identify_triangles(false)
   {
   }
   
   Config& width(int width) { m_width = width; return *this; }
   int width() const { return m_width;}
   
   Config& height(int height) { m_height = height; return *this; }
   int height() const { return m_height; }
   
   Config& views(std::vector<View>& views) { m_views = views; return *this; }
   const std::vector<View>& views() const { return m_views; }
   
   Config& framesPerSecond(float fps) { m_fps = fps; return *this; }
   float framesPerSecond() const { return m_fps; }
   
   Config& cutoffDistance(float cutoff) { m_cutoff_distance = cutoff; return *this; }
   float cutoffDistance() const { return m_cutoff_distance; }
   
   Config& attenuationConstant(float attenuation) { m_attenuation_constant = attenuation; return *this; }
   float attenuationConstant() const { return m_attenuation_constant; }
   
   Config& attenuationLinear(float attenuation) { m_attenuation_linear = attenuation; return *this; }
   float attenuationLinear() const { return m_attenuation_linear; }
   
   Config& attenuationSquare(float attenuation) { m_attenuation_square = attenuation; return *this; }
   float attenuationSquare() const { return m_attenuation_square; }
   
   Config& strafingSpeed(float speed) { m_strafing_speed = speed; return *this; }
   float strafingSpeed() const { return m_strafing_speed; }

   Config& keyboardHandler(KeyboardHandler* handler)
   {
      if (handler) m_keyboard_handler = handler;
      return *this;
   }
   KeyboardHandler& keyboardHandler() const { return *m_keyboard_handler; }
   
   Config& menu(MenuPresenter* presenter)
   {
      if (presenter) m_menu_presenter = presenter;
      return *this;
   }
   MenuPresenter* menu() const { return m_menu_presenter; }

   Config& labels3D(const std::vector<Label3D>& labels)
   {
      m_labels = &labels;
      return *this;
   }
   const std::vector<Label3D>& labels3D() const
   {
      if (m_labels)
      {
         return *m_labels; 
      }
      static std::vector<Label3D> dummy;
      return dummy;
   }
   bool& identifyTriangles()
   {
      return m_identify_triangles;
   }
   
private:
   
   class NullKeyboardHandler: public KeyboardHandler
   {
   public:
      virtual void handleKey(char){}
   } m_null_handler;
   
   int m_width;
   int m_height;
   float m_fps;
   float m_cutoff_distance;
   float m_attenuation_constant;
   float m_attenuation_linear;
   float m_attenuation_square;
   float m_strafing_speed;
   std::vector<View> m_views;
   const std::vector<Label3D>* m_labels;
   KeyboardHandler* m_keyboard_handler;
   MenuPresenter* m_menu_presenter;
   bool m_identify_triangles;
};

}

#endif // GFX_CONFIG_HPP
