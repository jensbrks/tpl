#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "config.h"

static void die(const char *fmt, ...);

void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':')
		fputc(' ', stderr);
	else
		fputc('\n', stderr);

	exit(1);
}

int
main(int argc, char *argv[])
{
	char *buf, *ptr, *start, *end;
	if (!(buf = calloc(1, BUFSIZ)))
		die("could not calloc() %u bytes\n", BUFSIZ);

	int i;
	int open_delim_len = strlen(open_delim);
	int close_delim_len = strlen(close_delim);

	unsigned long len = 0;
	unsigned long size = BUFSIZ;

	while ((i = fread(buf + len, 1, BUFSIZ, stdin))) {
		len += i;
		if (BUFSIZ + len + 1 > size) {
			size += BUFSIZ;
			if (!(buf = realloc(buf, size)))
				die("could not realloc() %lu bytes\n", size);
		}
	}

	buf[len] = '\0';
	ptr = buf;

	while ((start = strstr(ptr, open_delim))) {
		fwrite(ptr, 1, start - ptr, stdout);
		ptr = start + open_delim_len;

		if ((end = strstr(ptr, close_delim))) {
			int evallen = end - ptr;

			char *evalbuf;
			if (!(evalbuf = calloc(1, evallen)))
				die("could not calloc() %u bytes\n", evallen);

			memmove(evalbuf, ptr, evallen);

			fflush(stdout);
			system(evalbuf);

			free(evalbuf);
			ptr = end + close_delim_len + 1;
		} else
			fwrite(open_delim, 1, open_delim_len, stdout);
	}

	fwrite(ptr, 1, strlen(ptr), stdout);
	free(buf);
	return 0;
}
