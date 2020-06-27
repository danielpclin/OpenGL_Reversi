#version 330
uniform vec4 u_Color;
layout(location = 0) out vec4 color;

void main() {
    color = u_Color;
}
