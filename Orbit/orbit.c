//
// Torbert, 8 November 2018
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//
// https://physics.nist.gov/cgi-bin/cuu/Value?bg
//
#define G 6.674e-11 // (m^3)(kg^-1)(s^-2)
//
// https://ssd.jpl.nasa.gov/?planet_phys_par
//
#define M 5.972e+24 // kg
#define R 6.371e+6  // m
#define LUNAR_DIST 3.84402e+8//distance to moon (m)
#define LUNAR_M 7.34767309e+22//mass of mooon (kg)
#define LUNAR_R 1.737e+6
//
// parameters
//
#define DT 5     // s
//
int main()
{
	//
	// time intervals - duration is 90 minutes
	//
	int n = (int)( 0.5 + (10* 24 * 60 * 60 ) / DT ) ;
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
	////////////////////////////////////////////////// //
	FILE* fout   ;
	int j        ; 
	double rA, rM, aM, axA, ayA , a, magAM, magAE, rAM ; // 
	double m = 28801.0 ; // kg - Apollo 11 //
	//double m = 419725.0 ; // kg - ISS // 
	////////////////////////////////////////////////// // // International Space Station 
	// // https://www.nasa.gov/sites/default/files/atoms/files/np-2015-05-022-jsc-iss-guide-2015-update-111015-508c.pdf 
	// // Page 54 - altitude : 370 km to 460 km // Page 54 - speed    : 
	//28,000 km per hour // 
	//double v = 11080; orbit
	//double alt = R + 400000.0;
	//double angle = 37*M_PI/128; //orbit
	//double v = 10840;
	//angle = 86*M_PI/64;
	//v = sqrt(G*M/alt);
	
	double v = 1527.048;
	double angle = M_PI/6;	
	double alt = 202751774.4;	

	t[0]  = 0.0          ; 
	xA[0]  = alt*cos(angle); 
	yA[0]  = alt*sin(angle); 
	//vxA[0] = v*cos(angle+(M_PI/2))    ;
	//vyA[0] = v*sin(angle+(M_PI/2))      	;
	vxA[0] = v*cos(angle);
	vyA[0] = v*sin(angle);	

	xM[0]  = LUNAR_DIST  ;
	yM[0]  = 0 ;
	vxM[0] = 0     ;
	vyM[0] = sqrt(G*M/xM[0])      ;
	//vx[0] = sqrt(G*(M) / y[0]);
	//vx[0]=sqrt(G*M*(2.0/y[0]-1.0/(R*1.5)));
	//vx[0]=sqrt(G*M*(2.0/y[0]+1.0/(R*1.5)));
	printf("(x,y) = (%f,%f)\n", xA[0], yA[0]);	
	printf("(x,y) = (%f,%f)\n", vxA[0], vyA[0]);	
	//
	// hyperbolic changes both initial (x,y) and (vx,vy) - compare to circular
	//
	//////////////////////////////////////////////////
	//
	int burn = 1;
	for( j = 1 ; j < n ; j ++ )
	{
		t[j] = t[j-1] + DT           ;
		xM[j] = xM[j-1] + DT * vxM[j-1] ;
		yM[j] = yM[j-1] + DT * vyM[j-1] ;
		
		xA[j] = xA[j-1] + DT * vxA[j-1] ;
		yA[j] = yA[j-1] + DT * vyA[j-1] ;
		//
		rA = hypot( xA[j-1], yA[j-1]) ;
		rM = hypot( xM[j-1], yM[j-1]) ;
		rAM = hypot(xM[j-1]-xA[j-1], yM[j-1]-yA[j-1]);
		//
		aM = G * M / ( rM * rM ) ;
		magAE = G*M/(rA*rA);
		magAM = G*LUNAR_M/(rAM*rAM);
		axA = (-xA[j-1]/rA)*magAE + ((xM[j-1]-xA[j-1])/rAM)*magAM;
		ayA = (-yA[j-1]/rA)*magAE + ((yM[j-1]-yA[j-1])/rAM)*magAM;
		if((rAM < LUNAR_R) ||(rA < R)){
			printf("Apollo Crashed\n");
			n=j;
			break;
		}			
		if((fabs(rA-rM) < v*DT) && (burn==0)){
				printf("%f %f\n", magAE, magAM);
				double theta = atan2(yA[j-1]-yM[j-1], xA[j-1]-xM[j-1]);
				double phi = theta + M_PI/2;
				double speed = magAM*rAM/1000;
				printf("%f\n", speed);
				vxA[j] = speed*cos(phi) + vxM[j-1];
				vyA[j] = speed*sin(phi) + vyM[j-1];
				printf("%f %f %f %f\n", vxA[j], vyA[j], vxM[j-1], vyM[j-1]);	
				
				burn = 1;
		}
		else{
			vxA[j] = vxA[j-1] + axA * DT ;
			vyA[j] = vyA[j-1] + ayA * DT ;
		}
		vxM[j] = vxM[j-1] - aM * ( xM[j-1] / rM ) * DT ;
		vyM[j] = vyM[j-1] - aM * ( yM[j-1] / rM ) * DT ;
		//
		
	}
	//
	//////////////////////////////////////////////////
	//
	fout = fopen( "orbit.txt" , "w" ) ;
	//
	for( j = 0 ; j < n ; j ++ )
	{
		fprintf( fout , "%d %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f\n" , j , t[j] 
		, xM[j] , yM[j] , vxM[j] , vyM[j] , sqrt( vxM[j]*vxM[j] + 
		vyM[j]*vyM[j] ), sqrt( xM[j]*xM[j] + yM[j]*yM[j] ) , xA[j] , 
		yA[j] , vxA[j] , vyA[j] , sqrt( vxA[j]*vxA[j] + vyA[j]*vyA[j] 
		), sqrt( xA[j]*xA[j] + yA[j]*yA[j] )) ;
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
