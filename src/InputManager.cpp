#include "InputManager.hpp"

sf::Vector2u InputManager::GetMousePosition() {
	return _mousePosition;
}

void InputManager::MoveMouse(sf::Vector2u position) {
	_mousePosition = position;
}

void InputManager::PressMouse(sf::Mouse::Button button) {
	_mouseStates[button] = true;
}

void InputManager::ReleaseMouse(sf::Mouse::Button button) {
	_mouseStates[button] = false;
}

void InputManager::PressKey(sf::Keyboard::Key key) {
	_keyStates[key] = true;
}

void InputManager::ReleaseKey(sf::Keyboard::Key key) {
	_keyStates[key] = false;
}

void InputManager::ReleaseAllKeys() {
	for (auto it = _keyStates.begin(); it != _keyStates.end(); ++it) {
		it->second = false;
	}
}

bool InputManager::IsKeyPressed(sf::Keyboard::Key key) {
	if (_keyStates.find(key) == _keyStates.end()) {
		_keyStates[key] = false;
	}
	return _keyStates[key];
}

bool InputManager::IsMousePressed(sf::Mouse::Button button) {
	if (_mouseStates.find(button) == _mouseStates.end()) {
		_mouseStates[button] = false;
	}
	return _mouseStates[button];
}

bool* InputManager::GetKeyRef(sf::Keyboard::Key key) {
	if (_keyStates.find(key) == _keyStates.end()) {
		return &_keyStates[key];
	}
	return nullptr;
}
