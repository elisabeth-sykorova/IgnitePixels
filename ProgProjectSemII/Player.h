// Player class declaration
#pragma once

#include "SFML/Graphics.hpp" 
#include "SFML/Audio.hpp" 
#include <iostream>
#include <cstdlib>  // include support for randomizing
#include <ctime>   // supports ctime function
#include "Globals.h"

class Player
{
	// private data members

	// textures
	sf::Texture m_texture;
	sf::Texture m_fullTexture;
	sf::Texture m_midTexture;
	sf::Texture m_lowTexture;
	// boost textures
	sf::Texture m_boostFullTexture;
	sf::Texture m_boostMidTexture;
	sf::Texture m_boostLowTexture;

	sf::Sprite m_sprite;

	const sf::Vector2i m_spriteDimensions{ 55,25 };
	sf::Vector2i m_dashBarDimensions{ 13,54 };

	sf::Vector2f m_velocity{ 0.0f, 0.0f };
	float m_speed{ 10.0f };
	const float DEFAULT_SPEED{ 10.0f };
	const float BOOST_SPEED = 18.0f;
	sf::Vector2f m_location{ 0.0f, 0.0f };

	// health bar
	sf::RectangleShape healthBar;
	const float MAX_HEALTH = SCREEN_WIDTH - 80;
	float m_health = MAX_HEALTH; // the value of the health (also the length for the rectangle shape)
	float m_barHeight = 4.0f;

	// boost
	bool m_boosting = false;
	float m_boostValue = 0;
	const float MAX_BOOST_VALUE = 25;
	const float BOOST_INCREMENT = 0.05;
	const float BOOST_DECREMENT = 0.4;

	// abilities
	int m_abilityPoints = 3;
	const int MAX_ABILITY_POINTS = 3;
	sf::Texture m_waveIconTexture;
	sf::Sprite m_waveIconSprite;
	sf::Vector2i m_waveIconDimensions{ 19,40 };

	// animation
	int m_lastFrame = 0;
	float m_frameCounter = 0.0f;
	int m_currentFrame = 0;
	float m_frameIncrement = 0.3f;

	// dash bar
	sf::Texture m_dashBarTexture;
	sf::Sprite m_dashBarSprite;

	// health bar texture
	sf::Texture m_healthBarTexture;
	sf::Sprite m_healthBarSprite;

	// sound effects
	sf::Sound m_boostSound;
	sf::SoundBuffer m_boostBuffer;
	sf::Sound m_pointGainSound;
	sf::SoundBuffer m_pointGainBuffer;

public:

	void setupSprite();
	void loadTextures();
	sf::Sprite getSprite();

	void update(sf::Vector2f t_mousePosition);
	void move(float t_length);
	sf::Vector2f getPosition();

	void setupHealthBar();
	void setupDashBar();
	void setupWaveIcon();
	sf::Sprite getWaveIcon();
	sf::Sprite getDashBar();

	sf::Sprite getHealthBar();

	void decreaseHealth(float t_decreaseAmount);
	void increaseHealth();
	void updateHealthBar();
	void updateTexture();
	void animateTexture();
	void updateDashBar();
	void updateWaveIcon();
	float getHealth();
	sf::Vector2f getVelocity();

	void updateAbilityPoints(int t_amount);
	int getAbilityPoints();
	void setSpeed(float t_newSpeed);
	float getDefaultSpeed();
	float getSpeed();
	float getBoostSpeed();
	bool getBoosting();

	void incrementBoostValue();
	void decrementBoostValue();

	void setBoosting(bool t_boosting);

	void loadSounds();


	void reset();

	Player();
};