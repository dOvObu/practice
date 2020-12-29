#include "Selector.h"
void* Selector::someone_selected{ nullptr };
bool  Selector::is_selecting_by  (sf::Mouse::Button btn) { return someone_selected == nullptr && sf::Mouse::isButtonPressed(btn); }
bool  Selector::try_to_release_by(sf::Mouse::Button btn) { return !sf::Mouse::isButtonPressed(btn) ? someone_selected = nullptr, true : false; }