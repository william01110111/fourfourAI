//
//  Game.hpp
//  fourfourAI
//
//  Created by William Wold on 8/4/16.
//  Copyright © 2016 Widap. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <string>

inline bool isIn(int x, int y)
{
	return x>=0 && x<10 && y>=0 && y<10;
}

inline bool notInMiddle(int x, int y)
{
	return (x!=4 && x!=5) || (y!=4 && y!=5);
}

enum Player
{
	NO_PLAYER=0,
	WHITE,
	BLACK
};

class Game
{
public:
	Game();
	bool move(int x, int y);
	
	static std::string nameOfPlayer(Player in);
	
	Player getPlayer() {return playerToMove;}
	Player getWinner() {return winner;}
	Player * getData() {return (Player *)data;}
	Player getSquare(int x, int y) {return data[x][y];}
	
	std::string toString();
	
	void error(std::string);
	
private:
	void checkWinner();
	bool chackIfMoveValid(int x, int y);
	
	Player playerToMove;
	Player winner;
	Player data[10][10];
};

#endif /* Game_hpp */
