#ifndef INCLUDED_SCALABLE_RECT_H
#define INCLUDED_SCALABLE_RECT_H

#include "Utils.h"
#include <SFML/Graphics.hpp>


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

#endif // ! INCLUDED_SCALABLE_RECT_H
