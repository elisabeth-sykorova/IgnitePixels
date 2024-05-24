#pragma once

#include "SFML/Graphics.hpp" 
#include "SFML/Audio.hpp" 
#include <iostream>
#include <cstdlib>  // include support for randomizing
#include <ctime>   // supports ctime function
#include "Globals.h"

class FleeingEnemy
{
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_spriteDimensions{ 31.0f, 28.0f }; // the dimensions of one frame

	sf::Vector2f m_velocity{ 0.0f,0.0f };

	float m_speed{ 2.2f };

	sf::Vector2f m_location{ SCREEN_WIDTH/4,SCREEN_HEIGHT/4 }; // vector to save the position of the sprite

	// animation
	int m_lastFrame = 0;
	float m_frameCounter = 0.0f;
	int m_currentFrame = 0;
	float m_frameIncrement = 0.15f;
	int m_noFrames = 8;


public:

	FleeingEnemy();

	void setupSprite();
	void animateTexture();

	sf::Sprite getSprite();

	void update(sf::Vector2f t_playerPosition);
	void move(float t_length);

	void spawn();

};