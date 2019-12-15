#include "EntityManager.h"

EntityManager::~EntityManager() {
	elements.clear();
}

void EntityManager::setPlayer( PlayableEntity* player) {
	this->player = player;
	elements.push_back(player);
}

void EntityManager::addElement( Element* elm) {
	elements.push_back(elm);

}

void EntityManager::removeElement(const Element* comp) {
	for (int i = 0; i < elements.size() / sizeof(Element); i++) {
		if (elements.at(i) == comp) {
			elements.erase(elements.begin() + i);
		}
	}
}

std::vector<Element*> EntityManager::getEntities() {
	return elements;
}

void EntityManager::triggerEvent(SDL_Event event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		for (int i = 0; i < elements.size(); i++) {
			elements.at(i)->keyDown(event.key.keysym.sym);
			
		}
		break;
	case SDL_KEYUP:
		for (Element* e : elements) 
			e->keyUp(event.key.keysym.sym);
		break;
	case SDL_MOUSEBUTTONDOWN:
		for (Element* e : elements) 
			e->mouseDown(event.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		for (Element* e : elements)
			e->mouseUp(event.button.button);
		break;
	}
}

int EntityManager::rectsCollide(const SDL_Rect* r1, const SDL_Rect* r2) {
	if (r1->x + r1->w >= r2->x &&
		r2->x + r2->w >= r1->x &&
		r1->y + r1->h >= r2->y &&
		r2->y + r2->h >= r1->y)
		if ((r1->x + r1->w <= r2->x || r2->x + r2->w <= r1->x) && (r1->y + r1->h >= r2->y || r2->y + r2->h >= r1->y))
			return 1;
		else
			return 2;
	return 0;
}

int EntityManager::windowCollide(const SDL_Rect* r1){
	
	return 0;
}
 



void EntityManager::updateElements(SDL_Renderer* ren) {
	for (Element* e : elements) {
		e->colliding = false;
		if (e->collidable) {
			int colDir = 0;
			e->changeVectors(0, 0.32);
			bool colFound = false;
			for (Element* e2 : elements) {
				if (e != e2 && !colFound) {
					colDir = rectsCollide(e->getMovedRect(), e2->getRect());
					
					if (colDir == 1) {
						//e->resetMovedRect();
						e->changeVectors(0 - e->getXVector() * e2->getElasticity(), 0 - e->getYVector() * 0.5);
						std::cout << colDir;
					}
					else if (colDir == 2) {
						//e->resetMovedRect();
						e->changeVectors(0 - e->getXVector() * 0.5, 0 - e->getYVector() * e2->getElasticity());
						std::cout <<  e2->getElasticity();
					}
				}
			}

		}
		
		e->setMovedRect();
		e->tick();
		e->draw(ren);
	}

}

EntityManager::EntityManager() {

}