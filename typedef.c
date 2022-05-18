# include <stdio.h>

//typedef double (*func)(double,double);

double plus (double x, double y)
{
	return x+y;
}

int main ()
{
	double x,y;
	double (*f)(double,double);
	f=plus;
	scanf("%lf %lf",&x,&y);
	printf("%lf\n",x);
	printf("%lf\n",y);
	printf("%lf\n",f(x,y));
	return 0;
}
