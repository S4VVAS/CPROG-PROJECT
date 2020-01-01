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

#include "EXP_ELM.h"
#include "EXP_PLAY.h"

const int WALL = 10;
const int PLAYER = 10;
const int screenX = 1000, screenY = 700;

int main(int argc, char* argv[]) {
	GameWindow win("Hello", screenX, screenY);
	PhysicsHandler physics(0, screenX, screenY);
	Background back(screenX, screenY);
	
	Game game(&win, &physics, &back);
	Texture texture(win.ren, "C:/Users/savva/source/repos/CPROG-PROJECT/SDL2-Exp/Images/hello.jpg");




	back.setColor(100,100,0,255);

	EXP_PLAY play(600,300, 60, 40, PLAYER);
	play.setTexture(texture.getTexture());
	play.collidable = true;
	play.setMaxSpeed(100);
	

	Layer lay = Layer(&play);
	lay.addElement(new EXP_ELM(rand() % 1000, rand() % 700, 100, 100, WALL));
	lay.addElement(new EXP_ELM(rand() % 1000, rand() % 700, 10, 100, WALL));
	lay.addElement(new EXP_ELM(rand() % 1000, rand() % 700, 100, 10, WALL));
	lay.addElement(new EXP_ELM(0, screenY - 100, screenX, 100, WALL));
	game.addLayer(&lay);

	

	std::vector<Element*> elements;
	for (int i = 0; i < 1; i++) {
		elements.push_back(new EXP_ELM(rand() % 1000, rand() % 700, 10, 10, WALL));
	}
	for(Element* e : elements){
		e->collidable = true;
		e->setMaxSpeed(100);
		e->setElasticity(0);
		e->setColor(155, 133, 0, 255);
		e->setTexture(texture.getTexture());
		game.addElement(e);
	}
	elements.clear();
	
	game.setPlayer(&play);


	game.run(30);

	game.~Game();
	
	return 0;
}