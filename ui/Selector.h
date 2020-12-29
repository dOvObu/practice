#ifndef INCLUDED_SELECTOR_H
#define INCLUDED_SELECTOR_H
#include <SFML/Window/Mouse.hpp>

class Selector {
public:
	static bool is_selecting_by  (sf::Mouse::Button btn);
	static bool try_to_release_by(sf::Mouse::Button btn);
	template<typename T> static bool has_selected(T* p) { return reinterpret_cast<void*>(p) == someone_selected; }
	template<typename T> static T&   selected   (    ) { return *reinterpret_cast<T*>(someone_selected); }
	template<typename T> static void select     (T* p) { someone_selected = p; }
	static void release() { someone_selected = nullptr; }
private:
	static void* someone_selected;
};

#endif // ! INCLUDED_SELECTOR_H
