# demo3：CRTP 编译期多态

这个示例使用 CRTP（Curiously Recurring Template Pattern，奇异递归模板模式）实现
`Circle` 和 `Rectangle` 的统一调用接口。这里没有使用 `virtual`，具体调用目标在编译期确定。

## 1. 核心继承写法

```cpp
class Circle final : public Shape<Circle> { /* ... */ };
class Rectangle final : public Shape<Rectangle> { /* ... */ };
```

子类继承基类模板时，把自己的类型作为模板参数传给基类：

- `Circle` 的基类是 `Shape<Circle>`；
- `Rectangle` 的基类是 `Shape<Rectangle>`。

这就是 CRTP 最有辨识度的写法。模板参数 `Derived` 让基类在编译期知道真正的子类类型。

## 2. `getArea()` 的调用链

基类对外提供统一接口：

```cpp
template <typename Derived>
class Shape {
public:
    double getArea() const {
        return derived().getAreaImpl();
    }

private:
    const Derived& derived() const {
        return static_cast<const Derived&>(*this);
    }
};
```

以 `Circle circle(5.0); circle.getArea();` 为例，调用过程是：

```text
circle.getArea()
    -> Shape<Circle>::getArea()
    -> derived() 把基类引用转换为 const Circle&
    -> Circle::getAreaImpl()
```

调用 `Rectangle::getArea()` 时，编译器实例化的是 `Shape<Rectangle>`，最后调用
`Rectangle::getAreaImpl()`。这种“同一个接口，不同实现”就是本例中的多态；它由模板和
静态类型完成，因此也叫静态多态或编译期多态。

## 3. 为什么使用 `friend`

`getAreaImpl()` 和 `printImpl()` 是子类的私有成员。下面的声明只授权对应的 CRTP 基类
访问它们：

```cpp
friend class Shape<Circle>;
friend class Shape<Rectangle>;
```

这样，外部代码只使用公开的 `getArea()` 和 `operator<<`，实现细节仍保留在子类内部。

## 4. 通用函数怎样接收不同图形

```cpp
template <typename Derived>
void showShape(const Shape<Derived>& shape) {
    std::cout << shape << " | Area: " << shape.getArea() << '\n';
}

Circle circle(5.0);
Rectangle rectangle(4.0, 6.0);
showShape(circle);       // Derived 推导为 Circle
showShape(rectangle);    // Derived 推导为 Rectangle
```

`showShape` 看起来只写了一次，但编译器会分别生成适用于 `Shape<Circle>` 和
`Shape<Rectangle>` 的函数版本。这是 CRTP 的典型用法：基类复用统一逻辑，各子类只提供
自己的 `...Impl()` 实现。

输出运算符也采用相同思路。`Shape<Derived>` 中的 `operator<<` 调用
`Derived::printImpl()`，所以两个图形会打印各自的信息。

## 5. 与 `virtual` 运行时多态的区别

| CRTP 编译期多态 | `virtual` 运行时多态 |
| --- | --- |
| 通过模板和 `static_cast` 分派 | 通过虚函数分派 |
| 编译期确定具体实现 | 运行时根据对象确定实现 |
| 不需要虚函数表 | 通常需要虚函数表 |
| 不同子类对应不同基类类型 | 所有子类可以共享同一个非模板基类类型 |

需要特别注意：`Shape<Circle>` 和 `Shape<Rectangle>` 是两个不同类型。因此，虽然
`showShape` 可以分别处理两者，但不能直接创建
`std::vector<std::unique_ptr<Shape>>` 来混装它们。本例的 `canvas` 是
`std::vector<std::unique_ptr<Circle>>`，只能存放圆形。如果需要异构容器，可以改用
虚函数多态、`std::variant` 或类型擦除；这不属于 CRTP 本身提供的能力。

## 6. 本例中的计数器

`inline static int shapeCount` 位于类模板中，所以每个模板实例都有自己的一份：

```cpp
Circle::getShapeCount();       // Shape<Circle> 的计数
Rectangle::getShapeCount();    // Shape<Rectangle> 的计数
```

它不是所有图形共享的总数。本例中的 `id` 也按图形类型分别从 1 开始。

当前示例同时使用 `shapeCount` 生成 ID 和统计存活对象；对象销毁后计数会减小，所以后续
新建对象时 ID 可能重复。学习 CRTP 不受影响，但正式项目应拆成两个变量：一个只增不减的
`nextId` 和一个随构造、析构变化的 `liveCount`。

## 7. 编译和运行

在 `demo3` 目录执行：

```bash
mkdir -p ../../bin
clang++ -std=c++17 -Wall -Wextra -pedantic main.cpp Shape.cpp \
  -o ../../bin/demo3
../../bin/demo3
```
