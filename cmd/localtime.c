#include <u.h>
#include <libc.h>

int rflag;

void
usage(void)
{
	fprint(2, "usage: localtime [-w] [file...]\n");
	exits("usage");
}

void
prtime(long clock)
{
	Tm *t;

	t = localtime(clock);
	print("%11uld %d\n", clock, rflag ? -t->tzoff : t->tzoff);
}

void
main(int argc, char **argv)
{
	Dir *d;

	ARGBEGIN{
	case 'r':
		rflag++;
		break;
	default:
		usage();
	}ARGEND

	if(argc < 1)
		prtime(time(nil));
	else{
		do{
			d = dirstat(*argv);
			if(d == nil)
				sysfatal("%r");
			prtime(d->mtime);
			free(d);
		}while(++argv, --argc);
	}
	exits(nil);
}
