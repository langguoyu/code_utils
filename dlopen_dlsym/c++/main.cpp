//----------
// main.cpp:
//----------
#include <dlfcn.h>

#include <iostream>

#include "polygon.hpp"

int main() {
  using std::cerr;
  using std::cout;

  // load the triangle library
  void* triangle = dlopen("./triangle.so", RTLD_LAZY);
  if (!triangle) {
    cerr << "Cannot load library: " << dlerror() << '\n';
    return 1;
  }

  // reset errors
  dlerror();

  // load the symbols
  create_t* create_triangle = (create_t*)dlsym(triangle, "create");
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    cerr << "Cannot load symbol create: " << dlsym_error << '\n';
    return 1;
  }

  destroy_t* destroy_triangle = (destroy_t*)dlsym(triangle, "destroy");
  dlsym_error = dlerror();
  if (dlsym_error) {
    cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
    return 1;
  }

  // create an instance of the class
  polygon* poly = create_triangle();

  // use the class
  poly->set_side_length(7);
  cout << "The area is: " << poly->area() << '\n';

  // destroy the class
  destroy_triangle(poly);

  // unload the triangle library
  dlclose(triangle);
}