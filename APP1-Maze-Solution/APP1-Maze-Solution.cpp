// APP1-Maze-Solution.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <bitset>
#include <set>
using namespace std;

#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"
#define HasRightBorder 0
#define HasBottomBorder 1



typedef vector<vector<bitset<2>>> Matrix;
enum Direction
{
	None,
	Left,
	Right,
	Up,
	Down 
};

void GenerateRandomMatrixTest(Matrix& matrix)
{
	for (int i = 0; i < 50; i++)
	{
		int a = rand() % (matrix.size() - 1) + 1;
		int b = rand() % (matrix[0].size() - 1) + 1;
		matrix[a][b].set(HasRightBorder);
		a = rand() % (matrix.size() - 1) + 1;
		b = rand() % (matrix[0].size() - 1) + 1;
		matrix[a][b].set(HasBottomBorder);
	}
}

void generateMatrixRec(Matrix& m, int i1, int j1, int i2, int j2)
{
	int iDiff = i2 - i1;
	int jDiff = j2 - j1;
	if (iDiff <= 0 && jDiff <= 0) return;
	if (iDiff > jDiff) {
		//int iSplit = (i1+i2) / 2;
		int iSplit = i1 + rand() % (i2 - i1);
		int doorPos = j1 + rand() % (j2 - j1 + 1);
		for (int j = j1; j <= j2; j++)
			if (j != doorPos)
			m[iSplit][j].set(HasBottomBorder);
		generateMatrixRec(m, i1, j1, iSplit, j2);
		generateMatrixRec(m, iSplit+1, j1, i2, j2);
	}
	else {
		//int jSplit = (j1+j2) / 2;
		int jSplit = j1 + rand() % (j2 - j1);
		int doorPos = i1 + rand() % (i2 - i1 + 1);
		for (int i = i1; i <= i2; i++)
			if(i!=doorPos)
				m[i][jSplit].set(HasRightBorder);
		generateMatrixRec(m, i1, j1, i2, jSplit);
		generateMatrixRec(m, i1, jSplit + 1, i2, j2);
	}
}
//fix base cases
Matrix generateMatrix(int rows, int cols)
{
	vector<vector<bitset<2>>> matrix(rows + 1, vector<bitset<2>>(cols + 1, 0));
	for (int i = 1; i <= cols; i++) {
		matrix[0][i].set(HasBottomBorder);
		matrix[rows][i].set(HasBottomBorder);
	}
	for (int i = 1; i <= rows; i++) {
		matrix[i][0].set(HasRightBorder);
		matrix[i][cols].set(HasRightBorder);
	}
	//GenerateRandomMatrixTest(matrix);
	generateMatrixRec(matrix, 1,1, rows, cols);
	return matrix;
}

bool cellInMatrix(Matrix& m, int i, int j)
{
	return(i >= 1 && i < m.size()) &&  m.size()>0 && (j >= 0 && j < m[0].size());
}

bool canMove(Matrix& m,Direction direction, int i, int j)
{
	switch (direction)
	{
	case Left:
		return canMove(m, Right,i, j - 1);
		break;
	case Right:
		return cellInMatrix(m, i, j) && cellInMatrix(m, i, j + 1) && !m[i][j].test(HasRightBorder);
		break;
	case Up:
		return canMove(m,Down, i - 1, j);
		break;
	case Down:
		return cellInMatrix(m, i, j) && cellInMatrix(m, i + 1, j) && !m[i][j].test(HasBottomBorder);
		break;
	default:
		return cellInMatrix(m, i, j);
		break;
	}
	
}


bool getShortestPathHelper(Matrix& m, int i1, int j1, int i2, int j2, Direction from,set<pair<int,int>> &res)
{
	if (i1 == i2 && j1 == j2)
	{
		//cout << i1 << ' ' << j1 << endl;
		res.insert(make_pair(i1, j1));
		return true;
	}
	bool found = false;
	if (from != Down && canMove(m, Down, i1, j1) && getShortestPathHelper(m, i1 + 1, j1, i2, j2, Up, res))
		found = true;
	if (!found && from != Right && canMove(m, Right, i1, j1) && getShortestPathHelper(m, i1, j1 + 1, i2, j2, Left, res))
		found = true;
	if (!found && from != Up && canMove(m, Up, i1, j1) && getShortestPathHelper(m, i1 - 1, j1, i2, j2, Down, res))
		found = true;
	if (!found && from != Left && canMove(m, Left, i1, j1) && getShortestPathHelper(m, i1, j1 - 1, i2, j2, Right, res))
		found = true;
	if (found) {
		res.insert(make_pair(i1, j1));
		//cout << i1 << ' ' << j1 << endl;
	}
		
	return found;
}


set<pair<int, int>> getShortestPath(Matrix& matrix)
{
	set<pair<int, int>> res;
	if (matrix.size() == 0)
		return res;
	getShortestPathHelper(matrix,matrix.size() - 1, matrix[0].size() - 1,1,1,None, res);
	return res;
}

void printMatrix(Matrix m, set<pair<int, int>>& res)
{
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m[i].size(); j++)
		{
			bool isPartOfTheShortestPath = res.count(make_pair(i, j));

			if (m[i][j].test(HasBottomBorder))
			{
				if (isPartOfTheShortestPath)
					cout << UNDERLINE << "o" << CLOSEUNDERLINE;
				else cout << "_";
			}
			else cout << (isPartOfTheShortestPath ? "o" : " ");

			cout << (m[i][j].test(HasRightBorder) ? "|" : " ");
		}
		cout << endl;
	}
}

int main()
{
	srand(time(0));
	
	//for(int i=1;i<10;i++)
		//printMatrix(generateMatrix(i,i));

	auto matrix = generateMatrix(9, 40);

	set<pair<int, int>> empty;
	printMatrix(matrix, empty);

	auto s = getShortestPath(matrix);
	printMatrix(matrix, s);
	
}
