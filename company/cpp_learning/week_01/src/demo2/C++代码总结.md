# demo2：三个 C++ 文件总结

## 1. 这组代码在做什么

这组代码用“形状”演示了 C++ 面向对象和现代资源管理的几个重要概念：

- `Shape` 是抽象基类，只规定所有形状都必须能计算面积、输出自身信息。
- `Circle` 是 `Shape` 的派生类，真正保存半径和标签，并实现圆的面积计算和输出。
- `main.cpp` 不直接管理裸指针，而是使用 `std::unique_ptr` 自动管理对象的生命周期。
- 程序通过 `std::move` 转移对象所有权，并用 `std::vector<std::unique_ptr<Shape>>` 在同一个容器中保存不同种类的形状。
- `Shape::shapeCount` 记录当前仍然存活的形状对象数量。

虽然文件中目前只有 `Circle` 一种具体形状，但以后可以继续增加 `Rectangle`、`Triangle` 等派生类，而 `main.cpp` 中保存形状的容器不必改变。这就是多态带来的扩展性。

## 2. 三个文件各自的职责

### `Shape.h`：声明类型和公共规则

这是整组代码的核心。它定义了抽象基类 `Shape`、具体派生类 `Circle`，以及两者对外提供的函数。

头文件主要回答：“有哪些类型？这些类型拥有哪些成员？外部代码可以怎样使用它们？”

### `Shape.cpp`：实现 `Circle` 的具体行为

这个文件实现了在 `Shape.h` 中只声明、没有写函数体的三个函数：

- `Circle` 构造函数；
- `Circle::getArea()`；
- `Circle::print()`。

把较长的实现放在 `.cpp` 中，可以让头文件更像一份清晰的接口说明。

### `main.cpp`：使用和验证这些类型

程序依次演示：

1. 创建一个由 `std::unique_ptr<Shape>` 管理的 `Circle`；
2. 用 `std::move` 把所有权从 `s1` 转给 `s2`；
3. 验证转移后 `s1` 为空；
4. 把两个圆放进多态容器 `canvas`；
5. 遍历容器，通过基类指针调用实际对象的 `getArea()` 和 `print()`；
6. 输出当前存活的形状数量；
7. `main` 结束时，智能指针自动销毁对象。

## 3. 重点解析 `Shape.h`

### 3.1 头文件保护

```cpp
#ifndef SHAPE_H
#define SHAPE_H
// 头文件内容
#endif
```

这叫“头文件保护宏”。第一次包含该文件时会定义 `SHAPE_H`；以后再次包含时，`#ifndef` 条件不成立，于是跳过文件内容，防止同一个类被重复定义。

### 3.2 引入标准库声明

```cpp
#include <vector>
#include <iostream>
#include <string>
```

- `<string>` 提供 `std::string`。
- `<iostream>` 提供输入输出流相关声明，这里使用了 `std::ostream`。
- `<vector>` 提供 `std::vector`，但 `Shape.h` 自身没有使用它，因此这一行目前可以删除；真正使用 `vector` 的是 `main.cpp`。

更精确的头文件依赖通常会让 `Shape.h` 包含 `<ostream>`，而不是功能更大的 `<iostream>`。

### 3.3 抽象基类 `Shape`

```cpp
class Shape {
```

`Shape` 表达所有形状共有的规则。因为它含有纯虚函数，所以不能直接创建 `Shape` 对象，只能创建实现了这些规则的派生类对象。

#### 私有静态计数器

```cpp
private:
    inline static int shapeCount = 0;
```

- `private` 表示只有 `Shape` 自己的成员函数和友元能直接访问该成员。
- `static` 表示所有 `Shape` 对象共享同一份 `shapeCount`，不是每个对象各有一份。
- `inline` 允许这个静态数据成员直接在头文件中定义，并避免多个 `.cpp` 包含头文件时产生重复定义错误。这种写法需要 C++17 或更高版本。
- `int` 是整数类型。

#### 构造函数与对象编号

```cpp
Shape() {
    shapeCount++;
    id = shapeCount;
}
```

`Shape()` 是默认构造函数。每构造一个形状，计数器加一，并把加一后的值保存为该对象的 `id`。

```cpp
Shape(const Shape& /*other*/) { id = ++shapeCount; }
Shape(Shape&& /*other*/) noexcept { id = ++shapeCount; }
```

这两行分别定义复制构造函数和移动构造函数：

- 复制或移动产生的对象会得到一个新的 `id`，不会沿用原对象的编号。
- `const Shape&` 是对不可修改的 `Shape` 的左值引用，复制构造通常使用这种参数。
- `Shape&&` 是右值引用，常用于接收即将被移动的对象。
- `/*other*/` 是注释掉的参数名：保留参数类型但不使用参数，避免“未使用参数”的警告。
- `noexcept` 承诺移动构造函数不会抛出异常。
- 前置 `++shapeCount` 会先加一，再返回加一后的值。

```cpp
Shape& operator=(const Shape& /*other*/) { return *this; }
Shape& operator=(Shape&& /*other*/) noexcept { return *this; }
```

这是复制赋值运算符和移动赋值运算符。当前实现不复制任何数据，只返回当前对象，因此赋值不会改变 `id`。`return *this` 返回当前对象本身的引用。

#### 虚析构函数

```cpp
virtual ~Shape() { shapeCount--; }
```

析构函数在对象销毁时把存活数量减一。`virtual` 非常重要：当程序通过 `Shape*` 或 `std::unique_ptr<Shape>` 删除实际的 `Circle` 对象时，它保证先执行派生类析构，再执行基类析构。没有虚析构，通过基类指针销毁派生类对象可能产生未定义行为。

#### 静态查询函数

```cpp
static int getShapeCount() { return shapeCount; }
```

静态成员函数不依赖某一个具体对象，所以可以写成 `Shape::getShapeCount()`。它返回所有形状对象共享的计数器。

#### 纯虚面积函数

```cpp
[[nodiscard]] virtual double getArea() const = 0;
```

- `virtual` 允许通过基类指针或引用调用派生类版本，实现运行时多态。
- `double` 表示函数返回双精度浮点数。
- 末尾的 `const` 表示这个成员函数不应修改当前对象的可观察状态。
- `= 0` 把它声明为纯虚函数，并让 `Shape` 成为抽象类。
- `[[nodiscard]]` 提醒调用者不要随意丢弃返回的面积；忽略结果时，编译器通常会给出警告。

#### 受保护的数据与输出接口

```cpp
protected:
    int id = 0;
    virtual void print(std::ostream& os) const = 0;
```

- `protected` 成员可以被 `Shape` 自己及其派生类访问，但普通外部代码不能直接访问。
- `id` 保存对象编号。
- `void` 表示 `print` 不返回值。
- `std::ostream&` 是输出流的引用；传引用可以避免复制流对象，也允许函数把内容写进原来的流。
- `print` 是纯虚函数，要求每个具体形状决定怎样输出自身信息。

把 `print` 设为 `protected` 表示外部代码不直接调用它，而是统一使用下面的 `operator<<`。

#### 友元输出运算符

```cpp
friend std::ostream& operator<<(std::ostream& os, const Shape& shape) {
    os << "[Shape Info] ";
    shape.print(os);
    return os;
}
```

这段代码重载了 `<<`，因此可以写 `std::cout << shape`。

- `friend` 让这个非成员函数能够访问 `Shape` 的 `protected` 或 `private` 成员。
- 它先输出统一前缀，再调用虚函数 `shape.print(os)`。
- 因为 `print` 是虚函数，如果 `shape` 实际引用的是 `Circle`，就会调用 `Circle::print()`。
- 返回 `os` 的引用后，可以继续链式输出，例如 `std::cout << shape << "\n"`。

这种设计把“统一的输出入口”和“每种形状自己的输出细节”分开了。

### 3.4 派生类 `Circle`

```cpp
class Circle final : public Shape {
```

- `: public Shape` 表示 `Circle` 公有继承 `Shape`；一个 `Circle` 可以被当成一个 `Shape` 使用。
- `final` 表示不允许再从 `Circle` 派生新类。

```cpp
private:
    double radius;
    std::string tag;
```

每个圆独立保存半径 `radius` 和文本标签 `tag`。

```cpp
explicit Circle(double r, std::string t = "Default Circle");
```

- 这里只声明构造函数，函数体在 `Shape.cpp` 中。
- `explicit` 禁止编译器通过单个参数进行意外的隐式类型转换。
- `t = "Default Circle"` 是默认实参；调用者省略第二个参数时使用该字符串。

```cpp
[[nodiscard]] double getArea() const override;
void print(std::ostream& os) const override;
```

- `override` 明确表示这些函数要覆盖基类虚函数。
- 如果函数名、参数或 `const` 写错，导致没有真正覆盖基类函数，编译器会报错。
- `getArea` 是 `public`，外部可以调用；`print` 是 `protected`，主要由统一的输出运算符间接调用。

## 4. `Shape.cpp` 的实现细节

```cpp
Circle::Circle(double r, std::string t)
    : radius(r), tag(std::move(t)) {}
```

- `Circle::Circle` 中的 `::` 表示这个构造函数属于 `Circle`。
- 冒号后的部分叫成员初始化列表，它在进入函数体之前直接构造 `radius` 和 `tag`。
- `radius(r)` 用参数 `r` 初始化半径。
- `std::move(t)` 把参数 `t` 转成可移动的值，使 `tag` 可以接管字符串内部资源，通常比再复制一次更高效。
- `{}` 是空函数体，因为初始化工作已经完成。

```cpp
double Circle::getArea() const {
    return 3.14159 * radius * radius;
}
```

面积公式为 πr²。这里的 `3.14159` 是近似值。

```cpp
void Circle::print(std::ostream& os) const {
    os << ...;
}
```

它把 `Circle` 的编号、半径和标签写入调用者提供的输出流。

## 5. `main.cpp` 的关键流程

```cpp
std::unique_ptr<Shape> s1 =
    std::make_unique<Circle>(5.0, "Smart_Circle_A");
```

实际创建的是 `Circle`，管理它的指针类型却是 `unique_ptr<Shape>`。这同时体现了：

- `Circle` 到 `Shape` 的向上转型；
- 通过虚函数实现多态；
- 通过智能指针自动释放内存。

```cpp
std::unique_ptr<Shape> s2 = std::move(s1);
```

`unique_ptr` 表示独占所有权，不能复制，只能移动。移动后 `s2` 拥有对象，`s1` 变为空指针；对象本身没有被复制或移动，只是管理权发生了转移。

```cpp
if (!s1) { ... }
```

`!s1` 检查智能指针是否为空。为空时条件为真。

```cpp
std::vector<std::unique_ptr<Shape>> canvas;
canvas.push_back(std::move(s2));
canvas.push_back(std::make_unique<Circle>(10.0, "Smart_Circle_B"));
```

`canvas` 是动态数组，其中每个元素都是独占管理一个 `Shape` 的智能指针。`push_back` 把新元素追加到末尾。

```cpp
for (const auto& shape : canvas) {
    std::cout << *shape << " | Area: " << shape->getArea() << "\n";
}
```

这是范围 `for` 循环：

- `auto` 让编译器推导元素类型；
- `const auto&` 以只读引用访问元素，不复制 `unique_ptr`；
- `*shape` 解引用智能指针，得到它管理的 `Shape` 对象；
- `shape->getArea()` 通过指针调用成员函数；
- 虚函数机制会选择实际对象 `Circle` 的实现。

## 6. 代码中出现的 C++ 关键字

严格来说，“关键字”是语言保留的单词。下面覆盖这三个文件中实际出现的关键字：

| 关键字 | 含义 |
| --- | --- |
| `auto` | 让编译器根据初始化表达式推导变量类型。 |
| `class` | 定义类；类成员默认访问权限是 `private`。 |
| `const` | 表示对象、引用或成员函数具有“不修改”约束。 |
| `double` | 双精度浮点数类型。 |
| `explicit` | 禁止构造函数参与不期望的隐式转换。 |
| `final` | 禁止类继续被继承；也可用于禁止虚函数继续覆盖。 |
| `for` | 循环语句；这里使用范围 `for` 遍历容器。 |
| `friend` | 授予指定的非成员函数或类访问私有、受保护成员的权限。 |
| `if` | 条件判断语句。 |
| `inline` | 这里用于允许静态数据成员在头文件内定义；不等同于强制编译器内联函数。 |
| `int` | 整数类型。 |
| `noexcept` | 声明函数不会抛出异常。 |
| `operator` | 声明或定义重载运算符函数。 |
| `override` | 检查当前虚函数确实覆盖了基类虚函数。 |
| `private` | 只允许当前类及其友元直接访问后面的成员。 |
| `protected` | 允许当前类、派生类及友元直接访问后面的成员。 |
| `public` | 允许外部代码访问；公有继承也表达“派生类是一种基类”。 |
| `return` | 从函数返回值，或结束函数执行。 |
| `static` | 让数据成员由所有对象共享，或让成员函数不绑定具体对象。 |
| `this` | 当前成员函数所操作对象的隐藏指针；`*this` 表示当前对象本身。 |
| `void` | 表示函数没有返回值。 |
| `virtual` | 启用运行时多态；通过基类指针或引用调用时选择实际对象的实现。 |

补充说明：`main`、`std`、`string`、`vector`、`unique_ptr`、`move`、`cout` 等是标识符或标准库名称，不是 C++ 语言关键字。`#include`、`#ifndef`、`#define`、`#endif` 是预处理指令，也不是 C++ 关键字。

## 7. 重要标准库名称

| 名称 | 含义 |
| --- | --- |
| `std` | C++ 标准库使用的命名空间。 |
| `std::string` | 可自动管理内存的字符串类型。 |
| `std::ostream` | 输出流类型，是 `std::cout` 等输出对象的基础类型。 |
| `std::cout` | 向标准输出（通常是终端）写内容。 |
| `std::unique_ptr<T>` | 独占拥有一个动态对象的智能指针；离开作用域时自动删除对象。 |
| `std::make_unique<T>(...)` | 创建 `T` 对象并返回管理它的 `unique_ptr`，比直接写 `new` 更安全。 |
| `std::move(x)` | 把 `x` 转换成可被移动的右值表达式；它本身不执行移动。 |
| `std::vector<T>` | 大小可动态增长的连续数组容器。 |
| `push_back` | 在 `vector` 尾部添加一个元素。 |

## 8. 关键符号和语法

| 写法 | 含义 |
| --- | --- |
| `::` | 作用域解析符，例如 `std::cout`、`Circle::getArea`。 |
| `&` | 在声明中表示左值引用；例如 `const Shape&`。 |
| `&&` | 在声明中表示右值引用；常用于移动语义。 |
| `*p` | 解引用指针，取得指针所指的对象。 |
| `~Shape()` | `~` 放在类名之前表示析构函数，在对象销毁时执行清理工作。 |
| `p->member` | 通过指针访问对象成员。 |
| `!value` | 逻辑非；智能指针为空时 `!pointer` 为真。 |
| `++x` / `x++` | 把数值加一；前置形式先加再取值，后置形式先取旧值再加。 |
| `--` | 把数值减一；这里用于对象销毁时减少计数。 |
| `<<` | 对输出流表示写入；本项目还为 `Shape` 重载了它。 |
| `<T>` | 模板参数列表，例如 `unique_ptr<Shape>`。连续的 `>>` 是两层模板参数的结束符。 |
| `= 0` | 用在虚函数声明末尾时表示纯虚函数，不是普通赋值。 |
| `= 默认值` | 用在形参中表示默认实参。 |
| `:` | 用于继承、构造函数成员初始化列表、范围 `for`。具体意义由位置决定。 |
| `;` | 结束一条声明或语句；类定义结束后的分号不可省略。 |
| `{}` | 形成代码块、函数体或类体；空 `{}` 表示空函数体。 |
| `()` | 函数参数列表、函数调用，或构造初始化。 |
| `[]` | `[[nodiscard]]` 中的双方括号用于写标准属性。 |
| `//`、`/* */` | 单行注释和块注释，编译时不会作为程序执行。 |
| `\n` | 换行转义字符；`"\n\n"` 输出两个换行。 |

## 9. 编译与运行

在当前 `demo2` 目录中，可以用下面的命令编译：

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Shape.cpp -o ../../bin/demo2
../../bin/demo2
```

选项含义：

- `-std=c++17`：使用 C++17 标准；`inline static` 数据成员需要它。
- `-Wall`：开启一组常用警告。
- `-Wextra`：开启更多额外警告。
- `-pedantic`：对不符合标准的扩展给出诊断。
- `-o ../../bin/demo2`：把生成的可执行文件命名为 `demo2`，并放到与 `src/` 分开的 `week_01/bin/` 目录。

## 10. 值得注意的改进点

1. `Shape.cpp` 和 `main.cpp` 都使用了 `std::move`。规范写法应当直接 `#include <utility>`，不要依赖其他头文件间接包含它。
2. `Shape.h` 没有使用 `std::vector`，可以移除其中的 `#include <vector>`。
3. `Shape.h` 只需要输出流声明时，可以使用 `<ostream>` 代替 `<iostream>`。
4. 圆周率可以用更精确的常量；当前 `3.14159` 对演示来说足够，但存在近似误差。
5. `shapeCount` 统计的是当前存活对象数，不是永不重复的全局 ID 生成器。对象销毁后计数会减少，随后创建的新对象可能获得与旧对象相同的 `id`。如果要求 ID 永远唯一，应把“存活数量”和“下一个 ID”拆成两个静态变量。
