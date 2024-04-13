#version 330 core
in vec2 aTexCoord;
out vec4 fragcolor;

uniform sampler2D texture1;

void main() {
    fragcolor = texture(texture1, aTexCoord);
}