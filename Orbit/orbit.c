//
// Torbert, 8 November 2018
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//
// https://physics.nist.gov/cgi-bin/cuu/Value?bg
//
#define G 5.674e-11 // (m^3)(kg^-1)(s^-2)
//
// https://ssd.jpl.nasa.gov/?planet_phys_par
//
#define M 5.972e+24 // kg
#define R 6.371e+6  // m
#define LUNAR_DIST 3.84402e+8//distance to moon (m)
#define LUNAR_M 7.34767309e+22//mass of mooon (kg)
//
// parameters
//
#define DT 0.25     // s
//
int main()
{
	//
	// time intervals - duration is 90 minutes
	//
	int n = (int)( 0.5 + ( 5 * 60 * 60 ) / DT ) ;
	//
	//////////////////////////////////////////////////
	//
	double*  t = (double*)malloc(sizeof(double)*n);
	double*  xM = (double*)malloc(sizeof(double)*n);
	double*  yM = (double*)malloc(sizeof(double)*n);
	double* vxM = (double*)malloc(sizeof(double)*n);
	double* vyM = (double*)malloc(sizeof(double)*n);

	double*  xA = (double*)malloc(sizeof(double)*n);
	double*  yA = (double*)malloc(sizeof(double)*n);
	double* vxA = (double*)malloc(sizeof(double)*n);
	double* vyA = (double*)malloc(sizeof(double)*n);
	//////////////////////////////////////////////////
	//
	FILE* fout   ;
	//
	int j        ;
	//
	double r , a ;
	//
	double m = 28801.0 ; // kg - Apollo 11
	//double m = 419725.0 ; // kg - ISS
	//
	//////////////////////////////////////////////////
	//
	// International Space Station
	//
	// https://www.nasa.gov/sites/default/files/atoms/files/np-2015-05-022-jsc-iss-guide-2015-update-111015-508c.pdf
	//
	// Page 54 - altitude : 370 km to 460 km
	// Page 54 - speed    : 28,000 km per hour
	//
	t[0]  = 0.0          ;
	xA[0]  = 0.0          ;
	yA[0]  = R + 400000.0 ;
	vxA[0] = 7777.777     ;
	vyA[0] =   0      ;
		
	xM[0]  = LUNAR_DIST  ;
	yM[0]  = 0 ;
	vxM[0] = 0     ;
	vyM[0] = sqrt(G*M/xM[0])      ;

	//vx[0] = sqrt(G*(M) / y[0]);
	//vx[0]=sqrt(G*M*(2.0/y[0]-1.0/(R*1.5)));
	//vx[0]=sqrt(G*M*(2.0/y[0]+1.0/(R*1.5)));
	
	printf("(x,y) = (%f,%f)\n", x[0], y[0]);	
	printf("(vx,vy) = (%f,%f)\n", vx[0], vy[0]);	
	//
	// hyperbolic changes both initial (x,y) and (vx,vy) - compare to circular
	//
	//////////////////////////////////////////////////
	//
	for( j = 1 ; j < n ; j ++ )
	{
		t[j] = t[j-1] + DT           ;
		x[j] = x[j-1] + DT * vx[j-1] ;
		y[j] = y[j-1] + DT * vy[j-1] ;
		//
		r = sqrt( x[j-1]*x[j-1] + y[j-1]*y[j-1] ) ;
		//
		a = G * M / ( r * r ) ;
		//
		vx[j] = vx[j-1] - a * ( x[j-1] / r ) * DT ;
		vy[j] = vy[j-1] - a * ( y[j-1] / r ) * DT ;
	}
	//
	//////////////////////////////////////////////////
	//
	fout = fopen( "orbit.txt" , "w" ) ;
	//
	for( j = 0 ; j < n ; j ++ )
	{
		fprintf( fout , "%d %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f\n" ,
		j , t[j] , x[j] , y[j] , vx[j] , vy[j] , sqrt( vx[j]*vx[j] + vy[j]+vy[j] ), sqrt( x[j]*x[j] + y[j]*y[j] ) ) ;
	}
	//
	fclose( fout ) ;
	//
	//////////////////////////////////////////////////
	//
	fout = fopen( "earth.txt" , "w" ) ;
	//
	for( j = 0 ; j <= 360 ; j ++ )
	{
		fprintf( fout , "%0.16f %0.16f\n" , R * cos( j * M_PI / 180.0 ) , R * sin( j * M_PI / 180.0 ) ) ;
	}
	//
	fclose( fout ) ;
	//
	// done
	//
	return 0 ;
}
//
// end of file
//
