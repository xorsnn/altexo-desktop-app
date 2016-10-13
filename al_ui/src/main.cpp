
#include <imgui/imgui.h>

#include "imgui_impl_sdl_gl3.h"
#include <GL/gl3w.h>
// #define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include <stdio.h>

#include "manager.hpp"

int main(int, char **) {
  // TODO move it to stored settings
  int winWidth = 1280;
  int winHeight = 720;

  std::cout << "< 1" << std::endl;

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Setup window
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  SDL_Window *window = SDL_CreateWindow(
      "Altexo holographic chat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      winWidth, winHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
  gl3wInit();

  // Setup ImGui binding
  ImGui_ImplSdlGL3_Init(window);

  // local init
  SceneRenderer sceneRenderer(winWidth, winHeight);
  sceneRenderer.init();
  Manager manager;
  manager.initHoloRenderer(&sceneRenderer);
  // manager.initSensor(&(sceneRenderer.m_sensorDataFboRenderer));
  manager.initSdk();

#ifndef _WIN32
  manager.initWsConnection(&manager);
#endif
  // ~ local init

  // Load Fonts
  // (there is a default font, this is only if you want to change it. see
  // extra_fonts/README.txt for more details)
  // ImGuiIO& io = ImGui::GetIO();
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf",
  // 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese());

  std::cout << glGetString(GL_VERSION) << std::endl;

  bool show_test_window = true;

  ImVec4 clear_color = ImColor(114, 144, 154);

  int mouseButtonPressed = -1; // -1 - not pressed
  // Main loop
  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSdlGL3_ProcessEvent(&event);
      switch (event.type) {
      case SDL_MOUSEMOTION: {
        switch (mouseButtonPressed) {
        case SDL_BUTTON_LEFT: {
          sceneRenderer.onRotate(event.motion.x, event.motion.y);
        } break;
        case SDL_BUTTON_RIGHT: {
          sceneRenderer.onPan(event.motion.x, event.motion.y);
        }
        default: { } break; }
      } break;

      case SDL_MOUSEBUTTONDOWN: {
        mouseButtonPressed = int(event.button.button);
        sceneRenderer.OnStartMouseMove(event.button.x, event.button.y);
      } break;

      case SDL_MOUSEBUTTONUP: {
        mouseButtonPressed = -1;
      } break;

      case SDL_MOUSEWHEEL: {
        sceneRenderer.onZoom(event.wheel.y);
      } break;

      case SDL_WINDOWEVENT: {
        switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          sceneRenderer.onWinResize((int)ImGui::GetIO().DisplaySize.x,
                                    (int)ImGui::GetIO().DisplaySize.y);
        } break;
        default: { } break; }
      } break;

      case SDL_QUIT: {
        done = true;
      } break;

      default: { } break; }
    }

    ImGui_ImplSdlGL3_NewFrame(window);

    // TODO reimplement contact list later
    // // 1. Show a simple window
    // // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears
    // // in
    // // a window automatically called "Debug"
    // {
    //   ImGui::SetNextWindowSize(ImVec2(100, 400), ImGuiSetCond_Once);
    //   ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
    //   ImGui::Begin("Contacts", NULL);
    //   static int selected = 0;
    //   ImGui::BeginChild("left pane", ImVec2(-1, 0), true);
    //   for (int i = 0; i < manager.contactList.size(); i++) {
    //     std::string lb =
    //         manager.contactList[i].name + "-" + manager.contactList[i].id;
    //     // const char *label = manager.contactList[i].name.c_str();
    //     const char *label = lb.c_str();
    //     if (ImGui::Selectable(label, selected == i)) {
    //       selected = i;
    //       manager.callToPeer(manager.contactList[i].id);
    //     }
    //   }
    //   ImGui::EndChild();
    //   ImGui::End();
    // }

    {
      ImGui::SetNextWindowSize(ImVec2(100, 400), ImGuiSetCond_Once);
      ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
      ImGui::Begin("Input", NULL);
      static int selected = 0;
      ImGui::BeginChild("webcam list", ImVec2(-1, 180), true);
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
      ImGui::BeginChild("sensor list", ImVec2(-1, 180), true);
      for (int i = 0; i < manager.sensorList.size(); i++) {
        std::string devName = manager.sensorList[i].getText();
        const char *label = devName.c_str();
        if (ImGui::Selectable(label, selected == i)) {
          selected = i;
          manager.setDeviceName(manager.sensorList[i],
                                AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS);
          // TODO: be sure init once
          manager.initSensor(&(sceneRenderer.m_sensorDataFboRenderer));
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

    // RENDER SCENE
    sceneRenderer.render();
    // has to be called once
    if (!sceneRenderer.sendingFrames && manager.connectionInitialized &&
        manager.getDeviceType() ==
            AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS) {
      sceneRenderer.initFrameSending(manager.m_sdk.get());
    }

    ImGui::Render();
    SDL_GL_SwapWindow(window);

    // NOTE: redraw delay
    SDL_Delay(1000 / 30);
  }

  // Cleanup
  ImGui_ImplSdlGL3_Shutdown();
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  std::cout << "FIN!" << std::endl;
  return 0;
}
