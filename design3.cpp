#include <functional>
#include <iostream>
#include <vector>
 
void f(int& n1, int& n2, const int& n3)
{
    std::cout << "In function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    ++n1; // increments the copy of n1 stored in the function object
    ++n2; // increments the main()'s n2
    // ++n3; // compile error
}
 
int main()
{
    int n1 = 1, n2 = 2, n3 = 3;
    std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::cref(n3));
    n1 = 10;
    n2 = 11;
    n3 = 12;
    std::cout << "Before function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    bound_f();
    std::cout << "After function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';


    int x = 5;
    int& y = std::ref(x);
    std::cout << x << " " << y << std::endl;
    x = 10;
    std::cout << x << " " << y << std::endl;
    y = 20;
    std::cout << x << " " << y << std::endl;


    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
    
        // Bind a lambda that adds a constant value to an integer to a variable
        auto add_10 = std::bind([](int x, int y) { return x + y; }, std::placeholders::_1, 10);
        int x = 5;
        int result = add_10(x);
        std::cout << "result = " << result << std::endl; // output: result = 15
    
        // Bind a lambda that multiplies two integers together to a variable
        auto multiply = std::bind([](int x, int y) { return x * y; }, std::placeholders::_1, std::placeholders::_2);
        int a = 2;
        int b = 3;
        int product = multiply(a, b);
        std::cout << "product = " << product << std::endl; // output: product = 6
    }

    {
        int x = 5;
        auto expression = std::bind([](int& x, int y) {auto res = y*x + 2; x = 3; return res;}, std::ref(x), 10);
        int result = expression(x);
        std::cout << "result = " << result << std::endl; // output: result = 52
        std::cout << x << std::endl;
    }

}