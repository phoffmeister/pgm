typedef unsigned char grey;

typedef struct{
	int w;
	int h;
} dimension;

typedef struct {
	grey* map_grey;
	dimension dim;
	unsigned char max;
} greymap;

greymap* creategreymap(int, int, int);
greymap* opengreymap(char*);

grey getGrey(greymap*, int, int);

void setGrey(greymap* , grey, int, int);
void writeGreymap(char* , greymap*);
void freeMap(greymap*);
void invertGreymap(greymap*);
void mirrorHorGreymap(greymap*);
void mirrorVerGreymap(greymap*);
void setMaxGrey(greymap*,unsigned char);
void setMinGrey(greymap*,unsigned char);
void rotateGreymap(greymap**);

