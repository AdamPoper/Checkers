#include "Player.h"
//#include "PlayingPiece.h"
Player::Player(sf::Texture* textptr, unsigned int id, sf::Texture* kingTexture, const char* name)
{
	for (int i = 0; i < 12; i++)
	{
		PlayingPiece temp_piece(textptr, 50.0f, sf::Vector2f(0.0f, 0.0f), id, kingTexture);
		pieces.enQueue(temp_piece);
		num_pieces++;
	}
	this->name = name;
}
void Player::drawPieces(sf::RenderWindow& window)
{
	
	for(int i = 0; i < pieces.Count(); i++)
	{
		pieces[i].draw(window);
	}
	/*
	for (int i = 0; i < pieces.size(); i++)
	{
		pieces[i].draw(window);
	}
	*/
}
PlayingPiece& Player::getPiece(int index)
{
	return (pieces[index]);
}
int Player::piecesLeft()
{
	return num_pieces;
}
void Player::removePiece(int index)
{
	pieces.remove(index);
	num_pieces--;
}
void Player::removePiece(PlayingPiece* doomed)
{
	for (int i = 0; i < num_pieces; i++)
		if (doomed == &pieces[i])
			pieces.remove(i);
	num_pieces--;
}
std::string& Player::getName()
{
	return name;
}
