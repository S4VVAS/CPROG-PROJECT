#include "Element.h"


Element::Element(int x, int y, int w, int h) {
	SDL_Rect rect = { x, y, w, h };
	this->rect = rect;

}

void Element::moveFromCurrent(int x, int y) {
	this->rect.x = this->rect.x + x;
	this->rect.y = this->rect.y + y;

}

void Element::draw(SDL_Renderer* ren) {
	if (texture == NULL) {
		SDL_SetRenderDrawColor(ren, r, g, b, a);
		SDL_RenderFillRect(ren, &rect);
		SDL_RenderDrawRect(ren, &rect);
	}
	else
		SDL_RenderCopy(ren, texture, NULL, &rect);
}

void Element::moveToNew(int x, int y) {
	this->rect.x = x;
	this->rect.y = y;
}

void Element::setColor(int r, int g, int b, int a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void Element::setTexture(SDL_Texture* texture) {
	this->texture = texture;
}