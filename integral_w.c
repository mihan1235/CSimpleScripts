#include <stdio.h>
#include <math.h>

double f1 (double x){
    return log(x);
}

double f2 (double x){
    return 1/(2-x)+6;
}

double f3 (double x){
    return -2*x+14;
}

typedef double (*tf) (double);

double f (double x,tf f1, tf f2){
    return f1(x) - f2(x);
}

double root(tf f1,tf f2,double a, double b, double eps){
	//F(X)=0
	double c;
	while ((b-a)>eps){
		c=(a+b)/2;
		if (f(a,f1,f2)*f(c,f1,f2)<=0){
			b=c;
		}
		else {
			a=c;
		}
	}
	return (a+b)/2;
}

/*
 * {Integral variant 2}
 * Формула трапеций: I  In =  h(0.5F0+F1+F2+...+Fn-1+0.5Fn),
 * где Fi = F(a+ih), h=(b-a)/n 
 */
 
double integral (tf f, double a, double b, double eps2){
	long int n,i;
	double sum,h,h1,x,s1,s2;
    n=1;
    h=(b-a)/n;
    sum=(f(a)+f(b))/2;
    do{
		n=n*2;
		h1=h;
		h=h/2;
		s1=sum*h1;
		x=a+h;
		for (i=1;i<n/2;i++){
			sum=sum+f(x);
			x=x+h1;
		}
		s2=sum*h;
	}while(!(fabs(s2-s1)<eps2));
    return s2;
}

int main(int argc, char **argv)
{
	double x1,x2,x3,s,int1,int2,int3;
	x1 = root(f1,f3,4,8,0.001);
	printf("Пересечение g(x)=ln(x) с h(x)=-2*x+14 на [4..8]: x1 = %lf\n",x1);
	x2 = root(f2,f3,3.5,7,0.001);
	printf("Пересечение f(x)=1/(2-x)+6 с h(x)=-2*x+14 на [3.5..7]: x2 = %lf\n",x2);
	x3 = root(f2,f1,2.01,4,0.001);
	printf("Пересечение g(x)=ln(x) с f(x)=1/(2-x)+6 на [2..3]: x3 = %lf\n",x3);
	int1=integral(f1,x3,x1,0.001);
	printf("Интеграл ln(x) на [x1..x3]: %lf\n",int1);
	int2=integral(f3,x2,x1,0.001);
	printf("Интеграл -2*x+14 на [x2..x1]: %lf\n",int2);
	int3=integral(f2,x3,x2,0.001);
	printf("Интеграл 1/(2-x)+6 на [x3..x2]: %lf\n",int3);
	s = -int1 + int2 + int3;
	printf("S=%lf\n",s);
	return 0;
}

