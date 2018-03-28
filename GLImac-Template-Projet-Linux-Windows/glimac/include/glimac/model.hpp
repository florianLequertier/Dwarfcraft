#ifndef MODEL_HPP
#define MODEL_HPP


#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "glimac/common.hpp"
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/mesh.hpp>
#include <glimac/config.hpp>
#include <glimac/lightManager.hpp>
#include <glimac/light.hpp>
#include <glimac/transformable.hpp>






class Model: public Transformable
{

    private:
    ///Program :
    MeshProgram shader_program;
    MeshProgram_unlit shader_program_unlit;

        std::vector<Texture> textures_loaded;
        /*  Model Data  */
        std::vector<Mesh> meshes;
        std::string directory;
        glimac::FilePath applicationPath;
        std::string modelPath;

        /*  Functions   */
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    public:
        /*  Functions   */
        Model(const glimac::FilePath& _applicationPath = "", std::string path =""):Transformable(), applicationPath(_applicationPath), modelPath((std::string)path)
        {
            this->loadModel(path);
        }
        void Draw(glm::mat4 Perspective , glm::mat4 View);
        void Draw(glm::mat4 Perspective , glm::mat4 View, LightManager& lightManager, glm::vec3 positionView );
        std::string getPath() const;
        std::string getModelPath() const;
};

#endif // MODEL_HPP
