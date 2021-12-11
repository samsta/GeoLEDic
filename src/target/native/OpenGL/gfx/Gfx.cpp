#include "Gfx.hpp"
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#ifdef WITH_GLEW
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "Menu.hpp"
#include "shader.hpp"
#include "Window.hpp"
#include <iostream>
#include <unistd.h>


// FastLED needs this symbol. Find a better place for it.
// It is supposed to be provided by the application, so
// we make it weak to allow for it to be reimplemented
uint16_t XY(uint8_t, uint8_t) __attribute__ ((weak));
uint16_t XY(uint8_t, uint8_t)
{
   return 0;
}

namespace gfx {

using namespace glm;
using namespace std;

namespace {

const std::string TRIANGLE_VERTEX_SHADER =
#include "lit_triangle.vertexshader"
;

const int MAX_LEDS_PER_TRIANGLE = 400;

const std::string TRIANGLE_FRAGMENT_SHADER =
#include "lit_triangle.fragmentshader"
;


struct LedPosition
{
   GLfloat x,y,z;
   
   void operator=(const LED& l)
   {
      x = l.x;
      y = l.y;
      z = l.z;
   }
   
};

struct LedColor
{
   GLfloat r,g,b;
   
   void operator=(const LED& l)
   {
      if (l.color != nullptr)
      {
         r = float(l.color->r)/256;
         g = float(l.color->g)/256;
         b = float(l.color->b)/256;
      }
   }
};

vec3 toVec3(const Vertex& v)
{
   return vec3(v.x, v.y, v.z);
}

GLint getUniformLocation(GLuint program, const char* name)
{
   GLint uid = glGetUniformLocation(program, name);
   if (uid < 0)
   {
      std::cerr << "Failed getting uniform '" << name << "': " << glGetError() << std::endl;
   }
   return uid;
}

}

class Gfx::Impl
{
public:
   Impl(vector<LED>& led_coordinates, vector<Triangle>& triangles, const Config& config = Config());
   ~Impl();

   bool draw();
   
   void drawLitTriangles();
   void controlFrameRate();
   void extractUniformLocations();
   void registerForceBlank(bool& force_blank);
   
   std::vector<LED>& m_leds;
   std::vector<Triangle>& m_triangles;
   Config              m_config;
   Window              m_window;
   Menu                m_menu;
   GLuint              m_vertex_array_id;
   GLuint              m_program_id;
   vector<LedPosition> m_led_position_data;
   vector<LedColor>    m_led_color_data;
   GLuint              m_vertex_buffer;
   double              m_last_time;
   const double        m_frame_time;
   bool*               m_force_blank;
   
   struct {
      GLint projection;
      GLint view;
      GLint direction;
      GLint cutoff;
      GLint attenuation_constant;
      GLint attenuation_linear;
      GLint attenuation_square;
      GLint normal;
      GLint led_color;
      GLint led_pos;
      GLint num_leds;
   } m_uniforms;

};

Gfx::Impl::Impl(std::vector<LED>& leds, std::vector<Triangle>& triangles, const Config& config):
   m_leds(leds),
   m_triangles(triangles),
   m_config(config),
   m_window(m_config),
   m_menu(m_config, m_window.get()),
   m_led_position_data(m_leds.size()),
   m_led_color_data(m_leds.size()),
   m_frame_time(1.0/m_config.framesPerSecond())
{
   copy(m_leds.begin(), m_leds.end(), m_led_position_data.begin());
   
   // Enable depth test
   glEnable(GL_DEPTH_TEST);
   // Accept fragment if it closer to the camera than the former one
   glDepthFunc(GL_LESS);
   
   for (unsigned k = 0; k < m_triangles.size(); k++)
   {
      Triangle& t(m_triangles[k]);
      if (t.vertices[0].num_leds > MAX_LEDS_PER_TRIANGLE)
      {
         std::cerr << "Triangle " << k << " has " << t.vertices[0].num_leds << " LEDs but fragment shader only supports " << MAX_LEDS_PER_TRIANGLE << std::endl;
         t.vertices[0].num_leds = MAX_LEDS_PER_TRIANGLE;
      }
   }
   
   const unsigned fragment_shader_maxchars = TRIANGLE_FRAGMENT_SHADER.size() + 10;
   char* fragment_shader = new char[fragment_shader_maxchars];
   snprintf(fragment_shader, fragment_shader_maxchars-1, TRIANGLE_FRAGMENT_SHADER.c_str(), MAX_LEDS_PER_TRIANGLE);
   m_program_id = LoadShaders(TRIANGLE_VERTEX_SHADER.c_str(), fragment_shader);
   delete [] fragment_shader;
   
   // first, configure the cube's VAO (and VBO)
   glGenVertexArrays(1, &m_vertex_array_id);
   glGenBuffers(1, &m_vertex_buffer);

   glBindVertexArray(m_vertex_array_id);

   glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * m_triangles.size(), m_triangles.data(), GL_STATIC_DRAW);

   // position attribute for triangles
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   
   // front facing side of the triangles is with vertices going clockwise
   glFrontFace(GL_CW);
   
   extractUniformLocations();
}

void Gfx::Impl::controlFrameRate()
{
   if (m_last_time != 0)
   {
      double dt = glfwGetTime() - m_last_time;
      if (dt < m_frame_time)
      {
         usleep((m_frame_time - dt)*1000000);
      }
   }
   m_last_time = glfwGetTime();
   
   // wait for last iteration to finish rendering
   glFinish();
}

void Gfx::Impl::extractUniformLocations()
{
   glUseProgram(m_program_id);
   m_uniforms.projection = getUniformLocation(m_program_id, "projection");
   m_uniforms.view = getUniformLocation(m_program_id, "view");
   m_uniforms.direction = getUniformLocation(m_program_id, "direction");
   m_uniforms.cutoff = getUniformLocation(m_program_id, "cutoff_distance_square");
   m_uniforms.attenuation_constant = getUniformLocation(m_program_id, "attenuation_constant");
   m_uniforms.attenuation_linear = getUniformLocation(m_program_id, "attenuation_linear");
   m_uniforms.attenuation_square = getUniformLocation(m_program_id, "attenuation_square");
   m_uniforms.normal = getUniformLocation(m_program_id, "normal");
   m_uniforms.led_color = getUniformLocation(m_program_id, "led_color");
   m_uniforms.led_pos = getUniformLocation(m_program_id, "led_pos");
   m_uniforms.num_leds = getUniformLocation(m_program_id, "num_leds");
}

bool Gfx::Impl::draw()
{
   controlFrameRate();
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   m_window.processInputs();

   if (m_force_blank != nullptr and *m_force_blank)
   {
      LedColor black = {0};
      fill(m_led_color_data.begin(), m_led_color_data.end(), black);
   }
   else
   {
      copy(m_leds.begin(), m_leds.end(), m_led_color_data.begin());
   }
   
   glUseProgram(m_program_id);

   // view/projection transformations
   glUniformMatrix4fv(m_uniforms.projection, 1, GL_FALSE, &m_window.getProjectionMatrix()[0][0]);
   glUniformMatrix4fv(m_uniforms.view, 1, GL_FALSE, &m_window.getViewMatrix()[0][0]);
   glUniform3fv(m_uniforms.direction, 1, &m_window.getDirection()[0]);
   
   float cutoff = m_config.cutoffDistance();
   glUniform1f(m_uniforms.cutoff, cutoff * cutoff);
   glUniform1f(m_uniforms.attenuation_constant, m_config.attenuationConstant());
   glUniform1f(m_uniforms.attenuation_linear, m_config.attenuationLinear());
   glUniform1f(m_uniforms.attenuation_square, m_config.attenuationSquare());
   

   for (unsigned k = 0; k < m_triangles.size(); k++)
   {
      Triangle& t(m_triangles[k]);
      vec3 normal = glm::normalize(glm::cross(toVec3(t.vertices[1]) - toVec3(t.vertices[0]), toVec3(t.vertices[2]) - toVec3(t.vertices[0])));
      glUniform3fv(m_uniforms.normal, 1, &normal[0]);

      // led color and position
      glUniform3fv(m_uniforms.led_color, t.vertices[0].num_leds, &m_led_color_data[t.vertices[0].start_led_ix].r);
      glUniform3fv(m_uniforms.led_pos, t.vertices[0].num_leds, &m_led_position_data[t.vertices[0].start_led_ix].x);
      glUniform1i(m_uniforms.num_leds, t.vertices[0].num_leds);

      // render the triangle
      glBindVertexArray(m_vertex_array_id);
      glDrawArrays(GL_TRIANGLES, k*3, 3);
      glBindVertexArray(0);
   }
   
   m_menu.draw();
      
   m_window.swapBuffers();
   return not m_window.shouldClose();
}

void Gfx::Impl::registerForceBlank(bool& force_blank)
{
   m_force_blank = &force_blank;
}

Gfx::Impl::~Impl()
{
   // Cleanup VBO and shader
   glDeleteProgram(m_program_id);
   glDeleteVertexArrays(1, &m_vertex_array_id);
}

Gfx::Gfx(std::vector<LED>& led_coordinates, std::vector<Triangle>& triangles, const Config& config):
    m_i(*new Gfx::Impl(led_coordinates, triangles, config))
{
}

bool Gfx::draw()
{
   return m_i.draw();
}

void Gfx::registerForceBlank(bool& force_blank)
{
   m_i.registerForceBlank(force_blank);
}

Gfx::~Gfx()
{
   delete &m_i;
}

}
