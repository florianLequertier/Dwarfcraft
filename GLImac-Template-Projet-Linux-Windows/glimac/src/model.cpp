#include "glimac/SDLWindowManager.hpp"
#include <GL/glew.h>
#include <iostream>
#include <glimac/FilePath.hpp>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include <glimac/Image.hpp>
#include <glimac/config.hpp>
#include <glimac/lightManager.hpp>
#include <glimac/light.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#include "glimac/Program.hpp"
#include "glimac/mesh.hpp"
#include "glimac/model.hpp"

using namespace std;


void Model::Draw( glm::mat4 Perspective, glm::mat4 View, LightManager& lightManager, glm::vec3 positionView )
{
    glm::mat4 ModelMat = glm::mat4(1);
    computeTransformation(ModelMat);
    for(GLuint i = 0; i < this->meshes.size(); i++)
        this->meshes[i].Draw(Perspective, View, ModelMat, lightManager, positionView);
}

void Model::Draw( glm::mat4 Perspective, glm::mat4 View )
{
    glm::mat4 ModelMat = glm::mat4(1);
    computeTransformation(ModelMat);
    for(GLuint i = 0; i < this->meshes.size(); i++)
        this->meshes[i].Draw(Perspective, View, ModelMat );
}


void Model::loadModel(std::string path)
{
    std::cout<<"loadModel, entrée"<<std::endl;
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs  );

    std::cout<<"path d'importation du model : "<<path<<std::endl;

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    this->directory = path.substr(0, path.find_last_of('\\'))+'/';
    std::cout<<"directory : "<<directory<<std::endl;

    this->processNode(scene->mRootNode, scene);
    std::cout<<"nombre de mesh dans le model : "<<meshes.size()<<std::endl;
    std::cout<<"loadModel, sortie"<<std::endl;
}



void Model::processNode(aiNode* node, const aiScene* scene)
{
    std::cout<<"processNode, entrée"<<std::endl;
    std::cout<<"nombre de node : "<<node->mNumChildren<<std::endl;
    // Process all the node's meshes (if any)
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }

    std::cout<<"processNode, 01"<<std::endl;

    // Then do the same for each of its children
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }

    std::cout<<"processNode, sortie"<<std::endl;
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::cout<<"processMesh, entrée"<<std::endl;
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    std::cout<<"processMesh, 01"<<std::endl;

    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        std::cout<<"processMesh, 01.1."<<i<<std::endl;

        Vertex vertex;
        // Process vertex positions, normals and texture coordinates
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        std::cout<<"processMesh, 01.2."<<i<<std::endl;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        std::cout<<"processMesh, 01.3."<<i<<std::endl;

        if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        std::cout<<"processMesh, 01.4."<<i<<std::endl;

        vertices.push_back(vertex);


    }

    std::cout<<"processMesh, 02"<<std::endl;

    // Process indices
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    std::cout<<"processMesh, 03"<<std::endl;

    // Process material
    if(mesh->mMaterialIndex >= 0)
    {

            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    }


    std::cout<<"processMesh, sortie"<<std::endl;
    std::cout<<"info containers mesh : vertices size : "<<vertices.size()<<" indices size : "<<indices.size()<<" textures size : "<<textures.size()<<std::endl;
    //Mesh meshRetour(vertices, indices, textures);
    std::cout<<"processMesh, sortie 02"<<std::endl;
    return Mesh(vertices, indices, textures, applicationPath);
}


vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    std::cout<<"loadMaterialTextures, entrée"<<std::endl;
    vector<Texture> textures;
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        GLboolean skip = false;
        for(GLuint j = 0; j < textures_loaded.size(); j++)
        {
            if(textures_loaded[j].path == str)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {   // If texture hasn't been loaded already, load it
            Texture texture;
            texture.id = glimac::LoadImageFromFile( string(directory)+string(str.C_Str()) );
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            this->textures_loaded.push_back(texture);  // Add to loaded textures
        }
    }
    std::cout<<"loadMaterielTextures, sortie"<<std::endl;
    return textures;
}

std::string Model::getPath() const
{
    return directory;
}

std::string Model::getModelPath() const
{
    return modelPath;
}
