#ifndef CHUNK_HPP_INCLUDED
#define CHUNK_HPP_INCLUDED

//class Cube;

#include "vertexType.hpp"
#include "camera.h"
#include "Program.hpp"
#include "FilePath.hpp"
#include "perlin.h"
#include "cube.hpp"
#include "itemManager.hpp"
#include <glimac/FilePath.hpp>
#include <glimac/shared.hpp>
#include <glimac/particuleManager.hpp>


class Chunk{
private :
GLuint vao;
GLuint vbo_model;
GLuint vbo_position;
GLuint vbo_indiceTexture;
GLuint vbo_hasFocus;
GLuint ibo;

//VertexUD tab_data_vertexModel[8];
//uint32_t indices[36];
//std::vector<glm::vec3> tab_data_position;
std::vector<Cube*> tab_Cube;

//Cube_program cubeProgram;

int nbCube;
int nbCubeAffichage;
int nbTextureCube;

int positionOrigineX;
int positionOrigineY;
int positionOrigineZ;

public :
//Chunk(glimac::Program const& program);
Chunk(const glimac::FilePath& applicationPath, int _nbTextureCube, int longueur_chunk, int longueur_map, int nbOctave, int persistance, int pas, int seed, int mini, int maxi, bool surfacique, std::vector<int> hauteurStrates, int origineX = 0, int origineY = 0, int origineZ = 0 );
Chunk(Cube** tabCubeSurface, const glimac::FilePath& applicationPath, int _nbTextureCube, int longueur_chunk, int longueur_map, int nbOctave, int persistance, int pas, int seed, int mini, int maxi, bool surfacique, std::vector<int> hauteurStrates, int origineX = 0, int origineY = 0, int origineZ = 0 );
Chunk(int _nbTextureCube, int longueur_chunk, int origineX, int origineY, int origineZ );

Chunk(int _nbTextureCube, int longueur_chunk, int origineX, int origineY, int origineZ, int test );


~Chunk();
void Draw(glm::mat4 const& Projection, glm::mat4 const& View, GLuint textureCube);
//void Draw(Camera camera);
Cube* getCube(glm::vec3 positionInChunk)const;
void destroyCube(int i, int j, int k, glimac::FilePath &filePath, ItemManager &itemManager, ParticuleManager& particuleManager);
void sendDammage(float dammageCount, int i, int j, int k, glimac::FilePath& filePath, ItemManager& itemManager, ParticuleManager& particuleManager);
void AddCube(int i, int j, int k);
//void AddCube(int i, int j, int k, TYPE_CUBE _type);
void AddCube(int i, int j, int k, ParticuleManager& particuleManager, const glimac::FilePath& path, std::string texturePath, TYPE_CUBE _type = TYPE_CUBE::TERRE01 );
void updateTarget( int i, int j, int k);
glm::vec3 getPosition() const;
void spawnTree(glm::vec3 position ,int textureId_feuille, int textureId_tronc);
void init_addNewCube(Cube* newCube, int indice);
void updateChunkVBO();

Cube* updateTarget(glm::vec3 position,  bool isHover);
bool testCubeIsVisible(int i, int j, int k);

void setTabCube(std::vector<Cube*> tabCube);/**/
std::vector<Cube*> getAllCube() const;/**/
int getNbCube() const; /***/
int getNbCubeAff() const; /***/
int getNbTextureCube() const; /***/
void setNbCube(int NbCube);/**/
void setNbCubeAff(int NbCubeAff);/**/
void setNbTextureCube(int NbTextCube);/**/

};



#endif // CHUNK_HPP_INCLUDED
