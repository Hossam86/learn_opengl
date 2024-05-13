#version 330 core

out vec4 Fragcolor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {

	// ambient
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * lightColor;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 light_dir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * objectColor;
	Fragcolor = vec4(result, 1.0);
}
