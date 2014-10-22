//
//  main.m
//  jvd
//
//  Created by zjc on 14-9-10.
//  Copyright (c) 2014年 apple.com. All rights reserved.
//

//#import "zjcParser.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <curses.h>
#include <vector>
/*
 #include "bitmap_image.hpp"
 */
using namespace std;

const double FRESH_PER_SECONDS = 10;

const char random_set[]=
	{'a','b','c','d','e','f',
    'g','h','i','j','k','l',
    'm','n','o','p','q','r',
    's','t','u','v','w','x',
    'y','z','A','B','C','D',
    'E','F','G','H','I','J',
    'K','L','M','N','O','P',
    'Q','R','S','T','U','V',
    'W','X','Y','Z','#','%',
    '@','&','<','>','?','~'};


const char* generate_random_srting()
{
    srand((unsigned int)clock());
    int str_len = rand()%14+4; //4->18
    
    char *str = new char(str_len+1);
    for (int i=0; i<str_len; i++) {
        *(str+i) = random_set[rand()%60];
    }
    str[str_len] = '\0';
    return str;
}

void sleep(double seconds)
{
    clock_t start=clock();
    while(clock() - start < seconds*CLOCKS_PER_SEC); ///loop until time's up
}

int Matrix[100][200];//max size as i know!!


struct status
{
    enum {kBlank,kString} statusType;
    int length;
};

status Status[200];

int main(int argc, const char * argv[])
{
    const string fileName ="billgates.bmp";
    int MAXRow,MAXCol;
    WINDOW* wnd;
    wnd = initscr();
    getmaxyx(wnd, MAXRow, MAXCol);
    srand(time(NULL));
    
    cbreak();
    noecho();
    clear();
    refresh();

    int sum=-1;
    memset(Matrix,-1,sizeof Matrix);
    for(int i=0;i<MAXCol;i++)
    {
        status s;
		s.statusType = status::kBlank;
        s.length=0;//do not use memset pls
        Status[i]=s;
    }
    for(int i=0;i<6;i++)
    {
        int k = rand()%MAXCol;
        while(sum == (sum^k))
            k=rand()%MAXCol;
        sum^=k;
        status s;
        s.statusType = status::kString;
        s.length = rand()%35+3;
		Status[k] = s;
        int tmp = rand()%10+3;
        for(int row=0;row<tmp;row++)
            Matrix[row][k] = rand()%60;
    }
	
	int _Previous[100][200];
	start_color();
	
	if (use_default_colors() != ERR) {
	    init_pair(COLOR_BLACK, -1, -1);
	    init_pair(COLOR_GREEN, COLOR_GREEN, -1);
	    init_pair(COLOR_WHITE, COLOR_WHITE, -1);
	    init_pair(COLOR_RED, COLOR_RED, -1);
	    init_pair(COLOR_CYAN, COLOR_CYAN, -1);
	    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, -1);
	    init_pair(COLOR_BLUE, COLOR_BLUE, -1);
	    init_pair(COLOR_YELLOW, COLOR_YELLOW, -1);
	}
	
	short mcolor = COLOR_GREEN;
	attron(COLOR_PAIR(mcolor));
	while(1)
	{
        getmaxyx(wnd, MAXRow, MAXCol);
		memcpy(_Previous,Matrix,sizeof Matrix); 
		
		int col_ = rand()%MAXCol;
		if(Matrix[0][col_]==-1) 
		{
			int str_len = rand()%15+10;
			//Matrix[row][col_] = rand()%60;
			Status[col_].statusType = status::kString;
			Status[col_].length = str_len;
			/* code */
		} 
		
		for(int col=0;col<MAXCol;++col)
		{
			if(Status[col].statusType == status::kString && Status[col].length-- >0)
				Matrix[0][col] = rand()%60;
			else
				Matrix[0][col] = -1;
		}
		
	    for (int row=1 ; row<MAXRow; ++row ) {
	        for (int col=0; col<MAXCol; ++col) {
				
				move(row,col);
	            if(Matrix[row][col]==-1)
	                addch(' ');
	            else
	                addch(random_set[Matrix[row][col]]);
				
	              if(_Previous[row][col]!=-1 && _Previous[row-1][col]==-1)
	              {
	                Matrix[row][col]=-1;
	              }
	              if(_Previous[row][col]==-1 && _Previous[row-1][col]!=-1)
	              {
	                Matrix[row][col]=rand()%60;
	              }   
			}
	    }
	   refresh();		
	   memcpy(_Previous,Matrix,sizeof Matrix);
	   sleep(1/FRESH_PER_SECONDS);
	} 

    endwin();
	
	return 0;
}
