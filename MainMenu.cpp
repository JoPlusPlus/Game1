#include "MainMenu.h"
#include<SFML/Graphics.hpp>
#include<vector>
using namespace std;
using namespace sf;

MainMenu::MainMenu() {
	font.loadFromFile("Chosen/font/Playername font/ArtisanParis-Regular.otf");

	float text_y_position = 300;
	float text_x_position = 500;

	mainmenu[0].setFont(font);
	mainmenu[0].setFillColor(Color::Yellow);
	mainmenu[0].setString("Play");
	mainmenu[0].setCharacterSize(75);
	mainmenu[0].setPosition(text_x_position, text_y_position);

	mainmenu[1].setFont(font);
	mainmenu[1].setFillColor(Color::White);
	mainmenu[1].setString("Settings");
	mainmenu[1].setCharacterSize(100);
	mainmenu[1].setPosition(text_x_position, text_y_position + 150);

	mainmenu[2].setFont(font);
	mainmenu[2].setFillColor(Color::White);
	mainmenu[2].setString("Exit");
	mainmenu[2].setCharacterSize(100);
	mainmenu[2].setPosition(text_x_position, text_y_position + 300);

	selected = 0;
}
MainMenu::~MainMenu() {}

void MainMenu::set_selected(int n) {
	selected = n;
}

void MainMenu::draw(RenderWindow& window) {
	Texture this_mainmenu_tex;
	Sprite this_mainmenu_sprite;

	this_mainmenu_tex.loadFromFile("Chosen/Backgrounds/game_background_3.1.png");
	this_mainmenu_sprite.setTexture(this_mainmenu_tex);

	window.draw(this_mainmenu_sprite);
	for (int i = 0; i < 3; i++) {
		window.draw(mainmenu[i]);
	}
}

void MainMenu::move_up() {
	mainmenu[selected].setFillColor(Color::White);
	mainmenu[selected].setCharacterSize(100);
	selected--;
	if (selected == -1) selected = 2;
	mainmenu[selected].setFillColor(Color::Yellow);
	mainmenu[selected].setCharacterSize(75);
}

void MainMenu::move_down() {
	mainmenu[selected].setFillColor(Color::White);
	mainmenu[selected].setCharacterSize(100);
	selected++;
	if (selected == 3) selected = 0;
	mainmenu[selected].setFillColor(Color::Yellow);
	mainmenu[selected].setCharacterSize(75);
}

// ________________________________________________________________________________________


float text_x_position = 800;
float text_y_position = 300;


PauseMenu::PauseMenu() {



	font.loadFromFile("Chosen/font/Playername font/ArtisanParis-Regular.otf");




	pausemenu[0].setFont(font);
	pausemenu[0].setFillColor(Color::Yellow);
	pausemenu[0].setString("Continue");
	pausemenu[0].setCharacterSize(75);
	pausemenu[0].setPosition(text_x_position, text_y_position);

	pausemenu[1].setFont(font);
	pausemenu[1].setFillColor(Color::White);
	pausemenu[1].setString("Settings");
	pausemenu[1].setCharacterSize(100);
	pausemenu[1].setPosition(text_x_position, text_y_position + 150);

	pausemenu[2].setFont(font);
	pausemenu[2].setFillColor(Color::White);
	pausemenu[2].setString("Exit");
	pausemenu[2].setCharacterSize(100);
	pausemenu[2].setPosition(text_x_position, text_y_position + 300);



	selected = 0;

}
PauseMenu::~PauseMenu() {}

void PauseMenu::draw(RenderWindow& window) {
	RectangleShape rectangle(Vector2f(450, 600));
	rectangle.setPosition(750, 200);
	rectangle.setFillColor(Color(128, 128, 128));
	window.draw(rectangle);

	for (int i = 0; i < 3; i++) {
		window.draw(pausemenu[i]);
	}
}

void PauseMenu::move_up() {
	pausemenu[selected].setFillColor(Color::White);
	pausemenu[selected].setCharacterSize(100);
	selected--;
	if (selected == -1) selected = 2;
	pausemenu[selected].setFillColor(Color::Yellow);
	pausemenu[selected].setCharacterSize(75);
}

void PauseMenu::move_down() {
	pausemenu[selected].setFillColor(Color::White);
	pausemenu[selected].setCharacterSize(100);
	selected++;
	if (selected == 3) selected = 0;
	pausemenu[selected].setFillColor(Color::Yellow);
	pausemenu[selected].setCharacterSize(75);
}

void PauseMenu::set_selected(int n) {
	selected = n;

	pausemenu[0].setFont(font);
	pausemenu[0].setFillColor(Color::Yellow);
	pausemenu[0].setString("Continue");
	pausemenu[0].setCharacterSize(75);
	pausemenu[0].setPosition(text_x_position, text_y_position);

	pausemenu[1].setFont(font);
	pausemenu[1].setFillColor(Color::White);
	pausemenu[1].setString("Settings");
	pausemenu[1].setCharacterSize(100);
	pausemenu[1].setPosition(text_x_position, text_y_position + 150);

	pausemenu[2].setFont(font);
	pausemenu[2].setFillColor(Color::White);
	pausemenu[2].setString("Exit");
	pausemenu[2].setCharacterSize(100);
	pausemenu[2].setPosition(text_x_position, text_y_position + 300);


	
}

// ________________________________________________________________________________________


GameOverMenu::GameOverMenu() {

	int start_x_position = 500, start_y_position = 400, rate = 100;

	
	
	font.loadFromFile("Chosen/font/Playername font/ArtisanParis-Regular.otf");

	dead.setFont(font);
	dead.setFillColor(Color::Red);
	dead.setString("YOU DIED!");
	dead.setCharacterSize(150);
	dead.setPosition(200, 200);

	gameovermenu[0].setFont(font);
	gameovermenu[0].setFillColor(Color::Yellow);
	gameovermenu[0].setString("Try again");
	gameovermenu[0].setCharacterSize(75);
	gameovermenu[0].setPosition(start_x_position, start_y_position + rate);



	gameovermenu[1].setFont(font);
	gameovermenu[1].setFillColor(Color::White);
	gameovermenu[1].setString("Enter as a new player");
	gameovermenu[1].setCharacterSize(100);
	gameovermenu[1].setPosition(start_x_position, start_y_position + rate * 2);

	gameovermenu[2].setFont(font);
	gameovermenu[2].setFillColor(Color::White);
	gameovermenu[2].setString("LeaderBoard");
	gameovermenu[2].setCharacterSize(100);
	gameovermenu[2].setPosition(start_x_position, start_y_position + rate * 3);

	gameovermenu[3].setFont(font);
	gameovermenu[3].setFillColor(Color::White);
	gameovermenu[3].setString("Return to MainMenu");
	gameovermenu[3].setCharacterSize(100);
	gameovermenu[3].setPosition(start_x_position, start_y_position + rate * 4);

	gameovermenu[4].setFont(font);
	gameovermenu[4].setFillColor(Color::White);
	gameovermenu[4].setString("Exit");
	gameovermenu[4].setCharacterSize(100);
	gameovermenu[4].setPosition(start_x_position, start_y_position + rate * 5);



	selected = 0;
}

GameOverMenu::~GameOverMenu() {}

void GameOverMenu::move_up() {
	gameovermenu[selected].setFillColor(Color::White);
	gameovermenu[selected].setCharacterSize(100);
	selected--;
	if (selected == -1) selected = 4;
	gameovermenu[selected].setFillColor(Color::Yellow);
	gameovermenu[selected].setCharacterSize(75);
}

void GameOverMenu::move_down() {

	gameovermenu[selected].setFillColor(Color::White);
	gameovermenu[selected].setCharacterSize(100);
	selected++;
	if (selected == 5) selected = 0;
	gameovermenu[selected].setFillColor(Color::Yellow);
	gameovermenu[selected].setCharacterSize(75);
	
}

void GameOverMenu::draw(RenderWindow& window) {

	Texture tbackground;
	tbackground.loadFromFile("Chosen/Backgrounds/game_background_3.1.png");
	Sprite sbackground(tbackground);
	window.draw(sbackground);

	window.draw(dead);

	for (int i = 0; i < sizeof(gameovermenu) / sizeof(gameovermenu[0]); i++) {
		window.draw(gameovermenu[i]);
	}
}

void GameOverMenu::set_selected(int n) {
	selected = n;

	int start_x_position = 500, start_y_position = 400, rate = 150;



	font.loadFromFile("Chosen/font/Playername font/ArtisanParis-Regular.otf");

	dead.setFont(font);
	dead.setFillColor(Color::Red);
	dead.setString("YOU DIED!");
	dead.setCharacterSize(300);
	dead.setPosition(800, 200);

	gameovermenu[0].setFont(font);
	gameovermenu[0].setFillColor(Color::Yellow);
	gameovermenu[0].setString("Try again");
	gameovermenu[0].setCharacterSize(75);
	gameovermenu[0].setPosition(start_x_position, start_y_position + rate);

	gameovermenu[1].setFont(font);
	gameovermenu[1].setFillColor(Color::White);
	gameovermenu[1].setString("Enter as a new player");
	gameovermenu[1].setCharacterSize(100);
	gameovermenu[1].setPosition(start_x_position, start_y_position + rate * 2);

	gameovermenu[2].setFont(font);
	gameovermenu[2].setFillColor(Color::White);
	gameovermenu[2].setString("Return to MainMenu");
	gameovermenu[2].setCharacterSize(100);
	gameovermenu[2].setPosition(start_x_position, start_y_position + rate * 3);

	gameovermenu[3].setFont(font);
	gameovermenu[3].setFillColor(Color::White);
	gameovermenu[3].setString("Exit");
	gameovermenu[3].setCharacterSize(100);
	gameovermenu[3].setPosition(start_x_position, start_y_position + rate * 4);
}