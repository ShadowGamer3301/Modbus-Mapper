#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "mb_cmdline.h"

namespace Mapper
{
  class Window 
  {
  public:
    Window(const char* title, uint16_t width, uint16_t height);
    ~Window();

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;

    bool ProcessEvent();

  private:
    GLFWwindow* pWnd;
    Mapper::MB_CmdLine cmd;
  };
}
