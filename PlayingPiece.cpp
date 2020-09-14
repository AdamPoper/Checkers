#include "PlayingPiece.h"
//#include "Player.h"
PlayingPiece::PlayingPiece()
{
	radius = 0;
}

PlayingPiece::PlayingPiece(sf::Texture* textptr, float size, sf::Vector2f pos, unsigned int id, sf::Texture* kingTexture)
{
	radius = size;
	piece.setRadius(radius);
	piece.setOrigin(sf::Vector2f(0.0f, 0.0f));
	piece.setTexture(textptr);
	piece.setPosition(pos);
	identifier = id;
	this->king_texture = kingTexture;
}
void PlayingPiece::draw(sf::RenderWindow& window)
{
	window.draw(piece);
}
const sf::Vector2f& PlayingPiece::location()
{
	return piece.getPosition();
}
void PlayingPiece::setPosition(sf::Vector2f pos)
{
	piece.setPosition(pos);
}
void PlayingPiece::setColor(sf::Color c)
{
	piece.setFillColor(c);
}
void PlayingPiece::setRadius(int R)
{
	piece.setRadius(R);
}
const sf::Vector2f& PlayingPiece::getPosition()
{
	return (piece.getPosition());
}
void PlayingPiece::move(const sf::Vector2f pos)
{
	piece.move(pos);
}

bool PlayingPiece::isKinged()
{
	return kinged;
}
void PlayingPiece::toKing()
{
	kinged = true;
	piece.setTexture(king_texture);
}
int PlayingPiece::whatColor()
{
	return identifier;
}
void PlayingPiece::setID(unsigned int id)
{
	identifier = id;
}
void PlayingPiece::setMaster(void* ptr)
{
	master = ptr;
}
void* PlayingPiece::getMaster() const
{
	return master;
}

