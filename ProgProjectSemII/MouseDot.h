#pragma once

#include "SFML/Graphics.hpp" 
#include <iostream>
#include <cstdlib>  // include support for randomizing
#include <ctime>   // supports ctime function
#include "Globals.h"

class MouseDot
{
	sf::CircleShape m_dot; // dot that tracks the mouse cursor


public:

	MouseDot();

	void setupDot();

	sf::CircleShape getDot();

	void move(sf::Vector2f t_mousePosition);


};

