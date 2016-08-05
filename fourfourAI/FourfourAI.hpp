//
//  FourfourAI.hpp
//  fourfourAI
//
//  Created by William Wold on 8/4/16.
//  Copyright © 2016 Widap. All rights reserved.
//

#ifndef FourfourAI_hpp
#define FourfourAI_hpp

#include <stdio.h>

#include "Game.hpp"

struct BoardStats
{
	//U=unbound, B=bound
	
	bool win;
	int twoU, twoB;
	int threeU, threeB;
	
	std::string toString();
};

struct Move
{
	double score;
	int x, y;
};

struct AISettings
{
	double constWeight, twoBWeight, twoUWeight, threeBWeight, threeUWeight;
	
	int iters, recursPerIter;
};

class FourfourAI
{
public:
	FourfourAI(Game * gameIn, Player side);
	
	void move();
	
	Player getPlayer() {return side;}
	
	AISettings settings;
	
private:
	
	void copyData();
	double getBestScore(Player plr, int iter, bool playIt);
	double scoreBoard(Player plr);
	double getScoreFromStats(BoardStats in);
	BoardStats getStatsForPlayer(Player in);
	bool checkIfMoveValid(int x, int y);
	
	static const int WIN_SCORE=10000;
	
	std::string toString();
	
	Player getOther(Player in) {return in==WHITE?BLACK:WHITE;}
	
	Player data[10][10];
	
	Game * game;
	Player side;
};

#endif /* FourfourAI_hpp */
