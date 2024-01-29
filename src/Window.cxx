#include "Window.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include "Exception.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "loguru.hpp"

namespace Mapper {
  Window::Window(const char* title, uint16_t width, uint16_t height)
  {
    if(!glfwInit())
    {
     LOG_F(ERROR, "GLFW library cannot be initialized");
     throw Exception();
    }

    glfwWindowHint(GLFW_SAMPLES, 0); //Disable AA since it's not needed
    
    pWnd = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!pWnd)
    {
      LOG_F(ERROR, "Cannot create window");
      throw Exception();
    }

    glfwMakeContextCurrent(pWnd);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplGlfw_InitForOpenGL(pWnd, true);
    ImGui_ImplOpenGL3_Init();

    LOG_F(INFO, "ImGui initialized and ready to go");  
  }

  Window::~Window()
  {
    glfwDestroyWindow(pWnd);
    glfwTerminate();
  }

  bool Window::ProcessEvent()
  {
    if(glfwWindowShouldClose(pWnd)) return true;

    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if(ImGui::Begin("Modbus Mapper"))
    {
      static char ip[15] = "192.168.3.112";
      static char port[5] = "3301";
      static char name[256] = "TEST";
      ImGui::InputTextWithHint("IP", "IP address of your device", ip, IM_ARRAYSIZE(ip));
      ImGui::InputTextWithHint("Port", "Network port of your device", port, IM_ARRAYSIZE(port));
      ImGui::InputTextWithHint("Name", "Name of your client interface", name, IM_ARRAYSIZE(name));
      if(ImGui::Button("Make Client"))
      {
        cmd.CreateClient(ip, std::stoi(port), name);
      }
      ImGui::End();
    }

    if(ImGui::Begin("Client Map"))
    {
      if(!cmd.mClientMap.empty())
      {
        for(auto client : cmd.mClientMap)
        {
          if(ImGui::CollapsingHeader(client.first.c_str()))
          {
            if(ImGui::Button("Test Connection"))
            {
              cmd.ConnectClient(client.first.c_str());
            }
          }
        }
      }
      else {
        ImGui::Text("No clients avaliable");
      }
      ImGui::End();
    }
    ImGui::Render() ;
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(pWnd);
    return false;
  }
}
