#ifndef RENDERER_H
#define RENDERER_H

// Shader source
#define SCREENSPACE_VERTEX_SHADER_PATH Str8("D:/work/nameless/src/shaders/screenspace_vertex_shader.glsl")
#define WORLDSPACE_VERTEX_SHADER_PATH  Str8("D:/work/nameless/src/shaders/worldspace_vertex_shader.glsl")
#define FRAGMENT_SHADER_PATH           Str8("D:/work/nameless/src/shaders/fragment_shader.glsl")

// Font
#define FONT_INCONSOLATA_OTF   Str8("D:/work/nameless/fonts/Inconsolata.otf")
#define FONT_FIRA_MONO_REGULAT Str8("D:/work/nameless/fonts/FiraMono-Regular.ttf")
#define FONT_FIRA_MONO_MEDIUM  Str8("D:/work/nameless/fonts/FiraMono-Medium.ttf")
#define FONT_FIRA_MONO_BOLD    Str8("D:/work/nameless/fonts/FiraMono-Bold.ttf")

OGL_Shader WorldspaceProgram;
OGL_Shader ScreenspaceProgram;

typedef struct Renderer {
  Arena* arena;

} Renderer;

Renderer GRenderer;

internal void renderer_init();
internal void renderer_begin_frame();
internal void renderer_end_frame(Mat4f32 view, Mat4f32 Projection);

// Doc(fz): In the renderer namespace
// renderer_draW_* implies drawing in immediate mode I.e. data gets wiped and uploaded to GPU every frame
// renderer_push_* implies drawing in retained mode I.e. we keep the data in the buffers until it's flagged to be removed

// Immediate
internal void renderer_draw_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color);

// Retained
internal void renderer_push_quad(Vec2f32 ndc_bot_left, Vec2f32 ndc_top_right, Vec4f32 color);

#endif // RENDERER_H  