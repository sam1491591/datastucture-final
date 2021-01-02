#include <iostream>
#include <stdlib.h>
#include <time.h>
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

}