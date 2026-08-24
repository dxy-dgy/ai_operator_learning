#include "Shape.h"

#include <utility>

Circle::Circle(double radius, std::string tag)
    : radius(radius), tag(std::move(tag)) {}

double Circle::getAreaImpl() const {
    return 3.14159 * radius * radius;
}

void Circle::printImpl(std::ostream& os) const {
    os << "Circle [ID: " << id << "] (Radius: " << radius
       << ", Tag: " << tag << ")";
}
