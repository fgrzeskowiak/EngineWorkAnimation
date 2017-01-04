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

#include "stdafx.h"
#include <iostream>
#include "common.h"
#include "texture.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
}

bool Texture::Load()
{
	char *ImgData = NULL;
	int ImgWidth;
	int ImgHeight;
	LoadBMP(m_fileName.c_str(),ImgWidth,ImgHeight,&ImgData);


    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGB, ImgWidth, ImgHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, ImgData);
	free(ImgData);

	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}

void Texture::LoadBMP(const char *filename,int &_width, int &_height, char **pixels)
{
	char buf[100];  // 100B temporary buffer

	// some essential header fields
	// to be compared against the assumed bmp format
	int file_actual_size;
	int file_size;  
	int bmp_compression;

	// file handle
	FILE *fil =  NULL;
	fopen_s(&fil,filename,"rb");
	
	// check if file is opened 
	if (fil == NULL) ThrowException("Brak pliku BMP");

	fseek (fil, 0, SEEK_END);
    file_actual_size=ftell (fil); // obtain real filesize 
	rewind(fil); // make sure the file pointer is set to the beginning 

	//*********** READ THE HEADER ***************
	
	// read the signature 
	fread(buf,1,2,fil); 	
	if ((buf[0] !=0x42) || (buf[1] != 0x4d)) {fclose(fil); 
												//ThrowException("Invalid signature");
												}

	// read the filesize
	fread(&file_size,sizeof(file_size),1,fil);
	if (file_size != file_actual_size) {
										//ThrowException("Invalid file size");
										}

	// 12 dummies
	fread(buf,1,12,fil);

	fread(&_width,sizeof(_width),1,fil); 
	fread(&_height,sizeof(_height),1,fil); 
	
	// 2 dummies
	fread(buf,1,2,fil);

	// bits per pixel
	fread(buf,1,1,fil);
	if (buf[0] != 24) ;//ThrowException("Invalid BPP"); 

	// 1 dummy
	fread(buf,1,1,fil);

	//compression mode 
	fread(&bmp_compression,sizeof(bmp_compression),1,fil); 
	if (bmp_compression != 0) ;//ThrowException("Invalid Compression");; // invalid compression

	// 20 dummies
	fread(buf,1,20,fil);

	//********************* END OF THE HEADER ***********************

	// determine padding constant 
	int padding = 0;
	if (_width*3 % 4) padding = 4 - (_width*3) % 4;

	// allocate memory for pixeltable 
	*pixels  = (char *)calloc((_width*3+padding)*_height,sizeof(char));

	// read file content into pixeltable
	for (int h = 0; h < _height; h++)
	{
		fread(*pixels+h*(_width*3+padding),1,_width*3+padding,fil);
	}
	
	fclose(fil);
}