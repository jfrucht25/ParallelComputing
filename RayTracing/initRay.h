//
// Torbert, March 2015
//
#include <math.h>
typedef struct
{
   double x ;
   double y ;
   double z ;
   //
} triple ;
typedef struct
{
	int r;
	int g;
	int b;
} color;
//
triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple g = { 0.00 , 1.25 , -0.50 } ; // the light
//
typedef struct
{
	triple c;
	double r;
	color h;
} sphere;

double dotp( triple t , triple u )
{
   return t.x * u.x + t.y * u.y + t.z * u.z ;
}
//
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
triple mult(triple t, double f){
    triple out = {t.x*f,t.y*f,t.z*f};
    return out;
}
double magnitude2(triple t){
    return t.x*t.x+t.y*t.y+t.z*t.z;
}
triple normalize(triple t){
    double mag = sqrt(magnitude2(t));
    return mult(t, 1/mag);
}

//
void init(sphere a[])
{
   a[0].c.x =      0.50 ;
   a[0].c.y = -20000.00 ; // the floor
   a[0].c.z =      0.50 ;
   a[0].r   =  20000.25 ;
   a[0].h.r =    205    ; // color is Peru
   a[0].h.g =    133    ;
   a[0].h.b =     63    ;
   //
   a[1].c.x =      0.50 ;
   a[1].c.y =      0.50 ;
   a[1].c.z =      0.50 ;
   a[1].r   =      0.25 ;
   a[1].h.r =      0    ; // color is Blue
   a[1].h.g =      0    ;
   a[1].h.b =    255    ;
   //
   a[2].c.x =      1.00 ;
   a[2].c.y =      0.50 ;
   a[2].c.z =      1.00 ;
   a[2].r   =      0.25 ;
   a[2].h.r =      0    ; // color is Green
   a[2].h.g =    255    ;
   a[2].h.b =      0    ;
   //
   a[3].c.x =      0.00 ;
   a[3].c.y =      0.75 ;
   a[3].c.z =      1.25 ;
   a[3].r   =      0.50 ;
   a[3].h.r =    255    ; // color is Red
   a[3].h.g =      0    ;
   a[3].h.b =      0    ;
}
//
// end of file
//
