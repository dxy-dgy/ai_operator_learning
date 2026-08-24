#include "Shape.h"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

template <typename Derived>
void showShape(const Shape<Derived>& shape) {
    std::cout << shape << " | Area: " << shape.getArea() << '\n';
}

int main() {
    auto s1 = std::make_unique<Circle>(5.0, "CRTP_Circle_A");
    std::cout << "1. Construction: " << *s1 << "\n\n";

    std::cout << "2. Move Semantics...\n";
    std::unique_ptr<Circle> s2 = std::move(s1);

    if (!s1) {
        std::cout << "   s1 is now empty (nullptr).\n";
    }
    std::cout << "   s2 received resources: " << *s2 << "\n\n";

    std::cout << "3. CRTP Container...\n";
    std::vector<std::unique_ptr<Circle>> canvas;
    canvas.push_back(std::move(s2));
    canvas.push_back(std::make_unique<Circle>(10.0, "CRTP_Circle_B"));

    std::cout << "\n--- Iterating Canvas ---\n";
    for (const auto& shape : canvas) {
        showShape(*shape);
    }

    std::cout << "\n--- Main ending, objects going out of scope ---\n";
    std::cout << "Total circles in memory: " << Circle::getShapeCount() << '\n';
}
