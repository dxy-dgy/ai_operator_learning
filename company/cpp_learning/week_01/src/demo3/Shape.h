#ifndef DEMO3_SHAPE_H
#define DEMO3_SHAPE_H

#include <ostream>
#include <string>

// CRTP：Derived 把自己的类型传给基类模板。
template <typename Derived>
class Shape {
public:
    Shape() : id(++shapeCount) {}
    Shape(const Shape&) : id(++shapeCount) {}
    Shape(Shape&&) noexcept : id(++shapeCount) {}
    Shape& operator=(const Shape&) { return *this; }
    Shape& operator=(Shape&&) noexcept { return *this; }
    ~Shape() { --shapeCount; }

    [[nodiscard]] double getArea() const {
        return derived().getAreaImpl();
    }

    static int getShapeCount() { return shapeCount; }

    friend std::ostream& operator<<(std::ostream& os, const Shape& shape) {
        os << "[Shape Info] ";
        shape.derived().printImpl(os);
        return os;
    }

protected:
    int id;

private:
    const Derived& derived() const {
        return static_cast<const Derived&>(*this);
    }

    inline static int shapeCount = 0;
};

class Circle final : public Shape<Circle> {
public:
    explicit Circle(double radius, std::string tag = "Default Circle");

private:
    friend class Shape<Circle>;

    [[nodiscard]] double getAreaImpl() const;
    void printImpl(std::ostream& os) const;

    double radius;
    std::string tag;
};

#endif
