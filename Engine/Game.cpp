/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	bulletShotSound(L"bulletShot.wav"),
	player(Vec2(300.0f,300.0f),Vec2(0.0f,0.0f))
{
	walls[1] = RectF(100.0f, 200.0f, 100.0f, 200.0f);
}

void Game::Go()
{
	gfx.BeginFrame();	

	float timeperiod = ft.Mark();
	while (timeperiod > 0.0f)
	{
		const float dt = std::min(0.0025f, timeperiod);
		UpdateModel(dt);
		timeperiod -= dt;
	}

	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel(float dt)
{
	player.Update(wnd.kbd, wnd.mouse, dt);
	UpdateBullets(dt);
	HandleShooting();
}



void Game::ComposeFrame()
{
	player.Draw(gfx);
	DrawBullets();
	DrawWalls();
}


void Game::UpdateBullets(float dt)
{
	for (int i = 0; i < nBullets; i++)
	{
		if ( playerBullets[i].IsSpawned() )
		{
			playerBullets[i].Update(dt, walls, indexWalls, bulletShotSound);
		}
	}
}

void Game::DrawBullets()
{
	for (int i = 0; i < nBullets; i++)
	{
		if ( playerBullets[i].IsSpawned() )
		{
			playerBullets[i].Draw(gfx);
		}
	}
}

void Game::HandleShooting()
{
	while (!wnd.mouse.IsEmpty())
	{
		const Mouse::Event e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			player.Shoot(playerBullets, nBullets, bulletShotSound);

		}
	}
}

void Game::DrawWalls()
{
	for (int i = 1; i <= indexWalls; i++)
	{
		gfx.DrawRectPoints( walls[i], Colors::LightGray );
	}
}
