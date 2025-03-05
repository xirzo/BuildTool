#ifndef BUILD_TOOL_H
#define BUILD_TOOL_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BuildCompiler
{
    const char** flags;
    size_t flags_count;
} BuildCompiler;

BuildCompiler* createBuildCompiler() {
    BuildCompiler* compiler = (BuildCompiler*)malloc(sizeof(*compiler));

    if (compiler == NULL) {
        fprintf(stderr, "error: Failed to allocate memory for BuildCompiler");
        return NULL;
    }

    compiler->flags = NULL;
    compiler->flags = 0;

    return compiler;
}

void freeBuildCompiler(BuildCompiler* compiler) {
    if (compiler == NULL) {
        return;
    }

    free(compiler->flags);
    free(compiler);
}

void setCompilerFlags(BuildCompiler* compiler, size_t flags_count, const char* flag,
                      ...) {
    if (compiler == NULL || flag == NULL) {
        fprintf(stderr, "error: Invalid compiler or flag\n");
        return;
    }

    if (flags_count == 0) {
        fprintf(stderr, "error: flags count should not be zero\n");
        return;
    }

    compiler->flags = (const char**)malloc(flags_count * sizeof(const char*));
    compiler->flags_count = flags_count;

    if (compiler->flags == NULL) {
        fprintf(stderr, "error: Failed to allocate memory for flags\n");
        return;
    }

    va_list args;

    compiler->flags[0] = flag;

    va_start(args, flag);

    for (size_t i = 1; i < flags_count; i++) {
        const char* arg = va_arg(args, const char*);

        if (arg == NULL) {
            fprintf(stderr,
                    "error: Failed to get arg at index %zu, it`s likely you`ve provided "
                    "wrong flags count or put NULL into args\n",
                    i);
            return;
        }

        compiler->flags[i] = arg;
    }

    va_end(args);
}

#endif  // BUILD_TOOL_H
