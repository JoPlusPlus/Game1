#pragma once
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
using namespace std;
using namespace sf;

class MainMenu {
private:
	int selected;
	Text mainmenu[3];
	Font font;



public:

	MainMenu();
	void draw(RenderWindow& window);
	void move_up();
	void move_down();
	void set_selected(int n);
	int pressed() {
		return selected;
	}
	~MainMenu();

};


class PauseMenu {
private:
	int selected = 0;
	Text pausemenu[3];
	Font font;
	SoundBuffer pauseMenu_buff;
	Sound pauseMenu_sound; 
public:
	PauseMenu();
	void draw(RenderWindow& window);
	void set_selected(int n);
	void move_up();
	void move_down();
	int pressed() {
		return selected;
	}
	void playPauseMenuMusic();
	void stopPauseMenuMusic();
	~PauseMenu();
};

class GameOverMenu {
private:
	int selected = 0;
	Text gameovermenu[5];
	Text dead;
	Font font;
	
public:
	GameOverMenu();
	void draw(RenderWindow& window);
	void set_selected(int n);
	void move_up();
	void move_down();
	int pressed() {
		return selected;
	}
	~GameOverMenu();
};