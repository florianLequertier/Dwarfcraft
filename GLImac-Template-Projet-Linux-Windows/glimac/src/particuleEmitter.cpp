#include <cstdlib>
#include <GL/glew.h>
#include <glimac/glm.hpp>
#include <algorithm> //std::sort
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <cmath>

#include <glimac/particuleEmitter.hpp>
#include <glimac/particule.hpp>

/*
    ParticuleManager :
    Contient tout ce qui permet des simuler et afficher un grand nombre de Particules.
    Contient un container stockant les particules : m_particlesContainer
    Contient les VBOs :
        -de position et tailles : m_particuleData_positionSize et son identifiant : m_vertexBuffer_positionParticule
        -de couleur : m_particuleData_color et son identifiant : m_vertexBuffer_colorParticule
        -de forme : m_g_vertex_buffer_data et son identifiant : m_vertexBuffer_modelParticule

    remarque : m_newParticules sert juste à determiner combiens de particules vont être afficher en un tour de boucle.
*/


ParticuleEmitter::ParticuleEmitter( const glimac::FilePath& _path, std::string texturePath, TYPE_EMISSION _type):  m_newParticules(0), m_lastUsedParticle(0), m_particuleCount(0), nbNewParticulePre(0.f)
{
    typeEmitter = _type;
    path = _path;
    //shaderProgram = Particule_program(path);
    textureID = glimac::LoadImageFromFile(texturePath);

    m_particlesContainer = new Particle* [MAXPARTICULE];
    for (int i = 0; i < MAXPARTICULE; i++)
    {
        m_particlesContainer[i] = new Particle();

        m_particlesContainer[i]->setLife(-1.0f);
        m_particlesContainer[i]->setcameradistance(-1.0f);
        m_particlesContainer[i]->setColor(glm::vec4(0, 255, 0, 1));
        m_particlesContainer[i]->setAngle((rand()%200)/100.f);
        glm::vec2 posInit(glm::diskRand(4.f));
        m_particlesContainer[i]->setInitialPosition(glm::vec3(posInit.x, 0, posInit.y) + position);
        glm::vec2 speedInit(glm::diskRand(4.f));
        m_particlesContainer[i]->setInitialSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
        m_particlesContainer[i]->setWeight(1.f);

        m_particlesContainer[i]->setSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
        m_particlesContainer[i]->setPosition(glm::vec3(posInit.x, 0, posInit.y) + position);
        m_particlesContainer[i]->setSize(0.f);
        m_particlesContainer[i]->setInitSize(1.f);

    }

    m_particuleData_color = new GLubyte[MAXPARTICULE * 4];
    m_particuleData_positionSize = new GLfloat[MAXPARTICULE * 4];

    /*
    for (int i = 0; i<MAXPARTICULE; i++){
        m_particlesContainer[i]->setLife(-1.0f);
        m_particlesContainer[i]->setcameradistance(-1.0f);
        m_particlesContainer[i]->setColor(glm::vec4(0, 255, 0, 1));
    }*/

    //initiaisation des VBOs stockés dans l'objet :
    initialyseVBOs();
    initialyseVAO();
}

ParticuleEmitter::ParticuleEmitter( const glimac::FilePath& _path, TYPE_EMISSION _type) : m_newParticules(0), m_lastUsedParticle(0), m_particuleCount(0), nbNewParticulePre(0.f)
{
    typeEmitter = _type;
    path = _path;
    //shaderProgram = Particule_program(path);

    m_particlesContainer = new Particle* [MAXPARTICULE];
    for (int i = 0; i < MAXPARTICULE; i++)
    {
        m_particlesContainer[i] = new Particle();

    }

    m_particuleData_color = new GLubyte[MAXPARTICULE * 4];
    m_particuleData_positionSize = new GLfloat[MAXPARTICULE * 4];

    for (int i = 0; i<MAXPARTICULE; i++){
        m_particlesContainer[i]->setLife(-1.0f);
        m_particlesContainer[i]->setcameradistance(-1.0f);
        m_particlesContainer[i]->setColor(glm::vec4(0, 255, 0, 1));
    }

    //initiaisation des VBOs stockés dans l'objet :
    initialyseVBOs();
    initialyseVAO();
}

ParticuleEmitter::~ParticuleEmitter()
{
    for(int i=0; i<MAXPARTICULE; i++)
    {
        delete(m_particlesContainer[i]);
    }
    delete(m_particlesContainer);

    delete(m_particuleData_color);
    delete(m_particuleData_positionSize);

    glDeleteBuffers(1, &m_vertexBuffer_colorParticule);
    glDeleteBuffers(1, &m_vertexBuffer_positionParticule);
    glDeleteBuffers(1, &m_vertexBuffer_modelParticule);
    glDeleteVertexArrays(1, &vao);
}


/*
    Cette fonction est à appeler à chaque frame et gére l'animation du sytème de particule (vie et mort des particules, simulation physique...)
*/
void ParticuleEmitter::UpdateEmitter(float delai, glm::vec3 cameraPosition)
{
    int nombreNouvellesParticules;
    if(typeEmitter == ParticuleEmitter::TYPE_EMISSION::FIRE)
    nombreNouvellesParticules = 10;
    else if(typeEmitter == ParticuleEmitter::TYPE_EMISSION::FONTAINE)
    nombreNouvellesParticules = 5000;

    if(typeEmitter == ParticuleEmitter::TYPE_EMISSION::FONTAINE)
    {
        createNewParticules(delai, nombreNouvellesParticules);
        simulateParticules(delai, cameraPosition);
    }
    else if(typeEmitter == ParticuleEmitter::TYPE_EMISSION::FIRE)
    {
        createNewParticules_fire(delai, nombreNouvellesParticules);
        simulateParticules_fire(delai, cameraPosition);
    }

}

/*
    Cette fonction est à appeler à chaque frame et gére l'animation du sytème de particule (vie et mort des particules, simulation physique...)
*/
void ParticuleEmitter::UpdateEmitter(float delai, glm::vec3 cameraPosition, int nombreNouvellesParticules, ParticuleEmitter::TYPE_EMISSION type)
{

    if(type == ParticuleEmitter::TYPE_EMISSION::FONTAINE)
    {
        createNewParticules(delai, nombreNouvellesParticules);
        simulateParticules(delai, cameraPosition);
    }
    else if(type == ParticuleEmitter::TYPE_EMISSION::FIRE)
    {
        createNewParticules_fire(delai, nombreNouvellesParticules);
        simulateParticules_fire(delai, cameraPosition);
    }

}

/*
    Definie le nombre de particule à faire apparaitre en un tour de boucle, en fonction du framerate.
*/

void ParticuleEmitter::setNewParticules(float deltaTime, int nombreNouvellesParticules)
{

    //float baseNumberf = baseNumber/1000.f;
    m_newParticules = (int)(deltaTime*nombreNouvellesParticules);
    if(m_newParticules<1)
    {
        nbNewParticulePre += (deltaTime*(float)nombreNouvellesParticules);
        if(nbNewParticulePre>2.f)
        {
           m_newParticules = (int)nbNewParticulePre;
           nbNewParticulePre = 0.f;
        }
    }
    else if (m_newParticules > (int)(0.016f*nombreNouvellesParticules))
        m_newParticules = (int)(0.016f*nombreNouvellesParticules);
}

//simple getter sur m_newParticule (nombre de particule à creer dans la boucle principale)
int ParticuleEmitter::getNewParticules() const
{
    return m_newParticules;
}



/*
    Créé des nouvelles particules, qui ne demandent enssuite plus qu'à être simulée avec simulateParticules.
*/

void ParticuleEmitter::createNewParticules(float delai, int nombreNouvellesParticules)
{

    //definie le nombre de particule à creer :
    setNewParticules(delai, nombreNouvellesParticules);

    for (int i = 0; i<m_newParticules; i++){
        int particleIndex = FindUnusedParticle();

        m_particlesContainer[particleIndex]->setLife(5.0f); // This particle will live 5 seconds.
        m_particlesContainer[particleIndex]->setcameradistance(-1.0f);
        m_particlesContainer[particleIndex]->setColor(glm::vec4(0, 255, 0, 1));
        m_particlesContainer[particleIndex]->setAngle((rand()%200)/100.f);
        glm::vec2 posInit(glm::diskRand(1.f));
        m_particlesContainer[particleIndex]->setInitialPosition(glm::vec3(posInit.x, 0, posInit.y) + position);
        glm::vec2 speedInit(glm::diskRand(1.f));
        m_particlesContainer[particleIndex]->setInitialSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
        m_particlesContainer[particleIndex]->setWeight(1.f);



        m_particlesContainer[particleIndex]->setSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
        m_particlesContainer[particleIndex]->setPosition(glm::vec3(posInit.x, 0, posInit.y) + position);


        // Very bad way to generate a random color
        m_particlesContainer[particleIndex]->setColor(glm::vec4(rand() % 256, rand() % 256, rand() % 256, (rand() % 256) / 3));

        m_particlesContainer[particleIndex]->setSize((rand() % 1000) / 2000.0f + 0.1f);

    }

}


void ParticuleEmitter::createNewParticules_fire(float delai, int nombreNouvellesParticules)
{

    //definie le nombre de particule à creer :
    setNewParticules(delai, nombreNouvellesParticules);

    for (int i = 0; i<m_newParticules; i++){
        int particleIndex = FindUnusedParticle();

        m_particlesContainer[particleIndex]->setLife(2.0f); //initialisation de la vie
        m_particlesContainer[particleIndex]->setInitMaxLife(2.0f);
        m_particlesContainer[particleIndex]->setcameradistance(-1.0f);
        m_particlesContainer[particleIndex]->setColor(glm::vec4(200, 0, 0, 1));
        //m_particlesContainer[particleIndex]->setAngle((rand()%200)/100.f);
        glm::vec2 posInit(glm::diskRand(0.5f));
        m_particlesContainer[particleIndex]->setInitialPosition(glm::vec3( posInit.x, 0.f, posInit.y ) + position);

        glm::vec2 speedInitDir(glm::diskRand(1.f));
        glm::vec2 speedInitAmp(glm::linearRand(1.f, 1.5f));
        glm::vec2 speedInit(glm::normalize(speedInitDir)*speedInitAmp );

        float tempAngle = atan2( speedInit.y, speedInit.x );
        m_particlesContainer[particleIndex]->setAngle(tempAngle);
        m_particlesContainer[particleIndex]->setInitialSpeed(glm::vec3(speedInit.x, 0.f, speedInit.y) );
        m_particlesContainer[particleIndex]->setWeight(1.f);



        m_particlesContainer[particleIndex]->setSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
        m_particlesContainer[particleIndex]->setPosition(glm::vec3(posInit.x, 0, posInit.y) + position);



        m_particlesContainer[particleIndex]->setColor(glm::vec4(0,0,0,0));

        m_particlesContainer[particleIndex]->setSize(0.f);//initialisation de la taille de la particule
        m_particlesContainer[particleIndex]->setInitSize(1.f);

    }

}


/*
    recherche d'une particule non vivante, si aucune particule n'est trouvé, renvoie la particule 0 = premiere particule simulée.
*/

int ParticuleEmitter::FindUnusedParticle(){

    for (int i = m_lastUsedParticle; i<MAXPARTICULE; i++){
        if (m_particlesContainer[i]->getLife() < 0){
            m_lastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i<m_lastUsedParticle; i++){
        if (m_particlesContainer[i]->getLife() < 0){
            m_lastUsedParticle = i;
            return i;
        }
    }

    return 0;
}

/*
    Fonction de trie permettant de trier les particule de m_particuleContainer en fonction de leur distance par rapport à la caméra.
*/

void ParticuleEmitter::SortParticles()
{
    std::sort(&m_particlesContainer[0], &m_particlesContainer[MAXPARTICULE-1], [](Particle* a, Particle* b)->bool { return a->getCameraDistance() > b->getCameraDistance(); } );
}


/*
    Joue une simulation basique (une seule force : la gravité) sur l'ensemble des particules.
    Remplis les VBOs des positions, tailles et couleurs.
*/
void ParticuleEmitter::simulateParticules(float delta, glm::vec3 cameraPosition)
{
    m_particuleCount = 0;

    for (int i = 0; i<MAXPARTICULE; i++){

        Particle& p = *m_particlesContainer[i]; // shortcut

        if (p.getLife() > 0.0f){

            // Decrease life
            p.setLife( p.getLife() - delta );
            if (p.getLife() > 0.0f){

                // Simulate simple physics : gravity only, no collisions
                p.setSpeed( p.getSpeed() + glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f );
                p.setPosition(p.getPosition() + p.getSpeed() * (float)delta);
                p.setcameradistance(glm::length2(cameraPosition - p.getPosition())); //ou lenght2 ?
                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                // Fill the GPU buffer
                m_particuleData_positionSize[4 * m_particuleCount + 0] = p.getPosition().x;
                m_particuleData_positionSize[4 * m_particuleCount + 1] = p.getPosition().y;
                m_particuleData_positionSize[4 * m_particuleCount + 2] = p.getPosition().z;

                m_particuleData_positionSize[4 * m_particuleCount + 3] = p.getSize();

                m_particuleData_color[4 * m_particuleCount + 0] = p.getColor().r;
                m_particuleData_color[4 * m_particuleCount + 1] = p.getColor().g;
                m_particuleData_color[4 * m_particuleCount + 2] = p.getColor().b;
                m_particuleData_color[4 * m_particuleCount + 3] = p.getColor().a;

                m_particuleCount++;
            }
            else{
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.setcameradistance( -1.0f );
            }



        }
    }

    //evite les erreurs d'affichage
    SortParticles();

    updateVBOs();

}


void ParticuleEmitter::simulateParticules_fire(float delta, glm::vec3 cameraPosition)
{
    float attenuation = 5.f;
    m_particuleCount = 0;

    for (int i = 0; i<MAXPARTICULE; i++){

        Particle& p = *m_particlesContainer[i]; // shortcut
        //std::cerr<<"cameraDistance : "<<p.getCameraDistance()<<std::endl;

        if (p.getLife() > 0.0f){

            // Decrease life
            p.setLife( p.getLife() - delta );
            if (p.getLife() > 0.0f){

                float AmpliForceVol = 10.f;

                // Simulate simple physics : gravity only, no collisions
                glm::vec3 newSpeed = glm::vec3(0.0f, 9.f, 0.0f) * (float)delta * (1.f/p.getWeight()) - 8.f*glm::vec3(std::cos(p.getAngle()),0,std::sin(p.getAngle())) * (float)delta * (1.f/p.getWeight());
                newSpeed = glm::vec3(newSpeed.x/attenuation, newSpeed.y/attenuation, newSpeed.z/attenuation);
                p.setSpeed( p.getSpeed() +  newSpeed);
                p.setPosition(p.getPosition() + p.getSpeed() * (float)delta);
                p.setcameradistance(glm::distance(cameraPosition, p.getPosition() )); //ou lenght2 ?

                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                //Si la particule a une durée de vie max de 5 ...
                float Ml = p.getInitMaxLife();
                p.setColor( glm::vec4(0,0,0, glm::lerp(0.f,200.f,  (float)std::fmax( 0.0f, (0.2f - (Ml - p.getLife())/Ml)/0.2f ) )  +  glm::lerp(200.f,0.f,  (float)(std::fmax( 0.0,  ((Ml - p.getLife())/Ml  - 0.8f)/0.2f )) )  ));
                //Si la particule a une taille normale de 4 ...
                float Ms = p.getInitSize();
                p.setSize( glm::lerp(0.f,Ms,  (float)std::fmax( 0.0f, (0.2f - (Ml - p.getLife())/Ml)/0.2f ) )  +  glm::lerp(Ms,0.f,  (float)(std::fmax( 0.0f,  ((Ml - p.getLife())/Ml  - 0.8f)/0.2f )) )  );

                // Fill the GPU buffer
                m_particuleData_positionSize[4 * m_particuleCount + 0] = p.getPosition().x;
                m_particuleData_positionSize[4 * m_particuleCount + 1] = p.getPosition().y;
                m_particuleData_positionSize[4 * m_particuleCount + 2] = p.getPosition().z;

                m_particuleData_positionSize[4 * m_particuleCount + 3] = p.getSize();

                m_particuleData_color[4 * m_particuleCount + 0] = p.getColor().r;
                m_particuleData_color[4 * m_particuleCount + 1] = p.getColor().g;
                m_particuleData_color[4 * m_particuleCount + 2] = p.getColor().b;
                m_particuleData_color[4 * m_particuleCount + 3] = p.getColor().a;

                m_particuleCount++;
            }
            else{
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.setcameradistance( -1.0f );
            }



        }
    }

    //evite les erreurs d'affichage
    SortParticles();

    updateVBOs();

}





/*
    Initialisation des VBOs du programme.
    Un VBO pour stocker le model : 4 point pour un rectangle, donc 4*3 = 12 floats,
    un VBO pour stocker les donnés de position et de taille des particules,
    un VBO pour stocker les donnés couleur des particules.
*/


void ParticuleEmitter::initialyseVBOs()
{
    //création des points du triangle :
    //position
    m_g_vertex_buffer_data[0] = 0.5f;
    m_g_vertex_buffer_data[1] = -0.5f;
    m_g_vertex_buffer_data[2] = 0.0f;
    //texCoord
    m_g_vertex_buffer_data[3] = 1.f;
    m_g_vertex_buffer_data[4] = 1.f;

    //position
    m_g_vertex_buffer_data[5] = -0.5f;
    m_g_vertex_buffer_data[6] = -0.5f;
    m_g_vertex_buffer_data[7] = 0.0f;
    //texCoord
    m_g_vertex_buffer_data[8] = 0.f;
    m_g_vertex_buffer_data[9] = 1.f;

    //position
    m_g_vertex_buffer_data[10] = 0.5f;
    m_g_vertex_buffer_data[11] = 0.5f;
    m_g_vertex_buffer_data[12] = 0.0f;
    //texCoord
    m_g_vertex_buffer_data[13] = 1.f;
    m_g_vertex_buffer_data[14] = 0.f;

    //position
    m_g_vertex_buffer_data[15] = -0.5f;
    m_g_vertex_buffer_data[16] = 0.5f;
    m_g_vertex_buffer_data[17] = 0.0f;
    //texCoord
    m_g_vertex_buffer_data[18] = 0.f;
    m_g_vertex_buffer_data[19] = 0.f;


    //creation du VBO du model de la particule :
    glGenBuffers(1, &m_vertexBuffer_modelParticule);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_modelParticule);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_g_vertex_buffer_data), m_g_vertex_buffer_data, GL_STATIC_DRAW);

    //creation du VBO contenant la position et la taille de la particule :
    glGenBuffers(1, &m_vertexBuffer_positionParticule);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_positionParticule);
    glBufferData(GL_ARRAY_BUFFER, MAXPARTICULE * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    //creation du VBO contenant les informatino de couleur de la particule :
    glGenBuffers(1, &m_vertexBuffer_colorParticule);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_colorParticule);
    glBufferData(GL_ARRAY_BUFFER, MAXPARTICULE * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

}

/*
    Met à jour les VBOs de position et de couleur.
    Cette fonction est utile car on dessine en stream les particules, on peut donc remplir rapidement les VBOs à la volée à une grande fréquence.
*/

void ParticuleEmitter::updateVBOs()
{
    //update des buffers de position et de couleurs :
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_positionParticule);
    glBufferData(GL_ARRAY_BUFFER, MAXPARTICULE * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particuleCount * 4 * sizeof(GLfloat), m_particuleData_positionSize);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_colorParticule);
    glBufferData(GL_ARRAY_BUFFER, MAXPARTICULE * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particuleCount * 4 * sizeof(GLubyte), m_particuleData_color);

    //initialyseVAO();
}

/*
    Initialise les VAO, en faisant le lien entre les variable d'entré des shader et les VBOs du programe
*/

void ParticuleEmitter::initialyseVAO()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //On lie ces VBO au VAO :
    //le VBO du model de la particule et des texture coordonnées (tableau de 4*3 + 4*2 floats) :
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_modelParticule);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
    //le VBO du model de la particule (tableau de 6*3 floats) :
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

    //le VBO des positions et tailles de toutes les particules (tableau de MAXPARTICULE * 4 floats) :
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_positionParticule);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //le VBO des couleurs de toutes les particules (tableau de MAXPARTICULE * 4 GLubytes) :
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_colorParticule);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);



    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}

//ancienne fonction de draw :

void ParticuleEmitter::drawParticules(glm::mat4 Projection, glm::mat4 View)
{
    //shaderProgram.program.use();

//    glm::vec3 CameraRight_worldspace = {View[0][0], View[1][0], View[2][0]};
//    glm::vec3 CameraUp_worldspace = {View[0][1], View[1][1], View[2][1]};

//    glUniform3f(shaderProgram.uniformID_CameraRight_worldspace, CameraRight_worldspace.x, CameraRight_worldspace.y, CameraRight_worldspace.z );
//    glUniform3f(shaderProgram.uniformID_CameraUp_worldspace, CameraUp_worldspace.x, CameraUp_worldspace.y, CameraUp_worldspace.z );
//    glUniformMatrix4fv(shaderProgram.uniformID_VP, 1, GL_FALSE, glm::value_ptr(Projection*View) );
//    glUniformMatrix4fv(shaderProgram.uniform_View, 1, GL_FALSE, glm::value_ptr(View) );


//    glUniform1i(shaderProgram.uniform_uTexture, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //On lie les VBO au VAO :
    //initialyseVAO();
    glBindVertexArray(vao);

    //On dessine les vertices :
    //Le premiere argument correspond à l'indice opengl,
    //le deuxieme au decalage à effectuer dans le VBO à chaque tour de boucle
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    //Cette fonction est equivalente à une boucle for(int i=0; i<particuleCount, i++) sur : glDrawArrays(GL_TRIANGLE_STRIP,0,4)
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_particuleCount);

    /*glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2); */

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

//nouvelle fonction de draw :
void ParticuleEmitter::drawParticules()
{


    //On lie les VBO au VAO :
    //initialyseVAO();
    glBindVertexArray(vao);

    //On dessine les vertices :
    //Le premiere argument correspond à l'indice opengl,
    //le deuxieme au decalage à effectuer dans le VBO à chaque tour de boucle
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    //Cette fonction est equivalente à une boucle for(int i=0; i<particuleCount, i++) sur : glDrawArrays(GL_TRIANGLE_STRIP,0,4)
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_particuleCount);

    /*glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2); */

    glBindVertexArray(0);


}




/*
Initialise le manager, en particulier, va initialiser tous les VBOs de celui-ci.
Usage déprécié : appeler cette fonction juste aprés la création de l'objet de type ParticuleManager
->Amélioration : Initialisation directement lors de l'instantiation de l'objet.
*/
void ParticuleEmitter::InitManager()
{
    initialyseVBOs();
}


//Un simple getter pour retourner le nombre de particule considérés comme "vivantes"
int ParticuleEmitter::getParticuleCount() const
{
    return m_particuleCount;
}

void ParticuleEmitter::setPosition(glm::vec3 newPosition)
{
    position = newPosition;
}

glm::vec3 ParticuleEmitter::getPosition() const
{
    return position;
}
glimac::FilePath ParticuleEmitter::getPath() const
{
    return path;
}
