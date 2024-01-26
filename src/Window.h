#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>

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
  };
}
