//
//  FourfourAI.cpp
//  fourfourAI
//
//  Created by William Wold on 8/4/16.
//  Copyright © 2016 Widap. All rights reserved.
//

#include "FourfourAI.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

bool sortFunc (Move i, Move j) { return (i.score>j.score); }

FourfourAI::FourfourAI(Game * gameIn, Player sideIn)
{
	game=gameIn;
	side=sideIn;
	
	settings.constWeight=1.0;
	settings.twoBWeight=1.0;
	settings.twoUWeight=2.0;
	settings.threeBWeight=3.0;
	settings.threeUWeight=5.0;
	settings.recursPerIter=6;
	settings.iters=3;
}

void FourfourAI::move()
{
	//std::cout << "\n\n\n\n\n======================================================\n\n\n\n\n";
	
	if (game->getPlayer()!=side)
	{
		game->error("AI called with wrong player");
		return;
	}
	
	copyData();
	double score=getBestScore(side, settings.iters, true);
	
	std::cout << "\n\n\n" << Game::nameOfPlayer(side) << "'s score is " << score*100 << "%\n";
	
	//std::cout << "\n\n\n\n\n======================================================\n\n\n\n\n";
}

void FourfourAI::copyData()
{
	for (int y=0; y<10; ++y)
	{
		for (int x=0; x<10; ++x)
		{
			data[x][y]=game->getSquare(x, y);
		}
	}
}

double FourfourAI::getBestScore(Player plr, int iter, bool playIt)
{
	double bestScore=-1;
	int bestX=0, bestY=0;
	std::vector<Move> moves;
	Move move;
	
	moves.reserve(50);
	
	for (int y=0; y<10; ++y)
	{
		for (int x=0; x<10; ++x)
		{
			if (checkIfMoveValid(x, y))
			{
				data[x][y]=plr;
				
				move.x=x;
				move.y=y;
				move.score=scoreBoard(plr);
				
				moves.push_back(move);
				
				data[x][y]=NO_PLAYER;
			}
		}
	}
	
	std::sort(moves.begin(), moves.end(), sortFunc);
	
	int size=(int)moves.size()-1;
	
	if (iter<=0)
	{
		bestScore=moves[0].score;
		bestX=moves[0].x;
		bestY=moves[0].y;
	}
	else if (moves[0].score>0.98)
	{
		bestScore=moves[0].score;
		bestX=moves[0].x;
		bestY=moves[0].y;
	}
	else
	{
		for (int i=0; i<settings.recursPerIter && i<=size && (i==0 || moves[i].score>0.02); ++i)
		{
			int x=moves[i].x;
			int y=moves[i].y;
			
			data[x][y]=plr;
			
			double score=1-getBestScore(getOther(plr), iter-1, false);
			
			data[x][y]=NO_PLAYER;
			
			if (score>bestScore)
			{
				bestScore=score;
				bestX=x;
				bestY=y;
			}
		}
	}
	
	if (playIt)
	{
		if (!game->move(bestX, bestY))
		{
			std::cout << "AI fucked up" << std::endl;
		}
	}
	
	return bestScore;
}

bool FourfourAI::checkIfMoveValid(int xIn, int yIn)
{
	int xa[]={1, 0, -1, 0};
	int ya[]={0, 1, 0, -1};
	
	if (!isIn(xIn, yIn))
	{
		return false;
	}
	
	if (!notInMiddle(xIn, yIn))
	{
		return false;
	}
	
	if (data[xIn][yIn]!=NO_PLAYER)
	{
		return false;
	}
	
	for (int i=0; i<4; ++i)
	{
		int x=xIn;
		int y=yIn;
		
		if ((isIn(x-xa[i], y-ya[i]) && data[x-xa[i]][y-ya[i]]!=NO_PLAYER) || !notInMiddle(x-xa[i], y-ya[i]))
		{
			while (true)
			{
				x+=xa[i];
				y+=ya[i];
				
				if (!isIn(x, y))
				{
					return true;
				}
				
				if (!notInMiddle(x, y))
				{
					break;
				}
				
				if (data[x][y]!=NO_PLAYER)
				{
					break;
				}
			}
		}
	}
	
	return false;
}

double FourfourAI::scoreBoard(Player plr)
{
	BoardStats meStats=getStatsForPlayer(plr);
	BoardStats otherStats=getStatsForPlayer(getOther(plr));
	
	double me=getScoreFromStats(meStats);
	double other=getScoreFromStats(otherStats);
	double score;
	
	score=me/(me+other);
	
	//std::cout << "me:\n" << meStats.toString() << "score: " << me << "\n\n" << "other:\n" << otherStats.toString() << "score: " << other << "\n\n" << toString() << "composit score: " << score << "\n\n\n\n";
	
	return score;
}

BoardStats FourfourAI::getStatsForPlayer(Player plr)
{
	BoardStats out;
	
	out.win=false;
	out.twoB=0;
	out.twoU=0;
	out.threeB=0;
	out.threeU=0;
	
	int xa[]={ 1, 1, 1, 0};
	int ya[]={-1, 0, 1, 1};
	
	for (int y=0; y<10; ++y)
	{
		for (int x=0; x<10; ++x)
		{
			if (data[x][y]==plr)
			{
				for (int i=0; i<4; ++i)
				{
					int x0=x;
					int y0=y;
					
					if (!isIn(x0-xa[i], y0-ya[i]) || data[x0-xa[i]][y0-ya[i]]!=plr)
					{
						bool startsBound;
						bool endsBound;
						int count=0;
						
						if (isIn(x0-xa[i], y0-ya[i]) && notInMiddle(x0-xa[i], y0-ya[i]) && data[x0-xa[i]][y0-ya[i]]==NO_PLAYER)
						{
							startsBound=false;
						}
						else
						{
							startsBound=true;
						}
						
						while(true)
						{
							if (!isIn(x0, y0) || !notInMiddle(x0, y0))
							{
								endsBound=true;
								break;
							}
							else if (data[x0][y0]==NO_PLAYER)
							{
								endsBound=false;
								break;
							}
							else if (data[x0][y0]==plr)
							{
								/*if (plr==side)
									std::cout << x0 << ", " << y0 << std::endl;*/
								++count;
								x0+=xa[i];
								y0+=ya[i];
							}
							else
							{
								endsBound=true;
								break;
							}
						}
						
						if (count>=4)
						{
							out.win=true;
							return out;
						}
						else if (!startsBound || !endsBound)
						{
							if (count==2)
							{
								if (startsBound || endsBound)
								{
									++out.twoB;
								}
								else
								{
									++out.twoU;
								}
							}
							else if (count==3)
							{
								if (startsBound || endsBound)
								{
									++out.threeB;
								}
								else
								{
									++out.threeU;
								}
							}
						}
					}
				}
			}
		}
	}
	
	return out;
}

double FourfourAI::getScoreFromStats(BoardStats in)
{
	if (in.win)
		return WIN_SCORE;
	
	return settings.constWeight+in.twoB*settings.twoBWeight+in.twoU*settings.twoUWeight+in.threeB*settings.threeBWeight+in.threeU*settings.threeUWeight;
}


std::string FourfourAI::toString()
{
	std::string out;
	
	out+="  0 1 2 3 4 5 6 7 8 9\n";
	
	for (int y=9; y>=0; --y)
	{
		out+=(char)(y+'0');
		out+=" ";
		
		for (int x=0; x<10; ++x)
		{
			Player plr=data[x][y];
			
			if ((x==4 || x==5) && (y==4 || y==5))
				out+="#";
			else if (plr==NO_PLAYER)
				out+=" ";
			else if (plr==WHITE)
				out+="O";
			else if (plr==BLACK)
				out+="X";
			else
				out+="err";
			
			if (x<9)
				out+="|";
		}
		
		if (y>0)
			out+="\n  -+-+-+-+-+-+-+-+-+-\n";
	}
	
	out+="\n";
	
	return out;
}

std::string BoardStats::toString()
{
	std::string out;
	
	if (win)
		out+="Is winner!\n";
	
	out+="two bound: ";
	out+=std::to_string(twoB);
	out+="\n";
	
	out+="two unbound: ";
	out+=std::to_string(twoU);
	out+="\n";
	
	out+="three bound: ";
	out+=std::to_string(threeB);
	out+="\n";
	
	out+="three unbound: ";
	out+=std::to_string(threeU);
	out+="\n";
	
	return out;
}
