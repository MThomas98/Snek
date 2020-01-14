#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D TexSample;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
	float ambStr = 0.1f;
	ambient = ambStr * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec4 lightRes = vec4(ambStr, 1.0f);

	FragColor = texture(TexSample, TexCoords) * ambStr;
}
