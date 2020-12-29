#include "Slider.h"

sf::Vector2f Slider::start_position;
std::vector<Slider*> Slider::sliders;


Slider::Slider(UiDir type, float length, float x, float y) : orientation(type)
{
	bool const isVertical = orientation == UiDir::Vertical;
	auto const size = isVertical ? sf::Vector2f{ 1.f, length } : sf::Vector2f{ length, 1.f };
	path.setSize(size);
	float const sz = 25.f, const hsz = 0.5f * sz;
	slider.setSize(sf::Vector2f{ sz,sz });
	slider.setOrigin(sf::Vector2f{ hsz,hsz });
	path.setOrigin(isVertical ? size.x * 0.5f : 0.f, isVertical ? 0.f : size.y * 0.5f);

	slider.setPosition(x, y);
	path.setPosition(x, y);
}


void Slider::update(sf::RenderWindow& w)
{
	if (Selector::is_selecting_by(sf::Mouse::Left))
	{
		sf::Vector2f p{ sf::Mouse::getPosition(w) };
		if (slider.getGlobalBounds().contains(p))
		{
			start_position = p;
			Selector::select(this);
		}
	}
	else if (Selector::has_selected(this))
	{
		update_sliding_position(w);
		Selector::try_to_release_by(sf::Mouse::Left);
	}
}


void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(path);
	target.draw(slider);
}


Slider* Slider::make(UiDir vertical, float length, float x, float y)
{
	sliders.push_back(new Slider(vertical, length, x, y));
	return sliders.back();
}


void Slider::clear()
{
	for (auto slider : sliders) delete slider;
	sliders.clear();
}


void Slider::update_sliding_position(sf::RenderWindow& w)
{
	auto p = sf::Mouse::getPosition(w);
	auto path_size = path.getSize();
	auto path_position = path.getPosition();
	bool const isVertical = orientation == UiDir::Vertical;
	sf::Vector2f slider_position{

		isVertical
			? slider.getPosition().x
			: std::fmaxf(path_position.x, std::fminf(p.x, path_position.x + path_size.x)),

		isVertical
			? std::fmaxf(path_position.y, std::fminf(p.y, path_position.y + path_size.y))
			: slider.getPosition().y

	};
	slider.setPosition(slider_position);
	value = isVertical
		? (slider_position.y - path_position.y) / path_size.y
		: (slider_position.x - path_position.x) / path_size.x;
}

