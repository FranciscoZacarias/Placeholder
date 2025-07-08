#include "main.h"

internal void entry_point() {

  win32_init();
  set_error_log_file(Str8("./Placeholder/error.log"));
  win32_enable_window(1280, 720);
  win32_enable_console();
  win32_enable_opengl();
  win32_show_window(true);

  camera_init(&GlobalCamera, vec3f32(0.0f, 5.0f, 10.0f), vec3f32(0.0f, 0.0f, 0.0f), 8.0f, 1.0f);
  renderer_init();

  Mat4f32 view;
  Mat4f32 projection;
  while (win32_application_is_running()) {
    input_update();
    camera_update(&GlobalCamera, 0.0f);

    renderer_begin_frame();
	{
      view       = camera_get_view_matrix(&GlobalCamera);
      projection = mat4f32_perspective(GlobalCamera.fov, (f32)WindowWidth, (f32)WindowHeight, 0.1f, 100.0f);

      renderer_draw_quad(vec2f32(-0.2f, -0.2f), vec2f32(0.2f, 0.2f), Color_Red);
	}
    renderer_end_frame(view, projection);
  }
}

internal void input_update() {
  if (input_is_key_pressed(KeyboardKey_ESCAPE)) {
    application_stop();
  }
}

