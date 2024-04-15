#version 330 core
in vec2 aTexCoord;
out vec4 fragcolor;

uniform sampler2D texture1;

void main() {
    fragcolor = texture(texture1, aTexCoord);
    // Visualize depth buffer
    //-------------------------------------------
    // fragcolor = vec4(vec3(gl_FragColor.z), 1.0);
}