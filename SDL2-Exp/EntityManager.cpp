#include "EntityManager.h"

EntityManager::~EntityManager() {
	for (std::vector<Element*>::iterator it = elements.begin(); it != elements.end();) {
		delete* it;
		it = elements.erase(it);
	}
	elements.clear();

	for (std::vector<Layer*>::iterator it = bLayer.begin(); it != bLayer.end();) {
		delete* it;
		it = bLayer.erase(it);
	}
	bLayer.clear();

	for (std::vector<Layer*>::iterator it = fLayer.begin(); it != fLayer.end();) {
		delete* it;
		it = fLayer.erase(it);
	}
	fLayer.clear();

	delete physics;
}

EntityManager::EntityManager(PhysicsHandler* physics) : physics(physics){
	screenX = physics->screenX;
	screenY = physics->screenY;
}

void EntityManager::setPlayer( PlayableEntity* player) {
	this->player = player;
	elements.push_back(player);
}

void EntityManager::addElement( Element* elm) {
	elements.push_back(elm);

}

void EntityManager::removeElement(const Element* comp) {

	for (std::vector<Element*>::iterator it = elements.begin(); it != elements.end();) {
		if (*it == comp) {
			delete* it;
			it = elements.erase(it);
		}
		else
			it++;
	}

	std::cout << elements.size();
}

void EntityManager::addForeLayer(Layer* layer){
	fLayer.push_back(layer);
}

void EntityManager::addBackLayer(Layer* layer) {
	bLayer.push_back(layer);
}

void EntityManager::removeLayer(Layer* layer){
	for (int i = 0; i < fLayer.size(); i++) 
		if (fLayer.at(i) == layer) {
			fLayer.at(i)->~Layer();
			fLayer.erase(fLayer.begin() + i);
		}
	for (int i = 0; i < bLayer.size(); i++) 
		if (bLayer.at(i) == layer) {
			bLayer.at(i)->~Layer();
			bLayer.erase(bLayer.begin() + i);
		}
}

std::vector<Element*> EntityManager::getEntities() {
	return elements;
}

bool EntityManager::outOfBounds(Element* e){
	if (e->getRect()->x > screenX + 10 || e->getRect()->x + e->getRect()->w < -10 ||
		e->getRect()->y > screenY + 10 || e->getRect()->y + e->getRect()->h < -10)
		return true;
	return false;
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

void EntityManager::updateElements(SDL_Renderer* ren) {
//	std::cout << "Amt of elements: " << elements.size() << std::endl;

	for (Layer* l : bLayer) { //Handles all backLayers
		if (scrollingMap) //Moves layer in opposite dir of player vectors.
			l->moveLayer(-player->getXVector() * l->getMovementSpeed(), -player->getYVector() * l->getMovementSpeed());
		l->tickLayer();
		l->setMovedLayer();
		l->drawLayer(ren);
		l->tickLayer();
	}

	//SHOOT FIX LATER SOMETHING WIERD IS GOING ON
	Element* shot = player->shoot();
	if (shot != nullptr){
		addElement(shot); //Takes the shot element and adds it to element vector
	}
	else
		delete shot;
		
	bool colFound = false; //declare var outside loop so i dont have to do it in every itteration of the loop

	for (std::vector<Element*>::iterator it = elements.begin(); it != elements.end(); ) {  // Handles all elements
		colFound = false;

		physics->applyGravityVector((*it));

		if ((*it) != player && scrollingMap)
			(*it)->moveFromCurrent(-player->getXVector(), -player->getYVector());

		if ((*it) == player && !scrollingMap)
			if (physics->windowElementCollide(*it))
				colFound = true;

		if ((*it)->isCollidable()) {
			

		//THIS IS FOR LAYER COLLISION WHICH DOESNT WORK PROPPERLY yet :(
			for (Layer* l : fLayer) {
				if (l->isCollidable()) {
					for (Element* e2 : *(l->getElements())) {
						if (physics->elementsCollide((*it), e2) && physics->elementsCollide(e2, (*it)))
							colFound = true;
					}
				}
			}

			for (Element* e2 : elements) {
				if (e2->isCollidable()) {
					if (colFound)
						break;
					if (physics->elementsCollide((*it), e2))
						colFound = true;
				}
			}
		}

		if ((*it) == player && scrollingMap)	//Resets the player rect, so that the player stays in the same spot when map is scrolling.
			player->resetMovedRect();

		//if (!colFound)
		(*it)->tick();
		(*it)->setMovedRect();
		(*it)->draw(ren);
		

		if ((*it) != player && (outOfBounds((*it)) || (*it)->isDead())) {
			delete* it;
			it = elements.erase(it);
		}
		else
			it++;


	}
	
	for (Layer* l : fLayer) {
		for (Element* e1 : *(l->getElements())) { //Handles colisions within a flayer
			physics->applyGravityVector(e1);
			colFound = false;
			for (Element* e2 : *(l->getElements())) {
				if (colFound)
					break;
				if (physics->elementsCollide(e1, e2))
					colFound = true;
			}
		}

		for (Layer* l2 : fLayer) { //Handles collisions between layers
			for (Element* e1 : *(l->getElements())) {
				colFound = false;
				for (Element* e2 : *(l2->getElements())) {
					if (colFound)
						break;
					if (physics->elementsCollide(e1, e2))
						colFound = true;
				}
			}
		}
		
		if (scrollingMap)	//Moves layer in opposite dir of player vectors.
			l->moveLayer(-player->getXVector() * l->getMovementSpeed(), -player->getYVector() * l->getMovementSpeed());
		l->setMovedLayer();
		l->tickLayer();
		l->drawLayer(ren);
	}

}



