#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "headers.h"

// Texture Declarations.
class ImageTexture
{
public:
	// Texture Public Methods.
	ImageTexture(const std::string filePath);
	~ImageTexture();

	void Bind(GLenum textureUnit);
	void Preview();
	std::string GetPath() const { return texFilePath; }

private:
	// Texture Private Data.
	std::string texFilePath;
	GLuint textureObj;  // Opengl texture object(ID)
	int imageWidth;
	int imageHeight;
	int numChannels;
	cv::Mat texImage; // pixel data (2D array)
};

#endif

