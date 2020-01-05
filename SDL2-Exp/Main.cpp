#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "GameWindow.h"
#include "Game.h"
#include "Texture.h"
#include <stdlib.h> 
#include "Background.h"
#include "Layer.h"

#include "Asteroid.h"
#include "Player.h"
#include "Enemy.h"
#include "SALayer.h"
#include "EnemyLayer.h"

const int screenX = 2000, screenY = 1000;
const std::string path = "C:/Users/savva/source/repos/CPROG-PROJECT/SDL2-Exp/";

//ELM IDS: player = 1, enemy = 2, asteroid = 3

int main(int argc, char* argv[]) {
	GameWindow win("The Space Game! i dont even know lmao", screenX, screenY);
	PhysicsHandler physics(0, screenX, screenY);
	Background back(screenX, screenY);

	Texture star(win.ren, (path + "Images/Stars/star.png").c_str());
	Texture background(win.ren, (path + "Images/Stars/space.jpg").c_str());

	Texture boostingShip(win.ren, (path + "Images/shipON.png").c_str());
	Texture cruisingShip(win.ren, (path + "Images/shipOFF.png").c_str());

	Texture* Asteroidtextures[5] = { new Texture(win.ren, (path + "Images/Asteroids/asteroid1.png").c_str()),
									new Texture(win.ren, (path + "Images/Asteroids/asteroid2.png").c_str()) ,
									new Texture(win.ren, (path + "Images/Asteroids/asteroid3.png").c_str()) ,
									new Texture(win.ren, (path + "Images/Asteroids/asteroid4.png").c_str()) ,
									new Texture(win.ren, (path + "Images/Asteroids/asteroid5.png").c_str()) };
	
	back.setTexture(background.getTexture());
	Game game(&win, &physics, &back);
	game.setScrolling(true);
	game.setHitboxOffset(-10);
	
	//CREATE PLAYER
	Player play(screenX/2,screenY/2, 80, 80);
	play.setCruiseTexture(cruisingShip.getTexture());
	play.setMovingTexture(boostingShip.getTexture());
	play.setCollidable(true);
	play.setMaxSpeed(10);

	//LAYER OF STARS IN FAR BACK
	SALayer starLayer1 = SALayer(&play, screenX, screenY);
	starLayer1.setMovementSpeedToPlayer(10);
	for (int i = 0; i < 1000; i++) {
		int size = rand() % 10 + 2;
		Element* elm = new Asteroid(rand() % screenX, rand() % screenY, size, size);
		elm->setTexture(star.getTexture());
		starLayer1.addElement(elm);
	}

	//LAYER OF STARS IN FRONT OF FAR BACK
	SALayer starLayer2 = SALayer(&play, screenX, screenY);
	starLayer2.setMovementSpeedToPlayer(50);
	for (int i = 0; i < 50; i++) {
		int size = rand() % 10 + 10;
		Element* elm = new Asteroid(rand() % screenX, rand() % screenY, size, size);
		elm->setTexture(star.getTexture());
		starLayer2.addElement(elm);
	}

	//LAYER OF STARS CLOSEST
	SALayer starLayer3 = SALayer(&play, screenX, screenY);
	starLayer3.setMovementSpeedToPlayer(80);
	for (int i = 0; i < 10; i++) {
		int size = rand() % 20 + 10;
		Element* elm = new Asteroid(rand() % screenX, rand() % screenY, size, size);
		elm->setTexture(star.getTexture());
		starLayer3.addElement(elm);
	}

	//LAYER OF ASTEROIDS
	SALayer astroidLayer = SALayer(&play, screenX, screenY);
	astroidLayer.setMovementSpeedToPlayer(100);
	for (int i = 0; i <20; i++) {
		int x = rand() % 100 + 20;
		Element* elm = new Asteroid(rand() % (screenX + 300), rand() % (screenY + 300), x,x);
		elm->setMaxSpeed(10);
		elm->setTexture(Asteroidtextures[rand() % 5]->getTexture());
		astroidLayer.addElement(elm);
	}
	astroidLayer.setCollidable(true);

	//LAYER OF ENEMIES
	EnemyLayer enemyLayer = EnemyLayer(&play, screenX, screenY);

	//ADDING EVERYTHING TO THE GAME
	game.addBackLayer(&starLayer1);
	game.addBackLayer(&starLayer2);
	game.addBackLayer(&starLayer3);
	game.addForeLayer(&enemyLayer);
	game.addForeLayer(&astroidLayer);
	game.setPlayer(&play);

	//RUN!!
	game.run(30);
	//DESTROY :C
	game.~Game();
	
	return 0;
}