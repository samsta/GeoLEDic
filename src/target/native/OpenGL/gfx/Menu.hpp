#ifndef Menu_h
#define Menu_h

#include "Config.hpp"
#include "Gfx.hpp"
#include "Window.hpp"

struct GLFWwindow;

namespace gfx {

class Menu
{
public:
   Menu(Config& config, Window& window);
   ~Menu();
   
   void draw();
   
private:
   void drawLabels();

   Config& m_config;
   Window& m_window;
};

}


#endif /* Menu_h */
