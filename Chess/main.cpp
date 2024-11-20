#include "draw.h"

int main() {
	win.setFramerateLimit(30);

	const int xm = -8;
	const int ym = -30;

	Draw draw(&win);

	/*RectangleShape rect;
	rect.setPosition(0, 0);
	rect.setFillColor(Color::Green);
	rect.setSize(Vector2f(100, 100));*/

	while (win.isOpen()) {
		Event event;
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed) win.close();
		}

		Vector2i mouse = Mouse::getPosition();
		Vector2i wpos = win.getPosition();
		Vector2i result = Vector2i(mouse.x - wpos.x + xm, mouse.y - wpos.y + ym);
		draw.canSel(result);

		if (Mouse::isButtonPressed(Mouse::Left)) {
			draw.select();
		}

		win.clear();
		draw.draw();
		//win.draw(rect);
		win.display();
	}
}