/* File names for Abe's Amazing Adventure on AmigaOS (-Wl,--wrap=fopen,
 * open,opendir,stat,mkdir,chdir), from the Team's IceBreaker, Berusky and
 * Heroes ports. Abe joins its directory, PROGDIR:, and a name with "/", so
 * a name comes out as PROGDIR:/maps/...: to AmigaDOS ":/" (and "//") is the
 * parent drawer. Here ":/" becomes ":" and "//" "/" before the name goes to
 * libnix; Abe never means the parent.
 * The AmiCradle Team. GPL 2 or later, like Abe. */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

FILE *__real_fopen(const char *name, const char *mode);
int __real_open(const char *name, int flags, ...);
DIR *__real_opendir(const char *name);
int __real_stat(const char *name, struct stat *st);
int __real_mkdir(const char *name, mode_t mode);

/* name with the "//" folded; buf is used when it has to change */
static const char *tidy(const char *name, char *buf, size_t size)
{
	size_t i, n = 0;

	if (!name || !strstr(name, "//") && !strstr(name, ":/"))
		return name;
	for (i = 0; name[i] && n + 1 < size; i++) {
		if (name[i] == '/' && n > 0 && (buf[n - 1] == '/' || buf[n - 1] == ':'))
			continue;
		buf[n++] = name[i];
	}
	buf[n] = 0;
	return buf;
}

FILE *__wrap_fopen(const char *name, const char *mode)
{
	char buf[1024];
	return __real_fopen(tidy(name, buf, sizeof(buf)), mode);
}

int __wrap_open(const char *name, int flags, ...)
{
	char buf[1024];
	va_list ap;
	int mode;

	va_start(ap, flags);
	mode = va_arg(ap, int);
	va_end(ap);
	return __real_open(tidy(name, buf, sizeof(buf)), flags, mode);
}

DIR *__wrap_opendir(const char *name)
{
	char buf[1024];
	return __real_opendir(tidy(name, buf, sizeof(buf)));
}

int __wrap_stat(const char *name, struct stat *st)
{
	char buf[1024];
	return __real_stat(tidy(name, buf, sizeof(buf)), st);
}

int __wrap_mkdir(const char *name, mode_t mode)
{
	char buf[1024];
	return __real_mkdir(tidy(name, buf, sizeof(buf)), mode);
}

int __real_chdir(const char *name);

int __wrap_chdir(const char *name)
{
	char buf[1024];
	return __real_chdir(tidy(name, buf, sizeof(buf)));
}
