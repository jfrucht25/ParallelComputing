#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define M 640
#define N 480

typedef struct
{
   double x ;
   double y ;
   double z ;
} triple ;
typedef struct
{
	int r;
	int g;
	int b;
} color;

triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple g = { 0.00 , 1.25 , -0.50 } ; // the light
color background = {0,0,0};

typedef struct
{
	triple center;
	double radius;
	color h;
	double reflectivity;
	int texture;
	color **tcolor;
	int twidth;
	int theight;
	
} sphere;

void printSphere(sphere s){
	printf("%f %f %f %f %d %d %d %f %d %d %d\n", s.center.x, s.center.y, s.center.z, s.radius, s.h.r, s.h.g, s.h.b, s.reflectivity, s.texture, s.twidth, s.theight);
}

double dotp( triple t , triple u )
{
   return t.x * u.x + t.y * u.y + t.z * u.z ;
}
triple crossp(triple t, triple u){
		triple out;
		out.x = t.y*u.z-t.z*u.y;
		out.y = t.z*u.x-t.x*u.z;
		out.z = t.x*u.y-t.y*u.x;
}

triple diff(triple u , triple v ) // t = u - v
{
	triple t;
    t.x = u.x - v.x ;
	t.y = u.y - v.y ;
	t.z = u.z - v.z ;
	return t;
}

triple add(triple u, triple v)
{
	triple t;
    t.x = u.x + v.x ;
	t.y = u.y + v.y ;
	t.z = u.z + v.z ;
	return t;
}

color addC(color u, color v){
	color t;
    t.r = u.r + v.r ;
	t.g = u.g + v.g ;
	t.b = u.b + v.b ;
	return t;
}

triple mult(triple t, double f){
    triple out = {t.x*f,t.y*f,t.z*f};
    return out;
}


color multC(color c, double f){
	color out = {(int)(c.r*f),(int)(c.g*f),(int)(c.b*f)};
	return out;
}

double magnitude2(triple t){
    return t.x*t.x+t.y*t.y+t.z*t.z;
}

triple normalize(triple t){
    double mag = sqrt(magnitude2(t));
    return mult(t, 1/mag);
}

void init(sphere spheres[], int n, FILE* f){
	for(int i=0; i<n; i++){
		fscanf(f, "%lf", &spheres[i].center.x);
		fscanf(f, "%lf", &spheres[i].center.y);
		fscanf(f, "%lf", &spheres[i].center.z);
		fscanf(f, "%lf", &spheres[i].radius);
		fscanf(f, "%d", &spheres[i].h.r);
		fscanf(f, "%d", &spheres[i].h.g);
		fscanf(f, "%d", &spheres[i].h.b);
		fscanf(f, "%lf", &spheres[i].reflectivity);
		fscanf(f, "%d", &spheres[i].texture);
		char filename[256];
		fscanf(f, "%s", filename);
		if(spheres[i].texture==1){
			FILE* t;
			t=fopen(filename, "r");
			fscanf(t, "%*s %d %d %*s", &spheres[i].twidth, &spheres[i].theight);
			spheres[i].tcolor = (color**)(malloc(spheres[i].theight*sizeof(color *)));
			int r,g,b;
			for(int y=0; y<spheres[i].theight; y++){
				spheres[i].tcolor[y] = (color*)(malloc(spheres[i].twidth*sizeof(color)));
				for(int x=0; x<spheres[i].twidth; x++){
					fscanf(t,"%d %d %d",&r,&g,&b);
					spheres[i].tcolor[y][x].r=r;
					spheres[i].tcolor[y][x].g=g;
					spheres[i].tcolor[y][x].b=b;
				}
			}
			fclose(t);
		}
		//printSphere(spheres[i]);
	}
	printf("Data initiated\n");
	fclose(f);
}

double intersectTime(triple e, triple dp,sphere s)
{
	   double a,b,c;
	   double bb4ac;
	   a = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
	   b = 2 * (dp.x * (e.x - s.center.x) + dp.y * (e.y - s.center.y) + dp.z * (e.z - s.center.z));
	   c = s.center.x * s.center.x + s.center.y * s.center.y + s.center.z * s.center.z;
	   c += e.x * e.x + e.y * e.y + e.z * e.z;
	   c -= 2 * (s.center.x * e.x + s.center.y * e.y + s.center.z * e.z);
	   c -= s.radius * s.radius;
	   bb4ac = b * b - 4 * a * c;
	   if (bb4ac < 0) {
		  return -1;
	   }
		
	   double x1 = ((-b - sqrt(bb4ac)) / (2 * a));
	   double x2 = ((-b - sqrt(bb4ac)) / (2 * a));
		if(x1<x2 && x1>0){
			return x1;
		}
		if(x1<0&&x2<0){
			return -1;
		}
		return x2;
}

color getColor(sphere s, triple point){
		if(s.texture == 0){
				return s.h;
		}
		if(s.texture == 1){
				triple r = normalize(diff(point, s.center));
				triple pole = {0,1,0};
				triple equator = {1,0,0};
				
				
				double phi = acos(-dotp(pole, r));
				double lat = phi/M_PI;
				double lon;
				double theta = ( acos( dotp( r, equator ) / sin( phi )) ) / ( 2 * M_PI);
				if ( dotp( crossp( pole, equator ), r ) > 0 ){
					lon = theta;
				} 
				else{
					lon = 1 - theta;
				}
				
				int y = s.theight-(int)(lat*s.theight)-1;
				int x = s.twidth-1-(int)(lon*s.twidth);
				
				if(y<0) y=0;
				if(y>s.theight) y=s.theight-1;
				if(x<0) x=0;
				if(x>s.twidth) x=s.twidth-1;
				
				return s.tcolor[y][x];
		}
		else{
				int a = floor(5*(s.center.x-point.x));
				int b = floor(5*(s.center.z-point.z));
				if((a+b)%2==0){
					return s.h;
				}
				else{
					color c = {255-s.h.r, 255-s.h.g, 255-s.h.b};
					return c;
				}
		}
}

color traceRay(triple r, triple dir, sphere spheres[], int n){
		color c = background;
		int intersect = -1;
		double closest = -1;
		for(int i=0; i<n; i++){
			double t = intersectTime(r, dir, spheres[i]);
			if(t>=0){
				if(closest<0 || t<closest){
						intersect=i;
						closest=t;
				}
			}
		}
		if(intersect==-1){
			return c;
		}
		
		triple r2point = mult(dir,closest);
		triple point = add(mult(dir,closest),r);
		triple point2light = diff(g,point);
		point2light = normalize(point2light);
		r2point = normalize(r2point);
		int shadow = 0;
		for(int i=0; i<n; i++){
			if(i==intersect){
				continue;
			}
			double t = intersectTime(point, point2light, spheres[i]);
			if(t>0){
				shadow=1;
				break;
			}
		}
		double lightFactor = 0;
		triple radius = diff(point, spheres[intersect].center);
		radius = normalize(radius);
		if(shadow==0){
			lightFactor = dotp(radius, point2light);
			if(lightFactor < 0) lightFactor=0;
		}
		c = multC(getColor(spheres[intersect], point),(0.5+0.5*lightFactor));

		double cl = -dotp(radius, dir);
		triple newdir = add(dir, mult(radius, cl*2));
		if(spheres[intersect].reflectivity > 0){
			c = addC(multC(c,(1-spheres[intersect].reflectivity)),multC(traceRay(point, newdir, spheres, n), spheres[intersect].reflectivity));
		}
		return c;
}

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Please specify a file name as a command line argument\n");
		return 1;
	}
	
	int rgb[N][M][3];

	FILE *f;
	f=fopen(argv[1],"r");
	
	int n;
	fscanf(f, "%d", &n);
	sphere spheres[n];
	init(spheres,n,f);

	double x;
	double y;
	color c;
	for(int i=0; i<N; i++){
      			#pragma omp parallel for private(x,y,c)
			for(int j=0; j<M; j++){
				y=1.0*i/N;
				x=1.0*j/N-1.0/6;
				
				triple pixel = {x,y,0};
				triple direction = diff(pixel, e);
				direction = normalize(direction);
				
				c = traceRay(e, direction, spheres, n);
				rgb[i][j][0]=c.r;
				rgb[i][j][1]=c.g;
				rgb[i][j][2]=c.b;
			}
	}
	
	FILE *fout;
	
	fout = fopen( "out.ppm" , "w" ) ;

	fprintf( fout , "P3\n" ) ;
	fprintf( fout , "%d %d\n" , M , N ) ;
	fprintf( fout , "255\n" ) ;

	for(int i = 0 ; i < N ; i++ )
	{
		for(int j = 0 ; j < M ; j++)
		{
			fprintf( fout , "%d %d %d\n" ,
			rgb[N-i-1][j][0] , rgb[N-i-1][j][1] , rgb[N-i-1][j][2] ) ;
		}
	}
	fclose( fout ) ;

	return 0 ;

}
