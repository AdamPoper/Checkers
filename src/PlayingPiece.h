#pragma once
#include <SFML/Graphics.hpp>
//#include "Player.h"
class PlayingPiece
{
public:
	// default
	PlayingPiece();
	// init
	PlayingPiece(sf::Texture*, float, sf::Vector2f, unsigned int, sf::Texture*);
	// copy 
	//PlayingPiece(const PlayingPiece&);
	// move a piece
	void move(const sf::Vector2f);
	// set a position
	void setPosition(sf::Vector2f);
	// draw call
	void draw(sf::RenderWindow&);
	// where is it
	const sf::Vector2f& location();
	// give it a color
	void setColor(sf::Color);
	// what color is it
	int whatColor();
	// give it a radius
	void setRadius(int);
	// get the position
	const sf::Vector2f& getPosition();
	// is this piece kinged?
	bool isKinged();
	// make it a king
	void toKing();
	// set the identifier
	void setID(unsigned int);
	// set the owner
	void setMaster(void*);
	// who's is it
	void* getMaster() const;
private:
	void* master;
	sf::CircleShape piece;
	sf::Texture* king_texture;
	float radius;
	bool kinged = false;
	unsigned int identifier;
	bool justJumpedOver = false;
};
