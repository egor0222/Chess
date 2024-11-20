#pragma once
#include "global.h"
#include "TextureLoader.h"

std::string PieceToStr(Pieces pp) {
	switch (pp)
	{
	case Pieces::KING:
		return "king";
	case Pieces::KNIGHT:
		return "knight";
	case Pieces::PAWN:
		return "pawn";
	case Pieces::BISHOP:
		return "bishop";
	case Pieces::QUEEN:
		return "queen";
	case Pieces::ROOK:
		return "rook";
	default:
		return "";
	}
}




class Draw {
private:
	TextureLoader tl;
	bool selFirst;
	Vector2i firstSelected;
	Vector2i canSelected;

	bool StepColor;

	Field ff;
public:
	Draw(RenderWindow* rw) {
		tl.init();
		this->selFirst = false;
		this->firstSelected = { 0, 0 };
		Start(&ff);
		this->StepColor = White;
	}

	void draw() {
		bool color = true;
		for (int i = 0; i < TABLE_SIZE; i++) {
			for (int j = 0; j < TABLE_SIZE; j++) {
				Texture* text;
				if (color) text = this->tl("white");
				else text = this->tl("black");
				RectangleShape rect;
				rect.setTexture(text);
				rect.setPosition(j * STEP, i * STEP);
				rect.setSize(Vector2f(TEXTURE_SIZE, TEXTURE_SIZE));
				win.draw(rect);
				color = !color;
			}
			color = !color;
		}
		if (this->canSelected.x >= 0 && this->canSelected.y >= 0) {
			this->RendereSelectedTexture(this->canSelected);
		}
		if (this->selFirst) {
			this->RendereSelectedTexture(this->firstSelected);
		}
		for (int i = 0; i < TABLE_SIZE; i++) {
			for (int j = 0; j < TABLE_SIZE; j++) {
				if (ff(Vector2i(j, i)).type != Pieces::NONE) {
					std::string Typename = PieceToStr(ff(Vector2i(j, i)).type);
					std::string color = (ff(Vector2i(j, i)).color == White) ? "white" : "black";
					Texture* text = this->tl(Typename + color);
					RectangleShape rect;
					rect.setSize(Vector2f(TEXTURE_SIZE, TEXTURE_SIZE));
					rect.setTexture(text);
					rect.setPosition(j * STEP, i * STEP);
					win.draw(rect);
				}
			}
		}
	}

	void canSel(Vector2i pos) {
		int x = pos.x / STEP;
		int y = pos.y / STEP;

		std::cout << x << ' ' << y << std::endl;

		this->canSelected = Vector2i(x, y);
	}

	void select() {
		if (this->selFirst) {
			if (!rightMove(this->firstSelected, this->canSelected, &ff)) return;
			this->FigureMove(this->firstSelected, this->canSelected);
			this->selFirst = false;
			std::cout << "Moved: " << canSelected.x << ' ' << canSelected.y << std::endl;
		}
		else if (ff(this->canSelected).type != Pieces::NONE) {
			this->firstSelected = this->canSelected;
			this->selFirst = true;
			std::cout << "Do not move: " << canSelected.x << ' ' << canSelected.y << std::endl;
		}
		
	}

private:
	void RendereSelectedTexture(Vector2i pos) {
		Texture* text = this->tl("selected");
		RectangleShape rect;
		rect.setSize(Vector2f(TEXTURE_SIZE, TEXTURE_SIZE));
		rect.setTexture(text);
		rect.setPosition(this->canSelected.x * STEP, this->canSelected.y * STEP);
		win.draw(rect);
	}

	void FigureMove(Vector2i from, Vector2i to) {
		ff(to) = ff(from);
		ff(from).type = Pieces::NONE;
	}
};