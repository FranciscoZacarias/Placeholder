LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
    case WM_SETCURSOR:
      if (LOWORD(lParam) == HTCLIENT) {
        win32_set_cursor(CURSOR_ARROW);
        return TRUE;
      } break;

    case WM_SIZE: {
      _win32_window_resize_callback(LOWORD(lParam), HIWORD(lParam));
      return 0;
    } break;

    // Keyboard keys
    case WM_KEYDOWN: {
      _input_process_keyboard_key((Keyboard_Key)wParam, TRUE);
      return 0;
    } break;
    case WM_KEYUP: {
      _input_process_keyboard_key((Keyboard_Key)wParam, FALSE);
      return 0;
    } break;

    // Mouse Cursor
    case WM_MOUSEMOVE: {
      if (_IgnoreNextMouseMove) {
        _IgnoreNextMouseMove = false;
        return 0;
      }
      s32 x = LOWORD(lParam);
      s32 y = HIWORD(lParam);
      _input_process_mouse_cursor((f32)x, (f32)y);
      return 0;
    } break;
    
    // Mouse Buttons
    case WM_LBUTTONDOWN: {
      _input_process_mouse_button(MouseButton_Left, TRUE);
      return 0;
    } break;
    case WM_LBUTTONUP: {
      _input_process_mouse_button(MouseButton_Left, FALSE);
      return 0;
    } break;
    case WM_RBUTTONDOWN: {
      _input_process_mouse_button(MouseButton_Right, TRUE);
      return 0;
    } break;
    case WM_RBUTTONUP: {
      _input_process_mouse_button(MouseButton_Right, FALSE);
      return 0;
    } break;
    case WM_MBUTTONDOWN: {
      _input_process_mouse_button(MouseButton_Middle, TRUE);
      return 0;
    } break;
    case WM_MBUTTONUP: {
      _input_process_mouse_button(MouseButton_Middle, FALSE);
      return 0;
    } break;

    case WM_DESTROY: {
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(_RenderingContextHandle);
      ReleaseDC(hWnd, _DeviceContextHandle);
      PostQuitMessage(0);
      return 0;
    } break;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  _hInstance = hInstance;
  entry_point();
  return _ApplicationReturn;
}

internal void _win32_window_resize_callback(s32 width, s32 height) {
  if (height == 0) { 
    height = 1;
  }
  if (width == 0) {
    width = 1;
  }
  WindowWidth  = width;
  WindowHeight = height;
  if (_IsOpenGLContextEnabled) {
    glViewport(0, 0, width, height);
  }
}

internal void win32_timer_init() {
  AssertNoReentry();
  QueryPerformanceFrequency(&_PerformanceFrequency);
}

internal void win32_timer_start(PerformanceTimer* timer) {
  QueryPerformanceCounter(&timer->start);
}

internal void win32_timer_end(PerformanceTimer* timer) {
  QueryPerformanceCounter(&timer->end);
  LONGLONG difference = timer->end.QuadPart - timer->start.QuadPart;
  timer->elapsed_seconds = (f32)difference / (f32)_PerformanceFrequency.QuadPart;
}

internal HWND _win32_window_create(HINSTANCE hInstance, s32 width, s32 height) {
  HWND result = {0};

  WNDCLASS wc      = {0};
  wc.lpfnWndProc   = WndProc;
  wc.hInstance     = hInstance;
  wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
  wc.lpszClassName = "OpenGLWindow";
    
  RegisterClass(&wc);
    
#ifdef FZ_WINDOW_NAME
  LPCSTR app_name = FZ_WINDOW_NAME;
#else
  LPCSTR app_name = "f_program";
#endif

  result = CreateWindow("OpenGLWindow", app_name,
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                        NULL, NULL, hInstance, NULL);
  WindowWidth  = width;
  WindowHeight = height;
  return result;
}

internal void win32_set_cursor(CursorType cursor) {
  HCURSOR hCursor = NULL;

  switch (cursor) {
    case CURSOR_ARROW: {
      hCursor = LoadCursor(NULL, IDC_ARROW);
      break;
    }
    case CURSOR_HAND: {
      hCursor = LoadCursor(NULL, IDC_HAND);
      break;
    }
    case CURSOR_CROSSHAIR: {
      hCursor = LoadCursor(NULL, IDC_CROSS);
      break;
    }
    case CURSOR_IBEAM: {
      hCursor = LoadCursor(NULL, IDC_IBEAM);
      break;
    }
    case CURSOR_WAIT: { 
      hCursor = LoadCursor(NULL, IDC_WAIT);
      break;
    }
    case CURSOR_SIZE_ALL: {
      hCursor = LoadCursor(NULL, IDC_SIZEALL);
      break;
    }
    default: {
      hCursor = LoadCursor(NULL, IDC_ARROW);
      break;
    }
  }

  if (hCursor) {
    SetCursor(hCursor);
  }
}

internal void win32_set_cursor_position(s32 x, s32 y) {
  SetCursorPos(x, y);
}

internal void win32_lock_cursor(b32 lock) {
  if (lock) {
    RECT rect;
    GetClientRect(_WindowHandle, &rect);
    POINT center = {(rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2};
    ClientToScreen(_WindowHandle, &center);
    SetCursorPos(center.x, center.y);

    _IsCursorLocked      = true;
    _IgnoreNextMouseMove = true;

    // Reset deltas to avoid cursor jump
    _InputState.mouse_current.delta_x  = 0.0f;
    _InputState.mouse_current.delta_y  = 0.0f;
    _InputState.mouse_previous.delta_x = 0.0f;
    _InputState.mouse_previous.delta_y = 0.0f;
    MemoryCopyStruct(&_InputState.mouse_previous, &_InputState.mouse_current);
  } else {
    _IsCursorLocked = false;
  }
}

internal void win32_hide_cursor(b32 hide) {
  // Win32 quirk. It has an internal counter required to show the cursor.
  // The while loops just make sure it exhausts the counter and applies immediately.
  while (ShowCursor(hide ? FALSE : TRUE) >= 0 &&  hide);
  while (ShowCursor(hide ? FALSE : TRUE) < 0  && !hide);
}

internal void win32_put_pixel(s32 x, s32 y, COLORREF color) {
  if (_IsOpenGLContextEnabled) {
    ERROR_MESSAGE_AND_EXIT("Called win32_put_pixel with opengl context attached");
  }
  if (!_IsWindowEnabled) {
    ERROR_MESSAGE_AND_EXIT("Called win32_put_pixel win32_enable_window");
  }
  SetPixel(_DeviceContextHandle, x, y, color);
}

internal f32 win32_get_elapsed_time() {
  LARGE_INTEGER current;
  QueryPerformanceCounter(&current);
  LONGLONG ticks = current.QuadPart - _Timer_ElapsedTime.start.QuadPart;
  return (f32)ticks / (f32)_PerformanceFrequency.QuadPart;
}

internal f32 win32_get_frame_time() {
  return _Timer_FrameTime.elapsed_seconds;
}

internal b32 win32_enable_opengl() {
  b32 result = true;

  if (!_IsWindowEnabled) {
    return false;
  }

  _DeviceContextHandle = GetDC(_WindowHandle);
  if (!_DeviceContextHandle) {
    ERROR_MESSAGE_AND_EXIT("Failed to get device context\n");
    return false;
  }

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), 1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 16, 0, 0,
    PFD_MAIN_PLANE, 0, 0, 0, 0
  };

  s32 pixelFormat = ChoosePixelFormat(_DeviceContextHandle, &pfd);
  if (!pixelFormat) {
    ERROR_MESSAGE_AND_EXIT("Failed to choose pixel format\n");
    return false;
  }
  if (!SetPixelFormat(_DeviceContextHandle, pixelFormat, &pfd)) {
    ERROR_MESSAGE_AND_EXIT("Failed to set pixel format\n");
    return false;
  }

  HGLRC tempRC = wglCreateContext(_DeviceContextHandle);
  if (!tempRC) {
    ERROR_MESSAGE_AND_EXIT("Failed to create temporary context\n");
    return false;
  }
  if (!wglMakeCurrent(_DeviceContextHandle, tempRC)) {
    ERROR_MESSAGE_AND_EXIT("Failed to make temporary context current\n");
    return false;
  }

  typedef HGLRC (WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

  if (!wglCreateContextAttribsARB) {
    ERROR_MESSAGE_AND_EXIT("wglCreateContextAttribsARB not supported\n");
    wglDeleteContext(tempRC);
    return false;
  }

  s32 attribs[] = {
    0x2091, 4,      // WGL_CONTEXT_MAJOR_VERSION_ARB
    0x2092, 6,      // WGL_CONTEXT_MINOR_VERSION_ARB
    0x2094, 0x9126, // WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB
    0 // End
  };

  _RenderingContextHandle = wglCreateContextAttribsARB(_DeviceContextHandle, NULL, attribs);
  if (!_RenderingContextHandle) {
    ERROR_MESSAGE_AND_EXIT("Failed to create OpenGL 4.6 context\n");
    wglDeleteContext(tempRC);
    return false;
  }

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(tempRC);
  if (!wglMakeCurrent(_DeviceContextHandle, _RenderingContextHandle)) {
    ERROR_MESSAGE_AND_EXIT("Failed to make OpenGL 4.6 context current\n");
    return false;
  }

  if (!gladLoadGL()) {
    ERROR_MESSAGE_AND_EXIT("Failed to initialize GLAD\n");
    return false;
  }

  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  _IsOpenGLContextEnabled = true;
  return result;
}

internal void win32_show_window(b32 show_window) {
  ShowWindow(_WindowHandle, show_window ? SW_SHOW : SW_HIDE);
  UpdateWindow(_WindowHandle);
}

internal void win32_init() {
  win32_timer_init();
  win32_timer_start(&_Timer_ElapsedTime);

  thread_context_init_and_attach(&MainThreadContext);
  win32_timer_start(&_Timer_FrameTime);
}

internal b32 win32_enable_console() {
  BOOL result = AllocConsole();
  if (!result) {
    return false;
  }
  FILE* fp;
  freopen_s(&fp, "CONOUT$", "w", stdout);
  freopen_s(&fp, "CONOUT$", "w", stderr);
  _IsTerminalEnabled = true;
  return true;
}

internal b32 win32_enable_window() {
  _input_init();

  _WindowHandle = _win32_window_create(_hInstance, WindowWidth, WindowHeight);
  if (!_WindowHandle) {
    ERROR_MESSAGE_AND_EXIT("Failed to get window handle\n");
    return false;
  }

  _DeviceContextHandle = GetDC(_WindowHandle);
  if (!_DeviceContextHandle) {
      ERROR_MESSAGE_AND_EXIT("Failed to get device context\n");
      return false;
  }

  _IsWindowEnabled = true;
  return true;
}

internal void win32_application_stop() {
  IsApplicationRunning = false;
}

internal b32  win32_application_is_running() {
  win32_timer_end(&_Timer_FrameTime);
  win32_timer_start(&_Timer_FrameTime);

  MSG msg = {0};
  if (_IsWindowEnabled) {
    _input_update();
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT)  IsApplicationRunning = false;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  if (!IsApplicationRunning) {
    _ApplicationReturn = (s32)msg.wParam;
    return false;
  }

  return true;
}
