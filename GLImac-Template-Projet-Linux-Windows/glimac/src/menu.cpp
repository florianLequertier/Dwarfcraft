#include <cstdlib>
#include <vector>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/shared.hpp>
#include <glimac/menu.hpp>
#include <glimac/SDLWindowManager.hpp>


Menu::Menu(glimac::FilePath applicationPath, std::string texturePath): selectedOption(0)
{
    texture = glimac::LoadImageFromFile(texturePath);
    program_background = Basic_2D_Tex_program(applicationPath);
    program_barre = Basic_2D_program(applicationPath);
    program_bouton = Basic_2D_Tex_program(applicationPath);

    /*/////////barre//////////*/

    Simple2DVertex data_vertex_barre[] = {
     Simple2DVertex(glm::vec2(-1, -0.5)),
     Simple2DVertex(glm::vec2(1, -0.5)),
     Simple2DVertex(glm::vec2(1, 0.5)),
     Simple2DVertex(glm::vec2(-1, 0.5)),

    };


    glGenBuffers(1, &(vao_barre.vbo) );
    glBindBuffer(GL_ARRAY_BUFFER, vao_barre.vbo);
    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Simple2DVertex), data_vertex_barre, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t data_index_barre[] = {0, 1, 2, 3, 0, 2 };

    glGenBuffers(1, &(vao_barre.ibo) );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao_barre.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint32_t), data_index_barre, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &(vao_barre.vao));
    glBindVertexArray(vao_barre.vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao_barre.ibo);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao_barre.vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Simple2DVertex), (const GLvoid*)offsetof(Simple2DVertex,position) );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    /*/////////background//////////*/

    Simple2DTexVertex data_vertex_background[] = {
     Simple2DTexVertex(glm::vec2(-1, -1), glm::vec2(0,0.8)),
     Simple2DTexVertex(glm::vec2(1, -1), glm::vec2(1,0.8)),
     Simple2DTexVertex(glm::vec2(1, 1), glm::vec2(1,0)),
     Simple2DTexVertex(glm::vec2(-1, 1), glm::vec2(0,0)),

    };


    glGenBuffers(1, &(vao_background.vbo) );
    glBindBuffer(GL_ARRAY_BUFFER, vao_background.vbo);
    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Simple2DTexVertex), data_vertex_background, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t data_index_background[] = {0, 1, 2, 3, 0, 2 };

    glGenBuffers(1, &(vao_background.ibo) );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao_background.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint32_t), data_index_background, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    glGenVertexArrays(1, &(vao_background.vao));
    glBindVertexArray(vao_background.vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao_background.ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vao_background.vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Simple2DTexVertex), (const GLvoid*)offsetof(Simple2DTexVertex,position) );
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Simple2DTexVertex), (const GLvoid*)offsetof(Simple2DTexVertex,texCoords) );
   glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*/////////Boutons//////////*/

    Simple2DTexVertex data_vertex_bouton[] = {
     Simple2DTexVertex(glm::vec2(-1, -1), glm::vec2(0,1)),
     Simple2DTexVertex(glm::vec2(1, -1), glm::vec2(0.6,1)),
     Simple2DTexVertex(glm::vec2(1, 1), glm::vec2(0.6,0.8)),
     Simple2DTexVertex(glm::vec2(-1, 1), glm::vec2(0,0.8)),

    };


    glGenBuffers(1, &(vao_bouton.vbo) );
    glBindBuffer(GL_ARRAY_BUFFER, vao_bouton.vbo);
    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Simple2DTexVertex), data_vertex_bouton, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t data_index_bouton[] = {0, 1, 2, 3, 0, 2 };

    glGenBuffers(1, &(vao_bouton.ibo) );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao_bouton.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint32_t), data_index_bouton, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    glGenVertexArrays(1, &(vao_bouton.vao));
    glBindVertexArray(vao_bouton.vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao_bouton.ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vao_bouton.vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Simple2DTexVertex), (const GLvoid*)offsetof(Simple2DTexVertex,position) );
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Simple2DTexVertex), (const GLvoid*)offsetof(Simple2DTexVertex,texCoords) );
   glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);



}

void Menu::Draw()
{


    glm::mat3 model_matrix = glm::mat3(1);

    program_background.program.use();

    glUniformMatrix3fv(program_background.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniform1i(program_background.uniformID_uTexture, 0 );


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao_background.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);


    //////////////////////////////////////////////////

    if(selectedOption == 0)
    model_matrix = glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(-0.3,0,1)) * glm::mat3(glm::vec3(0.3,0,0), glm::vec3(0,0.4,0), glm::vec3(0,0,1));
    else if(selectedOption == 1)
    model_matrix = glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(+0.3,0,1)) * glm::mat3(glm::vec3(0.3,0,0), glm::vec3(0,0.4,0), glm::vec3(0,0,1));

    program_barre.program.use();

    glUniformMatrix3fv(program_barre.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniform4f(program_barre.uniformID_uColor, 0,1,0,0.5);


    glBindVertexArray(vao_barre.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //////////////////////////////////////////////////

    model_matrix = glm::mat3(glm::vec3(0.6,0,0), glm::vec3(0,0.2,0), glm::vec3(0,0,1));


    program_bouton.program.use();

    glUniformMatrix3fv(program_bouton.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao_bouton.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);


}


void Menu::setSelectedOption(int newState)
{
    selectedOption = newState;
}

Menu::~Menu()
{
    glDeleteTextures(1, &texture);

    glDeleteBuffers(1, &vao_background.vbo);
    glDeleteBuffers(1, &vao_background.ibo);
    glDeleteBuffers(1, &vao_background.vao);

    glDeleteBuffers(1, &vao_barre.vbo);
    glDeleteBuffers(1, &vao_barre.ibo);
    glDeleteBuffers(1, &vao_barre.vao);

    glDeleteBuffers(1, &vao_bouton.vbo);
    glDeleteBuffers(1, &vao_bouton.ibo);
    glDeleteBuffers(1, &vao_bouton.vao);
}

int Menu::getSelectedOption(){
    return selectedOption;
}
