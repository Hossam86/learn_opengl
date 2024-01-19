#version 330 core

out vec4 fragcolor;
in vec2 TexCoords;

uniform sampler2D texture1;

void main() {
    fragcolor = texture(texture1, TexCoords);
}