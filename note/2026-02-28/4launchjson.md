下面给你一份 **和前面两个一样规格的 launch.json 说明**。
纯字段解释 + 常见值 + 最小模板。
不废话。

---

````markdown
# VSCode `.vscode/launch.json` 字段速查

> 作用：配置调试器如何启动 / 附加程序  
> 不负责构建（那是 tasks.json）  
> 不负责 IntelliSense（那是 c_cpp_properties.json）

---

# 顶层结构

```json
{
  "version": "0.2.0",
  "configurations": []
}
````

---

## version

```json
"version": "0.2.0"
```

固定值：

```
0.2.0
```

---

## configurations

数组，每个对象代表一个调试配置。

---

# configurations[] 内部字段全集（常用 + 常见值）

---

## 1️⃣ name ⭐⭐⭐⭐⭐

```json
"name": "Debug Engine"
```

作用：

* 调试配置名称
* 在 F5 下拉列表中显示

常见值：

```
Debug
Debug Engine
Debug Testbed
Release
```

---

## 2️⃣ type ⭐⭐⭐⭐⭐

```json
"type": "cppdbg"
```

作用：

* 指定调试器类型

常见值：

### C/C++

```
cppdbg    （GDB / LLDB）
cppvsdbg  （MSVC 调试器，仅 Windows）
```

---

## 3️⃣ request ⭐⭐⭐⭐⭐

```json
"request": "launch"
```

作用：

* 调试方式

常见值：

```
launch   → 启动程序调试
attach   → 附加到已有进程
```

---

## 4️⃣ program ⭐⭐⭐⭐⭐

```json
"program": "${workspaceFolder}/bin/testbed.exe"
```

作用：

* 要调试的可执行文件路径

常见写法：

```
${workspaceFolder}/bin/app.exe
${workspaceFolder}/build/app
```

---

## 5️⃣ args ⭐⭐⭐⭐

```json
"args": ["--width", "1280"]
```

作用：

* 程序启动参数

常见：

```
[]
["-v"]
["--config", "debug.json"]
```

---

## 6️⃣ cwd ⭐⭐⭐⭐

```json
"cwd": "${workspaceFolder}"
```

作用：

* 程序运行时的工作目录

常见值：

```
${workspaceFolder}
${workspaceFolder}/bin
```

---

## 7️⃣ stopAtEntry

```json
"stopAtEntry": false
```

作用：

* 是否在 main 入口处暂停

常见：

```
true
false
```

---

## 8️⃣ environment

```json
"environment": [
  { "name": "VULKAN_SDK", "value": "C:\\VulkanSDK" }
]
```

作用：

* 运行时环境变量

常见：

```
PATH
VULKAN_SDK
LD_LIBRARY_PATH
```

---

## 9️⃣ externalConsole

```json
"externalConsole": false
```

作用：

* 是否使用外部终端

常见：

```
true   → 外部窗口
false  → VSCode 内置终端
```

---

## 🔟 MIMode（仅 cppdbg）

```json
"MIMode": "gdb"
```

常见值：

```
gdb
lldb
```

Windows + MSYS2：

```
gdb
```

Linux：

```
gdb
lldb
```

---

## 1️⃣1️⃣ miDebuggerPath

```json
"miDebuggerPath": "C:/msys64/mingw64/bin/gdb.exe"
```

作用：

* 指定 gdb 路径

Linux 常见：

```
/usr/bin/gdb
```

Windows 常见：

```
C:/msys64/mingw64/bin/gdb.exe
```

---

## 1️⃣2️⃣ preLaunchTask ⭐⭐⭐⭐

```json
"preLaunchTask": "Build Everything"
```

作用：

* 在调试前自动运行某个 tasks.json 任务

常见：

```
Build
Build Everything
```

---

## 1️⃣3️⃣ console

```json
"console": "integratedTerminal"
```

常见值：

```
internalConsole
integratedTerminal
externalTerminal
```

---

# 最常用字段总结（必须记住）

```
name
type
request
program
args
cwd
MIMode
miDebuggerPath
preLaunchTask
```

---

# 最小 C++ 调试模板（Windows + clang + gdb）

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug Testbed",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/bin/testbed.exe",
      "cwd": "${workspaceFolder}",
      "MIMode": "gdb",
      "miDebuggerPath": "C:/msys64/mingw64/bin/gdb.exe",
      "preLaunchTask": "Build Everything"
    }
  ]
}
```

