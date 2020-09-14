#pragma once
#include <QueueLinkedList.h>
#include <SFML/Graphics.hpp>
#include "PlayingPiece.h"
class Player
{
public:
	Player(sf::Texture*, unsigned int, sf::Texture*, const char*);
	PlayingPiece& getPiece(int);

	void drawPieces(sf::RenderWindow&);
	int piecesLeft();
	void removePiece(int);
	void removePiece(PlayingPiece*);
	std::string& getName();
private:
	ap::QueueLinkedList<PlayingPiece> pieces;
	//std::vector<PlayingPiece> pieces;
	int num_pieces = 0;
	std::string name;
};
