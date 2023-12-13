#include "tile.h"
#include "texture.h"
#include <vector>

Tile::Tile() {
	isMine = false;
	isRevealed = false;
	isFlagged = false;
	isDebugClicked = false;
}

Tile::Tile(map<string, sf::Texture>& textures, int x, int y) {
	tileRevealed.setTexture(textures["tileRevealed"]);
	tileHidden.setTexture(textures["tileHidden"]);
	flag.setTexture(textures["flag"]);

	tileRevealed.setPosition(x, y);
	tileHidden.setPosition(x, y);
	flag.setPosition(x, y);
	tileState.setPosition(x, y);

	flag.setColor(sf::Color::Transparent);
	isMine = false;
	isRevealed = false;
	isFlagged = false;
	isDebugClicked = false;
}

void Tile::RevealTile(map<string, sf::Texture>& textures) {
	numAdjacent = 0;
	for (int i = 0; i < 8; i++) {
		if (adjacentTiles[i] != nullptr && adjacentTiles[i]->isMine) {
			numAdjacent++;
		}
	}
	if (isMine) {
		tileState.setTexture(textures["mine"]);
	}
	else {
		switch (numAdjacent) {
		case 1:
			tileState.setTexture(textures["number_1"]);
			break;
		case 2:
			tileState.setTexture(textures["number_2"]);
			break;
		case 3:
			tileState.setTexture(textures["number_3"]);
			break;
		case 4:
			tileState.setTexture(textures["number_4"]);
			break;
		case 5:
			tileState.setTexture(textures["number_5"]);
			break;
		case 6:
			tileState.setTexture(textures["number_6"]);
			break;
		case 7:
			tileState.setTexture(textures["number_7"]);
			break;
		case 8:
			tileState.setTexture(textures["number_8"]);
			break;
		default:
			tileState.setTexture(textures["tileRevealed"]);
			break;
		}
	}
	tileState.setColor(sf::Color::Transparent);
	flag.setColor(sf::Color::Transparent);
	tileRevealed.setColor(sf::Color::Transparent);
	isRevealed = false;
	isFlagged = false;
	isDebugClicked = false;
}

void Tile::SetMine(bool tempIsMine) {
	isMine = tempIsMine;
}

bool Tile::clickFlag() {
	if (!isRevealed) {
		if (isFlagged) {
			isFlagged = false;
			flag.setColor(sf::Color::Transparent);
		}
		else {
			isFlagged = true;
			flag.setColor(sf::Color::White);
		}
	}
	return isFlagged;
}

void Tile::SetDebug() {
	if (isMine && !isRevealed){
		if (isDebugClicked) {
			isDebugClicked = false;
			tileState.setColor(sf::Color::Transparent);
		}
		else{
			isDebugClicked = true;
			tileState.setColor(sf::Color::White);
		}
	}
}


bool Tile::FindAdjacentTiles(int& num) {
	if (!isRevealed && !isFlagged) {
		isRevealed = true;
		num++;
		tileRevealed.setColor(sf::Color::White);
		tileState.setColor(sf::Color::White);
		if (numAdjacent == 0 && !isMine) {
			for (int i = 0; i < 8; i++) {
				if (adjacentTiles[i] != nullptr && !(adjacentTiles[i]->isMine)) {
					adjacentTiles[i]->FindAdjacentTiles(num);
				}
			}
		}
	}
	return isMine;
}

void Tile::DrawTile(sf::RenderWindow& window) {
	window.draw(tileHidden);
	window.draw(tileRevealed);
	window.draw(tileState);
	window.draw(flag);
}

void Tile::DrawTileWin(map<string, sf::Texture>& textures, sf::RenderWindow& window) {
	if (isRevealed && isMine) {
		tileState.setTexture(textures["tileRevealed"]);
		window.draw(tileState);
		window.draw(flag);
	}
	if (isFlagged) {
		window.draw(flag);
	}
}
