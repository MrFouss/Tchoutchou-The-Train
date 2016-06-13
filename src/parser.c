#include "parser.h"

Parser initParser(const char* file) {
	Parser p;

	strcpy(p.file, file);
	p.currPosition = 0;
	p.remEventNbr = 0;

	FILE* f = fopen(file, "r");

	if (f!=NULL) {
		fscanf(f, "%d", &(p.remEventNbr));
		p.currPosition = ftell(f);
		fclose(f);
	} else {
		fprintf(stderr, "Error while reading file.\n");
        fprintf(stderr, "\tError : %s\n", strerror(errno));
        exit(-1);
	}

	return p;
}

TrainEvent nextEvent(Parser* p) {
	FILE* f = fopen(p->file, "r");
	TrainEvent te;
	char type[4], dir[3];

	fseek(f, p->currPosition, SEEK_SET);

	fscanf(f, "%d %s %s", &(te.startTime), type, dir);

	if (strcmp(type, "M") == 0) {
		te.type = TYPE_M;
	} else if (strcmp(type, "TGV") == 0) {
		te.type = TYPE_TGV;
	} else {
		te.type = TYPE_GL;
	}
	
	if (strcmp(dir, "WE") == 0) {
		te.direction = DIR_WE;
	} else {
		te.direction = DIR_EW;
	}

	p->remEventNbr--;
	p->currPosition = ftell(f);

	fclose(f);

    return te;
}