#include "ScalableRect.h"
#include "Selector.h"


sf::Vector2f ScalableRect::start_difference;


ScalableRect::ScalableRect(float x, float y, float w, float h)
{
	rt.setPosition(x, y);
	rt.setSize({ w, h });
}

void ScalableRect::update(sf::RenderWindow& w)
{
	const float SIDE_BORDER = 15.f;

	if (Selector::is_selecting_by(sf::Mouse::Left))
	{
		sf::Vector2f p{ sf::Mouse::getPosition(w) };
		auto rect = rt.getGlobalBounds();
		if (rect.top - SIDE_BORDER < p.y && p.y < rect.height + rect.top + SIDE_BORDER)
		{
			if (std::fabsf(rect.left - p.x) < SIDE_BORDER) slide_dir_resizer = UiDir::Left;
			else if (std::fabsf(rect.left + rect.width - p.x) < SIDE_BORDER) slide_dir_resizer = UiDir::Right;
		}
		if (rect.left - SIDE_BORDER < p.x && p.y < rect.left + rect.width + SIDE_BORDER)
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
	if (slide_dir_resizer != UiDir::None)
	{
		sf::Vector2f m{ sf::Mouse::getPosition(w) };

		if (slide_dir_resizer == UiDir::Middle)
		{
			rt.setPosition(sf::Vector2f{ sf::Mouse::getPosition(w) } + start_difference);
		}
		if (slide_dir_resizer == UiDir::Right || slide_dir_resizer == UiDir::TopRight || slide_dir_resizer == UiDir::BottomRight)
		{
			rt.setSize({ m.x - rt.getPosition().x, rt.getSize().y });
		}
		if (slide_dir_resizer == UiDir::Bottom || slide_dir_resizer == UiDir::BottomLeft || slide_dir_resizer == UiDir::BottomRight)
		{
			rt.setSize({ rt.getSize().x, m.y - rt.getPosition().y });
		}
		if (slide_dir_resizer == UiDir::Top || slide_dir_resizer == UiDir::TopLeft || slide_dir_resizer == UiDir::TopRight)
		{
			sf::Vector2f p = rt.getPosition();
			sf::Vector2f sz = rt.getSize();

			rt.setPosition(p.x, m.y);
			rt.setSize({ sz.x, sz.y + p.y - m.y });
		}
		if (slide_dir_resizer == UiDir::Left || slide_dir_resizer == UiDir::TopLeft || slide_dir_resizer == UiDir::BottomLeft)
		{
			sf::Vector2f p = rt.getPosition();
			sf::Vector2f sz = rt.getSize();

			rt.setPosition(m.x, p.y);
			rt.setSize({ sz.x + p.x - m.x, sz.y });
		}
	}
}