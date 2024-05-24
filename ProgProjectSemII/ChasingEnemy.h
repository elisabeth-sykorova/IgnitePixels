#pragma once

#include "SFML/Graphics.hpp" 
#include <iostream>
#include <cstdlib>  // include support for randomizing
#include <ctime>   // supports ctime function
#include "Globals.h"

class ChasingEnemy
{
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_spriteDimensions{ 34.0f, 46.0f }; // dimensions of one frame

	sf::Vector2f m_velocity{ 0.0f,0.0f };

	float m_speed{ 3.0f };

	sf::Vector2f m_location{ SCREEN_WIDTH,SCREEN_HEIGHT };

	// animation
	int m_lastFrame = 0;
	float m_frameCounter = 0.0f;
	int m_currentFrame = 0;
	float m_frameIncrement = 0.3f;
	int m_noFrames = 6;



public:

	ChasingEnemy();

	void setupSprite();
	void animateTexture();

	sf::Sprite getSprite();

	void update(sf::Vector2f t_playerPosition);
	void waveUpdate(sf::Vector2f t_waveVelocity);
	void move(float t_length);
	void waveMove();

	void spawn();



};

