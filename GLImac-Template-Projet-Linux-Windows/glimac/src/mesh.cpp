#include "glimac/SDLWindowManager.hpp"
#include <GL/glew.h>
#include <iostream>
#include <glimac/FilePath.hpp>
#include <vector>
#include <cmath>
#include <sstream>
#include <glimac/lightManager.hpp>
#include <glimac/light.hpp>

#include "glimac/Program.hpp"
#include "glimac/mesh.hpp"


using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, const glimac::FilePath& applicationPath)
{
    shader_program = MeshProgram(applicationPath);
    shader_program_unlit = MeshProgram_unlit(applicationPath);

    std::cout<<"Mesh, constructeur, entrée"<<std::endl;
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->setupMesh();
    std::cout<<"Mesh, constructeur, sortie"<<std::endl;
}

void Mesh::setupMesh()
{
    std::cout<<"setupMesh, entrée"<<std::endl;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->IBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                 &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                 &this->indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
    std::cout<<"setupMesh, sortie"<<std::endl;
}

void Mesh::Draw( glm::mat4 Projection, glm::mat4 View, LightManager& lightManager, glm::vec3 positionView )
{

    shader_program.program.use();

    glm::mat4 modelMatrix = glm::mat4(1);

    ///Position,...
    glUniformMatrix4fv(shader_program.uniform_uModelMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)) );
    glUniformMatrix4fv(shader_program.uniform_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection) );
    glUniformMatrix4fv(shader_program.uniform_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View) );
    glUniformMatrix4fv(shader_program.uniform_uMVP, 1, GL_FALSE, glm::value_ptr(Projection*View*modelMatrix) );
    glUniformMatrix4fv(shader_program.uniform_uNormalMatrix, 1, GL_FALSE, glm::value_ptr( glm::transpose(glm::inverse(View*modelMatrix)) ));
    glUniform3f(shader_program.uniform_uPosition_glob, positionView.x, positionView.y, positionView.z);

    ///Materiaux :
    glUniform3f(shader_program.uniform_uKd, 0.2f,0.2f,0.2f);
    glUniform3f(shader_program.uniform_uKs, 0.2f,0.2f,0.2f);
    glUniform1f(shader_program.uniform_uShininess, 0.1f);

    ///Propriete de la lumiere :
    glUniform1i(shader_program.uniform_uNbPointLight, lightManager.getNbPointLight());
    glUniform1i(shader_program.uniform_uNbDirectionnalLight, lightManager.getNbDirectionnalLight());

    glUniform3fv(shader_program.uniform_uPointLightIntensity, lightManager.getNbPointLight()*3, lightManager.getPointLightIntentityTab_ptr());
    glUniform3fv(shader_program.uniform_uLightPos_vs, lightManager.getNbPointLight()*3, lightManager.getPointLightPositionTab_ptr() );

    glUniform3fv(shader_program.uniform_uDirectionnalLightIntensity, lightManager.getNbDirectionnalLight()*3, lightManager.getDirectionnalLightIntentityTab_ptr());
    glUniform3fv(shader_program.uniform_uLightDir_vs, lightManager.getNbDirectionnalLight()*3, lightManager.getDirectionnalLightDirectionTab_ptr()  );



    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();

        glUniform1f(glGetUniformLocation(shader_program.program.getGLId(), ("material_" + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

}



void Mesh::Draw( glm::mat4 Projection, glm::mat4 View )
{

    shader_program_unlit.program.use();

    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();

        glUniform1f(glGetUniformLocation(shader_program_unlit.program.getGLId(), ("material_" + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glm::mat4 modelMatrix = glm::mat4(1);

    glUniformMatrix4fv(shader_program_unlit.uniform_uModelMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)) );
    glUniformMatrix4fv(shader_program_unlit.uniform_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection) );
    glUniformMatrix4fv(shader_program_unlit.uniform_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View) );
    glUniformMatrix4fv(shader_program_unlit.uniform_uMVP, 1, GL_FALSE, glm::value_ptr(Projection*View*modelMatrix) );
    glUniformMatrix4fv(shader_program_unlit.uniform_uNormalMatrix, 1, GL_FALSE, glm::value_ptr( glm::transpose(glm::inverse(View*modelMatrix)) ));


    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

}


void Mesh::Draw( glm::mat4 Projection, glm::mat4 View, glm::mat4 ModelMat, LightManager& lightManager, glm::vec3 positionView )
{

    shader_program.program.use();


    ///Position,...
    glUniformMatrix4fv(shader_program.uniform_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMat) );
    glUniformMatrix4fv(shader_program.uniform_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection) );
    glUniformMatrix4fv(shader_program.uniform_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View) );
    glUniformMatrix4fv(shader_program.uniform_uMVP, 1, GL_FALSE, glm::value_ptr(Projection*View*ModelMat) );
    glUniformMatrix4fv(shader_program.uniform_uNormalMatrix, 1, GL_FALSE, glm::value_ptr( glm::transpose(glm::inverse(View*ModelMat)) ));
    glUniform3f(shader_program.uniform_uPosition_glob, positionView.x, positionView.y, positionView.z);

    ///Materiaux :
    glUniform3f(shader_program.uniform_uKd, 0.2f,0.2f,0.2f);
    glUniform3f(shader_program.uniform_uKs, 0.2f,0.2f,0.2f);
    glUniform1f(shader_program.uniform_uShininess, 0.1f);

    ///Propriete de la lumiere :
    glUniform1i(shader_program.uniform_uNbPointLight, lightManager.getNbPointLight());
    glUniform1i(shader_program.uniform_uNbDirectionnalLight, lightManager.getNbDirectionnalLight());

    glUniform3fv(shader_program.uniform_uPointLightIntensity, lightManager.getNbPointLight()*3, lightManager.getPointLightIntentityTab_ptr());
    glUniform3fv(shader_program.uniform_uLightPos_vs, lightManager.getNbPointLight()*3, lightManager.getPointLightPositionTab_ptr() );

    glUniform3fv(shader_program.uniform_uDirectionnalLightIntensity, lightManager.getNbDirectionnalLight()*3, lightManager.getDirectionnalLightIntentityTab_ptr());
    glUniform3fv(shader_program.uniform_uLightDir_vs, lightManager.getNbDirectionnalLight()*3, lightManager.getDirectionnalLightDirectionTab_ptr()  );



    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();

        glUniform1f(glGetUniformLocation(shader_program.program.getGLId(), ("material_" + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

}


void Mesh::Draw( glm::mat4 Projection, glm::mat4 View, glm::mat4 ModelMat, LightManager& lightManager)
{

    shader_program.program.use();


    ///Position,...
    glUniformMatrix4fv(shader_program.uniform_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMat) );
    glUniformMatrix4fv(shader_program.uniform_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection) );
    glUniformMatrix4fv(shader_program.uniform_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View) );
    glUniformMatrix4fv(shader_program.uniform_uMVP, 1, GL_FALSE, glm::value_ptr(Projection*View*ModelMat) );
    glUniformMatrix4fv(shader_program.uniform_uNormalMatrix, 1, GL_FALSE, glm::value_ptr( glm::transpose(glm::inverse(View*ModelMat)) ));

    ///Materiaux :
    glUniform3f(shader_program.uniform_uKd, 0.2f,0.2f,0.2f);
    glUniform3f(shader_program.uniform_uKs, 0.2f,0.2f,0.2f);
    glUniform1f(shader_program.uniform_uShininess, 0.1f);

    ///Propriete de la lumiere :
    glUniform1i(shader_program.uniform_uNbPointLight, lightManager.getNbPointLight());
    glUniform1i(shader_program.uniform_uNbDirectionnalLight, lightManager.getNbDirectionnalLight());

    glUniform3fv(shader_program.uniform_uPointLightIntensity, lightManager.getNbPointLight()*3, lightManager.getPointLightIntentityTab_ptr());
    glUniform3fv(shader_program.uniform_uLightPos_vs, lightManager.getNbPointLight()*3, lightManager.getPointLightPositionTab_ptr() );

    glUniform3fv(shader_program.uniform_uDirectionnalLightIntensity, lightManager.getNbDirectionnalLight()*3, lightManager.getDirectionnalLightIntentityTab_ptr());
    glUniform3fv(shader_program.uniform_uLightDir_vs, lightManager.getNbDirectionnalLight()*3, lightManager.getDirectionnalLightDirectionTab_ptr()  );



    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();

        glUniform1f(glGetUniformLocation(shader_program.program.getGLId(), ("material_" + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

}

void Mesh::Draw( glm::mat4 Projection, glm::mat4 View, glm::mat4 ModelMat )
{

    shader_program_unlit.program.use();


    glUniformMatrix4fv(shader_program_unlit.uniform_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMat) );
    glUniformMatrix4fv(shader_program_unlit.uniform_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection) );
    glUniformMatrix4fv(shader_program_unlit.uniform_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View) );
    glUniformMatrix4fv(shader_program_unlit.uniform_uMVP, 1, GL_FALSE, glm::value_ptr(Projection*View*ModelMat) );
    glUniformMatrix4fv(shader_program_unlit.uniform_uNormalMatrix, 1, GL_FALSE, glm::value_ptr( glm::transpose(glm::inverse(View*ModelMat)) ));



    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();

        glUniform1f(glGetUniformLocation(shader_program_unlit.program.getGLId(), ("material_" + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);



    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

}
