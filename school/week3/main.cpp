#include <iostream>

class Greeter {
public:
    void sayHello() const {
        std::cout << "Hello C++ world\n";
    }
};

int main() {
    Greeter g;
    g.sayHello();
    return 0;
}
