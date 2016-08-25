#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <fstream>

#define SIZE 168
const double e = 2.7182818;

using namespace std;

double theta[SIZE+3], feature[SIZE+3];

inline void Read_Parameters ( char* address ) 
{
	FILE* parameters = fopen ( address, "r" );
	for (int i = 0 ; i <= SIZE ; i++) {
		fscanf ( parameters, "%lf", theta + i );
	}
	fclose(parameters);
	return;
}

inline void Read_Features ( char* address ) 
{
	FILE* features = fopen ( address, "r" );
	for (int i = 1 ; i <= SIZE ; i++) {
		fscanf ( features, "%lf", feature + i ); 
	}
	fclose(features);
	return;
}

inline double hypothesis ( ) 
{
	double Z = 0;
	for (int i = SIZE ; i > 0 ; i++) {
		Z += ( feature[i] * theta[i] );
	}
	Z += feature[0];
	Z *= (-1);
	
	double exp = pow ( e, Z );
	return ( 1/(1+exp) ); 
}


int main ( int argc, char* argv[] ) 
{
	Read_Parameters ( argv[1] );

	Read_Features ( argv[2] );

	double hypo = hypothesis();
	if(hypo > 0.5) printf("Gradable Image\n");
	else printf("Non-gradable Image\n");
  return 0;
}

