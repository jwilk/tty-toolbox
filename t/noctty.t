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
xc=0
ptydo './noctty ./ctty > "$tmpdir/out" 2>"$tmpdir/err"' || xc=$?
if [ $xc -eq 1 ]
then
    echo ok 1
else
    echo not ok 1
fi
if ! [ -s "$tmpdir/out" ]
then
    echo ok 2
else
    sed -e 's/^/# /' "$tmpdir/out"
    echo not ok 2
fi
err=$(cat "$tmpdir/err")
sed -e 's/^/# /' "$tmpdir/err"
case $err in
    'ctty: /dev/tty: '*)
        echo ok 3;;
    *)
        echo not ok 3;;
esac
rm -rf "$tmpdir"

# vim:ts=4 sts=4 sw=4 et ft=sh
