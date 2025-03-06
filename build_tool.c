#include "build_tool.h"

int main(void) {
    BuildCompiler *compiler = createBuildCompiler();

    setCompilerFlags(compiler, 5, "-std=c99", "-Wall", "-Werror", "-o", "hello_worder");
    setIncludeDirectories(compiler, 1, "lib");
    setInputFiles(compiler, 3, "lib/log.c", "lib/log.h", "bin/main.c");

    executeBuildCompiler(compiler);

    freeBuildCompiler(compiler);
    return 0;
}
