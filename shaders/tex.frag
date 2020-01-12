#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D TexSample;
uniform vec4 color = vec4(1.0f);

void main() {
	FragColor = texture(TexSample, TexCoords) * color;
}
