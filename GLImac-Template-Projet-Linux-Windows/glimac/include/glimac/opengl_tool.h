#ifndef OPENGL_TOOL_ONCE
#define OPENGL_TOOL_ONCE

#include <iostream>
#include <cstdlib>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "glm.hpp"

static float PI = 3.1415;

/*Opengl_tool est l'espace de nommage permettant de regrouper toutes les fonctions/variables utiles pour la gestion
de l'opengl*/

namespace opengl_tool{

	/*enumeration des options à passer au VAO*/
	enum class opengl_option {OPENGL_POSITION, OPENGL_COLOR};

	/*charge deux shaders retourne l'id du programme*/
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

};


struct Vertex{
	glm::vec3 position;
	glm::vec3 color;

	Vertex() : position(0, 0, 0), color(0, 0, 0)
	{

	}

	Vertex(glm::vec3 pos, glm::vec3 col) : position(pos), color(col)
	{

	}

};


double interpolation_lineaire(double a, double b, double x);
double interpolation_cos(double a, double b, double x);
double interpolation_cos2D(double a, double b, double c, double d, double x, double y);

#endif
