#version 460 core

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec4 in_color;

out vec4 v_color;

void main() {
  gl_Position = vec4(in_pos, 0.0, 1.0);
  v_color     = in_color;
}
