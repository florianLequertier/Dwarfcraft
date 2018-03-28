#include <cstdlib>
#include <vector>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/shared.hpp>
#include <glimac/visualLoad.hpp>
#include <glimac/SDLWindowManager.hpp>


VisualLoad::VisualLoad(glimac::FilePath applicationPath, std::vector<std::string> texturePath)
{

    //chargement des textures :
    textureBackground = glimac::LoadImageFromFile(texturePath[0]);
    textureBackground02 = glimac::LoadImageFromFile(texturePath[1]);

    //initialisation des programmes :
    program_background = Basic_2D_Tex_program(applicationPath);
    program_barre = Basic_2D_program(applicationPath);


    //Création des VBO, IBO, VAO :
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




    Simple2DTexVertex data_vertex_background[] = {
     Simple2DTexVertex(glm::vec2(-1, -1), glm::vec2(0,1)),
     Simple2DTexVertex(glm::vec2(1, -1), glm::vec2(1,1)),
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



}

void VisualLoad::Draw(float pourcentage, glimac::SDLWindowManager& windowManager, int typeFond )
{
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat3 model_matrix = glm::mat3(1);

    program_background.program.use();

    glUniformMatrix3fv(program_background.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniform1i(program_background.uniformID_uTexture, 0 );


    glActiveTexture(GL_TEXTURE0);
    if(typeFond == 0)
    {
        glBindTexture(GL_TEXTURE_2D, textureBackground);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, textureBackground02);
    }
    glBindVertexArray(vao_background.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);


    //////////////////////////////////////////////////


    model_matrix = glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3((-1+pourcentage),0,1)) * glm::mat3(glm::vec3(pourcentage,0,0), glm::vec3(0,0.3,0), glm::vec3(0,0,1));

    program_barre.program.use();

    glUniformMatrix3fv(program_barre.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
    if(pourcentage<0.15)
    glUniform4f(program_barre.uniformID_uColor, 1,0,0,0.7);
    else if(pourcentage<0.6 && pourcentage>=0.15)
    glUniform4f(program_barre.uniformID_uColor, 1,1,0,0.7);
    else if(pourcentage>=0.6)
    glUniform4f(program_barre.uniformID_uColor, 0,1,0,0.7);

    glBindVertexArray(vao_barre.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    windowManager.swapBuffers();

    glEnable(GL_DEPTH_TEST);

}

VisualLoad::~VisualLoad()
{
    glDeleteTextures(1, &textureBackground);
}
