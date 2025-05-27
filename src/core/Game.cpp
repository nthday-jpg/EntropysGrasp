#include "../core/Game.h"
#include <iostream>

using namespace sf;
using namespace std;

Game::Game()
{
	window.create(VideoMode({ windowWidth, windowHeight}), "Entropy's Grasp");
	cameraView.setSize({windowWidth, windowHeight});
	window.setView(cameraView);
	window.setFramerateLimit(FPS);
	// Khởi tạo các thành phần cần thiết ở đây


}

Game::~Game()
{
	// Giải phóng tài nguyên nếu cần
}

void Game::run()
{
	while (window.isOpen())
	{
		processInput();
		

		//Xem "Fix timestep pattern" ở https://gafferongames.com/post/fix_your_timestep/
		float dt = gameClock.restart().asSeconds();
		accumulator += dt;
		while (accumulator >= timestep)
		{
			update(timestep);
			accumulator -= timestep;
		}

		render();
	}
}


//Xử lý input với command
void Game::processInput()
{
	while (const optional event = window.pollEvent())
	{
		if (event->is<Event::Closed>())
		{
			window.close();
		}
		else if (const auto keypressed = event->getIf<Event::KeyPressed>())
		{
			switch (keypressed->scancode)
			{
				case Keyboard::Scancode::W:
					cout << "Pressed W key" << endl;
					break;
				case Keyboard::Scancode::A:
					cout << "Pressed A key" << endl;
					break;
				case Keyboard::Scancode::S:
					cout << "Pressed S key" << endl;
					break;
				case Keyboard::Scancode::D:
					cout << "Pressed D key" << endl;
					break;
				case Keyboard::Scancode::Escape:
					window.close();
					break;
				default:
					break;
				}
		}
		else if (event->is<Event::MouseMoved>())
		{
			//Xử lý di chuyển chuột nếu cần
		}
		else if (event->is<Event::MouseButtonPressed>())
		{

		}
	}
}

//Cập nhật trạng thái của game
void Game::update(float dt)
{
	
}

//Render
void Game::render()
{
	window.clear();


	window.display();
}

