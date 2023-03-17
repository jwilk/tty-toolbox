# Copyright © 2023 Jakub Wilk <jwilk@jwilk.net>
# SPDX-License-Identifier: MIT

progs = noctty

.PHONY: all
all: $(progs)

.PHONY: clean
clean:
	rm -f $(progs)

# vim:ts=4 sts=4 sw=4 noet
