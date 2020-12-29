#include "ScalableRect.h"
#include "Slider.h"

class NineSlice {
public:
	NineSlice(float x, float y, float w, float h, float left_slider, float right_slider, float top_slider, float bottom_slider)
		:_left_slider (left_slider),_right_slider(right_slider),_top_slider(top_slider),_bottom_slider(bottom_slider)
	{
		rt.setPosition(x, y);
		rt.setSize({ w,h });
	}
private:
	sf::RectangleShape rt;
	float _left_slider;
	float _right_slider;
	float _top_slider;
	float _bottom_slider;
};


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
