#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char* texFile) {
	// Load in image information
	int width, height, nrChannels;
	unsigned char* image = stbi_load(texFile, &width, &height, &nrChannels, STBI_rgb_alpha);

	// Check if the image was loaded correctly
	if (!image) {
		printf("ERROR: Failed to load texture image \"%s\"\n", texFile);
		stbi_image_free(image);
		return;
	}

	// Generate the texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set the default texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Free the image
	stbi_image_free(image);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, texture);
}
