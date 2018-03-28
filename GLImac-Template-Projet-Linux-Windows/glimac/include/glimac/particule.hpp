#ifndef PARTICULE_ONCE
#define PARTICULE_ONCE

#include "glm.hpp"
#include <vector>
#include <cstdlib>



struct Particle{
protected:

	glm::vec3 m_pos, m_speed;
	unsigned char m_r, m_g, m_b, m_a;
	float m_size, m_angle, m_weight;
	float m_life;
	float m_cameraDistance;
    //std::vector<Particle*> m_tabParticules;
    int m_indexParticule; //particule physique
    glm::vec3 m_initialSpeed, m_initialPosition;
    float initSize, initMaxLife;
public:
	Particle();
    ~Particle();
	//getters :
	float getLife() const;
	glm::vec4 getColor() const;
	float getSize() const;
	float getAngle() const;
	float getWeight() const;
	glm::vec3 getSpeed() const;
	glm::vec3 getPosition() const;
	float getCameraDistance() const;
	int getIndexParticule() const;
    glm::vec3 getInitialSpeed()const;
    glm::vec3 getInitialPosition()const;
    float getInitMaxLife() const;
    float getInitSize() const;




	//setters :
	void setLife(float life);
	void setColor(glm::vec4 color);
	void setSize(float size);
	void setAngle(float angle);
	void setWeight(float weight);
	void setSpeed(glm::vec3 speed);
	void setPosition(glm::vec3 position);
	void setcameradistance(float distance);
	void setIndexParticule(int newIndex);
    void setInitialSpeed(glm::vec3 newInitSpeed);
    void setInitialPosition(glm::vec3 newInitPos);
    void setInitMaxLife(float newMaxLife);
    void setInitSize(float newSize);


	//Si les particules sont "physiques"
//	void sortParticule();
//	void sortParticule(int indexA, int indexB);
    //void ajouterParticuleVoisine(Particle* particule);
//	bool compare(Particle& a, Particle& b);
//	void InfluenceOtherParticles();

    bool estPlusPetitQue(Particle const& other) const
    {
        // Sort in reverse order : far particles drawn first.
        return this->getCameraDistance() > other.getCameraDistance();

    }


    //surcharge operateurs :

//    bool operator<(Particle& other){
//        // Sort in reverse order : far particles drawn first.
//        return this->getCameraDistance() > other.getCameraDistance();

//    }
//    bool operator>(Particle& other){
//        // Sort in reverse order : far particles drawn first.
//        return this->getCameraDistance() < other.getCameraDistance();

//    }


};

bool operator< (Particle const& p1, Particle const& p2);

bool sortFunct(const Particle* a, const Particle* b);




#endif
