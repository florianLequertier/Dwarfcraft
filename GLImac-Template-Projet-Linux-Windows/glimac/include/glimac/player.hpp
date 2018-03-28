#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <cstdlib>
#include "character.hpp"
#include "itemManager.hpp"
#include <glimac/particuleManager.hpp>
#include <glimac/cube.hpp>

class Player: public Character{
private:
float life;

public:
Player();
void swingSword(Map& environment, glimac::FilePath& filePath, ItemManager& itemManager, ParticuleManager& particuleManager);
void dropItem(Map& environment);
void dropItem(Map& environment, ParticuleManager& particuleManager, const glimac::FilePath& path, std::string texturePath, TYPE_CUBE _type = TYPE_CUBE::TERRE01 );
void updateTarget(Map& environnement);
float getLife();/**/
};


#endif // PLAYER_HPP_INCLUDED
