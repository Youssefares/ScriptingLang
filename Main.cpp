#include <cstdio>
#include "Helpers.h"
#include "hashset.h"

int main(int argc, char* argv[]){
	// double x = evaluate("");
	// printf("%lf", x);
	// getchar();
	int xValue = 32;
	char x = 'x';
	HashMap hashMap;
	HashMapNew(&hashMap, sizeof(int), 10, hashFunction,free);
	HashMapEnter(&hashMap, &x, &xValue);
}
