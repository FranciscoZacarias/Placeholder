#version 460 core

layout (location = 0) in vec2 Position;

out gl_PerVertex { vec4 gl_Position; };

void main() {
  vec2 pos = vec2(Position);
  gl_Position = vec4(pos, 0.0, 1.0);
}