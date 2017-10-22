#!/bin/sh
set -x
mkdir -p config
aclocal -I config
autoheader
libtoolize --automake
automake  --add-missing --copy --foreign
autoconf

