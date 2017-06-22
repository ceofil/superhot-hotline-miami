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

	start = Button(RectF::FromCenter(wCenter, hCenter - 100.0f, width, 50), "start", 2, Color(255, 165, 0));
}

void Menu::Draw(Graphics & gfx, Mouse & mouse, Text& txt)
{
	start.Draw(gfx, mouse, txt);
}

void Menu::HandleMousePressed(Mouse& mouse)
{
	if (start.IsMouseOver(mouse))
	{
		gameIsStarted = true;
	}
}
