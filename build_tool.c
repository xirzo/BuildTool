#include "build_tool.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    BuildCompiler *compiler = createBuildCompiler();

    setCompilerFlags(compiler, 3, "-std=c99", "-Wall", "-Werror");

    for (size_t i = 0; i < compiler->flags_count; ++i) {
        printf("%s\t", compiler->flags[i]);
    }

    freeBuildCompiler(compiler);
    return 0;
}
