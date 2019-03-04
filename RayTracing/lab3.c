//
// Torbert, 8 February 2016
//
#include <stdio.h>
#include <math.h>
#include "initRay.h"
//
#define M 640
#define N 480
#define SIZE 4
//
void scale(sphere a[]){
    for(int i=0; i<SIZE; i++){
        a[i].c.x = a[i].c.x*N+106;
        a[i].c.y = a[i].c.y*N;
        a[i].c.z = a[i].c.z*N;
        a[i].r   =  a[i].r*N;
    }
    e = mult(e, N);
    g = mult(g, N);
}

double raySphere(triple dp,triple e,sphere s)
{
   double a,b,c;
   double bb4ac;
   a = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
   b = 2 * (dp.x * (e.x - s.c.x) + dp.y * (e.y - s.c.y) + dp.z * (e.z - s.c.z));
   c = s.c.x * s.c.x + s.c.y * s.c.y + s.c.z * s.c.z;
   c += e.x * e.x + e.y * e.y + e.z * e.z;
   c -= 2 * (s.c.x * e.x + s.c.y * e.y + s.c.z * e.z);
   c -= s.r * s.r;
   bb4ac = b * b - 4 * a * c;
   if (bb4ac < 0) {
      return(0);
   }
    
   double x1 = ((-b - sqrt(bb4ac)) / (2 * a));
   double x2 = ((-b - sqrt(bb4ac)) / (2 * a));
    if(x1<x2 && x1>0){
        return x1;
    }
    if(x1<0&&x2<0){
        return 0;
    }
    return x2;
}

int main(void)
{
   int rgb[N][M][3] ; // red-green-blue for each pixel
   int intersect[N][M];
   //
    sphere spheres[SIZE];
    init(spheres);
    scale(spheres);
   int y , x ;
   //
   FILE* fout ;
    double currentClosest;
   //
   for( y = 0 ; y < N ; y++ )
   {
      for( x = 0 ; x < M ; x++)
      {
            currentClosest=0;
            rgb[y][x][0] = 0;
            rgb[y][x][1] = 0;
            rgb[y][x][2] = 0;
			triple pixel = {x,y,0};
			triple ray = diff(pixel, e);
			ray = normalize(ray);
			intersect[y][x]=-1;
            for(int i=0; i<SIZE; i++){
                double time = raySphere(ray,e,spheres[i]);
                if(time!=0){
                    if(currentClosest==0 || time<currentClosest){
                        currentClosest = time;
                        if(i==0){
							triple point = add(mult(ray,currentClosest),e);
							int a = floor(point.x/50);
							int b = floor(point.z/50);
							if((a+b)%2==0){
								rgb[y][x][0] = spheres[i].h.r/2;
								rgb[y][x][1] = spheres[i].h.g/2;
								rgb[y][x][2] = spheres[i].h.b/2;
							}
							else{
								rgb[y][x][0] = 128;
								rgb[y][x][1] = 128;
								rgb[y][x][2] = 128;
							}
						}else{
							rgb[y][x][0] = spheres[i].h.r/2;
							rgb[y][x][1] = spheres[i].h.g/2;
							rgb[y][x][2] = spheres[i].h.b/2;
						}
                        intersect[y][x] = i;
                    }
                }
            }

			triple point = add(mult(ray,currentClosest),e);
			triple eye2point = diff(point, e);
			triple point2light = diff(g,point);
			point2light = normalize(point2light);
			int done = 0;
			for(int j=0; j<SIZE && done==0; j++){
				if(j==intersect[y][x]){
					continue;
				}
				double time2 = raySphere(point2light, point, spheres[j]);
				if(time2!=0){
						done = 1;
				}
			}
			if(done==0){
				triple radius = diff(point, spheres[intersect[y][x]].c);
				radius = normalize(radius);
				double cosine = dotp(radius, point2light);
				rgb[y][x][0] += rgb[y][x][0]*cosine;
				rgb[y][x][1] += rgb[y][x][1]*cosine;
				rgb[y][x][2] += rgb[y][x][2]*cosine;
			}
      }
   }
   //
   //
   //
   fout = fopen( "out.ppm" , "w" ) ;
   //
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , M , N ) ;
   fprintf( fout , "255\n" ) ;
   //
   for( y = 0 ; y < N ; y++ )
   {
      for( x = 0 ; x < M ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[N-y-1][x][0] , rgb[N-y-1][x][1] , rgb[N-y-1][x][2] ) ;
      }
   }
   fclose( fout ) ;
   //
   return 0 ;
}
//
// end of file
//
