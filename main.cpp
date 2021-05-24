#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "windows.h"
#include <string>
#include <algorithm>

using namespace std;
using namespace sf;

const int sizeMap = 10;
const int sizeCell = 48;
const int offsetBorder = 100;

bool loadMap(string nameMapFile, int map[][sizeMap]) {
	ifstream file(nameMapFile);
	if (!file.is_open()) {
		cout << "Файл не может быть открыт!!!" << endl;
		return false;
	}
	while (file.is_open()) {
		while (!file.eof()) {
			for (int i = 0; i < sizeMap; i++) {
				for (int j = 0; j < sizeMap; j++) {
					file >> map[i][j];
				}
			}
		}
		file.close();
	}
}

void drawMap(RenderWindow &window, int map[][sizeMap]) {
	RectangleShape cell(Vector2f(sizeCell, sizeCell));
	cell.setOutlineColor(Color{ 0, 0, 0});
	cell.setOutlineThickness(1.f);
	for (int i = 0; i < sizeMap; i++) {
		for (int j = 0; j < sizeMap; j++) {
			if (map[i][j] == 0) {
				cell.setFillColor(Color::White);
			}
			else if (map[i][j] == 1) {
				cell.setFillColor(Color::Green);
			}
			else if (map[i][j] == 2) {
				cell.setFillColor(Color::Red);
			}
			else if (map[i][j] == 3) {
				cell.setFillColor(Color::Blue);
			}
			cell.setPosition(j * sizeCell + offsetBorder, i * sizeCell + offsetBorder);
			window.draw(cell);
		}
	}
}

void drawMapEnemy(RenderWindow& window, int map[][sizeMap]) {
	RectangleShape cell(Vector2f(sizeCell, sizeCell));
	cell.setOutlineColor(Color{ 0, 0, 0 });
	cell.setOutlineThickness(1.f);
	for (int i = 0; i < sizeMap; i++) {
		for (int j = 0; j < sizeMap; j++) {
			if (map[i][j] == 2) {
				cell.setFillColor(Color::Red);
			}
			else if (map[i][j] == 3){
				cell.setFillColor(Color::Blue);
			}
			else {
				cell.setFillColor(Color::White);	
			}
			cell.setPosition(j * sizeCell + offsetBorder + 600, i * sizeCell + offsetBorder);
			window.draw(cell);
		}
	}
}

int stepPlayer(int map[][sizeMap], RenderWindow& window) {
	Vector2i localPositionMouse = Mouse::getPosition(window);
	//Mouse::setPosition(Vector2i(700, 100), window);
	cout << (localPositionMouse.y - offsetBorder) / sizeCell << " " << (localPositionMouse.x - offsetBorder - 600) / sizeCell << endl;
	if (map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder - 600) / sizeCell] == 1)  {
		map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder - 600) / sizeCell] = 2;
		return 1;
	}
	else if (map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder - 600) / sizeCell] == 0) {
		map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder - 600) / sizeCell] = 3; {
			return 0;
		}
	}
}

void refactorMapPlayer(int map[][sizeMap], RenderWindow& window) {
	Vector2i localPositionMouse = Mouse::getPosition(window);
	cout << (localPositionMouse.y - offsetBorder) / sizeCell << " " << (localPositionMouse.x - offsetBorder) / sizeCell << endl;
	if (map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder) / sizeCell] == 1) {
		map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder) / sizeCell] = 0;
	}
	else if (map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder) / sizeCell] == 0) {
		map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder) / sizeCell] = 1;
	}
}

int stepEnemy(int map[][sizeMap], int x, int y) {
	if (map[y][x] == 1) {
		map[y][x] = 2;
		return 1;
	}
	else if (map[y][x] == 0){
		map[y][x] = 3;
		return 0;
	}
}

int main() {

	int mapPlayer[sizeMap][sizeMap];
	int mapEnemy[sizeMap][sizeMap];
	int countEnemy = 20, countPlayer = 20;
	bool refactor = true;
	string nameOffFile;
	getline(cin, nameOffFile);
	if (loadMap(nameOffFile, mapPlayer));
	else return 0;
	if (loadMap("mapSecond.txt", mapEnemy));
	else return 0;
	srand(time(NULL));
	RenderWindow mainWindow(VideoMode(1300, 600), "Main Window");
	while (mainWindow.isOpen()) {
		Event event;
		while (mainWindow.pollEvent(event)) {
			if (event.type == Event::Closed) {
				mainWindow.close();
			}
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right && refactor) {
				refactorMapPlayer(mapPlayer, mainWindow);
			}
			else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				refactor = false;
				int player = stepPlayer(mapEnemy, mainWindow);
				if (player == 1) {
					countPlayer--;
				}
				else if(player == 0) {
					int x = rand() % 10;
					int y = rand() % 10;
					while (mapPlayer[y][x] != 0 && mapPlayer[y][x] != 1) {
						x = rand() % 10;
						y = rand() % 10;
					}
					int enemy = stepEnemy(mapPlayer, x, y);
					while (enemy == 1) {
						enemy = stepEnemy(mapPlayer, x, y + 1);
						if (enemy == 1) {
							y++;
							enemy = stepEnemy(mapPlayer, x, y + 1);
						}
						else {
							enemy = stepEnemy(mapPlayer, x + 1, y);
							if (enemy == 1) {
								x++;
								countEnemy--;
								enemy = stepEnemy(mapPlayer, x + 1, y);
							}
						}
					}
				}
			}
			if (countEnemy == 0 || countPlayer == 0) {
				if (countEnemy == 0) {
					cout << "Computer wins this game!\n";
				}
				else {
					cout << "Player wins this game!" << endl;
				}
				mainWindow.close();
			}
		}
		drawMap(mainWindow, mapPlayer);
		drawMapEnemy(mainWindow, mapEnemy);
		mainWindow.display();
	}
	return 0;
}