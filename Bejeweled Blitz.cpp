#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<cstdlib>
#include<fstream>
#define CubeGem 100
using namespace std;void SaveGame(time_t, time_t);void LoadGame(time_t&, time_t&);void DrawShape(int Shape, int x, int y);;void Fill();void SimpleFill();void gotoxy(int x, int y);void ApplyGravity();bool BlastGems();void DestroyRowAndColumn(int row, int column, int arr[8][8]);int mod(int num);int modX(int num);void BurstSpecificGems(int Gem);void PhysicsOfGame(int r1, int c1, int r2, int c2);void myLine(int x1, int y1, int x2, int y2, int color);void myTri(int x1, int y1, int x2, int y2, int R, int G, int B, int FR, int FG, int FB);void myEllipse(int x1, int y1, int x2, int y2, int R, int G, int B, int FR, int FG, int FB);void myRect(int x1, int y1, int x2, int y2, int R, int G, int B, int FR, int FG, int FB);void mySquare(int x1, int y1, int x2, int y2,bool);bool isCursorKeyPressed(int& whickKey);void PrintCoolBoard();void DrawShape(int Shape, int x, int y);

int arr[8][8] = { 0 }, score = 0, collisions = 0; bool firstRun = true;


int main()
{
	bool EnterPressed = 0, CanSwap = 0, CanSave = 0, CanLoad = 0;
	int r = 0, c = 0, ri = 0, ci = 0, key = 0, GameTime = 60, i;

	SimpleFill(); // Fills random nums in array
	arr[0][0] = 1;
	arr[0][1] = 1;
	arr[0][2] = -1;	
	arr[0][3] = CubeGem;


	time_t start, end, S, E; start = time(NULL); // Stop watch start

	for (i = 0;key != 9;i++)
	{
		ci = c; ri = r; // making equal the selected and the swap row and column
		PrintCoolBoard();
		key = 0; isCursorKeyPressed(key); // key = 0 is very important
		if (key == 5) { EnterPressed = 1; cout << "."; }
		if (key == 1 && c > 0) { mySquare(45 + c * 40, 45 + r * 40, 75 + c * 40, 75 + r * 40, false);c--; ci--;if (EnterPressed == 1) { c++;EnterPressed = 0; CanSwap = 1;} }
		if (key == 2 && r > 0) { mySquare(45 + c * 40, 45 + r * 40, 75 + c * 40, 75 + r * 40, false);r--; ri--;if (EnterPressed == 1) { r++;EnterPressed = 0; CanSwap = 1;} }
		if (key == 3 && c < 7) { mySquare(45 + c * 40, 45 + r * 40, 75 + c * 40, 75 + r * 40, false);c++; ci++;if (EnterPressed == 1) { c--;EnterPressed = 0; CanSwap = 1;} }
		if (key == 4 && r < 7) { mySquare(45 + c * 40, 45 + r * 40, 75 + c * 40, 75 + r * 40, false);r++; ri++;if (EnterPressed == 1) { r--;EnterPressed = 0; CanSwap = 1;} }
		if(key==6){CanSave=1;cout<<"Saved  ";}
		if(key==7){CanLoad=1;cout<<"Loaded"; }
		//also load save;
		mySquare(45 + c * 40, 45 + r * 40, 75 + c * 40, 75 + r * 40,true);

		// for swap 41
		if (CanSwap == 1)
		{
			swap(arr[r][c], arr[ri][ci]);PrintCoolBoard();Sleep(100);
			PhysicsOfGame(r,c,ri,ci);
			CanSwap = 0;
		}
		

		end = time(NULL); // End of stopwatch
		
		if (difftime(end, start) > GameTime) key = 9; // This breaks the loop at 60 sec normally
		
		if (CanSave) { SaveGame(start, end);CanSave = 0; }
		if (CanLoad) { LoadGame(S, E);start = end + difftime(S, E) ;CanLoad = 0;}
		gotoxy(0, 0);
		cout <<"  " << 60-difftime(end, start) << " sec left ";
		cout << "\tScore = " << score;
		cout << "\tCollisions = " << collisions << "\t" << end <<" "<<start<<"\n";
	}

	myRect(40, 40, 370, 370, 0, 0, 0, 0, 0, 0);
	gotoxy(0,0);
	cout << "A fact: The board has been printed " << i << " times.\n\n\n\n\n" << "\tScore = " << score << "\tCollisions = " << collisions << "\t\n\n\n\n\n"; system("PAUSE");system("PAUSE");
	
}


void SaveGame(time_t start, time_t end)
{
	ofstream fout("BejeweledData.txt");
	fout << score<<endl << collisions<<endl << start<< endl << end << endl;
	for (int i = 0; i < 8; i++)	for (int j = 0; j < 8; j++)
		fout << arr[i][j] << " ";
}
void LoadGame(time_t &start, time_t &end)
{
	ifstream fin("BejeweledData.txt");
	fin >> score >> collisions >> start >> end;
	for (int i = 0; i < 8; i++)	for (int j = 0; j < 8; j++)
		fin >> arr[i][j];
}

void PhysicsOfGame(int r1, int c1, int r2, int c2)
{
	if (arr[r1][c1] == CubeGem || arr[r2][c2] == CubeGem)
		{
			if (arr[r1][c1] == CubeGem)
			{
				BurstSpecificGems(arr[r2][c2]);
				arr[r1][c1] = 0;
				arr[r2][c2] = 0;
			}
			else if (arr[r2][c2] == CubeGem)
			{
				BurstSpecificGems(arr[r1][c1]);
				arr[r2][c2] = 0;
				arr[r1][c1] = 0;
			}
		}

	else if (BlastGems() == false)
	{
		swap(arr[r1][c1], arr[r2][c2]);
		return;
	}
	
	ApplyGravity();Fill();collisions++;
	while (BlastGems() == true) 
	{ ApplyGravity();Fill();collisions++; }
	

}
void BurstSpecificGems(int Gem)
{
	// Convert not simple gem to simple gem
	if (Gem<0) Gem = mod(Gem); if (Gem>9) Gem = modX(Gem);
	
	for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++)
	if (arr[i][j] == Gem || mod(arr[i][j]) == Gem || modX(arr[i][j]) == Gem)
	{arr[i][j] = 0; score++;}
		
}
void ApplyGravity()
{
	bool ZeroFound = false;;
	for (int i = 0; i < 8; i++)
	{
		ZeroFound = false;
		for (int j = 0; j < 8; j++)
		{
			if (arr[i][j] == 0)
			{
				ZeroFound = true;
				for (int row = i; row != 0; row--)
				{
					swap(arr[row][j], arr[row - 1][j]);
				}
			}
		}
		
		if (ZeroFound == true)
		{
			Sleep(100);
			PrintCoolBoard();
		}
	}
}
void SimpleFill() 
{
	int Random;	srand(time(NULL));
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			Random = rand() % 5 + 1;
			if (arr[i][j] == 0)
			{
				arr[i][j] = Random;
			}
		}
}
void PrintCoolBoard()
{
	for (int i = 0; i < 8; i++)	for (int j = 0; j < 8; j++)DrawShape(arr[i][j], j, i);
}


void Fill()
{
	bool ZeroFound;
	int Random;
	srand(time(0));
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			ZeroFound = false;
			if (arr[i][j] == 0)
			{
				ZeroFound = true;
				Random = rand() % 5 + 1;
				if (Random == arr[i][j - 1])
				{
					if (Random == 5)
						arr[i][j] = Random - 1;
					else
						arr[i][j] = Random + 1;
				}
			
				else if (Random == arr[i+1][j])
				{
					if (Random == 5)
						arr[i][j] = Random - 1;
					else
						arr[i][j] = Random + 1;
				}
				else arr[i][j] = Random;
			}
		}
		if (firstRun == false && ZeroFound == true)
		{
			//Sleep(100);
			PrintCoolBoard();
		}
	}
	if (firstRun == true)
	{
		PrintCoolBoard();
	}

	firstRun = false;
}
void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
bool BlastGems()
{

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{




			bool ConditionFor3GemsInRow = arr[i][j] == arr[i][j + 1] && arr[i][j] == arr[i][j + 2] && arr[i][j] != 0 && j < 6;
			bool ConditionFor4GemsInRow = ConditionFor3GemsInRow && arr[i][j] == arr[i][j + 3] && j < 5;
			bool ConditionFor5GemsInRow = ConditionFor4GemsInRow && arr[i][j] == arr[i][j + 4] && j < 4;

			bool ConditionFor3GemsInRowOne = arr[i + 1][j] == arr[i + 1][j + 1] && arr[i + 1][j] == arr[i + 1][j + 2] && arr[i + 1][j] != 0 && j < 6;
			bool ConditionFor3GemsInRowTwo = arr[i + 2][j] == arr[i + 2][j + 1] && arr[i + 2][j] == arr[i + 2][j + 2] && arr[i + 2][j] != 0 && j < 6;
			bool ConditionFor3GemsInRowWithOneOrMoreSpecialGems = mod(arr[i][j]) == mod(arr[i][j + 1]) && mod(arr[i][j]) == mod(arr[i][j + 2]) && arr[i][j] != 0 && j < 6;

			bool ConditionFor3GemsInRowWithOneOrMoreDestroyerGems = modX(arr[i][j]) == modX(arr[i][j + 1]) && modX(arr[i][j]) == modX(arr[i][j + 2]) && arr[i][j] != 0 && j < 6;





			bool ConditionFor3GemsInColumn = arr[i][j] == arr[i + 1][j] && arr[i][j] == arr[i + 2][j] && arr[i][j] != 0 && i < 6;
			bool ConditionFor4GemsInColumn = ConditionFor3GemsInColumn && arr[i][j] == arr[i + 3][j] && i < 5;
			bool ConditionFor5GemsInColumn = ConditionFor4GemsInColumn && arr[i][j] == arr[i + 4][j] && i < 4;

			bool ConditionFor3GemsInColumnOne = arr[i][j + 1] == arr[i + 1][j + 1] && arr[i][j + 1] == arr[i + 2][j + 1] && arr[i][j + 1] != 0 && i < 6;
			bool ConditionFor3GemsInColumnTwo = arr[i][j + 2] == arr[i + 1][j + 2] && arr[i][j + 2] == arr[i + 2][j + 2] && arr[i][j + 2] != 0 && i < 6;
			bool ConditionFor3GemsInColumnWithOneOrMoreSpecialGems = mod(arr[i][j]) == mod(arr[i + 1][j]) && mod(arr[i][j]) == mod(arr[i + 2][j]) && arr[i][j] != 0 && i < 6;

			bool ConditionFor3GemsInColumnWithOneOrMoreDestroyerGems = modX(arr[i][j]) == modX(arr[i + 1][j]) && modX(arr[i][j]) == modX(arr[i + 2][j]) && arr[i][j] != 0 && i < 6;




			bool TopLeftElbowWithSameGems = ConditionFor3GemsInRow && ConditionFor3GemsInColumn;
			bool TopRightElbowWithSameGems = ConditionFor3GemsInRow && ConditionFor3GemsInColumnTwo;
			bool BottomLeftElbowWithSameGems = ConditionFor3GemsInColumn && ConditionFor3GemsInRowTwo;

			bool BottomRightElbowWithSameGems = ConditionFor3GemsInRowTwo && ConditionFor3GemsInColumnTwo;
			bool TopT = ConditionFor3GemsInRow && ConditionFor3GemsInColumnOne;
			bool BottomT = ConditionFor3GemsInRowTwo && ConditionFor3GemsInColumnOne;

			bool RightT = ConditionFor3GemsInColumnTwo &&  ConditionFor3GemsInRowOne;
			bool LeftT = ConditionFor3GemsInColumn && ConditionFor3GemsInRowOne;




			const int ScoreElbow = 5; // Elbow Score  
			const int ScoreT = 5;     // Score for making a T
			const int ScoreC = 5;     // Score for making a Cube Gem

			const int ScoreSp = 4;     // Score for making a Special Gem
			const int ScoreSpecial = 10; // Score for Busting with a special gem
			const int ScoreDestroyer = 20; // Score for Busting with a Desrtryer gem

			const int ScoreS = 3;	// Score for making 3 gems in row or column







			if (TopLeftElbowWithSameGems)
			{
				arr[i][j] = arr[i][j] * 10;

				arr[i][j + 1] = 0;
				arr[i][j + 2] = 0;

				arr[i + 1][j] = 0;
				arr[i + 2][j] = 0;

				score += ScoreElbow;
				return true;
			}
			else if (TopRightElbowWithSameGems)
			{
				arr[i][j] = arr[i][j] * 10;

				arr[i][j + 1] = 0;
				arr[i][j + 2] = 0;

				arr[i + 1][j + 2] = 0;
				arr[i + 2][j + 2] = 0;

				score += ScoreElbow;
				return true;
			}
			else if (BottomLeftElbowWithSameGems)
			{
				arr[i + 2][j] = arr[i][j] * 10;

				arr[i][j] = 0;
				arr[i + 1][j] = 0;

				arr[i + 2][j + 1] = 0;
				arr[i + 2][j + 2] = 0;

				score += ScoreElbow;
				return true;
			}
			else if (BottomRightElbowWithSameGems)
			{
				arr[i][j + 2] = arr[i][j] * 10;

				arr[i + 1][j + 2] = 0;
				arr[i + 2][j + 2] = 0;

				arr[i + 2][j] = 0;
				arr[i + 2][j + 1] = 0;

				score += ScoreElbow;
				return true;
			}




			else if (TopT)
			{
				arr[i][j + 1] = arr[i][j + 1] * 10;

				arr[i][j] = 0;
				arr[i][j + 2] = 0;

				arr[i + 1][j + 1] = 0;
				arr[i + 2][j + 1] = 0;

				score += ScoreT;
				return true;
			}
			else if (BottomT)
			{
				arr[i + 2][j + 1] = arr[i + 2][j + 1] * 10;

				arr[i][j + 1] = 0;
				arr[i + 1][j + 1] = 0;

				arr[i + 2][j] = 0;
				arr[i + 2][j + 2] = 0;

				score += ScoreT;
				return true;
			}
			else if (LeftT)
			{
				arr[i + 1][j] = arr[i + 1][j] * 10;

				arr[i][j] = 0;
				arr[i + 2][j] = 0;

				arr[i + 1][j + 1] = 0;
				arr[i + 1][j + 2] = 0;

				score += ScoreT;
				return true;
			}
			else if (RightT)
			{
				arr[i + 1][j + 2] = arr[i + 1][j + 2] * 10;

				arr[i][j + 2] = 0;
				arr[i + 2][j + 2] = 0;

				arr[i + 1][j + 1] = 0;
				arr[i + 1][j] = 0;

				score += ScoreT;
				return true;
			}

			else if (ConditionFor3GemsInRow == true && ConditionFor5GemsInRow == false)
			{
				arr[i][j] = 0;
				arr[i][j + 1] = 0;
				arr[i][j + 2] = 0;

				score += ScoreS;
				return true;
			}

			else if (ConditionFor5GemsInRow == true)
			{
				arr[i][j] = CubeGem; // Making a cube gem
				arr[i][j + 1] = 0;
				arr[i][j + 2] = 0;
				arr[i][j + 3] = 0;
				arr[i][j + 4] = 0;

				score += ScoreC;
				return true;
			}
			else if (ConditionFor3GemsInRowWithOneOrMoreDestroyerGems)
			{
				if (arr[i][j] > 9)
					DestroyRowAndColumn(i, j, arr);
				else if (arr[i][j + 1] > 9)
					DestroyRowAndColumn(i, j + 1, arr);
				else if (arr[i][j + 2] > 9)
					DestroyRowAndColumn(i, j + 2, arr);

				score += ScoreDestroyer;
				return true;
			}
			else if (ConditionFor3GemsInRowWithOneOrMoreSpecialGems == true)
			{
				arr[i][j] = 0;
				arr[i][j + 1] = 0;
				arr[i][j + 2] = 0;

				arr[i + 1][j] = 0;
				arr[i + 1][j + 1] = 0;
				arr[i + 1][j + 2] = 0;

				arr[i - 1][j] = 0;
				arr[i - 1][j + 1] = 0;
				arr[i - 1][j + 2] = 0;

				score += ScoreSpecial;
				return true;
			}
			else if (ConditionFor4GemsInRow == true)
			{
				arr[i][j] = -arr[i][j]; // Making a special gem 
				arr[i][j + 1] = 0;
				arr[i][j + 2] = 0;
				arr[i][j + 3] = 0;

				score += ScoreSp;
				return true;
			}
			



			else if (ConditionFor3GemsInColumn == true)
			{
				arr[i][j] = 0;
				arr[i + 1][j] = 0;
				arr[i + 2][j] = 0;

				score += ScoreS;
				return true;
			}


			else if (ConditionFor5GemsInColumn == true)
			{
				arr[i][j] = CubeGem;
				arr[i + 1][j] = 0;
				arr[i + 2][j] = 0;
				arr[i + 3][j] = 0;
				arr[i + 4][j] = 0;

				score += ScoreC;
				return true;
			}
			else if (ConditionFor3GemsInColumnWithOneOrMoreSpecialGems == true)
			{
				arr[i][j] = 0;
				arr[i + 1][j] = 0;
				arr[i + 2][j] = 0;

				arr[i][j + 1] = 0;
				arr[i + 1][j + 1] = 0;
				arr[i + 2][j + 1] = 0;

				arr[i][j - 1] = 0;
				arr[i + 1][j - 1] = 0;
				arr[i + 2][j - 1] = 0;

				score += ScoreSpecial;
				return true;
			}
			else if (ConditionFor3GemsInColumnWithOneOrMoreDestroyerGems)
			{
				if (arr[i][j] > 9)
					DestroyRowAndColumn(i, j, arr);
				else if (arr[i + 1][j] > 9)
					DestroyRowAndColumn(i + 1, j, arr);
				else if (arr[i + 2][j] > 9)
					DestroyRowAndColumn(i + 2, j, arr);

				score += ScoreDestroyer;
				return true;
			}
			else if (ConditionFor4GemsInColumn == true)
			{
				arr[i][j] = -arr[i][j]; // Wow we are making a special gem
				arr[i + 1][j] = 0;
				arr[i + 2][j] = 0;
				arr[i + 3][j] = 0;

				score += ScoreSp;
				return true;
			}

		}
	}

	return false;

}
int mod(int num)
{
	if (num > 0) return num;
	else return -num;
}
int modX(int num) // Special mod. It gives 2 when takes 2 and gives 2 when takes 20.
{
	if (num > 9) return num / 10;
	else return num;
}
void DestroyRowAndColumn(int row, int column, int arr[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		arr[row][i] = 0;
		arr[i][column] = 0;
	}
}
void DrawShape(int Shape, int x, int y)
{
	int X, Y, Sx, Sy, Sxi, Syi;
	X = x * 40;
	Y = y * 40;

	Sx = 52; // for Size x
	Sy = 68; // for Size y

	Sxi = 45; // for Size x
	Syi = 75; // for Size y
			  //myRect(40, 40, 358, 358, 0, 0, 0, 0, 207, 209);
	for (int i = 0; i < 360; i = i + 40)
	{
		myLine(40 + i, 40, 40 + i, 360, 255);
		myLine(40, 40 + i, 360, 40 + i, 255);
	}


	if (Shape < 0)
	{
		myRect(43 + X, 43 + Y, 77 + X, 77 + Y, 0, 0, 0, 255, 0, 0);
		myEllipse(43 + X, 43 + Y, 77 + X, 77 + Y, 255, 140, 0, 0, 0, 0);
	}
	else if (Shape > 9 && Shape != 100)
	{
		myRect(43 + X, 43 + Y, 77 + X, 77 + Y, 0, 0, 0, 255, 0, 0);
		myEllipse(43 + X, 43 + Y, 77 + X, 77 + Y, 0, 255, 255, 0, 0, 0);
	}


	if (Shape == 0)
		myRect(Sx + X - 5, Sx + Y - 5, Sy + X + 5, Sy + Y + 5, 0, 0, 0, 0, 0, 0);

	if (mod(Shape) == 1 || modX(Shape) == 1)
	{
		myRect(Sx + X - 5, Sx + Y - 5, Sy + X + 5, Sy + Y + 5, 0, 0, 0, 0, 0, 0);
		myEllipse(Sx + X, Sx + Y, Sy + X, Sy + Y, 200, 200, 200, 255, 255, 255);
	}

	else if (mod(Shape) == 2 || modX(Shape) == 2)
	{
		myRect(Sx + X - 5, Sx + Y - 5, Sy + X + 5, Sy + Y + 5, 0, 0, 0, 0, 0, 0);
		myEllipse(Sx + X, Sx + Y, Sy + X, Sy + Y, 204, 204, 0, 255, 255, 51);
	}
	else if (mod(Shape) == 3 || modX(Shape) == 3)
	{
		myRect(Sx + X - 5, Sx + Y - 5, Sy + X + 5, Sy + Y + 5, 0, 0, 0, 0, 0, 0);
		myEllipse(Sx + X, Sx + Y, Sy + X, Sy + Y, 0, 150, 0, 0, 255, 0);
	}
	else if (mod(Shape) == 4 || modX(Shape) == 4)
	{
		myRect(Sx + X - 5, Sx + Y - 5, Sy + X + 5, Sy + Y + 5, 0, 0, 0, 0, 0, 0);
		myEllipse(Sx + X, Sx + Y, Sy + X, Sy + Y, 192, 5, 248, 121, 61, 244);
	}
	else if (mod(Shape) == 5 || modX(Shape) == 5)
	{
		myRect(Sx + X - 5, Sx + Y - 5, Sy + X + 5, Sy + Y + 5, 0, 0, 0, 0, 0, 0);
		myEllipse(Sx + X, Sx + Y, Sy + X, Sy + Y, 200, 0, 0, 255, 0, 0);
	}

	else if (Shape == 100)
	{
		myRect(Sx + X - 5, Sx + Y - 5, Sy + X + 5, Sy + Y + 5, 0, 0, 0, 0, 0, 0);
		myTri(Sx + X, Sx + Y, Sy + X, Sy + Y, 200, 0, 0, 255, 0, 0);
	}



}


void myLine(int x1, int y1, int x2, int y2, int color) //use three 3 integers if you want colored lines.
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255) to get shades of gray. For other colors use 3 integers for colors.
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(color, color, color)); //2 is the width of the pen
	SelectObject(device_context, pen);
	MoveToEx(device_context, x1, y1, NULL);
	LineTo(device_context, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);

}
void myTri(int x1, int y1, int x2, int y2, int R, int G, int B, int FR, int FG, int FB)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	int mid = (x1 + x2) / 2;
	MoveToEx(device_context, mid, y1, NULL);
	LineTo(device_context, x2, y2);
	LineTo(device_context, x1, y2);
	LineTo(device_context, mid + 1, y1);
	HBRUSH brush = ::CreateSolidBrush(RGB(FR, FG, FB)); //Fill color is passed as parameter to the function!!!
	SelectObject(device_context, brush);
	FloodFill(device_context, mid, (y1 + y2) / 2, RGB(R, G, B));

	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void myEllipse(int x1, int y1, int x2, int y2, int R, int G, int B, int FR, int FG, int FB)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(FR, FG, FB));  //Fill color is black
	SelectObject(device_context, brush);
	Ellipse(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);


}
void myRect(int x1, int y1, int x2, int y2, int R, int G, int B, int FR, int FG, int FB)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(FR, FG, FB)); //Fill color is passed as parameter to the function!!!

	SelectObject(device_context, brush);

	Rectangle(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void mySquare(int x1, int y1, int x2, int y2, bool color)
{
	int a = 20, c;
	if (color == true)
		c = 255;
	else
		c = 0;

	myLine(x1, y1, x1, y2 - a, c);
	myLine(x1, y1 + a, x1, y2, c);
							   
	myLine(x1 + a, y2, x2, y2, c);
	myLine(x1, y2, x2 - 20, y2,c);
							   
	myLine(x1 + a, y1, x2, y1, c);
	myLine(x1, y1, x2 - a, y1, c);
							   
	myLine(x2, y1 + a, x2, y2, c);
	myLine(x2, y1, x2, y2 - a, c);
}
bool isCursorKeyPressed(int& whickKey)   //whichKey passed as reference.... 
{
	char key;
	key = GetAsyncKeyState(37);
	if (key == 1)
	{
		whickKey = 1;		// 1 if left key is pressed 
		return true;
	}
	key = GetAsyncKeyState(38);
	if (key == 1)
	{

		whickKey = 2;		// 2 if up key is pressed
		return true;
	}

	key = GetAsyncKeyState(39);
	if (key == 1)
	{

		whickKey = 3; // 3 if right key is pressed
		return true;
	}
	key = GetAsyncKeyState(40);
	if (key == 1)
	{

		whickKey = 4;   // 4 if down key is pressed
		return true;
	}

	// from https://www.amibroker.com/guide/afl/getasynckeystate.html
	key = GetAsyncKeyState(13);
	if (key == 1)
	{

		whickKey = 5;   // 5 if ENTER key is pressed
		return true;
	}
	key = GetAsyncKeyState(27);
	if (key == 1)
	{

		whickKey = 9;   // 9 if ESC key is pressed
		return true;
	}
	key = GetAsyncKeyState('S');
	if (key == 1)
	{

		whickKey = 6;   // 9 if ESC key is pressed
		return true;
	}
	key = GetAsyncKeyState('L');
	if (key == 1)
	{

		whickKey = 7;   // 9 if ESC key is pressed
		return true;
	}
	return false;
}


//SimpleFill(); while (BlastGems() == true) { ApplyGravity();SimpleFill();collisions++; }PrintCoolBoard(arr);
//	myRect(40, 40, 362, 362, 0, 0, 0, 250, 0, 0);
