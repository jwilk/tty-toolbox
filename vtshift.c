/* Copyright © 2023-2024 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/keyboard.h>
#include <linux/tiocl.h>

#define PROGRAM_NAME "vtshift"

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

#define KG(x) { .value = KG_ ## x, .name = "KG_" # x }
static struct {
    int value;
    const char *name;
} keys[] = {
    KG(SHIFT),
    KG(CTRL),
    KG(ALT),
    KG(ALTGR),
};
#undef KG

#define ARRAY_SIZE(a) (sizeof (a) / sizeof (a)[0])

int main(void)
{
    char subcode = TIOCL_GETSHIFTSTATE;
    int rc = ioctl(STDIN_FILENO, TIOCLINUX, &subcode);
    if (rc < 0)
        xerror("ioctl");
    int n = subcode;
    for (size_t i = 0; i < ARRAY_SIZE(keys); i++)
        if (n & (1 << keys[i].value))
            printf("%s\n", keys[i].name);
    close_stdout();
    return EXIT_SUCCESS;
}

/* vim:set ts=4 sts=4 sw=4 et:*/
