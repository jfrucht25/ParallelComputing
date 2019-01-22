#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "mpi.h"

#define G 6.674e-11 // (m^3)(kg^-1)(s^-2)
#define M 5.972e+24 // kg
#define R 6.371e+6  // m
#define LUNAR_DIST 3.84402e+8//distance to moon (m)
#define LUNAR_M 7.34767309e+22//mass of mooon (kg)
#define LUNAR_R 1.737e+6

#define DT 5
#define LENGTH 8

double runOrbit(double theta, double speed){

	int n = (int)( 0.5 + (LENGTH* 24 * 60 * 60 ) / DT ) ;
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
	
	double v = speed;
	double angle = theta;
	double alt = R+400000;	

	t[0]  = 0.0          ; 
	xA[0]  = alt*cos(angle); 
	yA[0]  = alt*sin(angle); 
	vxA[0] = v*cos(angle);
	vyA[0] = v*sin(angle);	

	xM[0]  = LUNAR_DIST  ;
	yM[0]  = 0 ;
	vxM[0] = 0     ;
	vyM[0] = sqrt(G*M/xM[0]);
		

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
//			printf("Apollo Crashed\n");
			return 0;
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
	}
	double finalSpeed = sqrt(vxA[n-1]*vxA[n-1] + vyA[n-1]*vyA[n-1]);
	free(t);
	free(xA);
	free(yA);
	free(vxA);
	free(vyA);
	free(xM);
	free(yM);
	free(vxM);
	free(vyM);
	return finalSpeed;
}

int main(int argc, char* argv[]){
	int rank;
	int size;
	MPI_Status status;
	int tag = 0;
	double end = -1;
	int result;
	MPI_Init(&argc, &argv) ;
	MPI_Comm_size( MPI_COMM_WORLD , &size ) ;
	MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ;

	double maxV=0;
	double maxAngle;
	double maxInitialV;
	double parameters[3];
	int x=0;
	if (rank==0){
		for(int i=0; i<100; i++){
			for(int j=0; j<100; j++){
				parameters[0] = M_PI*i*1.0/200.0;
				parameters[1] = 200*j;
				printf("I: %d J: %d\n", i, j);
				MPI_Send(&parameters, 2, MPI_DOUBLE, (x%(size-1))+1, tag, MPI_COMM_WORLD);
				x++;
			}
		}
		parameters[0] = -1;
		for(int k=1; k<size; k++){
			MPI_Send(parameters, 2, MPI_DOUBLE, k, tag, MPI_COMM_WORLD);
		}
		for(int k=0; k<10000; k++){
			MPI_Recv(&parameters, 3, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			if(parameters[2] > maxV){
				maxV = parameters[2];
				maxAngle = parameters[0];
				maxInitialV = parameters[1];
			}			
		}
		parameters[0] = -1;
		for(int j=1; j<size; j++){
                        MPI_Send(&parameters, 2, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
                }
		printf("Max Speed is %f at angle %f and intial speed %f\n", maxV, maxAngle, maxInitialV);
	}
	else{
		while(1){
			MPI_Recv(&parameters, 2, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
			if(parameters[0]==-1){
				break;
			}
			//printf("Angle: %f Speed: %f\n", parameters[0], parameters[1]);
			maxV = runOrbit(parameters[0], parameters[1]);
			parameters[2] = maxV;
			MPI_Send(&parameters, 3, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}
