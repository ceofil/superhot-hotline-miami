#include "Menu.h"

Menu::Menu(Enemy * enemies_in, int maxNumberEnemies_in, int & currNumberEnemies_in, 
	RectF * walls_in, int maxNumberWalls_in, int & currNumberWalls_in, 
	Soldier & player_in)
	:
	enemies(enemies_in),
	maxNumberEnemies(maxNumberEnemies_in),
	currNumberEnemies(currNumberEnemies_in),
	walls(walls_in),
	maxNumberWalls(maxNumberWalls_in),
	currNumberWalls(currNumberWalls_in),
	player(player_in)
{
	static constexpr float wCenter = Graphics::ScreenWidth / 2; 
	static constexpr float hCenter = Graphics::ScreenHeight / 2;
	static constexpr float width = 150.0f;

	start = Button(RectF::FromCenter(wCenter, hCenter - 100.0f, width, 50.0f), "start", 2, Color(255, 165, 0));
	save = Button(RectF::FromCenter(wCenter, hCenter, width, 50.0f), "save", 2, Color(255,0,165));
	implement = Button(RectF::FromCenter(wCenter, hCenter + 100.0f, width, 50.0f), "implement", 2, Color(0, 165, 255));
	load = Button(RectF::FromCenter(wCenter, hCenter + 200.0f, width, 50.0f), "load", 2, Color(0, 165, 255));
	addWall = Button(RectF::FromCenter(wCenter, hCenter + 275.0f, width, 50.0f), "add wall", 2, Color(165, 0, 255));
	
}

void Menu::Draw(Graphics & gfx, Mouse & mouse, Text& txt)
{

	switch (gameState)
	{
	case GameState::firstMenu:
	{
		start.Draw(gfx, mouse, txt);
		save.Draw(gfx, mouse, txt);
		implement.Draw(gfx, mouse, txt);
		load.Draw(gfx, mouse, txt);
		addWall.Draw(gfx, mouse, txt);
		break;
	}
	case GameState::levelEditor:
	{
		start.Draw(gfx, mouse, txt);
		save.Draw(gfx, mouse, txt);
		implement.Draw(gfx, mouse, txt);
		if (wallStarted)
		{
			gfx.DrawRectPoints(rectBuffer, Colors::LightGray);
		}
		level.Draw(gfx);
		break;
	}
	}
}


void Menu::HandleMousePressed(Mouse& mouse)
{
	switch (gameState)
	{
	case GameState::firstMenu:
	{
		if (start.IsMouseOver(mouse))
		{
			gameState = GameState::gameStarted;
		}
		if (save.IsMouseOver(mouse))
		{
			level.Save("level.txt");
		}
		if (implement.IsMouseOver(mouse))
		{
			level.Implement(walls, currNumberWalls);
		}
		if (load.IsMouseOver(mouse))
		{
			level.Load("level.txt");
		}
		if (addWall.IsMouseOver(mouse))
		{
			gameState = GameState::levelEditor;
			wallStarted = true;
		}
		break;
	}
	case GameState::levelEditor:
	{
		if (start.IsMouseOver(mouse))
		{
			gameState = GameState::gameStarted;
		}
		if (save.IsMouseOver(mouse))
		{
			level.Save("level.txt");
		}
		if (implement.IsMouseOver(mouse))
		{
			level.Implement(walls, currNumberWalls);
		}
		break;
	}
	}
}

void Menu::AddWall(Mouse& mouse)
{
	if (mouse.LeftIsPressed())
	{
		if (!wallStarted)
		{
			wallStarted = true;
			x1 = x2 = mouse.GetPosX();
			y1 = y2 = mouse.GetPosY();
			if (currNumberWalls + 1 < maxNumberWalls)
			{
				currNumberWalls++;
			}
		}
		else
		{
			x2 = mouse.GetPosX();
			y2 = mouse.GetPosY();
		}
		//this should be on the else if but I want it to be drawn while it's created
		rectBuffer = RectF(float(std::min(x1, x2)), float(std::max(x1, x2)), float(std::min(y1, y2)), float(std::max(y1, y2)));


	}
	else if (wallStarted)
	{
		wallStarted = false;
		level.AddWallEntry(rectBuffer);
	}
}
