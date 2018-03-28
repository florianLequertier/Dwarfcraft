#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP

#include <GL/glew.h>
#include "Program.hpp"
#include "FilePath.hpp"
#include "glm.hpp"
#include <cstdlib>
#include <string>
#include "light.hpp"
#include "directionnalLight.hpp"
#include "pointLight.hpp"


class LightManager{
private:
    int MaxLight;
    std::vector<DirectionnalLight*> tabDirectionnal;
    std::vector<PointLight*> tabPoint;
    float* PointlightIntensityTab;
    float* PointlightPositionTab;
    float* DirectionnalLightIntensityTab;
    float* DirectionnalLightDirectionTab;

public:
    LightManager(int _MaxLight);
    void addDirectionnalLight( DirectionnalLight* newLight);
    void addPointLight(PointLight* newLight);
    void computeLights(glm::mat4& View);
    void computeLights();
    int getNbPointLight() const;
    int getNbDirectionnalLight() const;
    float* getPointLightIntentityTab_ptr() const;
    float* getPointLightPositionTab_ptr() const;
    float* getDirectionnalLightIntentityTab_ptr() const;
    float* getDirectionnalLightDirectionTab_ptr() const;

};



#endif // LIGHTMANAGER_HPP
