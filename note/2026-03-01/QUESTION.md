# pushd / popd 是干什么的？

```bash
pushd engine
source build.sh
popd
```

## pushd

* 进入指定目录
* 同时把当前目录压入“目录栈”

等价于：

```bash
cd engine
```

但多了“保存原目录”的能力。

---

## popd

* 从目录栈弹出
* 返回到之前的目录

---

## 举例

假设当前在：

```bash
/home/user/project
```

执行：

```bash
pushd engine
```

现在在：

```bash
/home/user/project/engine
```

然后：

```bash
popd
```

自动回到：

```bash
/home/user/project
```

---

## 和 cd 的区别

```bash
cd engine
cd ..
```

你得手动写 `cd ..`。

pushd/popd 更安全，尤其在复杂脚本中。

---

# -fdeclspec 是什么？

```bash
-fdeclspec
```

这是 GCC/Clang 的一个选项：

👉 启用 `__declspec(...)` 语法支持

---

## 为什么需要它？

在 Windows 下你会写：

```c
__declspec(dllexport)
__declspec(dllimport)
```

但在 Linux 上：

* GCC 默认不支持 `__declspec`
* 需要加 `-fdeclspec`

否则会报：

```text
error: expected declaration specifiers
```

---

## 作用总结

```text
-fdeclspec = 让 GCC/Clang 支持 MSVC 的 __declspec 语法
```

适用于：

* 跨平台共享代码
* 同一套头文件同时支持 Windows/Linux

---

# -Wl,-rpath,. 是什么意思？

```bash
-Wl,-rpath,.
```

这个非常重要。

---

## 先拆开

```text
-Wl,xxx
```

意思是：

> 把后面的参数传给链接器（ld）

因为 gcc/clang 本身是驱动程序。

---

## -rpath 是什么？

rpath = runtime library search path

👉 运行时动态库搜索路径

---

## -Wl,-rpath,.

意思是：

```text
把 "."（当前目录）加入运行时库搜索路径
```

---

## 举例说明

假设你：

* 编译 testbed
* 链接 engine.so
* engine.so 在 ../bin

默认情况下：

Linux 运行时只会在：

```text
/lib
/usr/lib
LD_LIBRARY_PATH
```

找动态库。

如果你没设置 rpath：

```bash
./testbed
```

可能报：

```text
error while loading shared libraries: libengine.so: cannot open shared object file
```

---

## 加了 -rpath,. 会发生什么？

程序会记录：

```text
运行时在当前目录找库
```

这样：

```bash
./testbed
```

就能找到同目录下的 .so

---

## 更标准写法

一般推荐：

```bash
-Wl,-rpath,'$ORIGIN'
```

比 `.` 更安全。

$ORIGIN = 可执行文件所在目录

---


# 更稳的 Linux 链接写法

```bash
linkerFlags="-L../bin -lengine -Wl,-rpath,'\$ORIGIN'"
```

这样：

* 可执行文件在哪
* 就在哪找 .so
* 不依赖运行目录

这是更工程化的方式。
