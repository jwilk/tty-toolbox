/* Copyright © 2023 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define PROGRAM_NAME "noctty"

static void show_usage(FILE *fp)
{
    fprintf(fp, "Usage: %s COMMAND [ARG...]\n", PROGRAM_NAME);
}

static void xerror(const char *context)
{
    int orig_errno = errno;
    fprintf(stderr, "%s: ", PROGRAM_NAME);
    errno = orig_errno;
    perror(context);
    exit(EXIT_FAILURE);
}

static void close_stdout(void)
{
    int rc;
    if (ferror(stdout)) {
        fclose(stdout);
        rc = EOF;
        errno = EIO;
    } else
        rc = fclose(stdout);
    if (rc == EOF)
        xerror("stdout");
}


#define DEV_TTY "/dev/tty"

int main(int argc, char **argv)
{
    if (argc <= 1) {
        show_usage(stderr);
        exit(EXIT_FAILURE);
    }
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        show_usage(stdout);
        close_stdout();
        exit(EXIT_SUCCESS);
    }
    int fd = open(DEV_TTY, O_RDONLY);
    if (fd < 0 && errno != ENXIO)
        xerror(DEV_TTY);
    if (fd >= 0) {
        int rc = ioctl(fd, TIOCNOTTY);
        if (rc < 0)
            xerror("ioctl(..., TIOCNOTTY)");
        rc = close(fd);
        if (rc < 0)
            xerror("close()");
    }
    execvp(argv[1], argv + 1);
    xerror(argv[1]);
}

/* vim:set ts=4 sts=4 sw=4 et:*/
