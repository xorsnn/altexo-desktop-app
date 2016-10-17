
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <imgui/imgui.h>

#include "imgui_impl_sdl_gl3.h"
#include <GL/gl3w.h>
// #define SDL_MAIN_HANDLED
#include <SDL.h>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>

#include "manager.hpp"

using namespace boost::log::trivial;

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
  void _drawContactList();
  void _drawRoomSelectDialog();

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

  boost::log::sources::severity_logger<severity_level> m_lg;
};

#endif // MAINWINDOW_H
