#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D TexSample;
uniform vec3 viewPosition;
uniform bool lightEnabled;

struct Light {
	vec3 lightPos;
	vec3 lightColor;
	float ambStrength;
	float specStrength;
};

vec4 calcLight(Light light) {
	// Ambient lighting
	vec3 ambient = light.ambStrength * light.lightColor;

	// Diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * light.lightColor;

	// Specular lighting
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPosition - FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = light.specStrength * spec * light.lightColor;

	// Output lighting vector
	return vec4(ambient + diffuse + specular, 1.0f);
}
#define NUM_LIGHTS 2
uniform Light lights[NUM_LIGHTS];

void main() {
	if (lightEnabled) {
		vec4 lighting = vec4(0.0f);
		for (int i = 0; i < NUM_LIGHTS; i++) {
			lighting += calcLight(lights[i]);
		}
		FragColor = texture(TexSample, TexCoords) * lighting;
	} else {
		FragColor = texture(TexSample, TexCoords);
	}
}
