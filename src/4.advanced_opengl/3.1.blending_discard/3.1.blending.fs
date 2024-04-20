#version 330 core

in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D texture1;
void main() {
    vec4 texColor = texture(texture1, TexCoords);
    //OpenGL does not know what to do with alpha values, nor when to discard them. We have to manually do this ourselves
    //discard command that (once called) ensures the fragment will not be further processed 
    //and thus not end up into the color buffer.
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;

}