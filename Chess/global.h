#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

//#ifndef Vec
//#define Vec
//struct Vector2i {
//	int x;
//	int y;
//	Vector2i(int x, int y) { this->x = x; this->y = y; }
//};
//#endif // !Vec

/*
___________________________
       black false

	   white true
___________________________

white is under black
*/


#define TABLE_SIZE 8

#ifndef PIECES
enum class Pieces {
	KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, NONE
};
#define PIECES
#endif

#ifndef ChessStruct
struct Str {
public:
	Str(Pieces type, bool color) { this->type = type; this->color = color; }
	Str() = default;
	Pieces type;
	bool color;
	/*Str operator= (Str str) {
		this->color = str.color;
		this->type = str.type;
	}*/
};
#define ChessStruct
#define White 1
#define Black 0
#endif // !ChessStruct

#ifndef FIELD
#define FIELD
class Field {
private:
	Str data[TABLE_SIZE][TABLE_SIZE];
public:
	Field() {
		for (int i = 0; i < TABLE_SIZE; i++) {
			for (int j = 0; j < TABLE_SIZE; j++) {
				data[i][j].type = Pieces::NONE;
				data[i][j].color = Black;
			}
		}
	}

	bool setFigure(Vector2i pos, bool color, Pieces pp) {
		if (pp == Pieces::NONE) return false;
		if (this->data[pos.y][pos.x].type != Pieces::NONE) {
			return false;
		}
		else {
			this->data[pos.y][pos.x] = Str(pp, color);
			return true;
		}
	}

	Str& operator() (Vector2i pos) {
		return this->data[pos.y][pos.x];
	}
};

#endif // !


#ifndef GLOBAL_FUNC
#define GLOBAL_FUNC
#define WIN_SIZE 800
#define TEXTURE_SIZE 100

constexpr short STEP = WIN_SIZE / 8;

//using P = Pieces;
void Start(Field* ff) noexcept {
	for (int i = 0; i < TABLE_SIZE; i++) {
		ff->setFigure(Vector2i(i, 1), Black, Pieces::PAWN);
		ff->setFigure(Vector2i(i, 6), White, Pieces::PAWN);
	}

	ff->setFigure(Vector2i(0, 0), Black, Pieces::ROOK); // ладьи
	ff->setFigure(Vector2i(7, 0), Black, Pieces::ROOK);
	ff->setFigure(Vector2i(0, 7), White, Pieces::ROOK);
	ff->setFigure(Vector2i(7, 7), White, Pieces::ROOK);

	ff->setFigure(Vector2i(1, 0), Black, Pieces::KNIGHT); // конь
	ff->setFigure(Vector2i(6, 0), Black, Pieces::KNIGHT);
	ff->setFigure(Vector2i(1, 7), White, Pieces::KNIGHT);
	ff->setFigure(Vector2i(6, 7), White, Pieces::KNIGHT);

	ff->setFigure(Vector2i(2, 0), Black, Pieces::BISHOP); // слон
	ff->setFigure(Vector2i(5, 0), Black, Pieces::BISHOP);
	ff->setFigure(Vector2i(2, 7), White, Pieces::BISHOP);
	ff->setFigure(Vector2i(5, 7), White, Pieces::BISHOP);

	ff->setFigure(Vector2i(4, 0), Black, Pieces::KING);
	ff->setFigure(Vector2i(3, 0), Black, Pieces::QUEEN);

	ff->setFigure(Vector2i(4, 7), White, Pieces::KING);
	ff->setFigure(Vector2i(3, 7), White, Pieces::QUEEN);
}	

bool rightMove(Vector2i pos, Vector2i move, Field* field);

bool BishopMove(Vector2i pos, Vector2i move, Field& field) {
	int x = pos.x - move.x;
	int y = pos.y - move.y;

	if (abs(x) != abs(y)) return false;
	
	int xstep = (x > 0) ? 1 : -1;
	int ystep = (y > 0) ? 1 : -1;
	int xpos = pos.x;
	int ypos = pos.y;

	for (; xpos != move.x || ypos != move.y; xpos += xstep, ypos += ystep) {
		if (field(Vector2i(xpos, ypos)).type != Pieces::NONE) return false;
	}

	return true;
}

bool RookMove(Vector2i pos, Vector2i move, Field& field) {
	int x = pos.x - move.x;
	int y = pos.y - move.y;

	if (x == 0) {
		for (int i = min(pos.y, move.y); i <= max(pos.y, move.y); i++) {
			if (field(Vector2i(x, i)).type != Pieces::NONE) return false;
		}
		return true;
	}
	else if (y == 0) {
		for (int i = min(pos.x, move.x); i <= max(pos.x, move.x); i++) {
			if (field(Vector2i(y, i)).type != Pieces::NONE) return false;
		}
		return true;
	}
	return false;
}

bool QueenMove(Vector2i pos, Vector2i move, Field& field) {
	int x = pos.x - move.x;
	int y = pos.y - move.y;

	if (x == 0 && y != 0 || x != 0 && y == 0) {
		return RookMove(pos, move, field);
	}
	else if (abs(x) == abs(y)) {
		return BishopMove(pos, move, field);
	}
	return false;
}

bool KingMove(Vector2i pos, Vector2i move, Field& field) {
	int x = pos.x - move.x;
	int y = pos.y - move.y;

	if (abs(x) > 1 || abs(y) > 1) return false;

	for (int i = 0; i < TABLE_SIZE; i++) {
		for (int j = 0; j < TABLE_SIZE; j++) {
			if (field(Vector2i(i, j)).color != field(pos).color) {
				if (field(Vector2i(i, j)).type == Pieces::KING) continue;
				if (rightMove(Vector2i(i, j), move, &field)) return false;
			}
		}
	}
	return true;
}

bool KnightMove(Vector2i pos, Vector2i move, Field& field) {
	int x = pos.x - move.x;
	int y = pos.y - move.y;

	if (abs(x) == 1 && abs(y) == 2 || abs(x) == 2 && abs(y) == 1) {
		if (field(move).color == field(pos).color) return false;
		else return true;
	}
	else return false;
}

bool PawnMove(Vector2i pos, Vector2i move, Field& field) {
	int x = pos.x - move.x;
	int y = pos.y - move.y;

	if (field(pos).color == White) {
		if (x == 0 && y == 1 && field(move).type == Pieces::NONE) return true;
		else if (abs(x) == 1 && y == -1 && field(move).type != Pieces::NONE 
			&& field(move).color != field(pos).color) 
			return true;
	}
	else {
		if (x == 0 && y == -1 && field(move).type == Pieces::NONE) return true;
		else if (abs(x) == 1 && y == 1 && field(move).type != Pieces::NONE
			&& field(move).color != field(pos).color)
			return true;
	}
	return false;
}

bool rightMove(Vector2i pos, Vector2i move, Field* field) {
	switch ((*field)(pos).type)
	{
	case Pieces::KING:
		return KingMove(pos, move, *field);
		break;
	case Pieces::KNIGHT:
		return KnightMove(pos, move, *field);
		break;
	case Pieces::PAWN:
		return PawnMove(pos, move, *field);
		break;
	case Pieces::BISHOP:
		return BishopMove(pos, move, *field);
		break;
	case Pieces::QUEEN:
		return QueenMove(pos, move, *field);
		break;
	case Pieces::ROOK:
		return RookMove(pos, move, *field);
		break;
	default:
		break;
	}
}
#endif

#ifndef WIN
RenderWindow win(VideoMode(WIN_SIZE, WIN_SIZE), "Chess");
#define WIN
#endif