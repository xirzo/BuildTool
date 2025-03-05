#include "build_tool.h"

int main(void) {
    BuildCompiler *compiler = createBuildCompiler();

    setCompilerFlags(compiler, 5, "-std=c99", "-Wall", "-Werror", "-o", "hello_worder");
    setInputFiles(compiler, 3, "log.c", "log.h", "main.c");

    executeBuildCompiler(compiler);

    freeBuildCompiler(compiler);
    return 0;
}
