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
	player(Vec2(50.0f,50.0f),Vec2(0.0f,0.0f))
{
	walls[++indexWalls] = RectF(100.0f, 200.0f, 100.0f, 200.0f);
	walls[++indexWalls] = RectF(300.0f, 400.0f, 100.0f, 200.0f);
	walls[++indexWalls] = RectF(500.0f, 600.0f, 200.0f, 350.0f);
	enemies[0].Spawn(Vec2(150.0f, 450.0f), 0.0f);
	
	enemies[1].Spawn(Vec2(250.0f, 450.0f), 90.0f);
	enemies[2].Spawn(Vec2(350.0f, 450.0f), 0.0f);
	enemies[3].Spawn(Vec2(450.0f, 450.0f), 45.0f);
	enemies[4].Spawn(Vec2(550.0f, 450.0f), -30.0f);
	
	
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
	player.Update(wnd.kbd, wnd.mouse, walls, indexWalls, playerBullets, nBullets, enemyBullets, nBulletsForEnemies, dt);
	UpdateEnemies(dt);
	UpdateBullets(dt);
	HandleShooting();
}



void Game::ComposeFrame()
{
	player.Draw(gfx, Colors::Green);

	DrawEnemies();
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
	for (int i = 0; i < nBulletsForEnemies; i++)
	{
		if (enemyBullets[i].IsSpawned())
		{
			enemyBullets[i].Update(dt, walls, indexWalls, bulletShotSound);
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
	for (int i = 0; i < nBulletsForEnemies; i++)
	{
		if (enemyBullets[i].IsSpawned())
		{
			enemyBullets[i].Draw(gfx);
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

void Game::UpdateEnemies(float dt)
{
	for (int i = 0; i < nEnemies; i++)
	{
		if (enemies[i].IsAlive())
		{
			enemies[i].Update(player, walls, indexWalls, enemyBullets, nBulletsForEnemies, playerBullets, nBullets, bulletShotSound, dt);
		}
	}
}

void Game::DrawEnemies()
{
	for (int i = 0; i < nEnemies; i++)
	{
		if (enemies[i].IsAlive())
		{
			enemies[i].Draw(gfx);
		}
	}
}
