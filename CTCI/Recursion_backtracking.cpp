#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>

using namespace std;

//Generate Parenthesis 
void generateParenthesisHelper(int left, int right, string out, vector<string> &res)
{
	if(left > right)
	return;
	if(left == 0 && right == 0)
	{
		res.push_back(out);
		return;
	}
	if(left > 0)
		generateParenthesisHelper(left-1, right, out+'(', res);
	if(right > 0)
		generateParenthesisHelper(left, right-1, out+')', res);
}

vector<string> generateParenthesis(int n)
{
	string out;
	vector<string> res;
	generateParenthesisHelper(n, n, out, res);
	return res;
}

/*********************************************************************************************************/

//Paint Fill Problem









/**********************************************************************************************************/

//Sudoku Solver
bool solveSudokuHelper(vector<vector<char>> &grid, int i, int j)
{
	if(i == 9)
		return true;
	if(j == 9)
		return solveSudokuHelper(grid, i+1 , j);
	if(grid[i][j] != '.')
		return solveSudokuHelper(grid, i , j+1);
	
	for(char c = '1' ; c <= '9'; c++)
	{
		if(isValid(board, i , j, c))
		{
			grid[i][j] = c;
			if(solveSudokuHelper(grid, i , j+1))
				return true;
			grid[i][j] = '.';
		}

	}
	return false;
}

bool solveSudoku(vector<vector<char>> &grid)
{
	return solveSudokuHelper(grid, 0, 0); 
}

bool isValid(vector<vector<char>> &grid, int i , int j, char c)
{
	int row = i - i%3;
	int col = j - j%3;
	
	for(int x = 0 ; x < 9 ; x++)
		if(grid[x][j] == c)
			return false;

	for(int y = 0; y < 9 ; y++)
		if(grid[i][y] == c)
			return false;

	for(int x = 0 ; x < 9 ; x++)
	{
		for(int y = 0 ; y < 9; y++)
		if(grid[x+row][y+col] == c)
			return false;
	}
	return true;
}



/**********************************************************************************************************/

//Letter combinations of phone number






/*********************************************************************************************************/

//N queens problem (Queens dont attack each other . They should not share same row , column , diagonal)

bool solveNQ(vector<vector<int>> &chess , int N )
{
	return solveNQUtil(chess, 0, N);
}

bool solveNQUtil(vector<vector<int>> &chess, int col, int N)
{
	if(col >= N)
		return true;
	for(int i = 0; i < N; i++)
	{
		if(isSafe(chess, i, col, N))
		{
			chess[i][col] = 1;
			if(solveNQUtil(chess, col+1, N))
			return true;
			chess[i][col] = 0;
		}
	}
	return false;
}

bool isSafe(vector<vector<int>> &chess , int row , int col, int N)
{
	int x, y;
	for(x = 0 ; x < col; x++)
		if(chess[row][x])
		return false;


	for(x = row , y = col ; x >= 0 && y >= 0; x--, y--)
		if(chess[x][y])
			return false;

	for(x =  row, y = col ; x < N && y >= 0 ; x++, y--)
		if(chess[x][y])
			return false;

	return true;
}

/************************************************************************************************************/

int main()
{
	vector<string> parenthesis = generateParenthesis(3);
	return 0;
}

