#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* An elevated invocation must never follow a caller-writable path. */
static bool trusted_script_path(const char *path)
{
    char *prefix = strdup(path);
    if (prefix == NULL)
        return false;

    bool trusted = true;
    for (char *cursor = prefix + 1; ; cursor++) {
        bool is_script = *cursor == '\0';
        if (!is_script && *cursor != '/')
            continue;

        struct stat entry;
        *cursor = '\0';
        if (lstat(prefix, &entry) != 0 || entry.st_uid != 0 ||
            (entry.st_mode & (S_IWGRP | S_IWOTH)) ||
            (is_script ? !S_ISREG(entry.st_mode) : !S_ISDIR(entry.st_mode))) {
            trusted = false;
            break;
        }
        if (is_script)
            break;
        *cursor = '/';
    }

    free(prefix);
    return trusted;
}

int main(int argc, char **argv)
{
    char script_path[] = SCRIPTPATH;
    uid_t caller = getuid();
    uid_t effective_user = geteuid();
    if (script_path[0] != '/' || script_path[1] == '\0' ||
        (caller != effective_user && !trusted_script_path(script_path))) {
        fprintf(stderr, "serveqcow2: untrusted script path: %s\n", script_path);
        return EXIT_FAILURE;
    }

    char **script_args = calloc((size_t)argc + 3, sizeof(*script_args));
    if (script_args == NULL) {
        perror("serveqcow2: allocating arguments");
        return EXIT_FAILURE;
    }
    script_args[0] = script_path;
    for (int i = 1; i < argc; i++)
        script_args[i] = argv[i];

    char uid[32];
    if (argc > 1 && (strcmp(argv[1], "mount") == 0 || strcmp(argv[1], "m") == 0)) {
        snprintf(uid, sizeof(uid), "%ju", (uintmax_t)caller);
        script_args[argc] = "--user-id";
        script_args[argc + 1] = uid;
    }

    /* Use the system tools and prevent shell startup settings from the caller. */
    char *const environment[] = {"PATH=/usr/sbin:/usr/bin:/sbin:/bin", "LC_ALL=C", NULL};
    if (setuid(effective_user) == 0)
        execve(script_path, script_args, environment);

    perror("serveqcow2");
    free(script_args);
    return EXIT_FAILURE;
}
