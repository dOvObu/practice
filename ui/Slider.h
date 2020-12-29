#ifndef INCLUDED_SLIDER_H
#define INCLUDED_SLIDER_H

#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <vector>


class Slider : public sf::Drawable {
public:
	Slider(UiDir type, float length, float x, float y);
	
	void update(sf::RenderWindow& w);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	static Slider* make(UiDir type, float length, float x, float y);
	static void clear();

private:
	sf::RectangleShape slider, path;
	UiDir orientation{ UiDir::Vertical };
	float value{ 0.f };

	void update_sliding_position(sf::RenderWindow& w);

	static sf::Vector2f start_position;
	static std::vector<Slider*> sliders;
};


#endif // ! INCLUDED_SLIDER_H
