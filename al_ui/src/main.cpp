// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top
// of imgui.cpp.

// #include "allog.hpp"
#include "hologramrenderer.hpp"
#include "imgui/imgui.h"
#include "imgui_impl_sdl.h"
#include "manager.hpp"
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <stdio.h>

#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*_ARR)))

int main(int, char **) {
  // TODO move it to stored settings
  int winWidth = 1280;
  int winHeight = 720;

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Setup window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  SDL_Window *window = SDL_CreateWindow(
      "Altexo holographic chat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      winWidth, winHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  // Opengl

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
  }

  // Use Vsync
  if (SDL_GL_SetSwapInterval(1) < 0) {
    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
  }

  // // Initialize OpenGL
  // if (!initGL()) {
  //   printf("Unable to initialize OpenGL!\n");
  //   // success = false;
  // }
  // ~ opengl

  // Setup ImGui binding
  ImGui_ImplSdl_Init(window);

  // local init
  HologramRenderer hologramRenderer(winWidth, winHeight);
  hologramRenderer.init();
  Manager manager;
  manager.initHoloRenderer(&hologramRenderer);
  manager.initSensor(&(hologramRenderer.m_sensorDataFboRenderer));
  manager.initSdk();
  manager.initWsConnection(&manager);
  // ~ local init

  // Load Fonts
  // (there is a default font, this is only if you want to change it. see
  // extra_fonts/README.txt for more details)
  // ImGuiIO& io = ImGui::GetIO();
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf",
  // 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf",
  // 13.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf",
  // 10.0f);
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese());

  bool show_test_window = true;

  ImVec4 clear_color = ImColor(114, 144, 154);

  bool mouseDown = false;
  // Main loop
  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSdl_ProcessEvent(&event);
      switch (event.type) {
      case SDL_MOUSEMOTION: {
        if (mouseDown) {
          hologramRenderer.OnMouseMove(event.motion.x, event.motion.y);
        }
      } break;
      case SDL_MOUSEBUTTONDOWN: {
        hologramRenderer.OnStartMouseMove(event.button.x, event.button.y);
        mouseDown = true;
      } break;
      case SDL_MOUSEBUTTONUP: {
        mouseDown = false;
      } break;
      case SDL_WINDOWEVENT: {
        switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          hologramRenderer.onWinResize((int)ImGui::GetIO().DisplaySize.x,
                                       (int)ImGui::GetIO().DisplaySize.y);
        } break;
        default: { } break; }
      } break;
      case SDL_QUIT: {
        done = true;
      } break;
      default: { } break; }
    }

    ImGui_ImplSdl_NewFrame(window);

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears
    // in
    // a window automatically called "Debug"
    {
      ImGui::SetNextWindowSize(ImVec2(100, 400), ImGuiSetCond_Once);
      ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
      ImGui::Begin("Contacts", NULL);
      static int selected = 0;
      ImGui::BeginChild("left pane", ImVec2(-1, 0), true);
      for (int i = 0; i < manager.contactList.size(); i++) {
        std::string lb =
            manager.contactList[i].name + "-" + manager.contactList[i].id;
        // const char *label = manager.contactList[i].name.c_str();
        const char *label = lb.c_str();
        if (ImGui::Selectable(label, selected == i)) {
          selected = i;
          manager.callToPeer(manager.contactList[i].id);
        }
      }
      ImGui::EndChild();
      ImGui::End();
    }

    {
      ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiSetCond_Once);
      ImGui::SetNextWindowPos(ImVec2(10, 420), ImGuiSetCond_Once);
      ImGui::Begin("Input", NULL);
      static int selected = 0;
      ImGui::BeginChild("webcam list", ImVec2(-1, 50), true);
      for (int i = 0; i < manager.webcamList.size(); i++) {
        std::string devName = manager.webcamList[i].getText();
        const char *label = devName.c_str();
        if (ImGui::Selectable(label, selected == i)) {
          selected = i;
          manager.setDeviceName(manager.webcamList[i],
                                AlSdkAPI::DesiredVideoSource::CAMERA);
          std::cout << selected << std::endl;
        }
      }
      ImGui::EndChild();
      ImGui::BeginChild("sensor list", ImVec2(-1, 50), true);
      for (int i = 0; i < manager.sensorList.size(); i++) {
        std::string devName = manager.sensorList[i].getText();
        const char *label = devName.c_str();
        if (ImGui::Selectable(label, selected == i)) {
          selected = i;
          manager.setDeviceName(manager.sensorList[i],
                                AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS);
          std::cout << selected << std::endl;
        }
      }
      ImGui::EndChild();
      ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in
    // ImGui::ShowTestWindow()
    if (show_test_window) {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ImGui::ShowTestWindow(&show_test_window);
    }

    // Rendering

    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x,
               (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    hologramRenderer.render();

    // has to be called once
    if (!hologramRenderer.sendingFrames && manager.connectionInitialized &&
        manager.getDeviceType() ==
            AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS) {
      hologramRenderer.initFrameSending(manager.m_sdk.get());
    }

    ImGui::Render();
    SDL_GL_SwapWindow(window);
    SDL_Delay(1000 / 30);
  }

  // Cleanup
  ImGui_ImplSdl_Shutdown();
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
