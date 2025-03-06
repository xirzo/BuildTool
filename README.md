# 🛠️ BuildTool 🏗️

## 📋 Overview

**BuildTool** is a single-header C library that simplifies the process of compiling C programs programmatically. It was created just for fun while watching [Tsoding youtube video](https://www.youtube.com/watch?v=GrG5_U0NHKk) of same topic.

## 📥 Installation

Simply include the header file in your project:

```c
#include "build_tool.h"
```

## 🚀 Usage

### Basic Example

```c
#include "build_tool.h"

int main() {
    // Create a compiler instance
    BuildCompiler* compiler = createBuildCompiler();
    
    // Set compiler flags
    setCompilerFlags(compiler, 2, "-Wall", "-O2");
    
    // Set input files
    setInputFiles(compiler, 2, "main.c", "utils.c");
    
    // Set include directories
    setIncludeDirectories(compiler, 1, "include");
    
    // Execute the build
    executeBuildCompiler(compiler);
    
    // Free resources
    freeBuildCompiler(compiler);
    
    return 0;
}
```

### 🏗️ Build Your Project

Compile the build script and run it:

```bash
cc -o build_tool build_tool.c
./build_tool
```
