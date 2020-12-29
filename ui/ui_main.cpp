#include "ScalableRect.h"
#include "Slider.h"
#include <iostream>


int main()
{
	sf::RenderWindow w({ 1280,720 }, "");

	ScalableRect x{ 100,100,200,200 };
	Slider x2(UiDir::Vertical, 200, 400, 60);
	while (w.isOpen())
	{
		sf::Event ev;
		while (w.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				w.close();
			}
		}
		w.clear();
		x2.update(w);
		x.update(w);
		w.draw(x2);
		w.draw(x);
		w.display();
	}
}
