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

void loadMap(string nameMapFile, int map[][sizeMap]) {
	ifstream file(nameMapFile);
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
	for (int i = 0; i < sizeMap; i++) {
		for (int j = 0; j < sizeMap; j++) {
			cout << map[i][j] << " ";
		}
		cout << endl;
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
			if (map[i][j] == 1) {
				cell.setFillColor(Color::Green);
			}
			if (map[i][j] == 2) {
				cell.setFillColor(Color::Red);
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
			else {
				cell.setFillColor(Color::White);	
			}
			cell.setPosition(j * sizeCell + offsetBorder + 600, i * sizeCell + offsetBorder);
			window.draw(cell);
		}
	}
}

int stepPlayer(int map[][sizeMap], RenderWindow &window) {
	while (true) {
		if (Mouse::isButtonPressed(Mouse::Left)) {
			Vector2i localPositionMouse = Mouse::getPosition(window);
			cout << localPositionMouse.y - offsetBorder << " " << localPositionMouse.x - offsetBorder - 600 << endl;
			if (map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder - 600) / sizeCell] == 1) {
				map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder - 600) / sizeCell] = 2;
				return 1;
			}
			else {
				return 0;
			}
		}
	}
}

int stepEnemy(int map[][sizeMap], RenderWindow& window) {
	int x = rand() % 3;
	int y = rand() % 10;
	cout << x << " " << y << endl;
	if (map[y][x] == 1) {
		map[y][x] = 2;
		return 1;
	}
	else {
		return 0;
	}
}

int main() {

	int mapPlayer[sizeMap][sizeMap];
	int mapEnemy[sizeMap][sizeMap];
	loadMap("mapFirst.txt", mapPlayer);
	loadMap("mapSecond.txt", mapEnemy);
	srand(time(NULL));
	RenderWindow mainWindow(VideoMode(1300, 600), "Main Window");
	while (mainWindow.isOpen()) {
		Event event;
		while (mainWindow.pollEvent(event)) {
			if (event.type == Event::Closed) {
				mainWindow.close();
			}
		}
		drawMap(mainWindow, mapPlayer);
		drawMapEnemy(mainWindow, mapEnemy);
		while (stepPlayer(mapEnemy, mainWindow) == 1) {
			drawMapEnemy(mainWindow, mapEnemy);
			stepPlayer(mapEnemy, mainWindow);
		}
		while (stepEnemy(mapPlayer, mainWindow) == 1) {
			drawMap(mainWindow, mapPlayer);
			stepEnemy(mapPlayer, mainWindow);
		}
		//stepEnemy(mapPlayer, mainWindow);
		//stepPlayer(mapEnemy, mainWindow);
		mainWindow.display();
		Sleep(100);
	}
	return 0;
}