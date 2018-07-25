#!/bin/sh

touch NEWS README AUTHORS ChangeLog

aclocal
autoheader
autoconf
automake --add-missing
