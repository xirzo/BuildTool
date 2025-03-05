#ifndef BUILD_TOOL_H
#define BUILD_TOOL_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BuildCompiler
{
    const char** flags;
    size_t flags_count;

    const char** filepaths;
    size_t filepaths_count;
} BuildCompiler;

BuildCompiler* createBuildCompiler() {
    BuildCompiler* compiler = (BuildCompiler*)malloc(sizeof(*compiler));

    if (compiler == NULL) {
        fprintf(stderr, "error: Failed to allocate memory for BuildCompiler");
        return NULL;
    }

    compiler->flags = NULL;
    compiler->flags = 0;

    compiler->filepaths = NULL;
    compiler->filepaths_count = 0;
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
        fprintf(stderr, "error: Flags count should not be zero\n");
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
        char* arg = va_arg(args, char*);

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

void setInputFiles(BuildCompiler* compiler, size_t Filepaths_count, const char* Filepath,
                   ...) {
    if (compiler == NULL || Filepath == NULL) {
        fprintf(stderr, "error: Invalid compiler or Filepath\n");
        return;
    }

    if (Filepaths_count == 0) {
        fprintf(stderr, "error: Filepaths count should not be zero\n");
        return;
    }

    compiler->filepaths = (const char**)malloc(Filepaths_count * sizeof(const char*));
    compiler->filepaths_count = Filepaths_count;

    if (compiler->filepaths == NULL) {
        fprintf(stderr, "error: Failed to allocate memory for Filepaths\n");
        return;
    }

    va_list args;

    compiler->filepaths[0] = Filepath;

    va_start(args, Filepath);

    for (size_t i = 1; i < Filepaths_count; i++) {
        char* arg = va_arg(args, char*);

        if (arg == NULL) {
            fprintf(stderr,
                    "error: Failed to get arg at index %zu, it`s likely you`ve provided "
                    "wrong Filepaths count or put NULL into args\n",
                    i);
            return;
        }

        compiler->filepaths[i] = arg;
    }

    va_end(args);
}

void executeBuildCompiler(BuildCompiler* compiler) {
    size_t length = 0;

    for (size_t i = 0; i < compiler->flags_count; i++) {
        length += strlen(compiler->flags[i]);
    }

    for (size_t i = 0; i < compiler->filepaths_count; i++) {
        length += strlen(compiler->filepaths[i]);
    }

    length += compiler->flags_count + compiler->filepaths_count;

    char* request = (char*)malloc(length + 1);

    if (request == NULL) {
        fprintf(stderr, "error: Allocation failed for request line\n");
        return;
    }

    request[0] = '\0';

    for (size_t i = 0; i < compiler->flags_count; i++) {
        strcat(request, compiler->flags[i]);
        strcat(request, " ");
    }

    for (size_t i = 0; i < compiler->filepaths_count; i++) {
        strcat(request, compiler->filepaths[i]);

        if (i < compiler->filepaths_count - 1) {
            strcat(request, " ");
        }
    }

    size_t command_length = strlen("cc ") + strlen(request) + 1;
    char* command = (char*)malloc(command_length);

    if (command == NULL) {
        fprintf(stderr, "error: Allocation failed for command\n");
        free(request);
        return;
    }

    snprintf(command, command_length, "cc %s", request);

    int result = system(command);

    if (result == -1) {
        fprintf(stderr, "error: Failed to execute command\n");
        return;
    }

    printf("Succesfully built program with code %d!\n", result);
    free(request);
}

#endif  // BUILD_TOOL_H
