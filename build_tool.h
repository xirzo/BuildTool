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

    const char** include_dirs;
    size_t include_dirs_count;
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

    compiler->include_dirs = NULL;
    compiler->include_dirs_count = 0;

    return compiler;
}

void freeBuildCompiler(BuildCompiler* compiler) {
    if (compiler == NULL) {
        return;
    }

    free(compiler->flags);
    free(compiler->filepaths);
    free(compiler->include_dirs);
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

void setInputFiles(BuildCompiler* compiler, size_t filepaths_count, const char* filepath,
                   ...) {
    if (compiler == NULL || filepath == NULL) {
        fprintf(stderr, "error: Invalid compiler or Filepath\n");
        return;
    }

    if (filepaths_count == 0) {
        fprintf(stderr, "error: Filepaths count should not be zero\n");
        return;
    }

    compiler->filepaths = (const char**)malloc(filepaths_count * sizeof(const char*));
    compiler->filepaths_count = filepaths_count;

    if (compiler->filepaths == NULL) {
        fprintf(stderr, "error: Failed to allocate memory for Filepaths\n");
        return;
    }

    va_list args;

    compiler->filepaths[0] = filepath;

    va_start(args, filepath);

    for (size_t i = 1; i < filepaths_count; i++) {
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

void setIncludeDirectories(BuildCompiler* compiler, size_t dir_count, const char* dir,
                           ...) {
    if (compiler == NULL || dir == NULL) {
        fprintf(stderr, "error: Invalid compiler or include directory\n");
        return;
    }

    if (dir_count == 0) {
        fprintf(stderr, "error: Include directory count should not be zero\n");
        return;
    }

    compiler->include_dirs = (const char**)malloc(dir_count * sizeof(const char*));
    compiler->include_dirs_count = dir_count;

    if (compiler->include_dirs == NULL) {
        fprintf(stderr, "error: Failed to allocate memory for include directories\n");
        return;
    }

    va_list args;

    compiler->include_dirs[0] = dir;

    va_start(args, dir);

    for (size_t i = 1; i < dir_count; i++) {
        char* arg = va_arg(args, char*);

        if (arg == NULL) {
            fprintf(stderr,
                    "error: Failed to get arg at index %zu, it`s likely you`ve provided "
                    "wrong include directory count or put NULL into args\n",
                    i);
            return;
        }

        compiler->include_dirs[i] = arg;
    }

    va_end(args);
}

void executeBuildCompiler(BuildCompiler* compiler) {
    if (compiler == NULL) {
        fprintf(stderr, "error: Invalid compiler\n");
        return;
    }

    size_t length = 0;

    for (size_t i = 0; i < compiler->flags_count; i++) {
        length += strlen(compiler->flags[i]) + 1;
    }

    for (size_t i = 0; i < compiler->include_dirs_count; i++) {
        length += strlen("-I") + strlen(compiler->include_dirs[i]) + 1;
    }

    for (size_t i = 0; i < compiler->filepaths_count; i++) {
        const char* filepath = compiler->filepaths[i];
        size_t filepath_len = strlen(filepath);

        if (filepath_len > 2 && filepath[filepath_len - 2] == '.' &&
            filepath[filepath_len - 1] == 'c') {
            length += filepath_len + 1;
        }
    }

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

    for (size_t i = 0; i < compiler->include_dirs_count; i++) {
        strcat(request, "-I");
        strcat(request, compiler->include_dirs[i]);
        strcat(request, " ");
    }

    for (size_t i = 0; i < compiler->filepaths_count; i++) {
        const char* filepath = compiler->filepaths[i];
        size_t filepath_len = strlen(filepath);

        if (filepath_len > 2 && filepath[filepath_len - 2] == '.' &&
            filepath[filepath_len - 1] == 'c') {
            strcat(request, filepath);
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
        free(request);
        free(command);
        return;
    }

    printf("\033[1;32mSuccessfully\033[0m built program with \033[1;32mcode\033[0m %d!\n",
           result);
    free(request);
    free(command);
}

#endif  // BUILD_TOOL_H
