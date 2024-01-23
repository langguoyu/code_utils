#include <iostream>
#include "wrap.h"
#include <stdio.h>
#include <stdlib.h>

void* __wrap_malloc(size_t size){
    std::cout << "call wrap malloc" << std::endl;
    return __real_malloc(size);
}

void __wrap_free(void* ptr){
    std::cout << "call wrap free" << std::endl;
    __real_free(ptr);
}

void __wrap_foo(){
    std::cout << "call wrap foo"<<std::endl;
    __real_foo();
}


void* __wrap__Znwm(unsigned long size)
{
	fprintf(stdout, "call __wrap__Znwm funtcion, size: %d\n", size);
	return __real__Znwm(size);
}
 
void __wrap__ZdlPv(void* ptr)
{
	fprintf(stdout, "call __wrap__ZdlPv function\n");
	__real__ZdlPv(ptr);
}
