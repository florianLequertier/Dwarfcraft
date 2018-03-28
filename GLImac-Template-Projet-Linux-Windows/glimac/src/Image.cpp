#include "glimac/Image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "glimac/config.hpp"

namespace glimac {

std::unique_ptr<Image> loadImage(const FilePath& filepath) {
    int x, y, n;
    unsigned char *data = stbi_load(filepath.c_str(), &x, &y, &n, 4);
    if(!data) {
        std::cerr << "loading image " << filepath << " error: " << stbi_failure_reason() << std::endl;
        return std::unique_ptr<Image>();
    }
    std::unique_ptr<Image> pImage(new Image(x, y));
    unsigned int size = x * y;
    auto scale = 1.f / 255;
    auto ptr = pImage->getPixels();
    for(auto i = 0u; i < size; ++i) {
        auto offset = 4 * i;
        ptr->r = data[offset] * scale;
        ptr->g = data[offset + 1] * scale;
        ptr->b = data[offset + 2] * scale;
        ptr->a = data[offset + 3] * scale;
        ++ptr;
    }
    stbi_image_free(data);
    return pImage;
}

std::unordered_map<FilePath, std::unique_ptr<Image>> ImageManager::m_ImageMap;

const Image* ImageManager::loadImage(const FilePath& filepath) {
    auto it = m_ImageMap.find(filepath);
    if(it != std::end(m_ImageMap)) {
        return (*it).second.get();
    }
    auto pImage = glimac::loadImage(filepath);
    if(!pImage) {
        return nullptr;
    }
    auto& img = m_ImageMap[filepath] = std::move(pImage);
    return img.get();
}


GLuint LoadImageFromFile(std::string path_string, Config& confData)
{
    std::unique_ptr<Image> ptr_texture = loadImage(path_string.c_str());

    if(ptr_texture ==NULL)
    {
       std::cout<<"coucou je suis une erreur de chargement de texture !!! "<<std::cout;
    }
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ptr_texture->getWidth(), ptr_texture->getHeight(), 0, GL_RGBA, GL_FLOAT, ptr_texture->getPixels());

       //glEnable(GL_BLEND);
       //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

       if(confData.getMipMapFilter())
       {
           glEnable(GL_TEXTURE_2D);
           glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
       }
       else
       {
           if(confData.getMinFilter()==0)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
           else if(confData.getMinFilter()==1)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

           if(confData.getMaxFilter()==0)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           if(confData.getMaxFilter()==1)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       }




    glBindTexture(GL_TEXTURE_2D, 0);

    return  textureId;
}

GLuint LoadImageFromFile(std::string path_string)
{
    std::unique_ptr<Image> ptr_texture = loadImage(path_string.c_str());

    if(ptr_texture ==NULL)
    {
       std::cout<<"coucou je suis une erreur de chargement de texture !!! "<<std::cout;
    }
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ptr_texture->getWidth(), ptr_texture->getHeight(), 0, GL_RGBA, GL_FLOAT, ptr_texture->getPixels());

       //glEnable(GL_BLEND);
       //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);





    glBindTexture(GL_TEXTURE_2D, 0);

    return  textureId;
}



}


