#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#ifdef __WIN32
#define clearScreen system("CLS")
#else
#define clearScreen system("clear")
#endif

using namespace std;

// Cell structure to represent each cell on the board
struct Cell 
{
	bool isMine;
	bool isRevealed;
	int adjacentMines;
	bool isFlag;
};

class Minesweeper 
{
private:
	int rows;
	int columns;
	int totalMines;
	vector<vector<Cell>> board;

public:
	Minesweeper(int rows, int columns, int mines) 
	{
        this->rows = rows;
    	this->columns = columns;
    	totalMines = mines;
        board.resize(rows, vector<Cell>(columns, {false, false, 0, false}));
	}
	void placeMines() 
    {
        int count = 0;
        while (count < totalMines) 
        {
            int row = rand() % rows;
            int col = rand() % columns;
            if (!board[row][col].isMine) 
            {
                board[row][col].isMine = true;
                count++;
            }
        }
    }

    void calculateAdjacentMines() 
    {
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) 
            {
                if (!board[i][j].isMine) 
                {
                    int count = 0;
                    for (int k = 0; k < 8; k++) 
                    {
                        int ni = i + dx[k];
                        int nj = j + dy[k];
                        if (isValid(ni, nj) && board[ni][nj].isMine) count++;
                    }
                    board[i][j].adjacentMines = count;
                }
            }
        }
    }

    bool isValid(int row, int col)
    {
        return (row >= 0 && row < rows && col >= 0 && col < columns);
    }
    void revealCell(int row, int col) // gonna change this cause i repeated myself, but clean the mess first 
    {
        if (!isValid(row, col) || board[row][col].isRevealed || board[row][col].isFlag) return;

        board[row][col].isRevealed = true;

        if (board[row][col].adjacentMines == 0) 
        {
            int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
            for (int k = 0; k < 8; k++) 
            {
                int ni = row + dx[k];
                int nj = col + dy[k];
                revealCell(ni, nj);
            }
        }
    }

    void gameOver() 
    {
	    clearScreen;
        cout << "Trafiles na mine.\nJuz nawet nie ma co zbierac.\nKoniec gry." << '\n';
        // Display the full board
        revealAllCells();
        displayBoard();
    }

    void revealAllCells() 
    {
        for (int i = 0; i < rows; i++) 
        {
            for (int j = 0; j < columns; j++) 
            {
                board[i][j].isRevealed = true;
            }
        }
    }

    bool isGameWon() 
    {
        int unrevealedCount = 0;
        for (int i = 0; i < rows; i++) 
        {
            for (int j = 0; j < columns; j++) 
            {
                if (!board[i][j].isRevealed && !board[i][j].isMine)
                    unrevealedCount++;
            }
        }
        return unrevealedCount == 0;
    }

    void displayBoard() 
    {
        cout << "  ";
        for (int j = 0; j < columns; j++) 
        {
            cout << j << " ";
        }
        cout << '\n';
        for (int i = 0; i < rows; i++) 
        {
            cout << i << " ";
            for (int j = 0; j < columns; j++) 
            {
                if (!board[i][j].isRevealed && !board[i][j].isFlag)	    cout << "- ";
		        else if (board[i][j].isRevealed && board[i][j].isMine)	cout << "* ";
		        else if (board[i][j].isFlag)				            cout << "! ";
		        else							                        cout << board[i][j].adjacentMines << " ";
            }
            cout << '\n';
        }
        
    }


    void play() 
    {
        placeMines();
        calculateAdjacentMines();

        while (true) 
        {
            clearScreen;    
            displayBoard();
            char flagOrRev;
            string dump;
            int row, col;
            cout << "Wybierz opcje: (O)dsłoń, (F)laguj\n";
            cin >> flagOrRev;
            if (flagOrRev!='O' && flagOrRev!='o' && flagOrRev!='F' && flagOrRev!='f') flagOrRev=0;
            cout << "Podaj wspolrzedne pola (rzad <spacja> kolumna) : ";
            cin >> row >> col;
            if (!isValid(row, col) || flagOrRev==0) 
            {
                cout << "Cos nie wyszlo (bledne dane?). Sprobuj ponownie" << '\n';
                cin >> dump;
                continue;
            }
            if ((flagOrRev == 'o' || flagOrRev == 'O') && !board[row][col].isFlag)
                revealCell(row, col);
            else
                board[row][col].isFlag = !board[row][col].isFlag;
            if (board[row][col].isMine && board[row][col].isRevealed) 
            {
                gameOver();
                break;
            }
            //if((flagOrRev=='o' || flagOrRev=='O') || !board[row][col].isFlag) 	revealCell(row, col);
            //else if(flagOrRev=='f' || flagOrRev=='F')                           board[row][col].isFlag=true;
            //if (board[row][col].isMine)
            //{
            //    gameOver();
            //    break;
            //}
            if (isGameWon()) 
            {
                cout << "Przezyles! Gratulacje przesyla Badyl." << '\n';
                break;
            }
        }
    }
};

int main() 
{
    srand(static_cast<unsigned>(time(0)));

    int rows, columns, mines;
    cout << "Witaj w Saperze!\nPodaj liczbe rzedow, kolumn i min (oddzielone spacjami):\n";
    cin >> rows >> columns >> mines;

    Minesweeper game(rows, columns, mines);
    game.play();

    return 0;
}
