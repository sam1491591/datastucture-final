#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/board.h"
#include "../include/player.h"
#include "../include/rules.h"
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/


class Board2 {
private:
    Cell cells[ROW][COL];                       // The 5*6 board whose index (0,0) is start from the upper left corner
    void cell_reaction_marker();                // After the explosion, mark all the cell that  will explode in next iteration
    bool cell_is_full(Cell* cell);              // Check wether the cell is full of orbs and set the explosion variable to be true
    void add_orb(int i, int j, char color);     // Add orb into the cell (i, j)
    void cell_reset(int i, int j);              // Reset the cell to the initial state (color = 'w', orb_num = 0)
    void cell_explode(int i, int j);            // The explosion of cell (i, j). It will add the orb into neighbor cells
    void cell_chain_reaction(Player player);    // If there is aa explosion, check wether the chain reaction is active

public:
    Board2();

    // The basic functions of the Board
    int get_orbs_num(int i, int j);
    int get_capacity(int i, int j);
    char get_cell_color(int i, int j);

    void set_orbs_num(int i, int j, int num);

    void set_cell_color(int i, int j, char color);

    bool place_orb(int i, int j, Player* player);      // Use this function to place a orb into a cell
    void print_current_board(int i, int j, int round); // Print out the current state of the hole board

    bool win_the_game(Player player);                  // The function that is used to check wether the player wins the game after his/her placemnet operation
};

Board2::Board2() {

    ////// Initialize the borad with correct capacity //////
    // The corners of the board
    cells[0][0].set_capacity(3), cells[0][5].set_capacity(3),
        cells[4][0].set_capacity(3), cells[4][5].set_capacity(3);

    // The edges of the board
    cells[0][1].set_capacity(5), cells[0][2].set_capacity(5), cells[0][3].set_capacity(5), cells[0][4].set_capacity(5),
        cells[1][0].set_capacity(5), cells[2][0].set_capacity(5), cells[3][0].set_capacity(5),
        cells[1][5].set_capacity(5), cells[2][5].set_capacity(5), cells[3][5].set_capacity(5),
        cells[4][1].set_capacity(5), cells[4][2].set_capacity(5), cells[4][3].set_capacity(5), cells[4][4].set_capacity(5);

}

void Board2::print_current_board(int i, int j, int round) {

    int orb_num;
    char symbol;

    ////// Print out the current state of the board //////
    //system(CLEAR);
    cout << "Round: " << round << endl;
    cout << "Place orb on (" << i << ", " << j << ")" << endl;
    cout << "=============================================================" << endl;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            symbol = cells[i][j].get_color();
            switch (symbol) {
            case 'r':
                symbol = 'O';
                break;
            case 'b':
                symbol = 'X';
                break;
            default:
                break;
            }

            orb_num = cells[i][j].get_orbs_num();
            switch (orb_num) {
            case 0:
                cout << "|       | ";
                break;
            case 1:
                cout << "|" << symbol << "      | ";
                break;
            case 2:
                cout << "|" << symbol << symbol << "     | ";
                break;
            case 3:
                cout << "|" << symbol << symbol << symbol << "    | ";
                break;
            case 4:
                cout << "|" << symbol << symbol << symbol << symbol << "   | ";
                break;
            case 5:
                cout << "|" << symbol << symbol << symbol << symbol << symbol << "  | ";
                break;
            case 6:
                cout << "|" << symbol << symbol << symbol << symbol << symbol << symbol << " | ";
                break;
            default:
                cout << "|" << symbol << symbol << symbol << symbol << symbol << symbol << symbol << "| ";
                break;

            }
        }
        cout << endl;
    }
    cout << "=============================================================" << endl << endl;
}

bool Board2::place_orb(int i, int j, Player* player) {

    if (!index_range_illegal(i, j) || !placement_illegal(*player, cells[i][j])) {
        int temp = cells[i][j].get_orbs_num();
        temp += 1;
        cells[i][j].set_orbs_num(temp);
        cells[i][j].set_color(player->get_color());
    }
    else {
        player->set_illegal();
        return false;
    }

    if (cell_is_full(&cells[i][j])) {
        cell_explode(i, j);
        cell_reaction_marker();
        cell_chain_reaction(*player);
    }

    return true;
}

bool Board2::cell_is_full(Cell* cell) {
    if (cell->get_orbs_num() >= cell->get_capacity()) {
        cell->set_explode(true);
        return true;
    }
    else return false;
}

void Board2::add_orb(int i, int j, char color) {
    int orb_num = cells[i][j].get_orbs_num();
    orb_num++;
    cells[i][j].set_orbs_num(orb_num);
    cells[i][j].set_color(color);
}

void Board2::cell_reset(int i, int j) {
    cells[i][j].set_orbs_num(0);
    cells[i][j].set_explode(false);
    cells[i][j].set_color('w');
}

void Board2::cell_explode(int i, int j) {

    int orb_num;
    char color = cells[i][j].get_color();

    cell_reset(i, j);

    if (i + 1 < ROW) {
        add_orb(i + 1, j, color);
    }

    if (j + 1 < COL) {
        add_orb(i, j + 1, color);
    }

    if (i - 1 >= 0) {
        add_orb(i - 1, j, color);
    }

    if (j - 1 >= 0) {
        add_orb(i, j - 1, color);
    }
    if (i + 1 < ROW && j - 1 >= 0) {
        add_orb(i + 1, j - 1, color);
    }
    if (i - 1 >= 0 && j - 1 >= 0) {
        add_orb(i - 1, j - 1, color);
    }
    if (i + 1 < ROW && j + 1 < COL) {
        add_orb(i + 1, j + 1, color);
    }
    if (i - 1 >= 0 && j + 1 < COL) {
        add_orb(i - 1, j + 1, color);
    }
}

void Board2::cell_reaction_marker() {

    // Mark the next cell whose number of orbs is equal to the capacity
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            cell_is_full(&cells[i][j]);
        }
    }
}

void Board2::cell_chain_reaction(Player player) {

    bool chain_reac = true;

    while (chain_reac) {

        chain_reac = false;

        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                if (cells[i][j].get_explode()) {
                    cell_explode(i, j);
                    chain_reac = true;
                }
            }
        }

        if (win_the_game(player)) {
            return;
        }

        cell_reaction_marker();
    }
}

bool Board2::win_the_game(Player player) {

    char player_color = player.get_color();
    bool win = true;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (cells[i][j].get_color() == player_color || cells[i][j].get_color() == 'w') continue;
            else {
                win = false;
                break;
            }
        }
        if (!win) break;
    }
    return win;
}

int Board2::get_orbs_num(int i, int j) {
    return cells[i][j].get_orbs_num();
}

int Board2::get_capacity(int i, int j) {
    return cells[i][j].get_capacity();
}

char Board2::get_cell_color(int i, int j) {
    return cells[i][j].get_color();
}

void Board2::set_orbs_num(int i, int j,int num) {
    return cells[i][j].set_orbs_num(num);
}

void Board2::set_cell_color(int i, int j,char color) {
    return cells[i][j].set_color(color);
}

int available[5][6] = { '0' };
int lesscapacity[5][6] = { '0' };

void algorithm_A(Board board, Player player, int index[]){
    
    //////your algorithm design///////////
    //cout << player.get_color() << endl;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            //cout << board.get_cell_color(i, j);
            if (board.get_cell_color(i, j) == player.get_color() || board.get_cell_color(i, j) == 'w')
            {

                available[i][j] = 1;
            }
            else
                available[i][j] = 0;
            //cout <<available[i][j]<<" "<< i << " " << j << endl;
        }
        //cout << endl;
    }
    /*
    int near = 8;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (available[i][j] == 1)
            {
                lesscapacity[i][j] = board.get_capacity(i, j) - board.get_orbs_num(i, j);
                if (lesscapacity[i][j] < near)
                {
                    near = lesscapacity[i][j];
                    index[0] = i;
                    index[1] = j;
                }
            }
            //else
            //    cout << i << " " << j << endl;
        }
    }
    */
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (available[i][j] == 1)
            {
                Board2 board2;
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 6; y++)
                    {
                        board2.set_cell_color(x, y, board.get_cell_color(x, y));
                        board2.set_orbs_num(x, y, board.get_orbs_num(x, y));
                        //cout << board2.get_cell_color(x,y);
                    }
                    //cout << endl;
                }
                board2.place_orb(i, j, &player);
                //board2.print_current_board(i, j, 1);
                int chessnum = 0;
                int max = 0;
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 6; y++)
                    {
                        if (player.get_color() == board2.get_cell_color(x, y))
                        {
                            chessnum += board2.get_orbs_num(x,y);
                        }
                    }
                    //cout << endl;
                }
                if (chessnum > max)
                {
                    max = chessnum;
                    index[0] = i;
                    index[1] = j;
                }
                        //cout << chessnum << endl;
                /*
                lesscapacity[i][j] = board2.get_capacity(i, j) - board2.get_orbs_num(i, j);
                if (lesscapacity[i][j] < near)
                {
                    near = lesscapacity[i][j];
                    index[0] = i;
                    index[1] = j;
                }
                */
            }
            //else
            //    cout << i << " " << j << endl;
        }
    }

}