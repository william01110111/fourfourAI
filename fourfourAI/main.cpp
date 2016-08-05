//
//  main.cpp
//  fourfourAI
//
//  Created by William Wold on 8/4/16.
//  Copyright © 2016 Widap. All rights reserved.
//

#include <iostream>
#include "Game.hpp"
#include "FourfourAI.hpp"
#include <thread>

void waitFor(double duration);
void setupAI(FourfourAI * ai, int settingsNum);

int main(int argc, const char * argv[]) {
	
	Game game;
	FourfourAI ai0(&game, BLACK);
	FourfourAI ai1(&game, WHITE);
	
	setupAI(&ai0, 1);
	setupAI(&ai1, 0);
	
	std::cout << game.toString() << std::endl;
	
	while (game.getWinner()==NO_PLAYER)
	{
		/*if (game.getPlayer()==ai0.getPlayer())
		{
			ai0.move();
		}
		else*/ if (game.getPlayer()==ai1.getPlayer())
		{
			ai1.move();
		}
		else
		{
			int x, y;
			std::cin >> x >> y;
			game.move(x, y);
		}
		
		std::cout << game.toString() << std::endl;
		
		//waitFor(0.1);
	}
	
	if (game.getWinner()==WHITE)
	{
		std::cout << "O won!\n";
	}
	else if (game.getWinner()==BLACK)
	{
		std::cout << "X won!\n";
	}
	else
	{
		std::cout << "no winner\n";
	}
	
    return 0;
}

void setupAI(FourfourAI * ai, int settingsNum)
{
	//	O - 0
	//	X - 1
	
	switch (settingsNum) {
	
	case 1:
		ai->settings.constWeight=1.0;
		ai->settings.twoBWeight=1.0;
		ai->settings.twoUWeight=3.0;
		ai->settings.threeBWeight=5.0;
		ai->settings.threeUWeight=7.0;
		ai->settings.recursPerIter=4;
		ai->settings.iters=6;
		break;
			
	case 0:
		ai->settings.constWeight=1.0;
		ai->settings.twoBWeight=1.0;
		ai->settings.twoUWeight=3.0;
		ai->settings.threeBWeight=5.0;
		ai->settings.threeUWeight=7.0;
		ai->settings.recursPerIter=16;
		ai->settings.iters=4;
		break;
	}
}

void waitFor(double duration)
{
	std::this_thread::sleep_for(std::chrono::microseconds((int)(duration*1000000)));
}
