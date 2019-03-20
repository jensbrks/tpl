# tpl version
VERSION = 0.2

# paths
PREFIX = /usr/local

# flags
CFLAGS  = -std=c99 -pedantic -Wall -Os -DVERSION=\"${VERSION}\"
LDFLAGS = -static

# compiler and linker
CC = cc
