#include "main.h"

internal void entry_point() {

  win32_init();
  win32_enable_window();
  win32_enable_opengl();
  win32_show_window(true);

  camera_init(&GlobalCamera, vec3f32(0.0f, 5.0f, 10.0f), vec3f32(0.0f, 0.0f, 0.0f));
  renderer_init();

  while (win32_application_is_running()) {

    input_update();
    camera_update(&GlobalCamera, 0.0f);

    renderer_begin_frame();

    Mat4f32 view       = camera_get_view_matrix(&GlobalCamera);
    Mat4f32 projection = mat4f32_perspective(GlobalCamera.fov, (f32)WindowWidth, (f32)WindowHeight, 0.1f, 100.0f);
    renderer_draw_quad(vec2f32(-0.5, -0.5), vec2f32(0.5, 0.5), vec4f32(1.0f, 0.0f, 0.0f, 1.0f));

    renderer_end_frame(view, projection);
  }
}

internal void input_update() {
  if (input_is_key_pressed(KeyboardKey_ESCAPE)) {
    application_stop();
  }
}

