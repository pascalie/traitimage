#include <stdlib.h>
#include <stdio.h>

int main(){
	float a = 3.2;
	int b = (int)a;

	float c = a-b;
	printf("%d\n", b);
	printf("%f\n", a);
	printf("%f\n", c);
}