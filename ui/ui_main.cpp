#include "Slider.h"
#include <iostream>

class ScalableRect : public sf::Drawable {
public:
	ScalableRect(float x, float y, float w, float h);
	void update(sf::RenderWindow& w);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::RectangleShape rt;
	UiDir slide_dir_resizer{ UiDir::None };

	void update_position(sf::RenderWindow& w);

	static sf::Vector2f start_difference;
};
sf::Vector2f ScalableRect::start_difference;


ScalableRect::ScalableRect(float x, float y, float w, float h)
{
	rt.setPosition(x, y);
	rt.setSize({ w, h });
	rt.setFillColor(sf::Color(100,100,100));
}

void ScalableRect::update(sf::RenderWindow& w)
{
	const float SIDE_BORDER = 5.f;

	if (Selector::is_selecting_by(sf::Mouse::Left))
	{
		sf::Vector2f p{ sf::Mouse::getPosition(w) };
		auto rect = rt.getGlobalBounds();
		if (rect.top < p.y && p.y < rect.height + rect.top)
		{
			if (std::fabsf(rect.left - p.x) < SIDE_BORDER) slide_dir_resizer = UiDir::Left;
			else if (std::fabsf(rect.left + rect.width - p.x) < 5.f) slide_dir_resizer = UiDir::Right;
		}
		if (rect.left < p.x && p.y < rect.left + rect.width)
		{
			if (std::fabs(rect.top - p.y) < SIDE_BORDER)
			{
				slide_dir_resizer
					= slide_dir_resizer == UiDir::Left ? UiDir::TopLeft
					: slide_dir_resizer == UiDir::Right ? UiDir::TopRight
					: UiDir::Top;
			}
			else if (std::fabs(rect.top + rect.height - p.y) < SIDE_BORDER)
			{
				slide_dir_resizer
					= slide_dir_resizer == UiDir::Left ? UiDir::BottomLeft
					: slide_dir_resizer == UiDir::Right ? UiDir::BottomRight
					: UiDir::Bottom;
			}
		}
		if (slide_dir_resizer == UiDir::None && rect.contains(p))
		{
			slide_dir_resizer = UiDir::Middle;
		}
		if (slide_dir_resizer != UiDir::None)
		{
			if (slide_dir_resizer == UiDir::Middle) start_difference = rt.getPosition() - p;
			else if (slide_dir_resizer == UiDir::Left) start_difference = {rt.getPosition().x, rt.getSize().x};
			Selector::select(this);
		}
	}
	else if (Selector::has_selected(this))
	{
		update_position(w);
		if (Selector::try_to_release_by(sf::Mouse::Left))
		{
			slide_dir_resizer = UiDir::None;
		}
	}
}

void ScalableRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rt);
}

void ScalableRect::update_position(sf::RenderWindow& w)
{
	if (slide_dir_resizer == UiDir::Middle)
	{
		rt.setPosition(sf::Vector2f{ sf::Mouse::getPosition(w) } + start_difference);
	}
	else if (slide_dir_resizer == UiDir::Left)
	{
	}
}

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
