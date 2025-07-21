internal void renderer_init() {
  AssertNoReentry();
  MemoryZeroStruct(&GRenderer);
  GRenderer.arena = arena_init_sized(Megabytes(16), ARENA_COMMIT_SIZE);
  
  // Set up OpenGL state
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
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
  
  // Setup quad VAO and VBO
  {
    glGenVertexArrays(1, &GRenderer.quad.vao);
    glGenBuffers(1, &GRenderer.quad.vbo);
    
    glBindVertexArray(GRenderer.quad.vao);
    glBindBuffer(GL_ARRAY_BUFFER, GRenderer.quad.vbo);
    
    // Enable vertex attribute (position)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
    
    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}


internal void renderer_begin_frame() {
  glClearColor(1.0f, 1.0f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
  GRenderer.quad.vertex_count = 0;
}

// Updated renderer_end_frame
internal void renderer_end_frame(Mat4f32 view, Mat4f32 projection) {
  if (GRenderer.quad.vertex_count > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, GRenderer.quad.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * GRenderer.quad.vertex_count, GRenderer.quad.vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(GRenderer.quad.vao);
    glBindProgramPipeline(ScreenspacePipeline);

    glDrawArrays(GL_TRIANGLES, 0, GRenderer.quad.vertex_count / COMPONENTS_PER_VERTEX);

    glBindProgramPipeline(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  if (!SwapBuffers(_DeviceContextHandle)) {
    win32_fatal_error("Failed to swap opengl buffers.");
  }
}

internal void renderer_set_vsync(b32 on) {
  wglSwapIntervalEXT(on);
}

internal void renderer_draw_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color) {
  if (GRenderer.quad.vertex_count + 6 * 2 > ArrayCount(GRenderer.quad.vertices)) {
    return;
  }

  f32 *v = GRenderer.quad.vertices + GRenderer.quad.vertex_count;

  f32 vertices[] = {
    // First triangle
    ndc_bot_left.x,  ndc_bot_left.y,
    ndc_top_right.x, ndc_bot_left.y,
    ndc_top_right.x, ndc_top_right.y,

    // Second triangle
    ndc_bot_left.x,  ndc_bot_left.y,
    ndc_top_right.x, ndc_top_right.y,
    ndc_bot_left.x,  ndc_top_right.y
  };

  MemoryCopy(v, vertices, sizeof(vertices));
  GRenderer.quad.vertex_count += 6 * 2;
}

internal void renderer_push_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color) {
}
