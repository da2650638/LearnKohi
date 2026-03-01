````markdown
# VSCode `.vscode/tasks.json` 速查（按 `c_cpp_properties.json` 那种“字段-作用-常见值”规格）

> 作用：定义 VSCode 里的 Task（一键执行 build / run / clean / test 等命令）。  
> 只影响 VSCode 如何“调用命令”，不改变你的真实构建系统（CMake/Make/Ninja/脚本才是构建本体）。

---

## 顶层结构

```json
{
  "version": "2.0.0",
  "tasks": [ ... ]
}
````

### version

* 含义：tasks.json 的格式版本
* 常见值：`"2.0.0"`（基本固定）

### tasks

* 含义：任务数组；每个元素是一条 Task

---

# tasks[] 内部字段（最常用 + 常见值）

> 下面字段是你写任务时 90% 用到的。

---

## 1) label ⭐⭐⭐⭐⭐

```json
"label": "Build"
```

* 含义：任务名字（Run Task / Ctrl+Shift+B 列表里显示）
* 常见值：`Build`, `Build Debug`, `Clean`, `Run`, `Test`

---

## 2) type ⭐⭐⭐⭐⭐

```json
"type": "shell"
```

* 含义：任务执行方式
* 常见值：

  * `"shell"`：通过 shell 执行（最常用，适合脚本、管道、复合命令）
  * `"process"`：直接执行可执行文件（更“干净”，不经过 shell）

---

## 3) command ⭐⭐⭐⭐⭐

```json
"command": "cmake"
```

* 含义：要运行的命令/脚本
* 常见值：

  * `"cmake"`, `"ninja"`, `"make"`, `"msbuild"`
  * `"bash"`, `"pwsh"`, `"cmd"`
  * `"${workspaceFolder}/build.bat"`, `"${workspaceFolder}/build.sh"`

---

## 4) args ⭐⭐⭐⭐

```json
"args": ["--build", "build", "-j", "8"]
```

* 含义：命令行参数列表（比把参数写进 command 更稳）
* 常见场景：CMake / Ninja / 自己脚本参数

---

## 5) options ⭐⭐⭐⭐

```json
"options": {
  "cwd": "${workspaceFolder}",
  "env": { "FOO": "BAR" }
}
```

* 含义：任务运行环境配置
* 常用子字段：

  * `options.cwd`：工作目录（相当于先 cd 到这里）

    * 常见值：`${workspaceFolder}`, `${workspaceFolder}/build`
  * `options.env`：环境变量（只对这个 task 生效）

    * 常见：`CC`, `CXX`, `VULKAN_SDK`, `PATH` 等

---

## 6) problemMatcher ⭐⭐⭐⭐

```json
"problemMatcher": ["$gcc"]
```

* 含义：解析输出，把 error/warning 变成 VSCode 的“问题”（可点击跳转到文件行号）
* 常见值：

  * `"$gcc"`：GCC/Clang 风格输出（Linux/MinGW/clang 常用）
  * `"$msCompile"`：MSVC cl.exe 风格输出
  * `"$tsc"`：TypeScript（不做 C++ 时不用）
  * 自定义 matcher（大工程/特殊日志）

---

## 7) group ⭐⭐⭐⭐

```json
"group": "build"
```

或

```json
"group": { "kind": "build", "isDefault": true }
```

* 含义：把任务归类，并决定 Ctrl+Shift+B 触发哪个任务
* 常见值：

  * `"build"`：构建任务
  * `"test"`：测试任务
  * `{ "kind": "build", "isDefault": true }`：设为默认 build（Ctrl+Shift+B）

---

## 8) dependsOn ⭐⭐⭐

```json
"dependsOn": ["Configure", "Build"]
```

* 含义：这个任务运行前先运行哪些任务（按 label 引用）
* 常见值：数组或单个字符串

### dependsOrder

```json
"dependsOrder": "sequence"
```

* 常见值：

  * `"sequence"`：按顺序跑
  * `"parallel"`：并行跑（默认通常是 parallel）

---

## 9) presentation ⭐⭐⭐

```json
"presentation": {
  "reveal": "always",
  "panel": "shared",
  "clear": false,
  "focus": false
}
```

* 含义：终端面板展示行为（提升体验用）
* 常见值：

  * `reveal`: `"always" | "silent" | "never"`
  * `panel`: `"shared" | "dedicated" | "new"`
  * `clear`: `true/false`
  * `focus`: `true/false`

---

## 10) runOptions ⭐⭐⭐

```json
"runOptions": {
  "runOn": "default"
}
```

* 含义：任务运行策略
* 常见值：

  * `"default"`：默认手动触发
  * `"folderOpen"`：打开文件夹时自动跑（少用，慎用）

---

# 平台覆盖字段（跨平台常用）

> 这块就像 `c_cpp_properties.json` 的 `windows/linux/osx` 一样：按 OS 覆盖同名字段。

## windows / linux / osx ⭐⭐⭐⭐

```json
"windows": { "command": "build.bat" },
"linux":   { "command": "./build.sh" },
"osx":     { "command": "./build_macos.sh" }
```

* 含义：在特定 OS 下覆盖 `command/args/options/problemMatcher/...`

---

# 进阶字段（常见但不是必需）

## isBackground / background ⭐⭐⭐

```json
"isBackground": true,
"background": {
  "activeOnStart": true,
  "beginsPattern": "Starting watch",
  "endsPattern": "Watching for changes"
}
```

* 含义：后台任务（watch 模式：比如 `ninja -t recompact`/热重载/watch 编译）
* 常见场景：`webpack watch`、某些文件监视器

## detail

```json
"detail": "Build engine module"
```

* 含义：任务描述（UI 展示用）

---

# 最常用字段速记（你真的只要背这个）

```
label
type
command
args
options.cwd
options.env
problemMatcher
group
dependsOn
presentation
(windows/linux/osx)
```

---

# 两个最常见坑（Tasks 写不对基本都栽这）

1. **JSON 不允许尾逗号**

* `tasks.json` 不是 `jsonc`，别在最后一个字段后加 `,`

2. **problemMatcher 不匹配编译器输出**

* clang/gcc → `$gcc`
* MSVC cl.exe → `$msCompile`
* 不匹配就会“看起来能编译，但点不了报错跳转”

---
