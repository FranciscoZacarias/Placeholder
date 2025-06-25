

internal void renderer_init() {
  AssertNoReentry();

  MemoryZeroStruct(&GRenderer);
  GRenderer.arena = arena_init_sized(Megabytes(16), ARENA_COMMIT_SIZE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  {
    OGL_Shader screenspace_vertex_shader = opengl_make_shader(SCREENSPACE_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
    OGL_Shader fragment_shader           = opengl_make_shader(FRAGMENT_SHADER_PATH,           GL_FRAGMENT_SHADER);

    OGL_Shader shaders[] = { screenspace_vertex_shader, fragment_shader };
    ScreenspaceProgram   = opengl_make_program(shaders, 2);

    opengl_delete_shader(screenspace_vertex_shader);
    opengl_delete_shader(fragment_shader);
  }
}


internal void renderer_begin_frame() {
  glClearColor(0.0f, 0.8f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

internal void renderer_end_frame(Mat4f32 view, Mat4f32 projection) {
  SwapBuffers(_DeviceContextHandle);
}


internal void renderer_draw_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color) {
}

internal void renderer_push_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color) {
}
