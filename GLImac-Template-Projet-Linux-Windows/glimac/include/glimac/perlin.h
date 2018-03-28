#ifndef PERLIN_ONCE
#define PERLIN_ONCE

#include <cstdlib>
#include <iostream>
#include <vector>

class Perlin{
private :
	int pas;
	int nombre_octaves ;
	int persistance;
	int hauteur;
	int longueur;
	int longueur_max;
	std::vector<double> valeurs;
	int seed;

public :

	Perlin(int _longueur, int _hauteur, int _pas, int _nombre_octave, int _persistance, int seed, int mini, int maxi );
	~Perlin();
	double bruit2D(int i, int j);
	double fonction_bruit2D(double x, double y);
	double bruit_coherent2D(double x, double y);

    void setLongueurMax(int newL);
    int getLongueurMax() const;


};



#endif
