# C++ 每周练习

这是一个适合初学者的 C++ 命令行练习工程。

## 目录规则

每周创建一个独立目录，并把源码和编译结果分开存放：

```text
cpp_learning/
├── week_01/
│   ├── src/    # C++ 源文件
│   └── bin/    # 编译生成的二进制文件
├── week_02/
│   ├── src/
│   └── bin/
└── ...
```

## 编译与运行

先进入本工程目录：

```bash
cd cpp_learning
```

编译第 1 周的程序：

```bash
g++ -std=c++17 -Wall -Wextra -pedantic week_01/src/main.cpp -o week_01/bin/main
```

运行程序：

```bash
./week_01/bin/main
```

编译命令中的选项：

- `-std=c++17`：使用 C++17 标准。
- `-Wall -Wextra`：显示更多有助于学习的编译警告。
- `-pedantic`：对不符合标准的写法给出警告。
- `-o week_01/bin/main`：指定二进制文件的输出位置和名称。

## 开始新的一周

例如创建第 2 周：

```bash
mkdir -p week_02/src week_02/bin
touch week_02/src/main.cpp week_02/bin/.gitkeep
```

然后把编译命令中的 `week_01` 改成 `week_02`。以后按同样方式创建
`week_03`、`week_04` 等目录。

