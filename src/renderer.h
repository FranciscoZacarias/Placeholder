#ifndef RENDERER_H
#define RENDERER_H

// Shader source
#define SCREENSPACE_VERTEX_SHADER_PATH Str8("./src/shaders/screenspace_vertex_shader.glsl")
#define WORLDSPACE_VERTEX_SHADER_PATH  Str8("./src/shaders/worldspace_vertex_shader.glsl")
#define FRAGMENT_SHADER_PATH           Str8("./src/shaders/fragment_shader.glsl")

// Font
#define FONT_INCONSOLATA_OTF   Str8("./fonts/Inconsolata.otf")
#define FONT_FIRA_MONO_REGULAT Str8("./fonts/FiraMono-Regular.ttf")
#define FONT_FIRA_MONO_MEDIUM  Str8("./fonts/FiraMono-Medium.ttf")
#define FONT_FIRA_MONO_BOLD    Str8("./fonts/FiraMono-Bold.ttf")

// Shader programs
GLuint WorldspaceProgram;
GLuint ScreenspaceProgram;
GLuint ScreenspacePipeline;

// Textures
GLuint FallbackTexture = 0;

typedef struct Renderer {
  Arena* arena;
  
  // Screenspace
  struct {
    #define MAX_QUADS_PER_FRAME 1024
    #define VERTICES_PER_QUAD   6
    #define COMPONENTS_PER_VERTEX 2

    GLuint vao;
    GLuint vbo;

	f32 vertices[MAX_QUADS_PER_FRAME * VERTICES_PER_QUAD * COMPONENTS_PER_VERTEX];
	u32 vertex_count;
  } quad;

} Renderer;

Renderer GRenderer;

internal void renderer_init();
internal void renderer_begin_frame();
internal void renderer_end_frame(Mat4f32 view, Mat4f32 Projection);

internal void renderer_set_vsync(b32 on);

// Immediate
// renderer_draW_* implies drawing in immediate mode I.e. data gets wiped and uploaded to GPU every frame
internal void renderer_draw_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color);

// Retained
// renderer_push_* implies drawing in retained mode I.e. we keep the data in the buffers until it's flagged to be removed
internal void renderer_push_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color);

#endif // RENDERER_H  