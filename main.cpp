#include <iostream>
#include <queue>
#include <string>
#include <cmath>
#include <ctime>
#include <conio.h>
#include <Windows.h>

#define MAP_SIZE 20
#define SNAKE 1
#define FRUIT 7
#define EMPTY 0
#define EVENT 100

using namespace std;
//map[y][x];
int map[20][20];
int fruit, point;

class Snake {
private:
	deque<pair<int, int>> head;
public:
	// pair < x , y >
	Snake() {
		this->head.push_front({ 10,10 });
	}

	void printSnake() {
		system("cls");
		this->setSnakeToMap();
		cout << " [Snake Game] H:head,  x:body,  o:fruit \n";
		for (int i = 0; i < MAP_SIZE; ++i) {
			if (i == 0) cout << " ------------------------------------------\n";
			if(i <=MAP_SIZE-1) cout << "¦¢";
			for (int j = 0; j < MAP_SIZE; ++j) {
				cout << ' ';
				if (map[i][j] == EMPTY)
					cout << ' ';
				else if (map[i][j] == FRUIT)
					cout << 'o';
				
				else if (this->head.front().first == j && this->head.front().second == i) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					cout << 'H';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				/*else if (this->head.back().first == j && this->head.back().second == i)
					cout << 'T';*/
				else if (map[i][j] == SNAKE)
					cout << 'x';
			}
			if (i <= MAP_SIZE - 1) cout << " ¦¢";
			if (i == MAP_SIZE - 1) cout << "\n ------------------------------------------";
			cout << '\n';
		}
		cout << ' ';
		cout << "x _ " << this->head.front().first+1 << " y _ " << this->head.front().second+1 << "\n Fruit > " << fruit << "\n Point > " << point << '\n';
	}

	pair<int, int> snakeHead() {
		return this->head[0];
	}

	void setSnakeToMap() {
		for (int i = 0; i < 20; ++i) {
			for (int j = 0; j < 20; ++j) {
				if (map[i][j] != FRUIT) {
					map[i][j] = 0;
				}
			}
		}
		for (int i = 0; i < this->head.size(); ++i) {
			map[this->head[i].second][this->head[i].first] = SNAKE;
		}
	}

	bool isCollision(int x, int y) {
		for (int i = 1; i < this->head.size(); ++i) {
			if (this->head[i].first == x && this->head[i].second == y) {
				return true;
			}
		}
		return false;
	}

	void makeFruit() {
		srand((unsigned int)time(NULL));
		int x = rand() % 20, y = rand() % 20;
		if (!this->isCollision(x, y)) {
			map[x][y] = FRUIT;
			fruit++;
		}
	}

	void eatFruit(int x, int y) {
		this->head.push_back({ x,y });
		fruit--; point+=5;
	}
	//getch() = 224 after

	void controller(int key) {
		for (int i = this->head.size() - 1; i >= 1; --i) {
			this->head[i].first = this->head[i - 1].first;
			this->head[i].second = this->head[i - 1].second;
		}
		//UP
		if (key == 72) {
			this->head[0].second--;
			if (map[this->head[0].second][this->head[0].first] == FRUIT) {
				this->eatFruit(this->head.back().first, this->head.back().second+1);
			}
		}
		//DOWN
		else if (key == 80) {
			this->head[0].second++;
			if (map[this->head[0].second][this->head[0].first] == FRUIT) {
				this->eatFruit(this->head.back().first, this->head.back().second - 1);
			}
		}
		//left
		else if (key == 75) {
			this->head[0].first--;
			if (map[this->head[0].second][this->head[0].first] == FRUIT) {
				this->eatFruit(this->head.back().first + 1, this->head.back().second);
			}
		}
		//right
		else if (key == 77) {
			this->head[0].first++;
			if (map[this->head[0].second][this->head[0].first] == FRUIT) {
				this->eatFruit(this->head.back().first -1 , this->head.back().second);
			}
		}

		this->setSnakeToMap();
		this->printSnake();
	}

	bool gameOver() {
		if (this->head[0].first < 0 || this->head[0].second < 0
			|| this->head[0].first >19 || this->head[0].second>19) {
			return true;
		}
		else if (this->isCollision(this->head[0].first, this->head[0].second)) {
			return true;
		}
		return false;
	}
};


int main() {
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	clock_t start = clock(), end;
	Snake *snake = new Snake();
	snake->printSnake();
	while (!snake->gameOver()) {
		//map print
		//key input & move & eat
		_getch();
		snake->controller(_getch());
		//end clock
		end = clock();
		// make Fruit 3sec
		if ((end - start) / CLOCKS_PER_SEC % 3 == 2) {
			snake->makeFruit();
			cout << ' ';
			cout << "\n#####FRUIT#####" << '\n';
			start = end;
		}
	}
	cout << ' ';
	cout << "game over.." << endl;
	system("pause");
	return 0;
}