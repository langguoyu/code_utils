#include <iostream>
#include <cstdlib>
#include "foo.h"

int main(){
    std::cout << "foo test"<<std::endl;
    foo();
    
    std::cout << "malloc and free test"<<std::endl;
    void * p1 =malloc(10);
    free(p1);

    std::cout << "new test"<< std::endl;
    void* p2 = new int;
    std::cout << typeid(p2).name() << std::endl;
    delete p2;
    
    std::cout << "test finish"<<std::endl;
    return 0;
}