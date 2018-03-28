#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "glimac/opengl_tool.h"
#include "glimac/perlin.h"



Perlin::Perlin(int _longueur, int _hauteur, int _pas, int _nombre_octave, int _persistance, int _seed, int mini, int maxi)
{
    seed = _seed;
	nombre_octaves = _nombre_octave;
	persistance = _persistance;

	longueur = _longueur;
	hauteur = _hauteur;
	pas = _pas;
	longueur_max = (int)ceil(longueur * pow(2, nombre_octaves - 1) / pas);
	int hauteur_max = (int)ceil(hauteur * pow(2, nombre_octaves - 1) / pas);

	//valeurs = new double[longueur_max * hauteur_max];
	srand(seed);

	for (int i = 0; i < longueur_max * hauteur_max; i++)
		valeurs.push_back( ((double) (rand()%(maxi - mini) + mini)) );


}

Perlin::~Perlin()
{

}


double Perlin::bruit2D(int i, int j)
{
	return valeurs[i * longueur_max + j];
}



double Perlin::fonction_bruit2D(double x, double y)
{
	int i = (int)(x / pas);
	int j = (int)(y / pas);
	return interpolation_cos2D(bruit2D(i, j), bruit2D(i + 1, j), bruit2D(i, j + 1), bruit2D(i + 1, j + 1), fmod( (x / pas), 1), fmod( (y / pas), 1 ) );
	// Évidemment, on peut changer d'interpolation.
}

double Perlin::bruit_coherent2D(double x, double y)
{

	double somme = 0;
	double p = 1;
	int f = 1;
	for (int i = 0; i < nombre_octaves; i++) {
		somme += p * fonction_bruit2D(x * f, y * f);
		p *= persistance;
		f *= 2;
	}

	return somme * (1 - persistance) / (1 - p);
}


void Perlin::setLongueurMax(int newL)
{
    longueur_max = newL;
}

int Perlin::getLongueurMax() const
{
    return longueur_max;
}
