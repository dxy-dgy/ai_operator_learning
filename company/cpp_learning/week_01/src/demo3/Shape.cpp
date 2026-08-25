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

Rectangle::Rectangle(double width, double height, std::string tag)
    : width(width), height(height), tag(std::move(tag)) {}

double Rectangle::getAreaImpl() const {
    return width * height;
}

void Rectangle::printImpl(std::ostream& os) const {
    os << "Rectangle [ID: " << id << "] (Width: " << width
       << ", Height: " << height << ", Tag: " << tag << ")";
}
