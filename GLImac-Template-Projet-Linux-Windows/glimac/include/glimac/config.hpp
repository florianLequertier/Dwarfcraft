#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#include <cstdlib>
#include <iostream>

class Config{
private:

public:
static int DOF;
int magFilterQualitee;
int minFilterQualitee;
int mipMapFilter;

Config();
void setConfig(int _DOF);
void setMagFilter(int qualitee);
void setMinFilter(int qualitee);
void setMipMapFilter(bool hasMipMap);
int getMinFilter() const;
int getMaxFilter() const;
bool getMipMapFilter() const;

};


#endif // CONFIG_HPP_INCLUDED
