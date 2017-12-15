#include "all.h"

Db *db;
char path[10000];
int verbose;

void
usage(void)
{
	fprint(2, "usage: applytime [-r root] db\n");
	exits("usage");
}

void
main(int argc, char **argv)
{
	char *root;
	Avlwalk *w;
	Entry *e;
	Dir *d;

	root = ".";
	ARGBEGIN{
	case 'r':
		root = EARGF(usage());
		break;
	case 'v':
		verbose++;
		break;
	default:
		usage();
	}ARGEND

	if(argc < 1)
		usage();

	db = opendb(argv[0], OREAD);
	w = avlwalk(db->avl);
	while(e = (Entry*)avlprev(w)){
		snprint(path, sizeof path, "%s/%s", root, e->name);
		d = dirstat(path);
		if(d == nil){
			fprint(2, "warning: skipping missing entry %s\n", path);
			continue;
		}
		if(d->mtime != e->d.mtime){
			d->atime = d->mtime = e->d.mtime;
			if(dirwstat(path, d) < 0)
				fprint(2, "warning: cannot set mtime on %s\n", path);
			else if(verbose)
				print("%11lud %s\n", e->d.mtime, e->name);
		}
		free(d);
	}
	exits(nil);
}
