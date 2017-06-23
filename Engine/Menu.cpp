#include "Menu.h"

Menu::Menu(Enemy * enemies_in, int maxNumberEnemies_in, int & currNumberEnemies_in, 
	RectF * walls_in, int maxNumberWalls_in, int & currNumberWalls_in, 
	Soldier & player_in,
	bool& gameIsStarted)
	:
	enemies(enemies_in),
	maxNumberEnemies(maxNumberEnemies_in),
	currNumberEnemies(currNumberEnemies_in),
	walls(walls_in),
	maxNumberWalls(maxNumberWalls_in),
	currNumberWalls(currNumberWalls_in),
	player(player_in),
	gameIsStarted(gameIsStarted)
{
	static constexpr float wCenter = Graphics::ScreenWidth / 2; 
	static constexpr float hCenter = Graphics::ScreenHeight / 2;
	static constexpr float width = 150.0f;

	start = Button(RectF::FromCenter(wCenter, hCenter - 100.0f, width, 50.0f), "start", 2, Color(255, 165, 0));
	save = Button(RectF::FromCenter(wCenter, hCenter, width, 50.0f), "save", 2, Color(255,0,165));
	implement = Button(RectF::FromCenter(wCenter, hCenter + 100.0f, width, 50.0f), "implement", 2, Color(0, 165, 255));
	load = Button(RectF::FromCenter(wCenter, hCenter + 200.0f, width, 50.0f), "load", 2, Color(0, 165, 255));
	addWall = Button(RectF::FromCenter(wCenter, hCenter + 275.0f, width, 50.0f), "load", 2, Color(0, 165, 255));
	
}

void Menu::Draw(Graphics & gfx, Mouse & mouse, Text& txt)
{
	start.Draw(gfx, mouse, txt);
	save.Draw(gfx, mouse, txt);
	implement.Draw(gfx, mouse, txt);
	load.Draw(gfx, mouse, txt);
}

void Menu::HandleMousePressed(Mouse& mouse)
{
	if (start.IsMouseOver(mouse))
	{
		gameIsStarted = true;
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
