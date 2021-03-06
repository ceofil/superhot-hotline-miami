#include "Menu.h"
#include <algorithm> 

Menu::Menu(Enemy * enemies_in, int maxNumberEnemies_in, int & currNumberEnemies_in, 
	RectF * walls_in, int maxNumberWalls_in, int & currNumberWalls_in, 
	Soldier & player_in,
	Bullet * playerBullets, int maxNumberPlayerBullets, Bullet * enemyBullets, int maxNumberEnemyBullets)
	:
	enemies(enemies_in),
	maxNumberEnemies(maxNumberEnemies_in),
	currNumberEnemies(currNumberEnemies_in),
	walls(walls_in),
	maxNumberWalls(maxNumberWalls_in),
	currNumberWalls(currNumberWalls_in),
	player(player_in),
	playerBullets(playerBullets),
	maxNumberPlayerBullets(maxNumberPlayerBullets),
	enemyBullets(enemyBullets),
	maxNumberEnemyBullets(maxNumberEnemyBullets)
{
	const float sw = float(Graphics::ScreenWidth);
	const float sh = float(Graphics::ScreenHeight);
	const float wCenter = sw / 2; 
	const float hCenter = sh / 2;

	std::ifstream in("nr_levels.txt");
	in >> NumberOfLevels;

	char temp[50];
	strcpy(temp, "level ");

	for (int i = 1; i <= NumberOfLevels; i++)
	{
		IntToChar(temp + 6, i);
		levels[i] = Button(RectF::FromCenter( GetButtonCenter(i), buttonWidth/2.0f, buttonHeight / 2.0f), temp, 2, Color(0, 165, 255));
	}

	start = Button(RectF::FromCenter(wCenter, hCenter - 100.0f, buttonWidth, buttonHeight), "start game", 2, Color(255, 165, 0));
	restart = Button(RectF::FromCenter(wCenter, sh - buttonHeight, buttonWidth*2.0f, buttonHeight), "press r to restart", 2, Color(255, 50, 70));

	editor = Button(RectF::FromCenter(wCenter, hCenter + buttonHeight, buttonWidth, buttonHeight), "level editor", 2, Color(0, 165, 255));
	addLevel = Button(RectF::FromCenter(wCenter - buttonWidth, sh - 100.0f, buttonWidth, buttonHeight), "add level", 2, Color(25, 75, 255));
	deleteLevel = Button(RectF::FromCenter(wCenter + buttonWidth, sh - 100.0f, buttonWidth, buttonHeight), "delete level", 2, Color(220,0,200));

	back = Button(RectF::FromCenter(100.0f, buttonHeight, buttonWidth, buttonHeight), "back", 2, Color(165, 0, 255));
	save = Button(RectF::FromCenter(100.0f + buttonWidth, buttonHeight, buttonWidth, buttonHeight), "save", 2, Color(255, 0, 165));
	implement = Button(RectF::FromCenter(100.0f + buttonWidth * 2.0f, buttonHeight, buttonWidth, buttonHeight), "implement", 2, Color(0, 165, 255));

	addWall = Button(RectF::FromCenter(100.0f, sh- buttonHeight, buttonWidth, buttonHeight), "add wall", 2, Color(165, 165, 0));
	removeWall = Button(RectF::FromCenter(100.0f + buttonWidth, sh - buttonHeight, buttonWidth, buttonHeight), "remove wall", 2, Color(0, 165, 165));

	addEnemy = Button(RectF::FromCenter(100.0f + buttonWidth * 2.0f, sh - buttonHeight, buttonWidth, buttonHeight), "add enemy", 2, Color(165, 165, 0));
	removeEnemy = Button(RectF::FromCenter(100.0f + buttonWidth * 3.0f, sh - buttonHeight, buttonWidth, buttonHeight), "remove enemy", 2, Color(0, 165, 165));
	placePlayer = Button(RectF::FromCenter(100.0f + buttonWidth * 4.0f, sh - buttonHeight, buttonWidth, buttonHeight), "place player", 2, Color(200, 100, 50));

	player.Set( Vec2(600.0f, 660.0f), Vec2(0.0f, 1.0f) );
	level.SetPlayerEntry( player.GetPos(), 90.0f );

	std::strcpy(fileNameBuffer, "level ");
	
}

void Menu::Draw(Graphics & gfx, Mouse & mouse, Text& txt)
{
	switch (gameState)
	{
	case GameState::firstMenu:
	{
		editor.Draw(gfx, mouse, txt);
		start.Draw(gfx, mouse, txt);
		break;
	}
	case GameState::selectLevel:
	{
		for (int i = 1; i <= NumberOfLevels; i++)
		{
			levels[i].Draw(gfx, mouse, txt);
		}
		break;
	}
	case GameState::playerDied:
	{
		restart.Draw(gfx, mouse, txt);
		break;
	}
	case GameState::levelEditor:
	{
		if (editorState != EditorState::selectLevel && editorState != EditorState::deleteLevel)
		{
			level.Draw(gfx);
		}
		switch (editorState)
		{
			case EditorState::selectLevel:
			{
				{
					addLevel.Draw(gfx, mouse, txt);
					deleteLevel.Draw(gfx, mouse, txt);
					for (int i = 1; i <= NumberOfLevels; i++)
					{
						levels[i].Draw(gfx, mouse, txt);
					}
					break;
				}
			}
			case EditorState::deleteLevel:
			{
				deleteLevel.Draw(gfx, mouse, txt);
				for (int i = 1; i <= NumberOfLevels; i++)
				{
					levels[i].Draw(gfx, mouse, txt);
				}
				Vec2 cursor = mouse.GetPosVec2();
				int x = int(cursor.x);
				int y = int(cursor.y);
				txt.drawStringSizedCenter("DELETE", x, y, 2, Colors::Red);
				break;
			}
			case EditorState::nothing:
			{
				back.Draw(gfx, mouse, txt);
				save.Draw(gfx, mouse, txt);
				implement.Draw(gfx, mouse, txt);

				addWall.Draw(gfx, mouse, txt);
				removeWall.Draw(gfx, mouse, txt);

				addEnemy.Draw(gfx, mouse, txt);
				removeEnemy.Draw(gfx, mouse, txt);
				placePlayer.Draw(gfx, mouse, txt);
				break;
			}
			case EditorState::addWall:
			{
				if (wallStarted)
				{
					gfx.DrawRectPoints(rectBuffer, Colors::LightGray);
				}
				break;
			}
			case EditorState::addEnemy:
			{
				if (!enemyPlaced)
				{
					Enemy(mouse.GetPosVec2(), 180.0f).Draw(gfx);
				}
				else
				{
					Enemy(posBuffer, angleBuffer).Draw(gfx);
				}
				break;
			}
			case EditorState::placePlayer:
			{
				if (playerPlaced)
				{
					Soldier(posBuffer, Enemy::AngleToVec2(angleBuffer)).Draw(gfx);
				}
				break;
			}
			case EditorState::removeWall:
			case EditorState::removeEnemy:
			{
				Vec2 cursor = mouse.GetPosVec2();
				Vec2 dim = Vec2(5.0f,5.0f);
				gfx.DrawRectDiagonals(RectF(cursor-dim,cursor+dim), Colors::Red);
				break;
			}
			
		}
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
			gameState = GameState::selectLevel;
		}
		if (editor.IsMouseOver(mouse))
		{
			gameState = GameState::levelEditor;
			editorState = EditorState::selectLevel;
		}
		break;
	}
	case GameState::selectLevel:
	{
		for (int i = 1; i <= NumberOfLevels; i++)
		{
			if (levels[i].IsMouseOver(mouse))
			{
				ChangeLevel(i);
				level.Load(fileNameBuffer);
				level.Implement(walls, currNumberWalls, enemies, currNumberEnemies, player);
				RestartGame();
				gameState = GameState::gameStarted;
				break;
			}
		}
		break;
	}
	case GameState::playerDied:
	{
		if (restart.IsMouseOver(mouse))
		{
			RestartGame();
			gameState = GameState::gameStarted;
		}
		break;
	}
	case GameState::levelEditor:
	{
		switch (editorState)
		{
		case EditorState::selectLevel:
		{
			if (addLevel.IsMouseOver(mouse))
			{
				AddLevel();
			}
			if (deleteLevel.IsMouseOver(mouse))
			{
				editorState = EditorState::deleteLevel;
			}
			for (int i = 1; i <= NumberOfLevels; i++)
			{
				if (levels[i].IsMouseOver(mouse))
				{
					ChangeLevel(i);
					level.Load(fileNameBuffer);
					editorState = EditorState::nothing;
					break;
				}
			}
			break;
		}
		case EditorState::deleteLevel:
		{
			for (int i = 1; i <= NumberOfLevels; i++)
			{
				if (levels[i].IsMouseOver(mouse))
				{
					DeleteLevel(i);
					break;
				}
			}
			editorState = EditorState::selectLevel;
			break;
		}
		case EditorState::nothing:
		{
			if (back.IsMouseOver(mouse))
			{
				editorState = EditorState::selectLevel;
			}
			if (save.IsMouseOver(mouse))
			{
				level.Save(fileNameBuffer);
			}
			if (implement.IsMouseOver(mouse))
			{
				level.Implement(walls, currNumberWalls, enemies, currNumberEnemies, player);
			}

			if (addWall.IsMouseOver(mouse))
			{
				editorState = EditorState::addWall;
			}
			if (removeWall.IsMouseOver(mouse))
			{
				editorState = EditorState::removeWall;
			}
			if (addEnemy.IsMouseOver(mouse))
			{
				editorState = EditorState::addEnemy;
			}
			if (removeEnemy.IsMouseOver(mouse))
			{
				editorState = EditorState::removeEnemy;
			}
			if (placePlayer.IsMouseOver(mouse))
			{
				editorState = EditorState::placePlayer;
			}
			break;
		}
		case EditorState::removeWall:
		{
			RemoveWall(mouse);
			editorState = EditorState::nothing;
			break;
		}
		case EditorState::removeEnemy:
		{
			RemoveEnemy(mouse);
			editorState = EditorState::nothing;
			break;
		}
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
		editorState = EditorState::nothing;
	}
}

void Menu::RemoveWall(Mouse & mouse)
{
	Vec2 cursor = mouse.GetPosVec2();
	level.RemoveWallEntry( cursor );
}

void Menu::AddEnemy(Mouse & mouse)
{
	if (mouse.LeftIsPressed())
	{
		if (!enemyPlaced)
		{
			enemyPlaced = true;
			
			posBuffer = mouse.GetPosVec2();

			if (currNumberEnemies + 1 < maxNumberEnemies)
			{
				currNumberEnemies++;
			}
		}
		else
		{
			angleBuffer = Enemy::Vec2ToAngle((mouse.GetPosVec2() - posBuffer ).GetNormalized());
		}


	}
	else if (enemyPlaced)
	{
		enemyPlaced = false;
		level.AddEnemyEntry( posBuffer,angleBuffer );
		editorState = EditorState::nothing;
	}
}

void Menu::RemoveEnemy(Mouse & mouse)
{
	Vec2 cursor = mouse.GetPosVec2();
	level.RemoveEnemyEntry(cursor);
}

void Menu::PlacePlayer(Mouse & mouse)
{
	if (mouse.LeftIsPressed())
	{
		if (!playerPlaced)
		{
			playerPlaced = true;

			posBuffer = mouse.GetPosVec2();
		}
		else
		{
			angleBuffer = Enemy::Vec2ToAngle((mouse.GetPosVec2() - posBuffer).GetNormalized());
		}


	}
	else if (playerPlaced)
	{
		playerPlaced = false;
		level.SetPlayerEntry(posBuffer, angleBuffer);
		editorState = EditorState::nothing;
	}
}

void Menu::RestartGame()
{
	player.Respawn();
	for (int i = 0; i < currNumberEnemies; i++)
	{
		enemies[i].Respawn();
	}
	for (int i = 0; i < maxNumberPlayerBullets; i++)
	{
		playerBullets[i].Destroy();
	}
	for (int i = 0; i < maxNumberEnemyBullets; i++)
	{
		enemyBullets[i].Destroy();
	}
}

void Menu::GoBack()
{
	switch(gameState)
	{
	case GameState::selectLevel:
	{
		gameState = GameState::firstMenu;
		break;
	}
	case GameState::gameStarted:
	case GameState::playerDied:
	{
		gameState = GameState::selectLevel;
		break;
	}
	case GameState::levelEditor:
	{
		switch(editorState)
		{
		case EditorState::selectLevel:
		{
			gameState = GameState::firstMenu;
			break;
		}
		case EditorState::nothing:
		case EditorState::deleteLevel:
		{
			editorState = EditorState::selectLevel;
			break;
		}
		}
		break;
	}
	}
}

Vec2 Menu::GetButtonCenter(int level)
{
	float x = float( (level-1) % buttonsPerRow);
	float y = float( (level-1) / buttonsPerRow);
	return Vec2(wSpacing + buttonWidth / 2.0f + (wSpacing + buttonWidth) * x, 100.0f + (buttonHeight + 25.0f)*y);
}

void Menu::IntToChar(char * Dst, int number)
{
	char* start = Dst;
	while (number != 0)
	{
		*Dst = '0' + number % 10;
		number = number / 10;
		Dst++;
	}
	*Dst = 0;
	std::reverse(start, Dst);
}

void Menu::SetFileName(char * Dst, int number)
{
	IntToChar( Dst + 6, number );
	std::strcat( Dst, ".txt" );
}

void Menu::ChangeLevel(int wantedLevel)
{
	if (wantedLevel > 0 && wantedLevel <= NumberOfLevels)
	{
		SetFileName( fileNameBuffer, wantedLevel );
	}
}

void Menu::AddLevel()
{
	NumberOfLevels++;
	SetFileName(fileNameBuffer, NumberOfLevels);
	level.Clear();
	
	char temp[50];
	strcpy(temp, "level ");
	IntToChar(temp + 6, NumberOfLevels);

	levels[NumberOfLevels] = Button(RectF::FromCenter(GetButtonCenter(NumberOfLevels), buttonWidth / 2.0f, buttonHeight / 2.0f), temp, 2, Color(255, 100, 255));
	editorState = EditorState::nothing;
	
	std::ofstream out("nr_levels.txt");
	out << NumberOfLevels;
}

void Menu::DeleteLevel(int level)
{
	SetFileName(fileNameBuffer, level);
	remove(fileNameBuffer);

	char oldFileName[50];
	char newFileName[50];
	strcpy(oldFileName, "level ");
	strcpy(newFileName, "level ");
	for (int i = level + 1; i <= NumberOfLevels; i++)
	{
		SetFileName(oldFileName, i);

		SetFileName(newFileName, i - 1);

		rename(oldFileName, newFileName);
	}
	NumberOfLevels--;
	std::ofstream out("nr_levels.txt");
	out << NumberOfLevels;
}
