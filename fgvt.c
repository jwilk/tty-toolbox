/* Copyright © 2023-2024 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <limits.h>

#include <linux/tiocl.h>

#define PROGRAM_NAME "fgvt"

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

int main(void)
{
    char subcode = TIOCL_GETFGCONSOLE;
    int n = ioctl(STDIN_FILENO, TIOCLINUX, &subcode);
    if (n >= 0) {
        printf("/dev/tty%d\n", n + 1);
        close_stdout();
        return EXIT_SUCCESS;
    }
    const char *path = "/sys/class/tty/tty0/active";
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        xerror(path);
    char buf[PATH_MAX];
    ssize_t m = read(fd, buf, sizeof buf);
    if (m < 0)
        xerror(path);
    if (m > 0 && buf[m - 1] == '\n')
        buf[m - 1] = '\0';
    else {
        errno = EINVAL;
        xerror(path);
    }
    close(fd);
    printf("/dev/%s\n", buf);
    close_stdout();
    return EXIT_SUCCESS;
}

/* vim:set ts=4 sts=4 sw=4 et:*/
