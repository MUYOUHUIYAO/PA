#include <stdio.h>
#include <string.h>
#include "trap.h"

int main(){
	int l;
	l = strlen("Hello world!\n");
	nemu_assert(l == 13);
	return 0;
}