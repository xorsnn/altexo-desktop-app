#include "mainwindow.hpp"
// #include "allogger.hpp"
#include "alsettings.hpp"

MainWindow::MainWindow()
    : m_glcontext(NULL), m_window(NULL), m_mouseButtonPressed(-1),
      m_done(false), m_showTestWindow(true), m_showRoomSelectDialog(true),
      MAX_ROOM_NAME_SIZE(256), m_room(NULL) {

  // m_lg = al_logger::get();
  // m_room = new char[256];
  m_room = (char *)malloc(MAX_ROOM_NAME_SIZE);

  AlSettings st;
  std::string roomStr = st.get("last_used_room");
  std::copy(roomStr.begin(), roomStr.end(), m_room);
  m_room[roomStr.size()] = '\0';

  // TODO move it to stored settings
  m_winWidth = 1280;
  m_winHeight = 720;

  m_clearColor = ImColor(114, 144, 154);

  m_sceneRenderer = new SceneRenderer(m_winWidth, m_winHeight);
  m_manager = new Manager();
}

MainWindow::~MainWindow() {

  // delete[] m_room;
  free(m_room);

  if (m_sceneRenderer != NULL) {
    delete m_sceneRenderer;
  }
  if (m_manager != NULL) {
    delete m_manager;
  }
  // Cleanup
  ImGui_ImplSdlGL3_Shutdown();
  SDL_GL_DeleteContext(m_glcontext);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void MainWindow::init() {

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    return;
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
  m_window = SDL_CreateWindow("Altexo holographic chat", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, m_winWidth, m_winHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  m_glcontext = SDL_GL_CreateContext(m_window);
  gl3wInit();

  // Setup ImGui binding
  ImGui_ImplSdlGL3_Init(m_window);

  // local init
  m_sceneRenderer->init();

  m_manager->initHoloRenderer(m_sceneRenderer);
  // m_manager->initSensor(&(m_sceneRenderer.m_sensorDataFboRenderer));
  m_manager->initSdk();

#ifndef _WIN32
  m_manager->initWsConnection(m_manager);
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
}

void MainWindow::run() {
  // Main loop
  m_done = false;
  while (!m_done) {
    _render();
    // NOTE: redraw delay
    SDL_Delay(1000 / 30);
  }
}

void MainWindow::_render() {
  _handleWindowEvents();

  ImGui_ImplSdlGL3_NewFrame(m_window);

  _drawDeviceList();
  if (m_showRoomSelectDialog) {
    _drawRoomSelectDialog();
  }

  /*
   * NOTE: draw this only for debug
   */
  _drawSetTestSourceDialog();

  // 3. Show the ImGui test window. Most of the sample code is in
  // ImGui::ShowTestWindow()
  if (m_showTestWindow) {
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
    ImGui::ShowTestWindow(&m_showTestWindow);
  }

  // Rendering

  glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x,
             (int)ImGui::GetIO().DisplaySize.y);
  glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
  glClear(GL_COLOR_BUFFER_BIT);

  // RENDER SCENE
  m_sceneRenderer->render();
  // has to be called once
  if (!m_sceneRenderer->sendingFrames && m_manager->connectionInitialized &&
      m_manager->getDeviceType() ==
          AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS) {
    alLogger() << "======================================";
    alLogger() << "======================================";
    alLogger() << "AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS";
    alLogger() << "======================================";
    alLogger() << "======================================";
    m_sceneRenderer->initFrameSending(m_manager->m_sdk.get());
  }

  ImGui::Render();
  SDL_GL_SwapWindow(m_window);
}

void MainWindow::_handleWindowEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSdlGL3_ProcessEvent(&event);
    switch (event.type) {
    case SDL_MOUSEMOTION: {
      switch (m_mouseButtonPressed) {
      case SDL_BUTTON_LEFT: {
        m_sceneRenderer->onRotate(event.motion.x, event.motion.y);
      } break;
      case SDL_BUTTON_RIGHT: {
        m_sceneRenderer->onPan(event.motion.x, event.motion.y);
      }
      default: { } break; }
    } break;

    case SDL_MOUSEBUTTONDOWN: {
      m_mouseButtonPressed = int(event.button.button);
      m_sceneRenderer->OnStartMouseMove(event.button.x, event.button.y);
    } break;

    case SDL_MOUSEBUTTONUP: {
      m_mouseButtonPressed = -1;
    } break;

    case SDL_MOUSEWHEEL: {
      m_sceneRenderer->onZoom(event.wheel.y);
    } break;

    case SDL_WINDOWEVENT: {
      switch (event.window.event) {
      case SDL_WINDOWEVENT_RESIZED: {
        m_sceneRenderer->onWinResize((int)ImGui::GetIO().DisplaySize.x,
                                     (int)ImGui::GetIO().DisplaySize.y);
      } break;
      default: { } break; }
    } break;

    case SDL_QUIT: {
      m_done = true;
    } break;

    default: { } break; }
  }
}

void MainWindow::_drawDeviceList() {
  ImGui::SetNextWindowSize(ImVec2(100, 400), ImGuiSetCond_Once);
  ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
  ImGui::Begin("Input", NULL);
  static int selected = 0;
  ImGui::BeginChild("webcam list", ImVec2(-1, 180), true);
  for (int i = 0; i < m_manager->webcamList.size(); i++) {
    std::string devName = AlTextMessage::msgToString(m_manager->webcamList[i]);
    const char *label = devName.c_str();
    if (ImGui::Selectable(label, selected == i)) {
      selected = i;
      m_manager->setDeviceName(m_manager->webcamList[i],
                               AlSdkAPI::DesiredVideoSource::CAMERA);
      std::cout << selected << std::endl;
    }
  }
  ImGui::EndChild();
  ImGui::BeginChild("sensor list", ImVec2(-1, 180), true);
  for (int i = 0; i < m_manager->sensorList.size(); i++) {
    std::string devName = AlTextMessage::msgToString(m_manager->sensorList[i]);
    const char *label = devName.c_str();
    if (ImGui::Selectable(label, selected == i)) {
      selected = i;
      m_manager->setDeviceName(m_manager->sensorList[i],
                               AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS);
      // TODO: be sure init once
      m_manager->initSensor(&(m_sceneRenderer->m_sensorDataFboRenderer),
                            Manager::KINECT_1);
    }
  }
  ImGui::EndChild();
  ImGui::End();
}

void MainWindow::_drawRoomSelectDialog() {
  int dialogWidth = 200;
  int dialogHeight = 100;
  ImGui::SetNextWindowSize(ImVec2(dialogWidth, dialogHeight),
                           ImGuiSetCond_Once);
  ImGui::SetNextWindowPos(ImVec2(m_winWidth / 2 - dialogWidth / 2,
                                 m_winHeight / 2 - dialogHeight / 2),
                          ImGuiSetCond_Once);
  ImGui::Begin("Enter room name", NULL,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
  ImGui::BeginChild("left pane", ImVec2(-1, 0), true);
  ImGui::InputText("room", m_room, MAX_ROOM_NAME_SIZE);
  if (ImGui::Button("enter")) {
    AlSettings st;
    std::string roomStr(m_room);
    st.set("last_used_room", roomStr);
    // TODO: run entering room
    m_manager->m_wsClient.get()->roomOpen(AlTextMessage(roomStr));
    m_showRoomSelectDialog = false;
  }
  ImGui::EndChild();
  ImGui::End();
}

void MainWindow::_drawSetTestSourceDialog() {
  int dialogWidth = 200;
  int dialogHeight = 100;
  ImGui::SetNextWindowSize(ImVec2(dialogWidth, dialogHeight),
                           ImGuiSetCond_Once);
  ImGui::SetNextWindowPos(ImVec2(m_winWidth / 2 - dialogWidth / 2,
                                 m_winHeight / 2 - dialogHeight / 2 + 300),
                          ImGuiSetCond_Once);
  ImGui::Begin("Set test source", NULL,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
  ImGui::BeginChild("left pane", ImVec2(-1, 0), true);
  if (ImGui::Button("set test source")) {
    m_manager->setDeviceName(AlTextMessage::stringToMsg("test_source"),
                             AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS);
    // TODO: be sure init once
    m_manager->initSensor(&(m_sceneRenderer->m_sensorDataFboRenderer),
                          Manager::FAKE_SENSOR);
  }
  ImGui::EndChild();
  ImGui::End();
}
