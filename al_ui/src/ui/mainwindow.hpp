
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <imgui/imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include <GL/gl3w.h>
// #define SDL_MAIN_HANDLED
#include <SDL.h>

#include "manager.hpp"

class MainWindow {
public:
  MainWindow();
  ~MainWindow();

  void init();
  void run();

private:
  void _render();

  void _handleWindowEvents();
  // drawing sections
  void _drawDeviceList();
  void _drawRoomSelectDialog();
  void _drawSetTestSourceDialog();
  
  bool m_showRoomSelectDialog;

  SDL_GLContext m_glcontext;
  SDL_Window *m_window;

  int m_mouseButtonPressed;
  bool m_done;
  bool m_showTestWindow;
  ImVec4 m_clearColor;

  int MAX_ROOM_NAME_SIZE;
  char *m_room;

  int m_winWidth;
  int m_winHeight;

  SceneRenderer *m_sceneRenderer;
  Manager *m_manager;
};

#endif // MAINWINDOW_H
