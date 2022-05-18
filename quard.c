#include <stdio.h>
#include <math.h>

double det(double a, double b, double c)
 { 
	return b*b-4*a*c;
	
 }
int main()
 {
	double p,q,r,d;
	scanf ("%lf %lf %lf",&p,&q,&r);
	   if (p == 0) {
		   printf("Ti,durak,ne tak vvodish \n");
		   return 1;
	   }
	d=det(p,q,r);
	if (d<0) {
		printf("Nothing vot chto \n");
		
		return 1;
	}	
	d= pow(d,1/2); 
	printf("x1=%lf x2=%lf \n",(-q-d)/(2*p),(-q+d)/(2*p));
	return 0;		   
 }	 
 
 

