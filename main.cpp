#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
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

int stepPlayer(int map[][sizeMap], RenderWindow &window) {
	//Mouse::setPosition(Vector2i(offsetBorder, offsetBorder), window);
	while (!Mouse::isButtonPressed(Mouse::Left)) {
		continue;
	}
	Vector2i localPositionMouse = Mouse::getPosition(window);
	cout << (localPositionMouse.y - offsetBorder) / sizeCell << " " << (localPositionMouse.x - offsetBorder) / sizeCell << endl;
	if (map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder) / sizeCell] == 1) {
		map[(localPositionMouse.y - offsetBorder) / sizeCell][(localPositionMouse.x - offsetBorder) / sizeCell] = 2;
		return 1;
	}
	else {
		return 0;
	}
}

void mainGame() {

}

int main() {

	int map[sizeMap][sizeMap];
	loadMap("mapFirst.txt", map);

	RenderWindow mainWindow(VideoMode(800, 600), "Main Window");
	while (mainWindow.isOpen()) {
		Event event;
		while (mainWindow.pollEvent(event)) {
			if (event.type == Event::Closed) {
				mainWindow.close();
			}
		}
		mainWindow.clear();
		drawMap(mainWindow, map);
		stepPlayer(map, mainWindow);
		mainWindow.display();
	}
	return 0;
}