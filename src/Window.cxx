#include "Window.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include "Exception.h"
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
    glfwSwapBuffers(pWnd);
    return false;
  }
}
