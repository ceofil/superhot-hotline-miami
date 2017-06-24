#include "Menu.h"

Menu::Menu(Enemy * enemies_in, int maxNumberEnemies_in, int & currNumberEnemies_in, 
	RectF * walls_in, int maxNumberWalls_in, int & currNumberWalls_in, 
	Soldier & player_in,
	Bullet * playerBullets, int maxNumberBullets, Bullet * enemyBullets, int maxNumberBulletsEnemies)
	:
	enemies(enemies_in),
	maxNumberEnemies(maxNumberEnemies_in),
	currNumberEnemies(currNumberEnemies_in),
	walls(walls_in),
	maxNumberWalls(maxNumberWalls_in),
	currNumberWalls(currNumberWalls_in),
	player(player_in)
{
	static constexpr float sw = Graphics::ScreenWidth;
	static constexpr float sh = Graphics::ScreenHeight;
	static constexpr float wCenter = sw / 2; 
	static constexpr float hCenter = sh / 2;
	static constexpr float width = 150.0f;

	start = Button(RectF::FromCenter(wCenter, hCenter - 100.0f, width, 50.0f), "start game", 2, Color(255, 165, 0));
	load = Button(RectF::FromCenter(wCenter, hCenter -25.0f, width, 50.0f), "load level", 2, Color(0, 165, 255));
	editor = Button(RectF::FromCenter(wCenter, hCenter + 50.0f, width, 50.0f), "level editor", 2, Color(0, 165, 255));

	back = Button(RectF::FromCenter(100.0f, 50.0f, width, 50.0f), "back", 2, Color(165, 0, 255));
	save = Button(RectF::FromCenter(100.0f + width, 50.0f, width, 50.0f), "save", 2, Color(255, 0, 165));
	implement = Button(RectF::FromCenter(100.0f + width * 2.0f, 50.0f, width, 50.0f), "implement", 2, Color(0, 165, 255));

	addWall = Button(RectF::FromCenter(100.0f, sh-50.0f, width, 50.0f), "add wall", 2, Color(165, 165, 0));
	removeWall = Button(RectF::FromCenter(100.0f + width, sh - 50.0f, width, 50.0f), "remove wall", 2, Color(0, 165, 165));

	addEnemy = Button(RectF::FromCenter(100.0f + width * 2.0f, sh - 50.0f, width, 50.0f), "add enemy", 2, Color(165, 165, 0));
	removeEnemy = Button(RectF::FromCenter(100.0f + width * 3.0f, sh - 50.0f, width, 50.0f), "remove enemy", 2, Color(0, 165, 165));
	placePlayer = Button(RectF::FromCenter(100.0f + width * 4.0f, sh - 50.0f, width, 50.0f), "place player", 2, Color(200, 100, 50));
	level.SetPlayerEntry(player.GetPos(), 180.0f);
}

void Menu::Draw(Graphics & gfx, Mouse & mouse, Text& txt)
{

	switch (gameState)
	{
	case GameState::firstMenu:
	{
		editor.Draw(gfx, mouse, txt);
		start.Draw(gfx, mouse, txt);
		load.Draw(gfx, mouse, txt);
		break;
	}
	case GameState::levelEditor:
	{
		level.Draw(gfx);
		level.DrawPlayerEntry(gfx);
		switch (editorState)
		{
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
				Soldier(posBuffer, Enemy::AngleToVec2(angleBuffer)).Draw(gfx, Color(100, 150, 255));
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
			RestartGame();
			gameState = GameState::gameStarted;
		}
		if (load.IsMouseOver(mouse))
		{
			level.Load("level.txt");
		}
		if (editor.IsMouseOver(mouse))
		{
			gameState = GameState::levelEditor;
		}
		break;
	}
	case GameState::levelEditor:
	{
		switch (editorState)
		{
		case EditorState::nothing:
		{
			if (back.IsMouseOver(mouse))
			{
				gameState = GameState::firstMenu;
				editorState = EditorState::nothing;
			}
			if (save.IsMouseOver(mouse))
			{
				level.Save("level.txt");
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
	for (int i = 0; i < maxNumberBullets; i++)
	{
		playerBullets[i].Destroy();
	}
	for (int i = 0; i < maxNumberBulletsEnemies; i++)
	{
		enemyBullets[i].Destroy();
	}
}

