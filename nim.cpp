/************************
* Game of Nim - Two-player strategy game
* Each round, a player must select any number of tokens in a collection.
* The player faced with the last token on the gameboard at the start of their round loses.
************************/

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

void select(int num_selected, char selection, vector<Collection*> &gameboard);
void playerChoice(char *selection, int *num_selected, int row, vector<Collection*> &gameboard);
void computerChoice(char *selection, int *num_selected, int row, vector<Collection*> &gameboard);
void buildRows(int y, int x, vector<Collection*> &gameboard);
bool isGameOver(vector<Collection*> &gameboard);
void title(int y, int x);
bool boundsChecker(char selection, vector<Collection*> &gameboard);

int main(){
	bool turn = 0;  // 0 is player 1 (user), 1 is player 2 (computer)
	vector<Collection*> gameboard;		// holds pointers to collections of tokens
	srand(time(NULL));
	
	initscr();	//curses mode
	raw();  //disable line buffering (one character at a time)
	keypad(stdscr, TRUE);  //capture special keystrokes
	noecho();  //inverse of echo()
	
	int row, col;
	getmaxyx(stdscr, row, col);		//get number of rows and columns in the window
	
	//Colors
	if(has_colors() == 0){
		endwin();
		cout << "colors not supported!" << endl;
		exit(1);
	}
	
	start_color();
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);  //labels 1-5
	init_pair(2, COLOR_GREEN, COLOR_WHITE);
	init_pair(3, COLOR_CYAN, COLOR_WHITE);
	init_pair(4, COLOR_BLUE, COLOR_WHITE);
	init_pair(5, COLOR_RED, COLOR_WHITE);
	init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);	//tokens 6-10
	init_pair(7, COLOR_GREEN, COLOR_GREEN);
	init_pair(8, COLOR_CYAN, COLOR_CYAN);	
	init_pair(9, COLOR_BLUE, COLOR_BLUE);
	init_pair(10, COLOR_RED, COLOR_RED);
	
	curs_set(0);	//hide cursor
	wbkgd(stdscr, COLOR_PAIR(1));  //fill background	
	title(row, col);
	
	//generate between 3 - 5 collections of tokens
	int num_collections = rand() % 3 + 3;
	for (int i=0; i<num_collections; i++){
		Collection *c = new Collection;
		gameboard.push_back(c);
		gameboard[i]->label = (char)i + 65;
		gameboard[i]->number_left = rand() % 5 + i + 1;
		gameboard[i]->color = i + 6;
	}
			
	while(isGameOver(gameboard) == 0){
		attron(A_BOLD | COLOR_PAIR(1));
		move(1,col/3);
		printw("G A M E   O F   N I M");
		attroff(A_BOLD);
		char selection;
		int num_selected;
		
		buildRows(5,10,gameboard);
		
		switch(turn){
			case 0:
				playerChoice(&selection, &num_selected, row, gameboard);
				break;
			case 1:
				computerChoice(&selection, &num_selected, row, gameboard);
				break;
		}
		
		select(num_selected, selection, gameboard);
				
		erase();
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

/*
Function handles player's round
*/
void playerChoice(char *selection, int *num_selected, int row, vector<Collection*> &gameboard){
	do{
		mvaddstr(row-1, 0, "Select collection ");
		refresh();
		*selection = getch();
	}while (!boundsChecker(*selection,gameboard));		//input validation
	clrtoeol();		//clear line
	do{
		mvaddstr(row-1, 0, "Enter number to remove from ");
		addch(toupper(*selection));
		refresh();
		*num_selected = getch();
		*num_selected -= 48;
	}while(*num_selected == 0);		//ensure number is positive
	
}

/*
Function handles computer's round
*/
void computerChoice(char *selection, int *num_selected, int row, vector<Collection*> &gameboard){
	int selectionInt;  //collection choice
	do{
		selectionInt = rand() % gameboard.size();
	}while(gameboard[selectionInt]->number_left == 0);
	
	struct Collection *cPtr;
	cPtr = gameboard[selectionInt];
	*num_selected = rand() % cPtr->number_left + 1;
	*selection = cPtr->label;
	
	//output
	mvaddstr(row-1, 0, "Computer selects collection ");
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

/*
Creates gameboard
*/
void buildRows(int y, int x, vector<Collection*> &gameboard){
	for (int i=0; i<gameboard.size();i++){
		move(y,x);
		addch(gameboard[i]->label | COLOR_PAIR(gameboard[i]->color - 5));
		printw(" ");
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

/*
Decrements collection by number of tokens selected
*/
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

/*
Prints game title
*/
void title(int y, int x){
	y = y/3;
	move(y,x/3-4);
	attron(COLOR_PAIR(1));
	addstr("     /|  //   ||      /\\   /\\\\");
	y = y+1;
	move(y,x/3-4);
	attron(COLOR_PAIR(2));
	addstr("    //| //    ||     // \\ /  \\\\");
	y= y + 1;
	move(y,x/3-4);
	attron(COLOR_PAIR(3));
	addstr("   // |//     ||    //   \\    \\\\");
	y = y + 1;
	move(y, x/3-4);
	addstr("  //  //      ||   //          \\\\");
	refresh();
	getch();
	erase();
}

/*
Evaluates whether game is over by checking if all collections are empty
*/
bool isGameOver(vector<Collection*> &gameboard){
	for (int i=0; i<gameboard.size(); i++){
		if(gameboard[i]->number_left > 0){
			return false;
		}
	}
	return true;
}

/*
Validates player input
*/
bool boundsChecker(char selection, vector<Collection*> &gameboard){
	selection = toupper(selection);
	//validate selection is within bounds of gameboard
	if(selection > gameboard[gameboard.size()-1]->label || selection < 'A'){
		return false;
	}
	int idx = (int)selection;
	idx -= 65;
	//validate selection has tokens remaining
	if(gameboard[idx]->number_left > 0){		
		return true;
	}
	else{
		return false;
	}
}


