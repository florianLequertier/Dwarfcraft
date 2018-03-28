#include "glimac/SDLWindowManager.hpp"
#include <GL/glew.h>
#include <iostream>
#include <glimac/FilePath.hpp>
#include <vector>
#include <cmath>

#include "glimac/Program.hpp"
#include "glimac/vertexType.hpp"
#include "glimac/chunk.hpp"
#include "glimac/camera.h"
#include "glimac/perlin.h"
#include "glimac/map.hpp"
#include "glimac/config.hpp"
#include "glimac/lightManager.hpp"
#include "glimac/visualLoad.hpp"
#include "glimac/tinyxml.hpp"

int Map::MAX_BIOME = 3;

Map::~Map()
{
    std::cout<<"destruction map"<<std::endl;

    std::cout<<"tabHover"<<std::endl;
    for(int i=0; i<tabHover.size(); i++)
    {
        tabHover[i] = NULL;
    }


    std::cout<<"tab_chunk"<<std::endl;
    for(int i=0; i<tab_Chunk.size(); i++)
    {
        delete tab_Chunk[i];
        tab_Chunk[i] = NULL;
    }
    tab_Chunk.clear();

    tabHover.clear();


}

glm::vec3 Map::getPositionSpawn() const
{
    return positionSpawn;
}

Map::Map( const glimac::FilePath& applicationPath, GLuint _textureCube, int nbTexturecube, int _nbChunk_x, int _nbChunk_y, int _nbChunk_z, int _nbCubeByRowByChunk, int _seed, int nbStrates, VisualLoad* visualLoad, glimac::SDLWindowManager* windowManager): nbChunk_x( _nbChunk_x ), nbChunk_y( _nbChunk_y ), nbChunk_z( _nbChunk_z ), nbCubeByRowByChunk( _nbCubeByRowByChunk ), seed( _seed )
{

    ///petit compteur pour gerer mieux les barres de chargement :
    int compteur = 0;

    ///Initialisation des programes :
    cubeProgram = Cube_program(applicationPath);
    cubeLightProgram = CubeMultiLightProgram(applicationPath);

    textureCubes = _textureCube;

    ///Je defini ou vont se situer les grottes :
    int decalInterStrates = ( (nbChunk_y-1) * nbCubeByRowByChunk) / (nbStrates);
    for(int i=0; i<nbStrates; i++)
    {
        hauteurStrates.push_back(decalInterStrates*i-8 - 2*nbCubeByRowByChunk );
    }

    ///Tableau des cubes surfaciques :
    /// Un tableau de pointeur vers les cubes se trouvant en surface. Il me sert pour planter enssuite les arbres et faire un point de spawn pour le joueur
    Cube **tabCubeSurface = new Cube*[nbChunk_x*nbCubeByRowByChunk*nbChunk_z*nbCubeByRowByChunk];
    for(int i=0; i<nbChunk_x*nbCubeByRowByChunk*nbChunk_z*nbCubeByRowByChunk; i++)
    {
        tabCubeSurface[i] = NULL;
    }


    Chunk* temp_chunk_ptr;
    for(int k = 0, l=0; k< nbChunk_y; k++)
    {
        for(int j = 0; j< nbChunk_z; j++)
        {
            for(int i = 0; i< nbChunk_x; i++, l++)
            {

                if(k>nbChunk_y-3 && k<=nbChunk_y-1)
                {
                    temp_chunk_ptr = new Chunk( nbTexturecube,  nbCubeByRowByChunk,  i*nbCubeByRowByChunk, k*nbCubeByRowByChunk, j*nbCubeByRowByChunk );
                }
                else if(k==nbChunk_y-3)
                {
                    temp_chunk_ptr = new Chunk(tabCubeSurface, applicationPath, nbTexturecube, nbCubeByRowByChunk, nbChunk_x * nbCubeByRowByChunk, 8, 0.5, 128, seed, 0, fmax(nbCubeByRowByChunk - 5, 0), true, hauteurStrates, i*nbCubeByRowByChunk, k*nbCubeByRowByChunk, j*nbCubeByRowByChunk );
                }
                else
                {
                    temp_chunk_ptr = new Chunk(tabCubeSurface, applicationPath, nbTexturecube, nbCubeByRowByChunk, nbChunk_x * nbCubeByRowByChunk, 8, 0.5, 128, seed, 0, fmax(nbCubeByRowByChunk - 5, 0), false, hauteurStrates, i*nbCubeByRowByChunk, k*nbCubeByRowByChunk, j*nbCubeByRowByChunk );
                }

                tab_Chunk.push_back( temp_chunk_ptr );
                if(compteur%nbChunk_x == 0 && visualLoad != NULL && windowManager!=NULL)
                visualLoad->Draw(((float)(k*nbChunk_x*nbChunk_z + j*nbChunk_z + i)/(float)(2.f*nbChunk_x*nbChunk_z*nbChunk_z)), *windowManager, 0);

                compteur++;

//                if(l%5==0)
//                std::cout<<"chargement : "<<l<<"/"<<nbChunk_x*nbChunk_y*nbChunk_z<<std::endl;
            }
        }
    }


    spawnAllTrees(tabCubeSurface);

    for(int k = 0, l=0; k< nbChunk_y; k++)
    {
        for(int j = 0; j< nbChunk_z; j++)
        {
            for(int i = 0; i< nbChunk_x; i++, l++)
            {
                tab_Chunk[k*nbChunk_x*nbChunk_z + j*nbChunk_z + i ]->updateChunkVBO();
                if(compteur%nbChunk_x == 0 && visualLoad != NULL && windowManager!=NULL)
                visualLoad->Draw( ((float)((k*nbChunk_x*nbChunk_z + j*nbChunk_z + i) + (float)(nbChunk_x*nbChunk_z*nbChunk_z) )/(float)(2.f*nbChunk_x*nbChunk_z*nbChunk_z)) , *windowManager, 0);

                compteur++;

//                if(l%5==0)
//                std::cout<<"chargement02 : "<<l<<"/"<<nbChunk_x*nbChunk_y*nbChunk_z<<std::endl;
            }
        }
    }

//    std::cout<<"rechercher position spawn"<<std::endl;
    int randSpawn = rand()%(nbChunk_x*nbCubeByRowByChunk*nbChunk_z*nbCubeByRowByChunk - 1);
    positionSpawn = tabCubeSurface[randSpawn]->getPosition()+glm::vec3(0,1,0);
//    std::cout<<" position spawn trouvée"<<std::endl;


}








Map::Map( const glimac::FilePath& applicationPath, int nbTexturecube, int _nbChunk_x, int _nbChunk_y, int _nbChunk_z, int _nbCubeByRowByChunk, int _seed, ParticuleManager& particuleManager, VisualLoad *visualLoad, glimac::SDLWindowManager *windowManager): nbChunk_x( _nbChunk_x ), nbChunk_y( _nbChunk_y ), nbChunk_z( _nbChunk_z ), nbCubeByRowByChunk( _nbCubeByRowByChunk ), seed( _seed )
{
    int compteur = 0;

    ///Initialisation des programes :
    cubeProgram = Cube_program(applicationPath);
    cubeLightProgram = CubeMultiLightProgram(applicationPath);


    //affichage barre de progression :
    if(visualLoad != NULL && windowManager!=NULL)
    visualLoad->Draw(((float)(compteur)/(float)(nbChunk_x*nbChunk_z*nbChunk_z)), *windowManager, 0);


    TiXmlDocument doc("save.xml");            //chargement du doc xml
    if(!doc.LoadFile()){
        std::cerr << "erreur lors du chargement" << std::endl;
        std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
        exit(1);
    }


    TiXmlHandle hdl(&doc);

    TiXmlElement *player = hdl.FirstChildElement().FirstChild("Player").Element();    //premier noeud player
    if(!player){
        std::cerr << "le noeud à atteindre Player n'existe pas" << std::endl;
        exit(1);
    }

    glm::vec3 posPlayer;
    player->QueryFloatAttribute("PositionX", &posPlayer.x);
    player->QueryFloatAttribute("PositionY", &posPlayer.y);
    player->QueryFloatAttribute("PositionZ", &posPlayer.z);

    setPositionSpawn(posPlayer);

    TiXmlElement *map = hdl.FirstChildElement().FirstChild( "Map" ).Element();   //noeud map
    if(!map){
        std::cerr << "le noeud à atteindre Map n'existe pas" << std::endl;
        exit(1);
    }



    TiXmlElement *chunkElem = map->FirstChildElement("Chunk");    //premier noeud chunk
    if(!chunkElem){
        std::cerr << "le noeud à atteindre Chunk n'existe pas" << std::endl;
        exit(1);
    }

    std::vector<Cube*> tabCube;
    std::vector<Chunk*> tabChunk;


    while(chunkElem)      //tant qu'il y a des chunks
    {
        if(visualLoad != NULL && windowManager!=NULL)
        visualLoad->Draw(((float)(compteur)/(float)(nbChunk_x*nbChunk_z*nbChunk_z)), *windowManager, 0);


        int posX, posY, posZ, nbCubeAttr, nbCubeAffAttr, nbTextCubeAttr, test=1;

        //récupération des variables du chunk dans le xml
        chunkElem->QueryIntAttribute("PositionX", &posX);
        chunkElem->QueryIntAttribute("PositionY", &posY);
        chunkElem->QueryIntAttribute("PositionZ", &posZ);

        chunkElem->QueryIntAttribute("NbCube", &nbCubeAttr);
        chunkElem->QueryIntAttribute("NbCubeAff", &nbCubeAffAttr);
        chunkElem->QueryIntAttribute("NbTextureCube", &nbTextCubeAttr);

        //création chunk avec var récupérées précédemment
        Chunk* ch = new Chunk(nbTextCubeAttr, nbCubeAttr, posX, posY, posZ, test);
        //ch->setNbCube(nbCubeAttr);
        ch->setNbCubeAff(nbCubeAffAttr);
        //ch->setNbTextureCube(nbTextCubeAttr);


        TiXmlElement *cubeElem = chunkElem->FirstChildElement("Cube");  //premier noeud cube
        if(cubeElem!=NULL){
              const char* attr; //permet de savoir si un cube est NULL ou non

              while(cubeElem)   //tant qu'il existe des noeuds cube dans le chunk
              {
                  attr = cubeElem->Attribute("NULL");  //récup de l'attirbut NULL
                  if(attr!=NULL)   //si attr renvoie autre chose que NULL (si l'attribut NULL existe), alors le cube est NULL
                  {
                      tabCube.push_back(NULL);
                  }
                  else{
                      int cubePosX, cubePosY, cubePosZ, IDText;

                      //récup des var du cube dans le xml
                      cubeElem->QueryIntAttribute("PositionX", &cubePosX);
                      cubeElem->QueryIntAttribute("PositionY", &cubePosY);
                      cubeElem->QueryIntAttribute("PositionZ", &cubePosZ);
                      cubeElem->QueryIntAttribute("IDText", &IDText);

                      //création du cube
                      Cube* cu = new Cube(glm::vec3(cubePosX, cubePosY, cubePosZ) );

                      cu->setTextureIndice(IDText);
                      if(cu->getType() == TYPE_CUBE::BASE_FEU01)
                      {
                          cu->setEmitter( particuleManager, applicationPath, applicationPath.dirPath()+"assets/textures/fire_unity.png" , TYPE_CUBE::BASE_FEU01);

                      }

                    //  std::cout<< "indice : "<<IDText<<std::endl;

                      tabCube.push_back(cu);


                  }
                  cubeElem = cubeElem->NextSiblingElement();  //iteration (prochain cube)

                  if(compteur%1000==0 && visualLoad != NULL && windowManager!=NULL)
                  {
                      visualLoad->Draw(((float)(compteur)/(float)(nbChunk_x*nbChunk_z*nbChunk_z)), *windowManager, 0);
                  }
                  compteur++;

            }
            ch->setTabCube(tabCube);   //on met le tableau de cube dans le chunk

            for(int i=0; i<tabCube.size();i++){
                tabCube[i]=NULL;
            }
            tabCube.clear();
        }

        tabChunk.push_back(ch);

        chunkElem = chunkElem->NextSiblingElement(); // iteration (prochain chunk)
    }
    setTabChunk(tabChunk);  //on met le tableau de chunk dans la map


    for(int i=0; i<tabChunk.size();i++){
        tabChunk[i]=NULL;
    }

    for(int i=0;i<tab_Chunk.size();i++){
        tab_Chunk[i]->updateChunkVBO();

    }
}










void Map::updateTarget(glm::vec3 position)
{
    Chunk* activeChunk = NULL;

    for(int i=0; i<tabHover.size(); i++)
    {
        activeChunk = getActiveChunk_ptr( tabHover[i]->getPosition() );
        activeChunk->updateTarget( getPositionInActiveChunk( tabHover[i]->getPosition()), false );
    }
    tabHover.clear();

    activeChunk = getActiveChunk_ptr( position);
    Cube* newTarget = activeChunk->updateTarget(getPositionInActiveChunk( position), true);
    //Cube* newTarget = NULL;
    //activeChunk->updateTarget(getPositionInActiveChunk( position).x, getPositionInActiveChunk( position).z, getPositionInActiveChunk( position).y);
    if(newTarget != NULL)
    {
       tabHover.push_back(newTarget);
    }

}



/*
void Map::Draw(glm::mat4 const& Projection, glm::mat4 const& View)
{
    for(int k = 0, l=0; k< nbChunk_y; k++)
    {
        for(int j = 0; j< nbChunk_z; j++)
        {
            for(int i = 0; i< nbChunk_x; i++, l++)
            {
                tab_Chunk[l]->Draw( Projection,  View);
            }
        }
    }

}*/

void Map::spawnTree(glm::vec3 globalPosition ,int textureId_feuille, int textureId_tronc)
{

    int taille = rand()%2 + 8;
    int sommetPosition = globalPosition.y;
    Chunk* activeChunk = getActiveChunk_ptr(globalPosition);
    glm::vec3 origineChunk = getActiveChunkPosition(globalPosition);
    glm::vec3 relativePosition = getPositionInActiveChunk(globalPosition);
    Cube *newCube = NULL;
//    std::cout<<"test"<<std::endl;

    for(int /*k=relativePosition.y,*/ l=0; l<taille; /*k++,*/ l++)
    {
        activeChunk = getActiveChunk_ptr(globalPosition + glm::vec3(0,l,0));
        origineChunk = getActiveChunkPosition(globalPosition + glm::vec3(0,l,0));
        relativePosition = getPositionInActiveChunk(globalPosition + glm::vec3(0,l,0));


//        if(k<0 || k>=nbCubeByRowByChunk*nbChunk_y)
//            continue;
        if(activeChunk == NULL)
            continue;

        sommetPosition++;

        newCube = new Cube( glm::vec3(globalPosition.x, globalPosition.y + l, globalPosition.z) );
        newCube->setTextureIndice(textureId_tronc);

        activeChunk->init_addNewCube( newCube, relativePosition.y*nbCubeByRowByChunk*nbCubeByRowByChunk + relativePosition.z*nbCubeByRowByChunk + relativePosition.x);

//        std::cout<<"test"<<std::endl;
    }

    for(int k=sommetPosition-1; k>=fmax(sommetPosition-4, 0); k--)
    {
        if(k<0 || k>=nbCubeByRowByChunk*nbChunk_y)
            continue;
        for(int j=-2 + globalPosition.z; j<=2  + globalPosition.z; j++)
        {
            if(j<0 || j>=nbCubeByRowByChunk*nbChunk_z)
                continue;
            for(int i=-2 + globalPosition.x; i<=2 + globalPosition.x; i++)
            {
                if(i<0 || i>=nbCubeByRowByChunk*nbChunk_x)
                    continue;

                glm::vec3 newGlobalPosition =  glm::vec3(i,k,j);
                activeChunk = getActiveChunk_ptr(newGlobalPosition);
                origineChunk = getActiveChunkPosition(newGlobalPosition);
                relativePosition = getPositionInActiveChunk(newGlobalPosition);

                if(activeChunk == NULL)
                {
                    std::cout<<"probleme : Chunk nulle ! "<<std::endl;
                     continue;
                }


                if(i!= globalPosition.x || j!=  globalPosition.z)
                {
//                     std::cout<<"test"<<std::endl;
                    newCube = new Cube( newGlobalPosition );
                    newCube->setTextureIndice(textureId_feuille);

                    activeChunk->init_addNewCube( newCube, relativePosition.y*nbCubeByRowByChunk*nbCubeByRowByChunk + relativePosition.z*nbCubeByRowByChunk + relativePosition.x);
                }

            }
        }
    }

}

void Map::spawnAllTrees(Cube** tabCubeSurfaciques)
{
    ///Rappel des liens des textures :
    int textureBois = 3;
    int textureFeuille = 2;

//    std::cout<<"entree dans la fonction de creation des arbres..."<<std::endl;
    int hauteur = 0;
    Perlin bruitPerlin_biome(nbChunk_x*nbCubeByRowByChunk, nbChunk_x*nbCubeByRowByChunk, 16, 1, 0.99, seed, 0 , 4 );

    int typeBiome = 0;
    for(int i=0; i<nbCubeByRowByChunk*nbChunk_x; i++)
    {
        for(int j=0; j<nbCubeByRowByChunk*nbChunk_z; j++)
        {
            if(tabCubeSurfaciques[j*nbCubeByRowByChunk*nbChunk_z + i] == NULL)
            {
                std::cout<<"hahah y'a un probleme dans ton code ! "<<std::endl;
                continue;
            }

            hauteur = tabCubeSurfaciques[j*nbCubeByRowByChunk*nbChunk_z + i]->getPosition().y;

            typeBiome = (int)bruitPerlin_biome.bruit_coherent2D( (double)i, (double)j);
            //typeBiome = typeBiome%MAX_BIOME; ///On s'assure que la valeur du biome ne dépasse pas le nombre de biome max.

            ///Gestion des biomes :

            ///Spawn aléatoire des arbres sur le bon biome :
            int test_spawnTree = rand()%100;
//            std::cout<<"resultat rend + typeBiome = "<<test_spawnTree<<" "<<typeBiome<<std::endl;
            if(typeBiome == 0 && test_spawnTree<20 )
            {
                ///Test de proximité avec les autres arbres de la surface :
                bool bool_testArbre = true;
                for(int a = -6; a<6; a++)
                {
                    if(i + a < 0 || i + a >=nbCubeByRowByChunk*nbChunk_x)
                        continue;

                    for(int b=-6; b<6; b++)
                    {
                        if(j + b < 0 || j + b >=nbCubeByRowByChunk*nbChunk_z)
                            continue;

                        glm::vec3 globalPosition =  glm::vec3(i + a, hauteur +1 , j + b);
                        Chunk* activeChunk = getActiveChunk_ptr(globalPosition);
                        glm::vec3 origineChunk = getActiveChunkPosition(globalPosition);
                        glm::vec3  relativePosition = getPositionInActiveChunk(globalPosition);
                        if(activeChunk == NULL)
                            continue;

                        if(activeChunk->getCube(relativePosition)!=NULL)
                        {
                            if(activeChunk->getCube(relativePosition)->getTextureIndice() == textureBois )
                            {
                                bool_testArbre = false;
                            }
                        }
                    }
                }

                if(bool_testArbre)
                {
                    ///Spawn de l'arbre :
//                    std::cout<<"Spawn d'un arbre...' "<<std::endl;
                    spawnTree(glm::vec3(i,hauteur+1,j) , textureFeuille, textureBois);
                }

            }
        }
    }

//    std::cout<<"sortie de la fonction de creation des arbres..."<<std::endl;

}

void Map::Draw(glm::mat4 const& Projection, glm::mat4 const& View, glm::vec3 position_chunk, GLuint textureCube)
{
    cubeProgram.program.use();
    glm::mat4 model_matrix =  glm::mat4(1.0f);
    glUniformMatrix4fv(cubeProgram.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniformMatrix4fv(cubeProgram.uniformID_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View));
    glUniformMatrix4fv(cubeProgram.uniformID_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection));
    glUniform1i(cubeProgram.uniformID_uTexture, 0 );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureCube);
    for(int k = position_chunk.y-Config::DOF, l=0; k<= position_chunk.y+Config::DOF; k++)
    {
        if(k<0 || k> nbChunk_y-1)
        continue;

        for(int j = position_chunk.z-Config::DOF; j<= position_chunk.z+Config::DOF; j++)
        {
            if(j<0 || j> nbChunk_z-1)
            continue;

            for(int i = position_chunk.x-Config::DOF; i<= position_chunk.x+Config::DOF; i++, l++)
            {
                if(i<0 || i> nbChunk_x-1)
                continue;


                tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->Draw( Projection,  View, textureCube);
            }
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}


//ancienne version du draw :
void Map::Draw(glm::mat4 const& Projection, glm::mat4 const& View, glm::vec3 position_chunk, GLuint textureCube, glm::vec3 forward)
{

        cubeProgram.program.use();

        glm::mat4 model_matrix =  glm::mat4(1.0f);
        glUniformMatrix4fv(cubeProgram.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(cubeProgram.uniformID_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(cubeProgram.uniformID_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection));
        glUniform1i(cubeProgram.uniformID_uTexture, 0 );





    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureCube);
    for(int k = position_chunk.y-Config::DOF, l=0; k<= position_chunk.y+Config::DOF; k++)
    {
        if(k<0 || k> nbChunk_y-1)
        continue;

        for(int j = position_chunk.z-Config::DOF; j<= position_chunk.z+Config::DOF; j++)
        {
            if(j<0 || j> nbChunk_z-1)
            continue;

            for(int i = position_chunk.x-Config::DOF; i<= position_chunk.x+Config::DOF; i++, l++)
            {
                if(i<0 || i> nbChunk_x-1)
                continue;
                //retourne la position de la chunk - position du joueur
                glm::vec3 positionOther = glm::vec3 (tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->getPosition().x + nbCubeByRowByChunk/2, tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->getPosition().y + nbCubeByRowByChunk/2, tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->getPosition().z + nbCubeByRowByChunk/2  ) - glm::vec3(forward.x*10, forward.y*10, forward.z*10);
                glm::vec3 positionThis = glm::vec3(position_chunk.x + nbCubeByRowByChunk/2 , position_chunk.y + nbCubeByRowByChunk/2 , position_chunk.z + nbCubeByRowByChunk/2);
                glm::vec3 directionChunk = glm::abs(positionOther - positionThis);
                directionChunk = glm::normalize(directionChunk);
                float cos = glm::dot(forward, directionChunk);
                float angle = acos(glm::dot(forward, directionChunk));
                angle = abs( glm::degrees(angle) );
                if(angle<180 )
                tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->Draw( Projection,  View, textureCube);
            }
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}

//nouvelle version du draw de map (on doit donner la position du joueur et non celle de la chunk):
void Map::Draw(glm::mat4 const& Projection, glm::mat4 const& View, glm::vec3 position_joueur, GLuint textureCube, glm::vec3 forward, bool activeLight, LightManager& lightManager)
{
    glm::vec3 position_chunk = getActiveChunkPosition(position_joueur);

    ///Calcule des matrices :
    glm::mat4 model_matrix =  glm::mat4(1.0f);
    glm::mat4 MVMatrix = model_matrix;
    MVMatrix = View * model_matrix;
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));


    ///Choix du programe, si la lumiere est ou non affichée :
    if(activeLight)
    {
        cubeLightProgram.m_program.use();

        ///Variables uniformes :
        glUniformMatrix4fv(cubeLightProgram.uniformID_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(cubeLightProgram.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(cubeLightProgram.uniformID_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(cubeLightProgram.uniformID_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection));
        glUniformMatrix4fv(cubeLightProgram.uniformID_uMVP, 1, GL_FALSE, glm::value_ptr(Projection*View*model_matrix));
        glUniformMatrix4fv(cubeLightProgram.uniformID_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(cubeLightProgram.uniformID_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection));

        ///Proprietes du materiau :
        glUniform3f(cubeLightProgram.uniform_uKd, 0.2f,0.2f,0.2f);
        glUniform3f(cubeLightProgram.uniform_uKs, 0.2f,0.2f,0.2f);
        glUniform1f(cubeLightProgram.uniform_uShininess, 0.1f);

        ///Propriete de la lumiere :
        glUniform1i(cubeLightProgram.uniform_uNbPointLight, lightManager.getNbPointLight());
        glUniform1i(cubeLightProgram.uniform_uNbDirectionnalLight, lightManager.getNbDirectionnalLight());

        glUniform3fv(cubeLightProgram.uniform_uPointLightIntensity, lightManager.getNbPointLight()*3, lightManager.getPointLightIntentityTab_ptr());
        glUniform3fv(cubeLightProgram.uniform_uLightPos_vs, lightManager.getNbPointLight()*3, lightManager.getPointLightPositionTab_ptr() );

        glUniform3fv(cubeLightProgram.uniform_uDirectionnalLightIntensity, lightManager.getNbDirectionnalLight()*3, lightManager.getDirectionnalLightIntentityTab_ptr());
        glUniform3fv(cubeLightProgram.uniform_uLightDir_vs, lightManager.getNbDirectionnalLight()*3, lightManager.getDirectionnalLightDirectionTab_ptr()  );

        glUniform1i(cubeLightProgram.uniform_textureCube, 0 );

    }

    else
    {
        cubeProgram.program.use();

        //glm::mat4 model_matrix =  glm::mat4(1.0f);
        glUniformMatrix4fv(cubeProgram.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(cubeProgram.uniformID_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(cubeProgram.uniformID_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection));
        glUniform1i(cubeProgram.uniformID_uTexture, 0 );
    }




    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureCube);
    for(int k = position_chunk.y-Config::DOF, l=0; k<= position_chunk.y+Config::DOF; k++)
    {
        if(k<0 || k> nbChunk_y-1)
        continue;

        for(int j = position_chunk.z-Config::DOF; j<= position_chunk.z+Config::DOF; j++)
        {
            if(j<0 || j> nbChunk_z-1)
            continue;

            for(int i = position_chunk.x-Config::DOF; i<= position_chunk.x+Config::DOF; i++, l++)
            {
                if(i<0 || i> nbChunk_x-1)
                continue;

                //retourne la position de la chunk - position du joueur, normalisé
                glm::vec3 positionOther = glm::vec3 (tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->getPosition().x + nbCubeByRowByChunk/2.f, tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->getPosition().y + nbCubeByRowByChunk/2.f, tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->getPosition().z + nbCubeByRowByChunk/2.f  );// - glm::vec3(forward.x*10, forward.y*10, forward.z*10);
                glm::vec3 positionThis = position_joueur;
                glm::vec3 directionChunk = (positionOther - positionThis);
                directionChunk = glm::normalize(directionChunk);

                //Création d'un cone d'affichage grâce au produit scalaire entre le vecteur forward de la caméra et le vecteur position du chunk
                //On n'affiche que les chunks qui sont dans le cône
                float cos = glm::dot(directionChunk ,forward );
                if(cos>=0.3f || glm::length( (glm::vec3(i,k,j) - position_chunk) )<2 )
                tab_Chunk[k*nbChunk_z*nbChunk_x + j*nbChunk_z + i ]->Draw( Projection,  View, textureCube);
            }
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}

glm::vec3 Map::getActiveChunkPosition(glm::vec3 actorPosition)
{
    glm::vec3 positionchunk;
    positionchunk.x = actorPosition.x / nbCubeByRowByChunk;
    positionchunk.y= actorPosition.y / nbCubeByRowByChunk;
    positionchunk.z = actorPosition.z / nbCubeByRowByChunk;

    return positionchunk;
}

Chunk& Map::getActiveChunk(glm::vec3 actorPosition)
{
    glm::vec3 positionChunk = getActiveChunkPosition(actorPosition);
    return *tab_Chunk[(int)positionChunk.y*nbChunk_z*nbChunk_x + (int)positionChunk.z*nbChunk_z + (int)positionChunk.x ];
}

Chunk* Map::getActiveChunk_ptr(glm::vec3 actorPosition)
{
    glm::vec3 positionChunk = getActiveChunkPosition(actorPosition);
    return tab_Chunk[(int)positionChunk.y*nbChunk_z*nbChunk_x + (int)positionChunk.z*nbChunk_z + (int)positionChunk.x ];
}


glm::vec3 Map::getPositionInActiveChunk(glm::vec3 actorPosition)
{
    glm::vec3 positionInChunk;
    glm::vec3 activeChunkPosition = getActiveChunkPosition(actorPosition);
    positionInChunk = actorPosition - glm::vec3((int)activeChunkPosition.x*nbCubeByRowByChunk,(int)activeChunkPosition.y*nbCubeByRowByChunk,(int)activeChunkPosition.z*nbCubeByRowByChunk) ;
    return positionInChunk;
}
/*
void Map::Draw(Camera camera)
{
    for(int i = 0, l=0; i< nbChunk_x; i++)
    {
        for(int j = 0; j< nbChunk_y; j++)
        {
            for(int k = 0; k< nbChunk_z; k++, l++)
            {
                tab_Chunk[l]->Draw(camera);
            }
        }
    }

}*/

int Map::getNbCubeByRowByChunk() const
{
    return nbCubeByRowByChunk;
}

glm::vec3 Map::getNbChunk() const
{
    return glm::vec3(nbChunk_x, nbChunk_y, nbChunk_z);
}


std::vector<Chunk*> Map::getAllChunk() const/***/
{
    return tab_Chunk;
}

void Map::setTabChunk(std::vector<Chunk*> tabchunk){/**/
    tab_Chunk=tabchunk;
}

void Map::setPositionSpawn(glm::vec3 pos){
    positionSpawn.x=pos.x;
    positionSpawn.y=pos.y;
    positionSpawn.z=pos.z;
}
