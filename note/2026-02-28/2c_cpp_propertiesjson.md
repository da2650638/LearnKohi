````markdown
# VSCode `.vscode/c_cpp_properties.json` 字段全集（常用 + 常见值）

---

## 顶层结构

```json
{
  "configurations": [],
  "version": 4,
  "env": {}
}
````

---

# 顶层字段
v
---

## version

```json
"version": 4
```

固定值：

```
4
```

---

## configurations

```json
"configurations": [ { ... } ]
```

数组，可包含多个配置（Win/Linux/Debug/Release）

---

## env

```json
"env": {
  "myIncludePath": ["D:/libs"]
}
```

作用：

* 定义变量
* 可在 includePath 中使用 `${myIncludePath}`

---

# configurations 内部字段全集

---

## name

```json
"name": "Win64"
```

常见值：

```
Win64
Linux
Debug
Release
```

---

## includePath ⭐⭐⭐⭐⭐

```json
"includePath": [
  "${workspaceFolder}/**",
  "D:/libs/glm",
  "/usr/include"
]
```

常见值：

```
${workspaceFolder}/**
第三方库路径
/usr/include
```

---

## defines ⭐⭐⭐⭐

```json
"defines": [
  "_DEBUG",
  "NDEBUG",
  "_WIN32",
  "VK_USE_PLATFORM_WIN32_KHR"
]
```

常见值：

Debug：

```
_DEBUG
```

Release：

```
NDEBUG
```

Windows：

```
_WIN32
```

Vulkan：

```
VK_USE_PLATFORM_WIN32_KHR
VK_USE_PLATFORM_XCB_KHR
VK_USE_PLATFORM_WAYLAND_KHR
```

---

## compilerPath ⭐⭐⭐⭐⭐

```json
"compilerPath": "C:/msys64/mingw64/bin/g++.exe"
```

常见值：

MSVC：

```
C:/Program Files/.../cl.exe
```

MinGW：

```
C:/msys64/mingw64/bin/g++.exe
```

Linux：

```
/usr/bin/g++
/usr/bin/clang++
```

Mac：

```
/usr/bin/clang++
```

---

## compilerArgs

```json
"compilerArgs": [
  "-std=c++20",
  "-Wall"
]
```

作用：

* 额外编译参数
* 影响 IntelliSense

---

## cStandard

```json
"cStandard": "c17"
```

常见值：

```
c11
c17
c23
gnu11
gnu17
```

---

## cppStandard ⭐⭐⭐⭐⭐

```json
"cppStandard": "c++20"
```

常见值：

```
c++11
c++14
c++17
c++20
c++23
gnu++17
gnu++20
```

---

## intelliSenseMode ⭐⭐⭐⭐

```json
"intelliSenseMode": "windows-gcc-x64"
```

Windows：

```
windows-msvc-x64
windows-gcc-x64
windows-clang-x64
```

Linux：

```
linux-gcc-x64
linux-clang-x64
linux-gcc-arm64
```

Mac：

```
macos-clang-x64
macos-clang-arm64
```

---

## compileCommands ⭐⭐⭐⭐⭐

```json
"compileCommands": "${workspaceFolder}/build/compile_commands.json"
```

常见值：

```
${workspaceFolder}/build/compile_commands.json
```

---

## forcedInclude

```json
"forcedInclude": [
  "D:/project/pch.hpp"
]
```

作用：

* 强制包含头文件

---

## browse

```json
"browse": {
  "path": [
    "${workspaceFolder}"
  ],
  "limitSymbolsToIncludedHeaders": true,
  "databaseFilename": ""
}
```

字段：

browse.path
常见：

```
${workspaceFolder}
```

browse.limitSymbolsToIncludedHeaders

```
true
false
```

browse.databaseFilename

* 自定义索引数据库文件名

---

## configurationProvider

```json
"configurationProvider": "ms-vscode.cmake-tools"
```

常见值：

```
ms-vscode.cmake-tools
```

---

## windowsSdkVersion

```json
"windowsSdkVersion": "10.0.22621.0"
```

示例：

```
10.0.19041.0
10.0.22621.0
```

---

## macFrameworkPath

```json
"macFrameworkPath": [
  "/System/Library/Frameworks"
]
```

---

## dotConfig

```json
"dotConfig": "${workspaceFolder}/.config"
```

作用：

* 读取 Kconfig 风格配置文件

---

# 最常用字段总结（必须记住）

```
includePath
defines
compilerPath
cppStandard
intelliSenseMode
compileCommands
```

---

# 最小典型配置

```json
{
  "configurations": [
    {
      "name": "Win64",
      "includePath": [
        "${workspaceFolder}/**"
      ],
      "defines": [
        "_DEBUG",
        "_WIN32"
      ],
      "compilerPath": "C:/msys64/mingw64/bin/g++.exe",
      "cStandard": "c17",
      "cppStandard": "c++20",
      "intelliSenseMode": "windows-gcc-x64"
    }
  ],
  "version": 4
}
```

---

# CMake 工程推荐最小写法

```json
{
  "configurations": [
    {
      "name": "CMake",
      "compileCommands": "${workspaceFolder}/build/compile_commands.json"
    }
  ],
  "version": 4
}
```

```
```
