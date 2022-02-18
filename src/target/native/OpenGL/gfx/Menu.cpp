#include "Menu.hpp"
#ifdef WITH_GLEW
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include <glm/glm.hpp>

extern void drawProgram();

using namespace glm;

namespace gfx {

Menu::Menu(Config& config, Window& window):
   m_config(config),
   m_window(window)
{
   // Setup Dear ImGui context
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();

   // Setup Dear ImGui style
   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(m_window.get(), true);
   ImGui_ImplOpenGL3_Init("#version 150");
}

Menu::~Menu()
{
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();
}

void Menu::draw()
{
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();

   if (m_config.identifyTriangles()) 
   {
      drawLabels();
   }

   ImGui::SetNextWindowBgAlpha(0.6f);

   ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
   
   if (m_config.menu())
   {
      m_config.menu()->drawMenu();
   }
   
   ImGui::Separator();
   
   ImGui::Checkbox("Identify Triangles", &m_config.identifyTriangles());

   if (ImGui::TreeNode("Shader"))
   {
      ImGui::SliderFloat("Cutoff Distance", &m_config.m_cutoff_distance, 0.0f, 10.0f);
      ImGui::Text("Attenuation:");
      ImGui::SliderFloat("Constant", &m_config.m_attenuation_constant, 0.0f, 10.0f);
      ImGui::SliderFloat("Linear", &m_config.m_attenuation_linear, 0.0f, 300.0f);
      ImGui::SliderFloat("Square", &m_config.m_attenuation_square, 0.0f, 1000.0f);
      ImGui::TreePop();
   }
   ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
   ImGui::End();

   //ImGui::ShowDemoWindow();

   ImGui::Render();
   int display_w, display_h;
   glfwGetFramebufferSize(m_window.get(), &display_w, &display_h);
   glViewport(0, 0, display_w, display_h);
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::drawLabels()
{
   ImGuiWindowFlags window_flags = 
      ImGuiWindowFlags_NoDecoration | 
      ImGuiWindowFlags_AlwaysAutoResize | 
      ImGuiWindowFlags_NoSavedSettings | 
      ImGuiWindowFlags_NoFocusOnAppearing | 
      ImGuiWindowFlags_NoNav | 
      ImGuiWindowFlags_NoMouseInputs |
      ImGuiWindowFlags_NoMove;
   const ImGuiViewport* viewport = ImGui::GetMainViewport();
   ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
   //ImVec2 work_size = viewport->WorkSize;
   ImVec2 window_pos_pivot;
   window_pos_pivot.x = 0.0f;
   window_pos_pivot.y = 0.0f;
   int display_w, display_h;
   glfwGetFramebufferSize(m_window.get(), &display_w, &display_h);

   ImGui::SetNextWindowPos(work_pos, ImGuiCond_Always, window_pos_pivot);
   ImGui::SetNextWindowBgAlpha(0.f);
   ImGui::SetNextWindowSize(ImVec2(display_w, display_h));
   ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
   
   ImGui::Begin("Labels overlay", nullptr, window_flags);
   {
      for (auto l: m_config.labels3D())
      {
         const vec3 position(l.x, l.y, l.z);
         const vec4 clipSpacePos = m_window.getProjectionMatrix() * (m_window.getViewMatrix() * vec4(position, 1.0));

         if (clipSpacePos.z < 0) continue; // in front of screen

         const vec2 windowSpacePos = vec2((clipSpacePos.x / clipSpacePos.w + 1.0) / 2.0, (clipSpacePos.y / clipSpacePos.w + 1.0) / 2.0) * vec2(display_w, display_h);

         if (windowSpacePos.x < 0 or windowSpacePos.x > display_w or
             windowSpacePos.y < 0 or windowSpacePos.y > display_h) continue; // off screen

         const ImVec2 label_size1 = ImGui::CalcTextSize(l.line1.c_str(), NULL, true);
         const ImVec2 label_size2 = ImGui::CalcTextSize(l.line2.c_str(), NULL, true);
         ImGui::SetCursorPosX(windowSpacePos.x - label_size1.x/2);
         ImGui::SetCursorPosY(display_h - windowSpacePos.y - label_size1.y);
         ImGui::Text("%s", l.line1.c_str());
         ImGui::SetCursorPosX(windowSpacePos.x - label_size2.x/2);
         ImGui::SetCursorPosY(display_h - windowSpacePos.y);
         ImGui::Text("%s", l.line2.c_str());
      }
   }
   ImGui::End();
   ImGui::PopStyleVar();
}

}
