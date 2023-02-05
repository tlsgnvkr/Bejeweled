#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
using namespace std;

vector<vector<int>> board(20, vector<int>(20, 0));

int score = 0;
int countt = 0;
int st = 0;

void textColor(int, int);
void boardSet();
void printBoard(int, int);
int left(int);
int right(int);
int up(int);
int down(int);
void space(int, int);
void unSpace(int, int);
int checkPivot(int, int, int, int);
void swap(int, int, int, int);
int boardCheck();
void removeChain();
void removeChainReflect(int, int);
void boardDown(int, int, int, int);
void pushBoard();
void gameOver();

#define RED 12
#define GREEN 10
#define BLUE 9
#define YELLOW 6
#define CYAN 11
#define MAGENTA 13
#define WHITE 15
#define BLACK 0
#define LIGHTGREY 7
#define DARKGREY 8
#define LEFT 97
#define RIGHT 100
#define UP 119
#define DOWN 115
#define SPACE 32
#define ESC 101

void textColor(int foreground, int background) {
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void boardSet() {
	vector<int> X = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA };
	for (int i = 0; i < 20; i += 1) {
		for (int j = 0; j < 20; j += 1) {
			if (i < 2 && j < 2) {
				board[i][j] = X[rand() % 6];
			}
			else if (i >= 2 && j < 2) {
				while (1) {
					board[i][j] = X[rand() % 6];
					if (board[i - 2][j] != board[i][j] || board[i - 1][j] != board[i][j]) break;
				}
			}
			else if (i < 2 && j >= 2) {
				while (1) {
					board[i][j] = X[rand() % 6];
					if (board[i][j - 2] != board[i][j] || board[i][j - 1] != board[i][j]) break;
				}
			}
			else {
				while (1) {
					board[i][j] = X[rand() % 6];
					if (board[i - 2][j] != board[i][j] || board[i - 1][j] != board[i][j]) {
						if (board[i][j - 2] != board[i][j] || board[i][j - 1] != board[i][j]) break;
					}
				}
			}
		}
	}
}

void printBoard(int y, int x) {
	system("cls");
	for (int i = 0; i < 20; i += 1) {
		for (int j = 0; j < 20; j += 1) {
			if (i == y && j == x) {
				textColor(board[i][j] % 100, WHITE);
			}
			else {
				if (board[i][j] > 200) textColor(board[i][j] - 200, DARKGREY);
				else textColor(board[i][j], BLACK);
			}

			if (board[i][j] % 100 != 0) cout << " бс ";
			else {
				textColor(BLACK, BLACK);
				if (i == y && j == x) textColor(BLACK, WHITE);
				cout << "   ";
			}
			textColor(WHITE, BLACK);
		}
		cout << endl;
	}

	cout << endl << endl;
	cout << "Score: " << score;
	cout << endl << endl;
	cout << "Press \"E\" to End the Game.";
}

int left(int x) {
	if (x != 0) x -= 1;
	return x;
}

int right(int x) {
	if (x != 19) x += 1;
	return x;
}

int up(int y) {
	if (y != 0) y -= 1;
	return y;
}

int down(int y) {
	if (y != 19) y += 1;
	return y;
}

void space(int y, int x) {
	board[y][x] += 200;
}

void unSpace(int y, int x) {
	board[y][x] %= 100;
}

int checkPivot(int y0, int x0, int y1, int x1) {
	if ((y0 == y1 && x0 == x1) || (y0 != y1 && x0 != x1)) {
		return 0;
	}
	else {
		if (y0 == y1 && abs(x1 - x0) == 1) {
			return 1;
		}
		else if (x0 == x1 && abs(y1 - y0) == 1) {
			return 1;
		}
		else return 0;
	}
}

void swap(int y0, int x0, int y1, int x1) {
	int S = board[y0][x0];
	board[y0][x0] = board[y1][x1];
	board[y1][x1] = S;
}

int boardCheck() {
	int stack;

	for (int y = 0; y < 20; y += 1) {
		for (int x = 0; x < 20; x += 1) {
			stack = 0;

			for (int i = y + 1; i < 20; i += 1) {
				if (abs(board[i][x]) % 100 == abs(board[y][x]) % 100) stack += 1;
				else break;
			}

			if (stack >= 2) {
				for (int i = y; i < y + stack + 1; i += 1) {
					if (board[i][x] > 0) board[i][x] *= -1;
				}
			}

			stack = 0;

			for (int j = x + 1; j < 20; j += 1) {
				if (abs(board[y][j] % 100) == abs(board[y][x]) % 100) stack += 1;
				else break;
			}

			if (stack >= 2) {
				for (int j = x; j < x + stack + 1; j += 1) {
					if (board[y][j] > 0) board[y][j] *= -1;
				}
			}
		}
	}

	for (int i = 0; i < 20; i += 1) {
		for (int j = 0; j < 20; j += 1) {
			if (board[i][j] < 0) return 1;
		}
	}
	return 0;
}

void removeChain() {
	int N = 0;
	for (int y = 0; y < 20; y += 1) {
		for (int x = 0; x < 20; x += 1) {
			countt = 0;
			removeChainReflect(y, x);
			if (countt > 0) {
				st += N;
				N += 1;
				score += countt - 2 + st;
			}
		}
	}
}

void removeChainReflect(int y, int x) {
	for (int i = 0; i < 20; i += 1) {
		for (int j = 0; j < 20; j += 1) {
			if (board[i][j] >= 0) {
				board[i][j] %= 100;
			}
			else {
				board[i][j] = board[i][j] % 100 - 100;
			}
		}
	}
	
	if (board[y][x] < 0) {
		countt += 1;
		int N = board[y][x];
		board[y][x] = 0;

		if (x != 0) {
			if (N == board[y][x - 1]) removeChainReflect(y, x - 1);
		}

		if (x != 19) {
			if (N == board[y][x + 1]) removeChainReflect(y, x + 1);
		}

		if (y != 19) {
			if (N == board[y + 1][x]) removeChainReflect(y + 1, x);
		}
	}
}

void boardDown(int y0, int x0, int y1, int x1) {
	int c;
	int p = board[y0][x0] / 100;
	board[y0][x0] %= 100;
	int q = board[y1][x1] / 100;
	board[y1][x1] %= 100;
	for (int i = 19; i > 0; i -= 1) {
		for (int j = 0; j < 20; j += 1) {
			c = 0;
			while (1) {
				if (board[i][j] == 0) {
					for (int k = i; k > -1; k -= 1) {
						if (board[k][j] != 0) {
							c += 1;
							break;
						}
					}
					if (c) {
						for (int k = i; k > 0; k -= 1) {
							board[k][j] = board[k - 1][j];
						}
						board[0][j] = 0;
					}
					else break;
				}
				else break;
			}
		}
	}
	board[y0][x0] += p * 100;
	board[y1][x1] += q * 100;
}

void pushBoard() {
	vector<int> X = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA };

	for (int j = 0; j < 20; j += 1) {
		int k = 0;
		while (1) {
			if (board[k][j] == 0) k += 1;
			else break;
		}
		
		for (int i = 0; i < k; i += 1) {
			board[i][j] = X[rand() % 6];
		}
	}
}

void gameOver() {
	system("cls");
	for (int i = 0; i < 20; i += 1) {
		for (int j = 0; j < 20; j += 1) {
			textColor(board[i][j] % 100, BLACK);
			cout << " бс ";
		}
		cout << endl;
	}

	textColor(WHITE, BLACK);
	cout << endl << endl << "Game Over!" << endl << endl << "Score: " << score << endl;

	system("PAUSE");
}

int main() {
	srand((unsigned int)GetTickCount64());
	int y = 0, x = 0;
	int key;
	vector<int> pivot(2, 0);

	boardSet();
	printBoard(y, x);
	int N = 0;

	while (1) {
		key = _getch();
		switch (key) {
		case LEFT: x = left(x);
			break;

		case RIGHT: x = right(x);
			break;

		case UP: y = up(y);
			break;

		case DOWN: y = down(y);
			break;

		case SPACE:
			if (!N) {
				N = 1;
				space(y, x);
				pivot[0] = y;
				pivot[1] = x;
				break;
			}
			else {
				N = 0;
				space(y, x);
				printBoard(y, x);
				Sleep(250);

				if (checkPivot(y, x, pivot[0], pivot[1])) {
					swap(y, x, pivot[0], pivot[1]);
					printBoard(y, x);
					if (boardCheck()) {
						st = 0;
						removeChain();
						boardDown(y, x, pivot[0], pivot[1]);
						printBoard(y, x);
						Sleep(250);
						pushBoard();
						printBoard(y, x);
						Sleep(250);
						st += 1;

						while (1) {
							if (boardCheck()) {
								removeChain();
								boardDown(y, x, pivot[0], pivot[1]);
								printBoard(y, x);
								Sleep(250);
								pushBoard();
								printBoard(y, x);
								Sleep(250);
								st += 1;
							}
							else break;
						}
					}
					else {
						swap(y, x, pivot[0], pivot[1]);
					}
				}
				Sleep(250);
				unSpace(y, x);
				unSpace(pivot[0], pivot[1]);
				break;
			}

		case ESC: gameOver();
			return 0;
		}

		printBoard(y, x);
	}

	return 0;
}