#include "build_tool.h"

int main(void) {
    BuildCompiler *compiler = createBuildCompiler();

    setCompilerFlags(compiler, 3, "-std=c99", "-Wall", "-Werror");
    setInputFiles(compiler, 2, "build_tool.c", "build_tool.h");

    executeBuildCompiler(compiler);

    freeBuildCompiler(compiler);
    return 0;
}
