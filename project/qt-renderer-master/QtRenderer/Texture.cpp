#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"
#include <QImage>
#include <iostream>

Texture::Texture(const char* path) {
	img_data = nullptr;
	int channels = 0;
	img_data = stbi_load(path, &width, &height, &channels, 3);
	
	pixFunc = &Texture::pixel_with_interpolate;
	//pixFunc = &Texture::pixel_without_interpolate;
};
Texture::~Texture() {
	stbi_image_free(img_data);
}