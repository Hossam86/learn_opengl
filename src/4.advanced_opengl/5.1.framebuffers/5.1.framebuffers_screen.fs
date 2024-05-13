#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
    // vec3 col = texture(screenTexture, TexCoords).rgb;
    // FragColor = vec4(col, 1.0);

    // use case1: We have access to each of the colors of the render output
    // so it's not so hard to return the inverse of these colors in the fragment shade
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}