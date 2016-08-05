//
//  Game.cpp
//  fourfourAI
//
//  Created by William Wold on 8/4/16.
//  Copyright © 2016 Widap. All rights reserved.
//

#include "Game.hpp"
#include <iostream>

Game::Game()
{
	for (int i=0; i<10; ++i)
	{
		for (int j=0; j<10; ++j)
		{
			data[i][j]=NO_PLAYER;
		}
	}
	
	playerToMove=BLACK;
	winner=NO_PLAYER;
}

std::string Game::nameOfPlayer(Player in)
{
	switch (in) {
	case WHITE:
		return "O";
		break;
			
	case BLACK:
		return "X";
		break;
	
	default:
		return "no player";
		break;
	}
}

bool Game::move(int x, int y)
{
	if (chackIfMoveValid(x, y))
	{
		data[x][y]=playerToMove;
		playerToMove=(playerToMove==WHITE?BLACK:WHITE);
		checkWinner();
		return true;
	}
	
	return false;
}

bool Game::chackIfMoveValid(int xIn, int yIn)
{
	int xa[]={1, 0, -1, 0};
	int ya[]={0, 1, 0, -1};
	
	if (xIn<0 || xIn>9 || yIn<0 || yIn>9)
	{
		error("tried to move outside the board");
		return false;
	}
	
	if ((xIn==4 || xIn==5) && (yIn==4 || yIn==5))
	{
		error("tried to move in center square");
		return false;
	}
	
	if (data[xIn][yIn]!=NO_PLAYER)
	{
		error("tried to move on top of another player");
		return false;
	}
	
	for (int i=0; i<4; ++i)
	{
		int x=xIn;
		int y=yIn;
		
		if ((x-xa[i]>=0 && x-xa[i]<=9 && y-ya[i]>=0 && y-ya[i]<=9 && data[x-xa[i]][y-ya[i]]!=NO_PLAYER) || ((x-xa[i]==4 || x-xa[i]==5) && (y-ya[i]==4 || y-ya[i]==5)))
		{
			while (true)
			{
				x+=xa[i];
				y+=ya[i];
				
				if (x<0 || x>9 || y<0 || y>9)
				{
					return true;
				}
				
				if ((x==4 || x==5) && (y==4 || y==5))
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
	
	error(std::string()+"could not find path to location "+(char)(xIn+'0')+", "+(char(yIn+'0')));
	return false;
}

void Game::checkWinner()
{
	int xa[]={ 1, 1, 1, 0};
	int ya[]={-1, 0, 1, 1};
	
	for (int y=0; y<10; ++y)
	{
		for (int x=0; x<10; ++x)
		{
			Player plr;
			
			if ((plr=data[x][y])!=NO_PLAYER)
			{
				for (int i=0; i<4; ++i)
				{
					int c=0;
					int x0=x, y0=y;
					
					while (true)
					{
						if (!isIn(x0, y0))
							break;
						
						if (!notInMiddle(x0, y0))
							break;
						
						if (data[x0][y0]!=plr)
							break;
						
						++c;
						x0+=xa[i];
						y0+=ya[i];
					}
					
					if (c>=4)
					{
						winner=plr;
						std::cout << nameOfPlayer(winner) << " won at (" << x << ", " << y << ") in direction (" << xa[i] << ", " << ya[i] << ")\n";
					}
				}
			}
		}
	}
}

std::string Game::toString()
{
	std::string out;
	
	for (int y=9; y>=0; --y)
	{
		out+=(char)(y+'0');
		out+=" ";
		//out+="   ";
		
		for (int x=0; x<10; ++x)
		{
			Player plr=data[x][y];
			
			if ((x==4 || x==5) && (y==4 || y==5))
				out+="█";
			else if (plr==NO_PLAYER)
				out+=" ";
			else if (plr==WHITE)
				out+="O";
			else if (plr==BLACK)
				out+="X";
			else
				out+="err";
			
			if (x<9)
				out+=" ";
		}
		
		if (y>0)
			out+="\n";
	}
	
	out+="\n  0 1 2 3 4 5 6 7 8 9";
	//out+="\n    0   1   2   3   4   5   6   7   8   9";
	
	out+="\n";
	
	return out;
}

void Game::error(std::string msg)
{
	std::cout << "fourfour error:" << msg << std::endl;
}