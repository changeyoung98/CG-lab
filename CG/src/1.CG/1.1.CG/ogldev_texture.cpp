/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include<fstream>
#include "ogldev_texture.h"
#include "stb_image_aug.h"
Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
}


bool Texture::Load()
{
   
   //  m_image=cv::imread(m_fileName);
        //m_image.write(&m_blob, "RGBA");
   
   // catch (Magick::Error& Error) {
 //       std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
  //      return false;
  //  }
	 glGenTextures(1, &m_textureObj);
	 int width, height, nrComponents;
	 unsigned char *data = stbi_load(m_fileName.c_str(), &width, &height, &nrComponents, 0);
	 if (data)
	 {
		 GLenum format;
		 if (nrComponents == 1)
			 format = GL_RED;
		 else if (nrComponents == 3)
			 format = GL_RGB;
		 else if (nrComponents == 4)
			 format = GL_RGBA;

	//	 glBindTexture(GL_TEXTURE_2D, m_textureObj);
		 glBindTexture(m_textureTarget, m_textureObj);
		 glTexImage2D(m_textureTarget, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		// glGenerateMipmap(GL_TEXTURE_2D);
		 glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		 glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 glBindTexture(m_textureTarget, 0);
	//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		 stbi_image_free(data);
	 }
	 else
	 {
		 std::cout << "Texture failed to load at path: " << m_fileName << std::endl;
		 stbi_image_free(data);
	 }
   
   // glTexImage2D(m_textureTarget, 0, GL_RGBA, m_image.cols, m_image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.ptr());  
    
    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
