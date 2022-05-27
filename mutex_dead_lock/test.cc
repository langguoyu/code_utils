#include <mutex>
#include <iostream>
#include <thread>

std::mutex m;

void test()
{
    while (1)
    {
        std::cout << "before lock_guard" << std::endl;
        std::lock_guard<std::mutex> lk1(m);
        std::lock_guard<std::mutex> lk2(m);
        std::cout << "after lock_guard" << std::endl;

    }
}

int main()
{
    std::thread t1(test);
    t1.join();

    std::cout << "return 0" << std::endl;
    return 0;
}