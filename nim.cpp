/****************
* game of nim
*****************/

#include <curses.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void buildRow(int y, int x, struct Collection *c);

struct Collection{
	char label;
	int number_left;
	int color;
};

int main(){
	initscr();	//curses mode
	raw();  //disable line buffering (one character at a time)
	//cbreak();  //alternate to raw(); 
	keypad(stdscr, TRUE);  //capture special keystrokes
	//noecho();  //inverse of echo()
	
	int row, col;
	getmaxyx(stdscr, row, col);		//get number of rows and columns in the window
	
	//getyx(stdscr, row, rol);  //gets coordinates of cursor
	//move(y, x+5) //moves the cursor
	
	//Colors
	if(has_colors() == 0){
		endwin();
		cout << "colors not supported!" << endl;
		exit(1);
	}
	
	start_color();
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);  //standard background
	init_pair(2, COLOR_GREEN, COLOR_WHITE);		//standard background #2
	init_pair(3, COLOR_CYAN, COLOR_WHITE);		//standard background #3
	init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);	//box color
	init_pair(5, COLOR_GREEN, COLOR_GREEN);				//box color
	init_pair(6, COLOR_CYAN, COLOR_CYAN);					// box color
	wbkgd(stdscr, COLOR_PAIR(1));  //fill background	
	move(1,col/2);
	//printw("NIM");
	addch('N' | A_BOLD);
	addch('I' | A_BOLD | COLOR_PAIR(2));
	addch('M' | A_BOLD | COLOR_PAIR(3));

	struct Collection A;
	A.label = 'A';
	A.number_left = 3;
	A.color = 4;
	struct Collection B;
	B.label = 'B';
	B.number_left = 4;
	B.color = 5;
	struct Collection C;
	C.label = 'C';
	C.number_left = 5;
	C.color = 6;
	
	buildRow(5,10,&A);
	buildRow(7,10,&B);
	buildRow(9,10,&C);
	
	//mvaddstr(row-1, 0, "Select collection: ");
	//refresh();
	
	//Window UI
	//WINDOW *win = newwin(1, col, row-1, 0);
	WINDOW *win = newwin(1, col, row-1, 0);
	//box(win, 0,0);   //prints too many q's
	wbkgd(win, COLOR_PAIR(1));
	wprintw(win, "Select collection: ");
	wrefresh(win);
	getch();
	
	//char selection = getch();
	//cout << selection << endl;
	
	refresh();	
	
	getch();
	endwin();		// free memory and restore terminal settings
	return 0;
}

void buildRow(int y, int x, struct Collection *c){
	move(y,x);
	addch(c->label | COLOR_PAIR(c->color - 3));
	printw(" ");
	for(int i=0; i<c->number_left; i++){
		attron(COLOR_PAIR(c->color));
		printw("  ");
		attroff(COLOR_PAIR(c->color));
		addch(' ');
	}
	refresh();
}