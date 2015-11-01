#include "stdlib.h"
#include "stdio.h"

void func(int *p)
{

	free(p);

}
int main()
{
	
	int *p = (int*) calloc(1,sizeof(int));
	*p = 2;
	void * add = &p;
	printf("%p\n",add);

	func(p);


}

