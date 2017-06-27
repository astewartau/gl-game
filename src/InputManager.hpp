#pragma once
#include <map>
#include <SFML\Window.hpp>

class InputManager {
public:
	InputManager() {}
	sf::Vector2u GetMousePosition();
	void MoveMouse(sf::Vector2u position);
	void PressMouse(sf::Mouse::Button button);
	void ReleaseMouse(sf::Mouse::Button button);
	void PressKey(sf::Keyboard::Key key);
	void ReleaseKey(sf::Keyboard::Key key);
	void ReleaseAllKeys();
	bool IsKeyPressed(sf::Keyboard::Key key);
	bool IsMousePressed(sf::Mouse::Button button);
	bool* GetKeyRef(sf::Keyboard::Key key);
private:
	sf::Vector2u _mousePosition;
	std::map<sf::Keyboard::Key, bool> _keyStates;
	std::map<sf::Mouse::Button, bool> _mouseStates;
};