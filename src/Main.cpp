#include <SFML/Graphics.hpp>
#include <iostream>
//#include "PlayingPiece.h"
#include <QueueLinkedList.h>
#include "Player.h"
#include <string>
static const int BOARD_SIZE = 800;

enum ColorState
{
	Black, Red
};
struct Tile
{
	sf::RectangleShape rect;
	PlayingPiece* piece_ptr = nullptr;
	int removeIndex = 0;
};
void initBoard(const int* board, std::vector<Tile>& tiles, int width, int height, sf::Vector2f& t_size)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Tile tile;
			if (board[x + y * width] == ColorState::Black)
			{
				tile.rect.setSize(t_size);
				tile.rect.setFillColor(sf::Color::Black);
				tile.rect.setPosition(t_size.x * (float)x, t_size.y * (float)y);
				//tile.setOutlineColor(sf::Color::Green);
				tiles.emplace_back(tile);
			}
			if (board[x + y * width] == ColorState::Red)
			{
				tile.rect.setSize(t_size);
				tile.rect.setFillColor(sf::Color::Red);
				tile.rect.setPosition(t_size.x * (float)x, t_size.y * (float)y);
				tile.rect.setOutlineColor(sf::Color::Green);
				tiles.emplace_back(tile);
			}
		}
	}
}
void checkToKing(PlayingPiece& piece)
{
	sf::Vector2f   redKingPos[] =       { {100.0f, 0.0f}, {300.0f, 0.0f}, {500.0f, 0.0f}, {700.0f, 0.0f} };
	sf::Vector2f blackKingPos[] = { {0.0f, 700.0f}, {200.0f, 700.0f}, {400.0f, 700.0f}, {600.0f, 700.0f} };
	switch (piece.whatColor())
	{
	case ColorState::Red:
		for (int i = 0; i < 4; i++)
			if (piece.getPosition() == redKingPos[i])
				piece.toKing();
		break;
	case ColorState::Black:
		for (int i = 0; i < 4; i++)
			if (piece.getPosition() == blackKingPos[i])
				piece.toKing();
		break;
	default:
		std::cout << "No color" << std::endl;
	}
}
void initPlayerPieces(const int* coords, int start, int end, Player& player, std::vector<Tile>& tiles, ColorState id)
{
	int p_index = 0;
	for (int i = start; i <= end; i++)
	{
		if (coords[i] == ColorState::Red)
		{
			//std::cout << p_index << std::endl;
			sf::Vector2f pos = { (tiles[i].rect.getPosition().x), (tiles[i].rect.getPosition().y) };
			player.getPiece(p_index).setPosition(pos);
			player.getPiece(p_index).setID(id);
			player.getPiece(p_index).setMaster(&player);
			tiles[i].piece_ptr = &player.getPiece(p_index);
			p_index++;
		}
	}
}

unsigned int selectTile(const sf::Vector2i& mousPos)
{
	//std::cout << "Mouse.x: " << mousPos.x << " Mouse.y: " << mousPos.y << std::endl;
	const float raw_height    = 800.0f;
	const float raw_width     = 800.0f;
	const int tile_height     =      8;
	const int tile_width      =      8;

	int xpos = (mousPos.x / raw_width) * tile_width;
	//std::cout << "xpos: " << xpos << std::endl;
	int ypos = (mousPos.y / raw_height) * tile_height;
	//std::cout << "ypos: " << ypos << std::endl;
	
	unsigned int index = (ypos * 8) + (xpos);
	return index;
}
struct playerPiecePair
{
	Player* p = nullptr;
	int index = 0;
};
playerPiecePair pieceSelected(const sf::Vector2f t_pos, Player& P1, Player& P2)
{
	for (int i = 0; i < P1.piecesLeft(); i++)
		if (P1.getPiece(i).getPosition() == t_pos)
			return { &P1, i };
	for (int i = 0; i < P2.piecesLeft(); i++)
		if (P2.getPiece(i).getPosition() == t_pos)
			return { &P2, i };
	return { nullptr, 0 };
}
struct RecursionLimit // bandaid fix for double checking issue
{
	int RecCheck0 = 0;
	int RecCheck1 = 0;
	int RecCheck2 = 0;
	int RecCheck3 = 0;
};
void checkJumpPossibliitiesKings(std::vector<Tile>& allTiles, int posIndex, int blocked_index, sf::Vector2f* posPositions, int selectedTile, bool& jumpFound, RecursionLimit& limits)
{
	switch (posIndex)
	{
		case 0: 
			if (allTiles[selectedTile].piece_ptr->whatColor() != allTiles[blocked_index].piece_ptr->whatColor())
			{
				jumpFound = true;
				posPositions[0].x -= 100.0f;
				posPositions[0].y -= 100.0f;
			}
			break;
		case 1:
			if (allTiles[selectedTile].piece_ptr->whatColor() != allTiles[blocked_index].piece_ptr->whatColor())
			{
				jumpFound = true;
				posPositions[1].x += 100.0f;
				posPositions[1].y -= 100.0f;
			}
			break;
		case 2:
			if (allTiles[selectedTile].piece_ptr->whatColor() != allTiles[blocked_index].piece_ptr->whatColor()
				&& limits.RecCheck2 < 1)
			{
				jumpFound = true;
				limits.RecCheck2++;
				posPositions[2].x += 100.0f;
				posPositions[2].y += 100.0f;
			}
			break;
		case 3:
			if (allTiles[selectedTile].piece_ptr->whatColor() != allTiles[blocked_index].piece_ptr->whatColor()
				&& limits.RecCheck3 < 1)
			{
				jumpFound = true;
				limits.RecCheck3++;
				posPositions[3].x -= 100.0f;
				posPositions[3].y += 100.0f;
			}
	}
}
void checkJumpPossibilities(std::vector<Tile>& allTiles, int posIndex, int blocked_index, sf::Vector2f* posPos, int selectedTile, bool& jumpFound, RecursionLimit& limits)
{
	switch (posIndex)
	{
		case 1: 
			if (allTiles[selectedTile].piece_ptr->whatColor() == ColorState::Red && allTiles[blocked_index].piece_ptr->whatColor() == ColorState::Black)
			{
				jumpFound = true;
				
				allTiles[selectedTile].removeIndex = 1;
				posPos[1].x += 100.0f;
				posPos[1].y -= 100.0f;
			}
			if (allTiles[selectedTile].piece_ptr->whatColor() == ColorState::Black 
				&& allTiles[blocked_index].piece_ptr->whatColor() == ColorState::Red
				&& limits.RecCheck1 < 1)
			{
				
				allTiles[selectedTile].removeIndex = 1;
				limits.RecCheck1++;
				jumpFound = true;
			
				posPos[1].x += 100.0f;
				posPos[1].y += 100.0f; 
			}
			break;
		case 0:
			if (allTiles[selectedTile].piece_ptr->whatColor() == ColorState::Red && allTiles[blocked_index].piece_ptr->whatColor() == ColorState::Black)
			{
				jumpFound = true;
				
				allTiles[selectedTile].removeIndex = 0;
				posPos[0].x -= 100.0f;
				posPos[0].y -= 100.0f;
			}
			if (allTiles[selectedTile].piece_ptr->whatColor() == ColorState::Black 
				&& allTiles[blocked_index].piece_ptr->whatColor() == ColorState::Red
				&& limits.RecCheck0 < 1)
			{
				limits.RecCheck0++;
				jumpFound = true;
				
				allTiles[selectedTile].removeIndex = 0;
				posPos[0].x -= 100.0f;
				posPos[0].y += 100.0f;
			}
	}
}
void findPosTiles(std::vector<Tile>& allTiles, std::vector<Tile*>& posTiles, int selectedTile)
{
	RecursionLimit limits; // idk why this works but it won't without it
	bool jumpFound = false;
	int changed_index;
	posTiles.reserve(2);
	sf::Vector2f posPostions[2];
	
	// if the piece is red calculate 2 possible postions the piece could be moved to
	if (allTiles[selectedTile].piece_ptr->whatColor() == ColorState::Red)
	{
		// 0 is to the left and 1 is to the right
		posPostions[0].y = allTiles[selectedTile].rect.getPosition().y - 100.0f;
		posPostions[1].y = allTiles[selectedTile].rect.getPosition().y - 100.0f;
		posPostions[0].x = allTiles[selectedTile].rect.getPosition().x - 100.0f;
		posPostions[1].x = allTiles[selectedTile].rect.getPosition().x + 100.0f;
	}
	// if the piece is black calculate 2 possible positions the piece could be moved to
	if (allTiles[selectedTile].piece_ptr->whatColor() == ColorState::Black)
	{
		// 0 is to the left and 1 is to the right
		posPostions[0].y = allTiles[selectedTile].rect.getPosition().y + 100.0f;
		posPostions[1].y = allTiles[selectedTile].rect.getPosition().y + 100.0f;
		posPostions[0].x = allTiles[selectedTile].rect.getPosition().x - 100.0f;
		posPostions[1].x = allTiles[selectedTile].rect.getPosition().x + 100.0f;
	}
	// loop through only the tiles that are empty
	for (int i = 0; i < allTiles.size(); i++)
	{
		if (allTiles[i].piece_ptr == nullptr)			
			for (int j = 0; j < 2; j++)				
				if (allTiles[i].rect.getPosition() == posPostions[j])
				{
					posTiles.emplace_back(&allTiles[i]);
					allTiles[i].rect.setOutlineColor(sf::Color::Yellow);
					allTiles[i].rect.setOutlineThickness(5.0f);
				}
				
			
		if (allTiles[i].piece_ptr != nullptr)			
			for (int j = 0; j < 2; j++)				
				if (allTiles[i].rect.getPosition() == posPostions[j])					
					checkJumpPossibilities(allTiles, j, i, posPostions, selectedTile, jumpFound, limits);
	}
	if (jumpFound)
		for (int i = 0; i < allTiles.size(); i++)
			if(allTiles[i].piece_ptr == nullptr)
				for (int j = 0; j < 2; j++)
					if (allTiles[i].rect.getPosition() == posPostions[j])
					{
						posTiles.emplace_back(&allTiles[i]);
						allTiles[i].rect.setOutlineColor(sf::Color::Yellow);
						allTiles[i].rect.setOutlineThickness(5.0f);
					}
}
void findPosTilesKingCase(std::vector<Tile>& allTiles, std::vector<Tile*>& posTiles, int selectedTile)
{
	posTiles.reserve(4);
	sf::Vector2f posPositions[4];
	bool jumpFound = false;
	RecursionLimit limits;
	posPositions[0].x = allTiles[selectedTile].rect.getPosition().x - 100.0f;
	posPositions[0].y = allTiles[selectedTile].rect.getPosition().y - 100.0f;
	posPositions[1].x = allTiles[selectedTile].rect.getPosition().x + 100.0f;
	posPositions[1].y = allTiles[selectedTile].rect.getPosition().y - 100.0f;
	posPositions[2].x = allTiles[selectedTile].rect.getPosition().x + 100.0f;
	posPositions[2].y = allTiles[selectedTile].rect.getPosition().y + 100.0f;
	posPositions[3].x = allTiles[selectedTile].rect.getPosition().x - 100.0f;
	posPositions[3].y = allTiles[selectedTile].rect.getPosition().y + 100.0f;

	for (int i = 0; i < allTiles.size(); i++)
	{
		if (allTiles[i].piece_ptr == nullptr)
		{
			for (int j = 0; j < 4; j++)
				if (allTiles[i].rect.getPosition() == posPositions[j])
				{
					posTiles.emplace_back(&allTiles[i]);
					allTiles[i].rect.setOutlineColor(sf::Color::Yellow);
					allTiles[i].rect.setOutlineThickness(5.0f);
				}
		} 
		else if (allTiles[i].piece_ptr != nullptr)
		{
			for (int j = 0; j < 4; j++)				
				if (allTiles[i].rect.getPosition() == posPositions[j])					
					checkJumpPossibliitiesKings(allTiles, j, i, posPositions, selectedTile, jumpFound, limits);
		}
	}
	if(jumpFound)
		for(int i = 0; i < allTiles.size(); i++)
			if(allTiles[i].piece_ptr == nullptr)
				for(int j = 0; j < 4; j++)
					if (allTiles[i].rect.getPosition() == posPositions[j])
					{
						posTiles.emplace_back(&allTiles[i]);
						allTiles[i].rect.setOutlineColor(sf::Color::Yellow);
						allTiles[i].rect.setOutlineThickness(5.0f);
					}
}
void listenForStart(bool& gameStart)
{
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		gameStart = true; 
}
void findNextToSelected(std::vector<Tile>& tiles, Tile*& nextToSelected, sf::Vector2f& moveOverPos)
{
	for (int i = 0; i < tiles.size(); i++)
		if (tiles[i].rect.getPosition() == moveOverPos)
			nextToSelected = &tiles[i];			
}
void drawStart(sf::RenderWindow& window)
{
	sf::Text t;
	sf::Text startInstructions;
	sf::Font font;
	std::string startText = "           The rules are simple. When it is your turn \n"
			"     press the left mouse button to select one of your\n"
			"pieces and choose among the possible tiles highlighted\n"
			"    in yellow by right clicking. Player 1 is black, so they\n"
			"go first. You may only select a piece when it is your turn.\n"
			"    Please press the spacebar to continue to the game." ;
	font.loadFromFile("Arial.ttf");
	t.setString("Welcome");
	t.setCharacterSize(90);
	t.setStyle(sf::Text::Bold);
	t.setFont(font);
	t.setFillColor(sf::Color::Black);
	t.setPosition(sf::Vector2f(200.0f, 100.0f));
	startInstructions.setFont(font);
	startInstructions.setString(startText);
	startInstructions.setFillColor(sf::Color::Black);
	startInstructions.setCharacterSize(30);
	startInstructions.setPosition(sf::Vector2f(30.0f, 300.0f));
	sf::RectangleShape startDisplay;
	startDisplay.setSize(sf::Vector2f(800.0f, 800.0f));
	sf::Uint32 red = 255;
	sf::Uint32 blue = 255;
	sf::Uint32 green = 255;
	sf::Uint32 alpha = 192;
	startDisplay.setFillColor(sf::Color::Color(red, blue, green, alpha));
	window.draw(startDisplay);
	window.draw(t);
	window.draw(startInstructions);
}
void endGame(sf::RenderWindow& window, Player& winner)
{
	sf::RectangleShape endDisplay;
	endDisplay.setSize(sf::Vector2f(800.0f, 800.0f));
	sf::Uint32 red   = 255;
	sf::Uint32 blue  = 255;
	sf::Uint32 green = 255;
	sf::Uint32 alpha = 192;
	endDisplay.setFillColor(sf::Color::Color(red, blue, green, alpha));
	sf::Font font;
	font.loadFromFile("Arial.ttf");
	sf::Text endMessage;
	std::string gameOver = "  Game Over, \n";
	std::string wins = "\n      Wins!";
	std::string endMessageText = gameOver + winner.getName() + wins;
	endMessage.setString(endMessageText);
	endMessage.setStyle(sf::Text::Bold);
	endMessage.setCharacterSize(70);
	endMessage.setFont(font);
	endMessage.setPosition(sf::Vector2f(190.0f, 100.0f));
	endMessage.setFillColor(sf::Color::Black);
	window.draw(endDisplay);
	window.draw(endMessage);
}

bool checkValid(std::vector<Tile*>& posTiles, std::vector<Tile>& allTiles, int index)
{
	for (const auto& T : posTiles)
		if (T == &allTiles[index])
			return true;
	return false;
}
int main()
{
	std::string title = "checkers | ";
	bool P1TURN = true;
	sf::RenderWindow window(sf::VideoMode(BOARD_SIZE, BOARD_SIZE), title + "Black Player 1's Turn");
	bool gameStart = false;
	bool gameEnd = false;
	sf::Vector2f tileSize = { 100, 100 };
	std::vector<Tile> tiles;
	sf::Texture black_text;
	black_text.loadFromFile("general_black_piece.png");
	sf::Texture red_text;
	red_text.loadFromFile("general_red_piece.png");
	sf::Texture BlackkingTexture;
	BlackkingTexture.loadFromFile("king_black_piece.png");
	sf::Texture redKingTexture;
	redKingTexture.loadFromFile("king_red_piece.png");
	tiles.reserve(64);
	Player player1(&black_text, ColorState::Black, &BlackkingTexture, "Player 1 Black");
	Player player2(&red_text, ColorState::Red, &redKingTexture, "Player 2 Red");
	sf::Clock gameClock;
	const int board[64] =
	{
		0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0,
		0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0,
		0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0,
		0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0,
	};
	initBoard(board, tiles, 8, 8, tileSize);
	initPlayerPieces(board, 0, 23, player1, tiles, ColorState::Black);
	initPlayerPieces(board, 40, 63, player2, tiles, ColorState::Red);
	int indexSelected = 0;
	playerPiecePair selectedPair;
	bool is_pieceSelected = false;
	std::vector<Tile*> possibleTiles;
	listenForStart(gameStart);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		listenForStart(gameStart);
		// on left click to selected a tile/playing piece
		if (gameStart && !gameEnd && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && gameClock.getElapsedTime().asSeconds() > 0.50f)
		{
			gameClock.restart();
			for (int i = 0; i < possibleTiles.size(); i++)
			{
				possibleTiles[i]->rect.setOutlineThickness(0.0f);
				possibleTiles[i]->rect.setOutlineColor(sf::Color::Green);
			}
			possibleTiles.clear();
			int lastIndexSelected = indexSelected;
			tiles[lastIndexSelected].rect.setOutlineThickness(0.0f);
			
			indexSelected = selectTile(sf::Mouse::getPosition(window));
			
			if (tiles[indexSelected].rect.getFillColor() == sf::Color::Red 
				&& tiles[indexSelected].piece_ptr != nullptr 
				&& ((tiles[indexSelected].piece_ptr->whatColor() == ColorState::Black && P1TURN)
				|| ( tiles[indexSelected].piece_ptr->whatColor() == ColorState::Red && !P1TURN)))
			{
				selectedPair = pieceSelected(tiles[indexSelected].rect.getPosition(), player1, player2);
				if (selectedPair.p != nullptr)
				{
					if (tiles[indexSelected].piece_ptr->isKinged())
						findPosTilesKingCase(tiles, possibleTiles, indexSelected);
					else if(!tiles[indexSelected].piece_ptr->isKinged())
						findPosTiles(tiles, possibleTiles, indexSelected);
					
					is_pieceSelected = true;
					tiles[indexSelected].rect.setOutlineThickness(5.0f);
					tiles[indexSelected].piece_ptr = &(selectedPair.p->getPiece(selectedPair.index));
				}
				else is_pieceSelected = false;
			}
		}
		// on right click
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && gameClock.getElapsedTime().asSeconds() > 0.25f)
		{
			gameClock.restart();
			unsigned int nextSelected = selectTile(sf::Mouse::getPosition(window));
			if (tiles[nextSelected].rect.getFillColor() == sf::Color::Red && is_pieceSelected
				&& tiles[nextSelected].piece_ptr == nullptr && checkValid(possibleTiles, tiles, nextSelected))
			{
				switch (tiles[indexSelected].piece_ptr->whatColor())
				{
				case ColorState::Red:
					P1TURN = true;
					title = "Checkers | Black Player 1's Turn";
					window.setTitle(title);
					break;
				case ColorState::Black:
					P1TURN = false;
					title = "Checkers | Red Player 2's Turn";
					window.setTitle(title);
					break;
				default:
					std::cout << "Nobody's turn error" << std::endl;
				}
				float yOffset = tiles[nextSelected].rect.getPosition().y - selectedPair.p->getPiece(selectedPair.index).getPosition().y;
				float xOffset = tiles[nextSelected].rect.getPosition().x - selectedPair.p->getPiece(selectedPair.index).getPosition().x;
				int currentPieceColor = selectedPair.p->getPiece(selectedPair.index).whatColor();
				Tile* nextToSelected = nullptr;
				sf::Vector2f updateMove;
				sf::Vector2f moveOverPos;
				if (yOffset < 0 && xOffset > 0)
				{
					updateMove = sf::Vector2f(5.0f, -5.0f);
					moveOverPos = sf::Vector2f(tiles[indexSelected].rect.getPosition().x + 100, tiles[indexSelected].rect.getPosition().y - 100);
				}
				else if (yOffset < 0 && xOffset < 0)
				{
					updateMove = sf::Vector2f(-5.0f, -5.0f);
					moveOverPos = sf::Vector2f(tiles[indexSelected].rect.getPosition().x - 100, tiles[indexSelected].rect.getPosition().y - 100);
				}
				else if (yOffset > 0 && xOffset < 0)
				{
					updateMove = sf::Vector2f(-5.0f, 5.0f);
					moveOverPos = sf::Vector2f(tiles[indexSelected].rect.getPosition().x - 100, tiles[indexSelected].rect.getPosition().y + 100);
				}
				else if (yOffset > 0 && xOffset > 0)
				{
					updateMove = sf::Vector2f(5.0f, 5.0f);
					moveOverPos = sf::Vector2f(tiles[indexSelected].rect.getPosition().x + 100, tiles[indexSelected].rect.getPosition().y + 100);
				}
				findNextToSelected(tiles, nextToSelected, moveOverPos);
				while (selectedPair.p->getPiece(selectedPair.index).getPosition() != tiles[nextSelected].rect.getPosition())
				{
					selectedPair.p->getPiece(selectedPair.index).move(updateMove);
					//std::cout << "moved" << std::endl;
					//std::cout << nextToSelected << std::endl;
					if (nextToSelected->piece_ptr != nullptr && selectedPair.p->getPiece(selectedPair.index).getPosition() == nextToSelected->piece_ptr->getPosition())
					{
						//std::cout << "this shit's not empty" << std::endl;
						Player* player;
						player = (Player*)nextToSelected->piece_ptr->getMaster();
						player->removePiece(nextToSelected->piece_ptr);
						nextToSelected->piece_ptr = nullptr;
					}
				}
				for (const auto& T : possibleTiles)
					T->rect.setOutlineThickness(0.0f);

					tiles[indexSelected].rect.setOutlineThickness(0.0f);
					tiles[nextSelected].piece_ptr = tiles[indexSelected].piece_ptr;
					tiles[indexSelected].piece_ptr = nullptr;
					checkToKing(*(tiles[nextSelected].piece_ptr));
					is_pieceSelected = false;
			}
		}
		window.clear();
		for(int i = 0; i < tiles.size(); i++)
			window.draw(tiles[i].rect);

		if (is_pieceSelected)
		{
			window.draw(tiles[indexSelected].rect);
			for (const auto& T : possibleTiles)
				window.draw(T->rect);
			selectedPair.p->getPiece(selectedPair.index).draw(window); 
		}
		for (int i = 0; i < player1.piecesLeft(); i++)
			player1.getPiece(i).draw(window);

		for (int i = 0; i < player2.piecesLeft(); i++)
			player2.getPiece(i).draw(window);
		if (!gameStart)
			drawStart(window);
		if (player1.piecesLeft() == 0 || player2.piecesLeft() == 0)
		{
			gameEnd = true;
			player1.piecesLeft() == 0 ? endGame(window, player2) : endGame(window, player1);			
		}
			
		window.display();
	}
	return 0;
}
