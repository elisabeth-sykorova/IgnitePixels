#include "FleeingEnemy.h"

FleeingEnemy::FleeingEnemy()
{
	setupSprite();
	spawn();

}

// setting up sprite
void FleeingEnemy::setupSprite() 
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/fleeing_enemy.png"))
	{
		std::cout << "error with player texture";
	}

	m_sprite.setOrigin(m_spriteDimensions.x / 2, m_spriteDimensions.y / 2);
	m_sprite.setTexture(m_texture);
	spawn();
	m_sprite.setScale(2.5f, 2.5f);
	m_sprite.setRotation(90);

	m_sprite.setTextureRect(sf::IntRect(m_currentFrame * m_spriteDimensions.x, 0, m_spriteDimensions.x, m_spriteDimensions.y));
}

// returns sprite
sf::Sprite FleeingEnemy::getSprite()
{
	return m_sprite;
}

// rotates the sprite and calls the move and animation function
void FleeingEnemy::update(sf::Vector2f t_playerPosition)
{
	float length;
	float rotationAngle;

	m_velocity = t_playerPosition - m_location;
	
	// rotate sprite
	rotationAngle = atan2(m_velocity.x, m_velocity.y) * 180 / PI; // angle in degrees
	m_sprite.setRotation(-rotationAngle + 90);

	length = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);

	move(length);
	animateTexture();
}

void FleeingEnemy::animateTexture()
{
	m_lastFrame = m_currentFrame;
	m_frameCounter += m_frameIncrement;
	m_currentFrame = static_cast<int>(m_frameCounter) % m_noFrames;

	if (m_lastFrame != m_currentFrame) // animate the button
	{
		m_sprite.setTextureRect(sf::IntRect(m_currentFrame * m_spriteDimensions.x, 0, m_spriteDimensions.x, m_spriteDimensions.y));
	}

}

// length calculated in update is the distance between the player and the enemy
void FleeingEnemy::move(float t_length) 
{
	if ((m_location.x > 0 - m_spriteDimensions.x && m_location.x < SCREEN_WIDTH + m_spriteDimensions.x) && // stops behind boundaries
		(m_location.y > 0 - m_spriteDimensions.y && m_location.y < SCREEN_HEIGHT + m_spriteDimensions.y)) 
	{
		m_velocity /= t_length; // velocity is then divided by length
		m_velocity *= m_speed; // multiplies the velocity by the speed to extend the vector
		m_location -= m_velocity; // changes location of the enemy
		m_sprite.setPosition(m_location); // sprites are set to the position of the location
	}
	else
	{
		spawn(); // respawn
	}

}

// generates a random location inside the borders but not too close to the edges
void FleeingEnemy::spawn() 
{
	int border = 100; // outer border for spawning so the enemies dont spawn too close to the edge and despawn again instantly
	int xCoordinate = rand() % static_cast<int>(SCREEN_WIDTH - border) + border; // random number 
	int yCoordinate = rand() % static_cast<int>(SCREEN_HEIGHT - border) + border;

	m_location = sf::Vector2f(xCoordinate, yCoordinate);
}
