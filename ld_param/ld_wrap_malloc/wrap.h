#pragma once

extern "C"
{
    void __wrap_foo();
    void __real_foo();

    void* __real_malloc(size_t size);
    void __real_free(void* ptr);
    void* __wrap_malloc(size_t size);
    void __wrap_free(void* ptr);


    // c++filt: _Znwm ==> operator new(unsigned long)
    void* __wrap__Znwm(unsigned long size);
    // c++filt _ZdlPv ==> operator delete(void*)
    void __wrap__ZdlPv(void* ptr);
    
    void* __real__Znwm(unsigned long size);
    void __real__ZdlPv(void* ptr);
}