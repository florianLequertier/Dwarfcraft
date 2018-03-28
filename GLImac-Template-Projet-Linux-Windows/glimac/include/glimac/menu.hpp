#ifndef MENU_HPP
#define MENU_HPP


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



class Menu{
private:
    VAO_container vao_background;
    Basic_2D_Tex_program program_background;
    GLuint texture;

    VAO_container vao_barre;
    Basic_2D_program program_barre;

    VAO_container vao_bouton;
    Basic_2D_Tex_program program_bouton;

    int selectedOption;


public:
    Menu(glimac::FilePath applicationPath, std::string texturePath );
    ~Menu();
    void Draw();
    void setSelectedOption(int newState);
    int getSelectedOption();

};

#endif // MENU_HPP
