/*
 * blogc-git-receiver - A simple login shell/git hook to deploy blogc websites.
 * Copyright (C) 2015 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif


static void
mkdir_recursive(const char *filename)
{
    mode_t m = umask(0);
    umask(m);
    mode_t mode = (S_IRWXU | S_IRWXG | S_IRWXO) & ~m;
    char *fname = strdup(filename);

    for (char *tmp = fname; *tmp != '\0'; tmp++) {
        if (*tmp == '/') {
            char bkp = *tmp;
            *tmp = '\0';
            if ((0 < strlen(fname)) &&
                (-1 == mkdir(fname, mode)) &&
                (errno != EEXIST))
            {
                fprintf(stderr, "error: failed to create directory (%s): %s\n",
                    fname, strerror(errno));
                free(fname);
                exit(2);
            }
            *tmp = bkp;
        }
    }
    free(fname);

    if ((-1 == mkdir(filename, mode)) && (errno != EEXIST)) {
        fprintf(stderr, "error: failed to create directory (%s): %s\n",
            filename, strerror(errno));
        exit(2);
    }
}


static int
git_shell(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];

    // validate git command
    if (!((0 == strncmp(argv[2], "git-receive-pack ", 17)) ||
          (0 == strncmp(argv[2], "git-upload-pack ", 16)) ||
          (0 == strncmp(argv[2], "git-upload-archive ", 19))))
    {
        fprintf(stderr, "error: unsupported git command: %s\n", argv[2]);
        return 1;
    }

    // get shell path
    char *self = getenv("SHELL");
    if (self == NULL) {
        fprintf(stderr, "error: failed to find blogc-git-receiver path\n");
        return 1;
    }

    // get home path
    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "error: failed to find user home path\n");
        return 1;
    }

    // get git repository
    char *p, *r, *command = strdup(argv[2]);
    for (p = command; *p != ' ' && *p != '\0'; *p++);
    if (*p == ' ')
        *p++;
    if (*p == '\'' || *p == '"')
        *p++;
    if (*p == '/')
        *p++;
    for (r = p; *p != '\'' && *p != '"' && *p != '\0'; *p++);
    if (*p == '\'' || *p == '"')
        *p = '\0';
    if (*--p == '/')
        *p = '\0';

    if (BUFFER_SIZE < (strlen(r) + 7)) {
        fprintf(stderr, "error: repository name is too big\n");
        free(command);
        return 1;
    }

    snprintf(buffer, BUFFER_SIZE, "repos/%s", r);
    free(command);

    // check if repository is sane
    if (0 == strlen(buffer)) {
        fprintf(stderr, "error: invalid repository\n");
        return 1;
    }

    if (0 != chdir(home)) {
        fprintf(stderr, "error: failed to chdir (%s): %s\n", home,
            strerror(errno));
        return 1;
    }

    bool exists = (0 == access(buffer, F_OK));

    if (!exists)
        mkdir_recursive(buffer);

    if (0 != chdir(buffer)) {
        fprintf(stderr, "error: failed to chdir (%s/%s): %s\n", home, buffer,
            strerror(errno));
        return 1;
    }

    if (!exists) {
        if (0 != system(GIT_BINARY " init --bare > /dev/null")) {
            fprintf(stderr, "error: failed to create git repository: %s\n",
                buffer);
            return 1;
        }
    }

    if (0 != chdir("hooks")) {
        fprintf(stderr, "error: failed to chdir (%s/%s/hooks): %s\n", home,
            buffer, strerror(errno));
        return 1;
    }

    if (0 == access("pre-receive", F_OK)) {
        if (0 != unlink("pre-receive")) {
            fprintf(stderr, "error: failed to remove old symlink "
                "(%s/%s/hooks/pre-receive): %s", home, buffer, strerror(errno));
            return 1;
        }
    }

    if (0 != symlink(self, "pre-receive")) {
        fprintf(stderr, "error: failed to create symlink "
            "(%s/%s/hooks/pre-receive): %s", home, buffer, strerror(errno));
        return 1;
    }

    if (0 != chdir(home)) {
        fprintf(stderr, "error: failed to chdir (%s): %s\n", home,
            strerror(errno));
        return 1;
    }

    command = strdup(argv[2]);
    for (p = command; *p != ' ' & *p != '\0'; *p++);
    if (*p == ' ')
        *p = '\0';
    char *repo = strdup(buffer);
    snprintf(buffer, BUFFER_SIZE, "%s '%s'", command, repo);
    free(command);
    free(repo);

    char *args[4];
    args[0] = GIT_SHELL_BINARY;
    args[1] = "-c";
    args[2] = buffer;
    args[3] = NULL;

    execv(GIT_SHELL_BINARY, args);
    return 0;
}


typedef enum {
    START_OLD = 1,
    OLD,
    START_NEW,
    NEW,
    START_REF,
    REF
} input_state_t;


static int
git_hook(int argc, char *argv[])
{
    char c, buffer[BUFFER_SIZE];

    input_state_t state = START_OLD;
    size_t i = 0;
    size_t start = 0;

    char *new = NULL;

    while (EOF != (c = getc(stdin))) {

        buffer[i] = c;

        switch (state) {
            case START_OLD:
                start = i;
                state = OLD;
                break;
            case OLD:
                if (c != ' ')
                    break;
                // no need to store old
                state = START_NEW;
                break;
            case START_NEW:
                start = i;
                state = NEW;
                break;
            case NEW:
                if (c != ' ')
                    break;
                state = START_REF;
                new = strndup(buffer + start, i - start);
                break;
            case START_REF:
                start = i;
                state = REF;
                break;
            case REF:
                if (c != '\n')
                    break;
                state = START_OLD;
                // we just care about a ref (refs/heads/master), everything
                // else is disposable :)
                if (!((i - start == 17) &&
                      (0 == strncmp("refs/heads/master", buffer + start, 17))))
                {
                    free(new);
                    new = NULL;
                }
                break;
        }

        if (++i >= BUFFER_SIZE) {
            fprintf(stderr, "error: pre-receive hook payload is too big.\n");
            return 1;
        }
    }

    if (new == NULL) {
        fprintf(stderr,
            "No reference to master branch found. Nothing to deploy.\n");
        return 0;
    }

    fprintf(stderr, "%s\n", new);
    return 0;
}


int
main(int argc, char *argv[])
{
    if (argc > 0 && (0 == strcmp(basename(argv[0]), "pre-receive")))
        return git_hook(argc, argv);

    if (argc == 3 && (0 == strcmp(argv[1], "-c")))
        return git_shell(argc, argv);

    fprintf(stderr, "error: this is a special shell, go away!\n");
    return 1;
}
