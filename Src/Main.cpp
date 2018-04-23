/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include <iostream>

extern bool hasQuitRequest;
void initialize();
void application();

/**
* プログラムのエントリーポイント.
*/
int main()
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (!window.Init(800, 600, "OpenGL 2D")) {
    return 1;
  }

  initialize();
  
  while (!hasQuitRequest) {
    application();
  }
  std::cout << "Finish." << std::endl;
}