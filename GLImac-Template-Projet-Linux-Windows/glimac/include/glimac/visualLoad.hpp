#ifndef VISUALLOAD_HPP
#define VISUALLOAD_HPP


#include <cstdlib>
#include <vector>
#include "vertexType.hpp"
#include "camera.h"
#include "Program.hpp"
#include "FilePath.hpp"
#include "perlin.h"
#include "chunk.hpp"
#include "lightManager.hpp"
#include <glimac/shared.hpp>




class VisualLoad{
private:
    VAO_container vao_background;
    Basic_2D_Tex_program program_background;
    GLuint textureBackground;
    GLuint textureBackground02;
    VAO_container vao_barre;
    Basic_2D_program program_barre;


public:
    VisualLoad(glimac::FilePath applicationPath, std::vector<std::string> texturePath);
    ~VisualLoad();
    void Draw(float pourcentage, glimac::SDLWindowManager& windowManager, int typeFond);

};

#endif // VISUALLOAD_HPP
