#!/bin/bash
DEST=$1
PREFIX=$2
sed -i '/^SUBDIRS=lib/s/test //' ${DEST}/src/Makefile
sed -i '/^$(OBJDIR)\/%.o: %.c/i PEDANTIC = ' ${DEST}/Makefile.base
sed -i "/^PREFIX ?= /c PREFIX = ${PREFIX}" ${DEST}/Makefile.base
