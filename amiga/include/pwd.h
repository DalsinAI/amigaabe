/* Abe's Amazing Adventure for AmigaOS: libnix has no pwd.h, and
 * src/Directories.h includes it (its getpwuid() is not called here: the
 * game's own files go in the drawer's user/). From the Team's Mirror Magic
 * and IceBreaker ports. The AmiCradle Team. GPL 2 or later, like Abe. */
#ifndef AMIGA_PWD_H
#define AMIGA_PWD_H
#include <sys/types.h>
struct passwd {
	char *pw_name;
	char *pw_passwd;
	uid_t pw_uid;
	gid_t pw_gid;
	char *pw_gecos;
	char *pw_dir;
	char *pw_shell;
};
static inline struct passwd *getpwuid(uid_t uid) { (void)uid; return 0; }
#endif
