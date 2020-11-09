# Instructions
Nim is a two-player strategy game.  Each turn, a player may select any number of tokens in one collection.  The player who selects the last token on the gameboard wins.  To play, select easy mode or hard mode from the title screen.  This option programs the difficulty of the computer opponent.  Easy mode offers a good warmup, while hard mode presents a narrow path to victory.

# Building

1. Install the ncurses programming library
    - Debian example: `sudo apt-get install libncurses5-dev libncursesw5-dev`
    - Red Hat example: `sudo yum install ncurses-devel`

2) Compile the source code
    - `g++ -o nim nim.cpp -lncurses`
    
3) Run the application
    - `./nim`
