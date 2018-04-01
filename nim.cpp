/****************
* game of nim
*****************/

#include <curses.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

struct Collection{
	char label;
	int number_left;
	int color;
};

//void buildRow(int y, int x, struct Collection *c);
void select(int num_selected, char selection, vector<Collection*> &gameboard);
void playerChoice(char *selection, int *num_selected, int row);
void computerChoice(char *selection, int *num_selected, int row, vector<Collection*> &gameboard);
void buildRows(int y, int x, vector<Collection*> &gameboard);

int main(){
	bool turn = 0;  // 0 is player 1 (user), 1 is player 2 (computer)
	vector<Collection*> gameboard;		// holds pointers to collections of tokens
	srand(time(NULL));
	
	initscr();	//curses mode
	raw();  //disable line buffering (one character at a time)
	//cbreak();  //alternate to raw(); 
	keypad(stdscr, TRUE);  //capture special keystrokes
	noecho();  //inverse of echo()
	
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
	init_pair(2, COLOR_GREEN, COLOR_WHITE);
	init_pair(3, COLOR_CYAN, COLOR_WHITE);
	init_pair(4, COLOR_BLUE, COLOR_WHITE);
	init_pair(5, COLOR_RED, COLOR_WHITE);
	init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);	//box color
	init_pair(7, COLOR_GREEN, COLOR_GREEN);
	init_pair(8, COLOR_CYAN, COLOR_CYAN);	
	init_pair(9, COLOR_BLUE, COLOR_BLUE);
	init_pair(10, COLOR_RED, COLOR_RED);
	
	wbkgd(stdscr, COLOR_PAIR(1));  //fill background	
	move(1,col/2);
	//printw("NIM");
	addch('N' | A_BOLD);
	addch('I' | A_BOLD | COLOR_PAIR(2));
	addch('M' | A_BOLD | COLOR_PAIR(3));

	//generate between 3 - 5 collections of tokens
	int num_collections = rand() % 3 + 3;
	for (int i=0; i<num_collections; i++){
		Collection *c = new Collection;
		gameboard.push_back(c);
		gameboard[i]->label = (char)i + 65;
		gameboard[i]->number_left = rand() % 5 + i + 1;
		//gameboard[i]->number_left = 5;
		gameboard[i]->color = i + 6;
	}
		
	//Window UI
	//WINDOW *win = newwin(1, col, row-1, 0);
	//wbkgd(win, COLOR_PAIR(1));
	
	while(gameboard[0]->number_left + gameboard[1]->number_left + gameboard[2]->number_left > 0){
		char selection;
		int num_selected;
		
		buildRows(5,10,gameboard);
		//buildRow(5,10,gameboard[0]);
		//buildRow(8,10,gameboard[1]);
		//buildRow(11,10,gameboard[2]);
		
		switch(turn){
			case 0:
				playerChoice(&selection, &num_selected, row);
				break;
			case 1:
				computerChoice(&selection, &num_selected, row, gameboard);
				break;
		}
		
		//int num_selected = getch();
		//wprintw(win, "Select collection: ");
		//wrefresh(win);
		
		select(num_selected, selection, gameboard);
				
		erase();
		//werase(win);
		turn = !turn;		//change turn;
	}
	
	if(turn == 0){
		mvaddstr(row-1, 0, "YOU WIN!");
	}
	else{
		mvaddstr(row-1, 0, "COMPUTER WINS!");
	}
	
	getch();
	
	//free dynamically allocated memory
	for(int i=0; i<gameboard.size(); i++){
		delete gameboard[i];
		gameboard[i] = NULL;
	}
	
	
	endwin();		// free memory and restore terminal settings
	return 0;
}

void playerChoice(char *selection, int *num_selected, int row){
	mvaddstr(row-1, 0, "Select collection: ");
	refresh();
	*selection = getch();
	clrtoeol();		//clear line
	mvaddstr(row-1, 0, "Number to remove from ");
	addch(toupper(*selection));
	refresh();
	*num_selected = getch();
	*num_selected -= 48;
}

void computerChoice(char *selection, int *num_selected, int row, vector<Collection*> &gameboard){
	int selectionInt;  //collection choice
	do{
		selectionInt = rand() % gameboard.size();
		//addch(selectionInt + 65);
		//refresh();
	}while(gameboard[selectionInt]->number_left == 0);
	
	struct Collection *cPtr;
	cPtr = gameboard[selectionInt];
	*num_selected = rand() % cPtr->number_left + 1;
	*selection = cPtr->label;
	//*selection = selectionInt += 65;
	
	//output
	mvaddstr(row-1, 0, "Computer selects collection ");
	//addch(*selection);
	printw("%c",toupper(*selection));
	refresh();
	move(row-1,0);
	usleep(1500000);
	clrtoeol();		//clear line
	mvaddstr(row-1, 0, "Computer chooses to remove ");
	printw("%d",*num_selected);
	refresh();
	usleep(1500000);
}


void buildRows(int y, int x, vector<Collection*> &gameboard){
	for (int i=0; i<gameboard.size();i++){
		move(y,x);
		addch(gameboard[i]->label | COLOR_PAIR(gameboard[i]->color - 5));
		printw(" ");
		//printw("num left: %d", gameboard[i]->number_left);		//debugging
		for(int j=0; j<gameboard[i]->number_left; j++){
			attron(COLOR_PAIR(gameboard[i]->color));
			printw("  ");
			attroff(COLOR_PAIR(gameboard[i]->color));
			addch(' ');
		}
		move(y+1,x+2);
		for(int j=0; j<gameboard[i]->number_left; j++){
			attron(COLOR_PAIR(gameboard[i]->color));
			printw("  ");
			attroff(COLOR_PAIR(gameboard[i]->color));
			addch(' ');
		}
		y+=3;
	}
	refresh();
}	

void select(int num_selected, char selection, vector<Collection*> &gameboard){
	selection = toupper(selection);
	int idx = (int)selection;
	idx -= 65;
	for (int i=0; i<num_selected; i++){
		if(gameboard[idx]->number_left == 0){
			break;
		}
		else{
			gameboard[idx]->number_left--;
		}
	}
}

