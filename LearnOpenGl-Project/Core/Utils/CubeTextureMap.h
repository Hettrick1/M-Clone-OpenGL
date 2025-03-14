#pragma once

#include "OpenGL/imageLoader/stb_image.h"

#include <glad/glad.h>

#include <string>
#include <vector>
#include <iostream>

class CubeTextureMap
{
	public:

		void CreateCubeTextureMap(const std::vector<std::string>& cube_face_paths);
		inline GLuint GetID() const { return m_TextureID; }

	private:

		GLuint m_TextureID;

		std::vector<std::string> m_CubeFacePaths;
};

