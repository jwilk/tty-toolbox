# Copyright © 2024 Jakub Wilk <jwilk@jwilk.net>
# SPDX-License-Identifier: MIT

noctty()
{
    perl -e 'require "sys/ioctl.ph"; open(FH, "/dev/tty") or die $!; ioctl(FH, &TIOCNOTTY, my $buf) or die $!; exec {$ARGV[0]} @ARGV;' "$@"
}

# vim:ts=4 sts=4 sw=4 et
