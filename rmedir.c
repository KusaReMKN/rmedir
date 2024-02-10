#define _BSD_SOURCE	1
#define _DEFAULT_SOURCE	1
#define _XOPEN_SOURCE	700

#include <err.h>
#include <errno.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define verbose_warn(lvl, ...)	(verbose >= (lvl) && (warn(__VA_ARGS__), 0))

static int iflag = 0;
static int verbose = 0;

static int error = 0;

static int rmedir(const char *, const struct stat *, int, struct FTW *);
static void usage(void);
static int yes(void);

int
main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "iv")) != -1)
		switch (ch) {
		case 'i':
			iflag++;
			break;
		case 'v':
			verbose++;
			break;
		case '?':
		default:
			usage();
		}
	argc -= optind;
	argv += optind;

	if (argc < 1)
		usage();

	for (; argc > 0; argc--, argv++)
		if (nftw(*argv, rmedir, 20, FTW_DEPTH | FTW_PHYS) == -1) {
			warn("%s", *argv);
			error++;
		}

	exit(error ? EXIT_FAILURE : EXIT_SUCCESS);
}

static int
rmedir(const char *fpath, const struct stat *sb,
		int typeflag, struct FTW *tfwbuf)
{
	if (~typeflag & FTW_D)
		return 0;

	if (iflag) {
		(void)fprintf(stderr, "remove %s? ", fpath);
		(void)fflush(stderr);
		if (!yes())
			return 0;
	}

	errno = 0;
	if (rmdir(fpath) == -1)
		if (errno == EEXIST || errno == ENOTEMPTY) {
			verbose_warn(2, "%s", fpath);
		} else {
			warn("%s", fpath);
			error++;
		}
	else
		verbose_warn(1, "%s", fpath);

	return 0;
}

static void
usage(void)
{
	(void)fprintf(stderr, "usage: rmedir [-iv] directory ...\n");
	exit(EXIT_FAILURE);
}

static int
yes(void)
{
	int y, x;

	y = x = getchar();
	while (x != '\n' && x != EOF)
		x = getchar();

	return y == 'y';
}
