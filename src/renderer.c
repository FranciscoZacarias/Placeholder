internal void renderer_init() {
  AssertNoReentry();
  opengl_print_context();
  MemoryZeroStruct(&GRenderer);
  GRenderer.arena = arena_init_sized(Megabytes(16), ARENA_COMMIT_SIZE);
  
  // Set up OpenGL state
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); // NOTE(fz): Disabled.

  glDisable(GL_CULL_FACE);
  
  // Create pipelines
  {
    GLuint screenspace_vertex_shader = opengl_compile_program(SCREENSPACE_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
    GLuint fragment_shader           = opengl_compile_program(FRAGMENT_SHADER_PATH,           GL_FRAGMENT_SHADER);

    glGenProgramPipelines(1, &ScreenspacePipeline);
    glUseProgramStages(ScreenspacePipeline, GL_VERTEX_SHADER_BIT, screenspace_vertex_shader);
    glUseProgramStages(ScreenspacePipeline, GL_FRAGMENT_SHADER_BIT, fragment_shader);

    opengl_delete_program(screenspace_vertex_shader);
    opengl_delete_program(fragment_shader);
  }
}

internal void renderer_begin_frame() {
  glClearColor(1.0f, 1.0f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

internal void renderer_end_frame(Mat4f32 view, Mat4f32 projection) {
  glUseProgram(ScreenspaceProgram);
  glBindVertexArray(GRenderer.quad.vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
  glDrawArrays(GL_TRIANGLES, 0, 3);

  if (!SwapBuffers(_DeviceContextHandle)) {
    win32_fatal_error("Failed to swap opengl buffers.");
  }
}

internal void renderer_set_vsync(b32 on) {
  wglSwapIntervalEXT(on);
}

internal void renderer_draw_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color) {
}

internal void renderer_push_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color) {
}
