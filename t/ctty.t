#!/bin/sh

# Copyright © 2024 Jakub Wilk <jwilk@jwilk.net>
# SPDX-License-Identifier: MIT

set -e -u

if script --version | grep -q -w util-linux
then
    ptydo()
    {
        script -e -q -c "$*" /dev/null > /dev/null
    }
else
    ptydo()
    {
        script -q /dev/null sh -c "$*" > /dev/null
    }
fi

echo 1..3
cd "${0%/*}/.."
tmpdir=$(mktemp -dt tty-toolbox.XXXXXX)
export tmpdir
if ptydo './ctty > "$tmpdir/out" 2>"$tmpdir/err"'
then
    echo ok 1
else
    echo not ok 1
fi
out=$(cat "$tmpdir/out")
sed -e 's/^/# /' "$tmpdir/out"
if [ "$out" = /dev/tty ]
then
    echo ok 2
else
    echo not ok 2
fi
if ! [ -s "$tmpdir/err" ]
then
    echo ok 3
else
    sed -e 's/^/# /' "$tmpdir/err"
    echo not ok 3
fi
rm -rf "$tmpdir"

# vim:ts=4 sts=4 sw=4 et ft=sh
