#include <cstdlib>

#include "glimac/config.hpp"

int Config::DOF = 2;
Config::Config()
{

}
void Config::setConfig(int _DOF)
{
    DOF = _DOF;
}

void Config::setMagFilter(int qualitee)
{
    if(qualitee < 0 || qualitee >1)
    {
        magFilterQualitee = 0;
    }
    else
    {
        magFilterQualitee = qualitee;
    }

}

void Config::setMinFilter(int qualitee)
{
    if(qualitee < 0 || qualitee >1)
    {
        minFilterQualitee = 0;
    }
    else
    {
        minFilterQualitee = qualitee;
    }

}

int Config::getMinFilter() const
{
    return minFilterQualitee;
}

int Config::getMaxFilter() const
{
    return magFilterQualitee;
}

void Config::setMipMapFilter(bool hasMipMap)
{
    mipMapFilter = hasMipMap;
}

bool Config::getMipMapFilter() const
{
    return mipMapFilter;
}


