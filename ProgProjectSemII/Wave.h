// Player class declaration
#pragma once

#include "SFML/Graphics.hpp" 
#include <iostream>
#include <cstdlib>  // include support for randomizing
#include <ctime>   // supports ctime function
#include "Globals.h"


class Wave
{
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_location{0.0f,0.0f};
	sf::Vector2f m_velocity{ 0.0f,0.0f };
	sf::Vector2f m_spriteDimensions{ 33.0f,96.0f };

	int m_moveCounter = 0; // tracks how many times the wave has moved
	const int MAX_MOVE = 50; // limited movement

	bool m_release = false;

	// animation
	int m_lastFrame = 0;
	float m_frameCounter = 0.0f;
	int m_currentFrame = 0;
	float m_frameIncrement = 0.1f;
	int m_noFrames = 7;

public:

	Wave();
	void setupWave();

	sf::Sprite getSprite();

	void setupRelease(sf::Vector2f t_playerVelocity, sf::Vector2f t_playerPosition);

	void animateTexture();

	void move();
	int getMoveCounter();

	void setRelease(bool t_release);
	bool getRelease();

	sf::Vector2f getVelocity();

	void reset();

};

