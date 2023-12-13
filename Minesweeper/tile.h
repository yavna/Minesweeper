#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

class Tile {
private:
	sf::Sprite tileHidden;
	sf::Sprite tileRevealed;
	sf::Sprite tileState;
	sf::Sprite flag;
	bool isDebugClicked;

public:
	bool isMine;
	bool isRevealed;
	bool isFlagged;
	int numAdjacent;

	Tile();
	Tile(map<string, sf::Texture>& textures, int x, int y);
	vector<Tile*> adjacentTiles;
	bool FindAdjacentTiles(int& num);
	bool clickFlag();
	void SetMine(bool tempIsMine);
	void DrawTile(sf::RenderWindow& window);
	void RevealTile(map<string, sf::Texture>& textures);
	void SetDebug();
	void DrawTileWin(map<string, sf::Texture>& textures, sf::RenderWindow& window);
};