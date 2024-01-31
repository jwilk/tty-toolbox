/* Copyright © 2024 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define PROGRAM_NAME "ctty"

static void xerror(const char *context)
{
    int orig_errno = errno;
    fprintf(stderr, "%s: ", PROGRAM_NAME);
    errno = orig_errno;
    perror(context);
    exit(EXIT_FAILURE);
}

#define DEV_TTY "/dev/tty"

int main(void)
{
    int fd = open(DEV_TTY, O_RDONLY);
    if (fd < 0)
        xerror(DEV_TTY);
    printf(DEV_TTY "\n");
    return EXIT_SUCCESS;
}

/* vim:set ts=4 sts=4 sw=4 et:*/
