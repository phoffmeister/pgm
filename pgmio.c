#include <stdio.h>
#include <stdlib.h>

#include "pgmio.h"

greymap* creategreymap(int x, int y, int maxval){
	greymap* map;
	map = malloc(sizeof (greymap));
	map->map_grey = malloc(x*y);	
	map->dim.w=x;
	map->dim.h=y;
	map->max=maxval;
		
	return map;
}

void setGrey(greymap* map, grey gr, int x, int y){
	int w=map->dim.w;
	
	(map->map_grey)[x+w*y] = gr;
}

void writeGreymap(char* path, greymap* map){
	FILE* datei;
	int n,m;
	
	datei = fopen(path, "w");
	fprintf(datei, "P5\n%d %d\n255\n",map->dim.w,map->dim.h);
	for(n=0; n<map->dim.h; n++){
		for(m=0; m<map->dim.w; m++){
			fwrite((map->map_grey)+n*map->dim.w+m,1,1,datei);
		}
	}
	fclose(datei);
}

void freeGMap(greymap* map){
	free(map->map_grey);
	free(map);
}

grey getGrey(greymap* map, int x, int y){
	grey col;
	col = *((map->map_grey)+y*(map->dim.w)+x);
	return col;
}

void invertGreymap(greymap* map){
	int x,y,w,h,max;
	grey col;
	
	w = map->dim.w;
	h = map->dim.h;
	max = map->max;
	
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){
			col = getGrey(map,x,y);
			col=max-col;
			setGrey(map,col,x,y);
		}
	}
}

greymap* opengreymap(char* path){
	greymap* map;
	FILE* datei;
	unsigned char input;
	int w,h,maxval,n,m,count;
	
	datei = fopen(path, "r");
	
	input = fgetc(datei);
	if(input!='P'){
		printf("wrongFormat");
		return NULL;
	}
	
	input = fgetc(datei);
	if(input!='5'){
		printf("wrongFormat");
		return NULL;
	}
	
	input = fgetc(datei);
	while(input=='\n' || input ==' ' || input =='\r' || input =='\t'){
		input = fgetc(datei);
	}
	fseek(datei,-1,SEEK_CUR);
	
	fscanf(datei, "%d", &w);
	
	input = fgetc(datei);
	while(input=='\n' || input ==' ' || input =='\r' || input =='\t'){
		input = fgetc(datei);
	}
	fseek(datei,-1,SEEK_CUR);
	
	fscanf(datei, "%d", &h);
	
	input = fgetc(datei);
	while(input=='\n' || input ==' ' || input =='\r' || input =='\t'){
		input = fgetc(datei);
	}
	fseek(datei,-1,SEEK_CUR);
	
	fscanf(datei, "%d", &maxval);
	if(maxval>255){
		printf("Dateiformat nicht unterstützt");
		return NULL;
	}
	input = fgetc(datei);
	
	if(!(input=='\n' || input ==' ' || input =='\r' || input =='\t')){
		printf("wrong format");
		return 0;
	}
	
	map = creategreymap(w,h,maxval);
	
	count=0;
	for(n=0; n<h; n++){
		for(m=0; m<w; m++){
			fread((map->map_grey)+count,1,1,datei);
			count++;
		}
	}
	fclose(datei);
	return map;
}

void mirrorHorGreymap(greymap* map){
	int x,y,w,h;
	grey col;
	w=map->dim.w;
	h=map->dim.h;
	
	for(x=0; x<w; x++){
		for(y=0; y<h/2; y++){
			col= getGrey(map,x,h-y);
			setGrey(map,getGrey(map,x,y) ,x,h-y);
			setGrey(map,col,x,y); 
		}
	}
}

void mirrorVerGreymap(greymap* map){
	int x,y,w,h;
	grey col;
	w=map->dim.w;
	h=map->dim.h;
	
	
	for(y=0; y<h; y++){
		for(x=0; x<w/2; x++){
			col= getGrey(map,w-x,y);
			setGrey(map,getGrey(map,x,y) ,w-x,y);
			setGrey(map,col,x,y); 
		}
	}
}

void rotateGreymap(greymap** map){
/*
 * 90 degree counter clockwise
 */
	int x,y,w,h;
	greymap* newMap;
	greymap* oldMap;
	
	w=(*map)->dim.h;
	h=(*map)->dim.w;
	
	oldMap = *map;
	newMap = creategreymap( w, h, (*map)->max);
	
	
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){
			setGrey(newMap, getGrey(oldMap,y,x),x,y);
		}
	}
	mirrorHorGreymap(newMap);
	*map = newMap;
}

void setMinGrey(greymap* map,unsigned char min){
	int w,h,x,y;
	w=map->dim.w;
	h=map->dim.h;
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){	
			if(map->map_grey[w*y+x] < min){
				map->map_grey[w*y+x]=min;
			}
		}
	}
}

void setMaxGrey(greymap* map, unsigned char max){
	int w,h,x,y;
	w=map->dim.w;
	h=map->dim.h;
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){		
			if(map->map_grey[w*y+x] > max){
				map->map_grey[w*y+x]=max;
			}
		}
	}	
}

