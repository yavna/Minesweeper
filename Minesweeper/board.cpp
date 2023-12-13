#include <SFML/graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "random.h"
#include "tile.h"
#include "board.h"
#include "texture.h"
#include <numeric>

using namespace std;

Board::Board(int rows, int cols, int numMines) {
	this->rows = rows;
	this->cols = cols;
	this->numMines = numMines;
	this->numTestMines = numTestMines;
	this->testOneMines = 0;
	this->testTwoMines = 0;
	this->testThreeMines = 0;
	numCounter = this->numMines;

	ReadTestFile();
	LoadTextures();

	PlaceTiles();
	PlaceMines();
}
	
void Board::ReadTestFile() {
	ifstream testFile;
	string line;

	// Read testboard 1
	testFile.open("boards/testboard1.brd"); // To avoid vector subscript out of range error
	if (testFile.is_open()) {
		string line;
		while (getline(testFile, line)) {
			for (char x : line) {
				int num = x - '0';
				if (num == 1) {
					this->testOneMines++;
				}
				this->testOne.push_back(num);
			}
		}
	}
	testFile.close();

	// Read testboard 2
	testFile.open("boards/testboard2.brd");
	if (testFile.is_open()) {
		string line;
		while (getline(testFile, line)) {
			for (char x : line) {
				int num = x - '0';
				if (num == 1) {
					this->testTwoMines++;
				}
				this->testTwo.push_back(num);
			}
		}
	}
	testFile.close();

	//Read testboard 3
	testFile.open("boards/testboard3.brd");
	if (testFile.is_open()) {
		string line;
		while (getline(testFile, line)) {
			for (char x : line) {
				int num = x - '0';
				if (num == 1) {
					this->testThreeMines++;
				}
				this->testThree.push_back(num);
			}
		}
	}
	testFile.close();
}


void Board::LoadTextures() {

	textures.emplace("debug", TextureManager::GetTexture("debug.png"));
	textures.emplace("happy", TextureManager::GetTexture("face_happy.png"));
	textures.emplace("lose", TextureManager::GetTexture("face_lose.png"));
	textures.emplace("win", TextureManager::GetTexture("face_win.png"));
	textures.emplace("test_1", TextureManager::GetTexture("test_1.png"));
	textures.emplace("test_2", TextureManager::GetTexture("test_2.png"));
	textures.emplace("test_3", TextureManager::GetTexture("test_3.png"));
	textures.emplace("flag", TextureManager::GetTexture("flag.png"));
	textures.emplace("mine", TextureManager::GetTexture("mine.png"));
	textures.emplace("tileHidden", TextureManager::GetTexture("tile_hidden.png"));
	textures.emplace("tileRevealed", TextureManager::GetTexture("tile_revealed.png"));
	textures.emplace("number_1", TextureManager::GetTexture("number_1.png"));
	textures.emplace("number_2", TextureManager::GetTexture("number_2.png"));
	textures.emplace("number_3", TextureManager::GetTexture("number_3.png"));
	textures.emplace("number_4", TextureManager::GetTexture("number_4.png"));
	textures.emplace("number_5", TextureManager::GetTexture("number_5.png"));
	textures.emplace("number_6", TextureManager::GetTexture("number_6.png"));
	textures.emplace("number_7", TextureManager::GetTexture("number_7.png"));
	textures.emplace("number_8", TextureManager::GetTexture("number_8.png"));
	textures.emplace("digits", TextureManager::GetTexture("digits.png"));

	TextureManager::Clear();

	// drawing buttons
	debug.setTexture(textures["debug"]);
	debug.setPosition((this->cols * 32) - 256, (this->rows * 32));
	face.setTexture(textures["happy"]);
	face.setPosition(((this->cols * 32) / 2) - 32, (this->rows * 32));
	test_1.setTexture(textures["test_1"]);
	test_1.setPosition((this->cols * 32) - 192, (this->rows * 32));
	test_2.setTexture(textures["test_2"]);
	test_2.setPosition((this->cols * 32) - 128, (this->rows * 32));
	test_3.setTexture(textures["test_3"]);
	test_3.setPosition((this->cols * 32) - 64, (this->rows * 32));

	// drawing counter
	negative.setTexture(textures["digits"]);
	negative.setTextureRect(sf::Rect<int>(0, 0, 21, 32));
	negative.setPosition(-21, (this->rows * 32));
	digit3.setTexture(textures["digits"]);
	digit3.setTextureRect(sf::Rect<int>(0, 0, 21, 32));
	digit3.setPosition(0, (this->rows * 32));
	digit2.setTexture(textures["digits"]);
	digit2.setTextureRect(sf::Rect<int>(21 * 5, 0, 21, 32));
	digit2.setPosition(21, (this->rows * 32));
	digit1.setTexture(textures["digits"]);
	digit1.setTextureRect(sf::Rect<int>(0, 0, 21, 32));
	digit1.setPosition(42, (this->rows * 32));
}


void Board::PlaceTiles() {
	for (int i = 0; i < this->rows * this->cols; i++) {
		int x = (i % this->cols) * 32;
		int y = (i / this->cols) * 32;
		Tile tile = Tile(textures, x, y);
		tiles.push_back(tile);
	}
}

void Board::PlaceMines() {
	mines.clear();
	int totalTiles = this->rows * this->cols;
	vector<int> mineIndices(totalTiles);
	for (int i = 0; i < totalTiles; i++) {
		mineIndices[i] = i;
	}
	std::random_shuffle(mineIndices.begin(), mineIndices.end());
	mines.resize(totalTiles, false);
	for (int i = 0; i < this->numMines; i++) {
		mines[mineIndices[i]] = true;
	}
	SetMines();
	ShowNumAdjacent();
	UpdateCounter();
	tilesRevealed = 0;
	numCounter = this->numMines;
	numTestMines = numMines;
	isGameLost = false;
	isGameOver = false;
}

void Board::ShowNumAdjacent() {
	for (int i = 0; i < (this->rows * this->cols); i++) {
		tiles[i].adjacentTiles.clear();
		for (int j = 0; j < 8; j++) {
			tiles[i].adjacentTiles.push_back(nullptr);
		}
		int row = i / this->cols;
		int col = i % this->cols;
		if (row > 0) {
			tiles[i].adjacentTiles[1] = &(tiles[i - this->cols]);
			if (col > 0) {
				tiles[i].adjacentTiles[0] = &(tiles[i - (this->cols + 1)]);
			}
			if (col < this->cols - 1) {
				tiles[i].adjacentTiles[2] = &(tiles[i - (this->cols - 1)]);
			}
		}
		if (row < this->rows - 1) {
			tiles[i].adjacentTiles[6] = &(tiles[i + this->cols]);
			if (col > 0) {
				tiles[i].adjacentTiles[5] = &(tiles[i + (this->cols - 1)]);
			}
			if (col < this->cols - 1) {
				tiles[i].adjacentTiles[7] = &(tiles[i + (this->cols + 1)]);
			}
		}
		if (col > 0) {
			tiles[i].adjacentTiles[3] = &(tiles[i - 1]);
		}
		if (col < this->cols - 1) {
			tiles[i].adjacentTiles[4] = &(tiles[i + 1]);
		}
		tiles[i].RevealTile(textures);
	}
}

void Board::SetMines() {
	for (int i = 0; i < (this->rows * this->cols); i++){
		tiles[i].SetMine(mines[i]);
	}
}

void Board::UpdateCounter() {
	int count = numCounter;
	int firstDigit;
	int secDigit;
	int thirdDigit;

	// adds place when counter goes negative
	if (numCounter < 0) {
		count *= -1;
		thirdDigit = count / 100;
		firstDigit = count % 10;
		count /= 10;
		secDigit = count % 10;
		negative.setTextureRect(sf::Rect<int>((21 * 11) - 21, 0, 21, 32));
		negative.setPosition(0, (this->rows * 32));
		digit1.setPosition(63, (this->rows * 32));
		digit2.setPosition(42, (this->rows * 32));
		digit3.setPosition(21, (this->rows * 32));
	}
	else {
		thirdDigit = count / 100;
		firstDigit = count % 10;
		count /= 10;
		secDigit = count % 10;
		negative.setTextureRect(sf::Rect<int>(0, 0, 21, 32));
		negative.setPosition(-21, (this->rows * 32));
		digit1.setPosition(42, (this->rows * 32));
		digit2.setPosition(21, (this->rows * 32));
		digit3.setPosition(0, (this->rows * 32));
	}
	digit1.setTextureRect(sf::Rect<int>(21 * firstDigit, 0, 21, 32));
	digit2.setTextureRect(sf::Rect<int>(21 * secDigit, 0, 21, 32));
	digit3.setTextureRect(sf::Rect<int>(21 * thirdDigit, 0, 21, 32));
}

void Board::ResetCounterWin() {
	digit1.setTextureRect(sf::Rect<int>(0, 0, 21, 32));
	digit2.setTextureRect(sf::Rect<int>(0, 0, 21, 32));
	digit3.setTextureRect(sf::Rect<int>(0, 0, 21, 32));
	digit1.setPosition(42, (this->rows * 32));
	digit2.setPosition(21, (this->rows * 32));
	digit3.setPosition(0, (this->rows * 32));
}


void Board::UpdateFace() {
	if (isGameOver) {
		if (isGameLost) {
			face.setTexture(textures[("lose")]);
		}
		else {
			face.setTexture(textures["win"]);
		}
	}
	else {
		face.setTexture(textures[("happy")]);
	}
}

void Board::Debug() {
	for (int i = 0; i < (this->rows * this->cols); i++)
	{
		tiles[i].SetDebug();
	}
}

void Board::ShowTestMines(vector<int>& testMines, int testboard) {
	mines.clear();
	for (int i = 0; i < (this->rows * this->cols); i++) {
		if (testMines[i] == 0) {
			mines.push_back(false);
		}
		if (testMines[i] == 1) {
			mines.push_back(true);
		}
	}
	if (testboard == 1) {
		this->numTestMines = this->testOneMines;
	}
	else if (testboard == 2) {
		this->numTestMines = this->testTwoMines;
	}
	else if (testboard == 3) {
		this->numTestMines = this->testThreeMines;
	}
	SetMines();
	ShowNumAdjacent();
	UpdateCounter();
	numCounter = this->numTestMines;
	UpdateCounter();
	isGameLost = false;
	isGameOver = false;
	tilesRevealed = 0;
}

void Board::HandleLeftClick(sf::RenderWindow& window) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i position = sf::Mouse::getPosition(window);
		if (position.y >= 0 && position.y < (this->rows * 32) && !isGameOver) {
			if (position.x >= 0 && position.x <= (this->cols * 32)) {
				int x = (position.x / 32);
				int y = (position.y / 32);

				int location = (y * this->cols) + x;
				int count = 0;
				if (tiles[location].FindAdjacentTiles(count)) {
					isGameLost = true;
					isGameOver = true;
					int temp = 0;
					for (int i = 0; i < (this->rows * this->cols); i++) {
						if (tiles[i].isFlagged && tiles[i].isMine)
							tiles[i].clickFlag();
						if (tiles[i].isMine)
							tiles[i].FindAdjacentTiles(temp);
					}
				}
				else tilesRevealed += count;
			}
		}
		else if (position.y >= (this->rows * 32) && position.y < ((this->rows * 32) + 64)) {
			if (face.getGlobalBounds().contains(position.x, position.y)) {
				PlaceMines();
				UpdateCounter();
			}

			if (debug.getGlobalBounds().contains(position.x, position.y) && !isGameOver) {
				Debug();
			}

			if (test_1.getGlobalBounds().contains(position.x, position.y)) {
				ShowTestMines(testOne, 1);
			}

			if (test_2.getGlobalBounds().contains(position.x, position.y)) {
				ShowTestMines(testTwo, 2);
			}

			if (test_3.getGlobalBounds().contains(position.x, position.y)) {
				ShowTestMines(testThree, 3);
			}
		}
	}
	// Victory conditions
	if (tilesRevealed == ((this->rows * this->cols) - numTestMines) && !isGameLost) {
		isGameOver = true;
		for (int i = 0; i < (this->rows * this->cols); i++) {
			if (tiles[i].isMine && !tiles[i].isFlagged) {
				tiles[i].clickFlag();
				tiles[i].isRevealed = true;
				tiles[i].DrawTileWin(textures, window);
			}
		}
		ResetCounterWin();
	}
	UpdateFace();

	DrawBoard(window);
}

void Board::HandleRightClick(sf::RenderWindow& window) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !isGameOver) {
		sf::Vector2i position = sf::Mouse::getPosition(window);
		if (position.x >= 0 && position.x <= (this->cols * 32)) {
			if (position.y >= 0 && position.y <= (this->rows * 32)) {
				int x = (position.x / 32);
				int y = (position.y / 32);
				int location = (y * this->cols) + x;
				if (!tiles[location].isFlagged && !tiles[location].isRevealed) {
					tiles[location].clickFlag();
					numCounter--;
				}
				else if (!tiles[location].isRevealed && tiles[location].isFlagged) {
					tiles[location].clickFlag();
					numCounter++;
				}
				UpdateCounter();
			}
		}
	}
}

void Board::DrawBoard(sf::RenderWindow& window) {
	for (int i = 0; i < (this->rows * this->cols); i++) {
		tiles[i].DrawTile(window);
	}
	window.draw(face);
	window.draw(debug);
	window.draw(test_1);
	window.draw(test_2);
	window.draw(test_3);
	window.draw(negative);
	window.draw(digit1);
	window.draw(digit2);
	window.draw(digit3);

}
