#include <iostream>

class MyClass {
public:
    MyClass() {p = new char[1024];}
    ~MyClass() {if (!p) delete []p;}
private:
    char* p;
};


int main()
{
    char* p = (char*)malloc(16);
    free(p);

    MyClass myObj;

    return 0;
}