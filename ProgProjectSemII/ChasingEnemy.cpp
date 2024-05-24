#include "ChasingEnemy.h"

ChasingEnemy::ChasingEnemy()
{
	setupSprite();
	spawn();

}

void ChasingEnemy::setupSprite()
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/chasing_enemy.png"))
	{
		std::cout << "error with chasing enemy texture";
	}

	m_sprite.setOrigin(m_spriteDimensions.x / 2, m_spriteDimensions.y / 2); // centre 
	m_sprite.setTexture(m_texture);
	spawn();
	m_sprite.setScale(2.0f, 2.0f);
	m_sprite.setRotation(-90);
}

void ChasingEnemy::update(sf::Vector2f t_playerPosition)
{
	float length;
	float rotationAngle;

	m_velocity = t_playerPosition - m_sprite.getPosition();

	// rotation
	rotationAngle = atan2(m_velocity.x, m_velocity.y) * 180 / PI; // angle in degrees
	m_sprite.setRotation(-rotationAngle - 90);

	length = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);

	move(length);
	animateTexture();
}

void ChasingEnemy::waveUpdate(sf::Vector2f t_waveVelocity) // different update when wave is released
{
	m_velocity.x = t_waveVelocity.x / 1.5; // slows down movement
	m_velocity.y = t_waveVelocity.y / 1.5;
	waveMove();
	animateTexture();
}

void ChasingEnemy::animateTexture() // animation
{
	m_lastFrame = m_currentFrame;
	m_frameCounter += m_frameIncrement;
	m_currentFrame = static_cast<int>(m_frameCounter) % m_noFrames;

	if (m_lastFrame != m_currentFrame) // animate the button
	{
		m_sprite.setTextureRect(sf::IntRect(m_currentFrame * m_spriteDimensions.x, 0, m_spriteDimensions.x, m_spriteDimensions.y));
	}

}

sf::Sprite ChasingEnemy::getSprite() // returns body
{
	return m_sprite;
}

void ChasingEnemy::move(float t_length)
{
	if (t_length > m_speed) // stops jittering at one spot
	{
		m_velocity /= t_length; // velocity is then divided by length
		m_velocity *= m_speed; // multiplies the velocity by the speed to extend the vector
		m_location += m_velocity; // changes location of the enemy
		m_sprite.setPosition(m_location); // sprites are set to the position of the location
	}

}

void ChasingEnemy::waveMove()
{
	m_location += m_velocity;
	m_sprite.setPosition(m_location);
}

void ChasingEnemy::spawn() // generates random spawn outside of the screen borders
{
	int side = (rand() % 4) + 1; // 1-4 for north, south, east, west
	int xCoordinate = 0;
	int yCoordinate = 0;
	int border = 100; // size of the border past the screen borders

	switch (side) // in which part the respawning happens
		// based on whether it is NORTH/SOUTH or EAST/WEST, the other coordinate left gets randomized
	{
		case NORTH:
		{
			xCoordinate = rand() % static_cast<int>(SCREEN_WIDTH); // random coordinate on the x axis
			yCoordinate = 0 - border; // NORTH past screen border
			break;
		}
		case SOUTH:
		{
			xCoordinate = rand() % static_cast<int>(SCREEN_WIDTH);
			yCoordinate = SCREEN_HEIGHT + border; // SOUTH past screen border
			break;
		}
		case EAST:
		{
			xCoordinate = SCREEN_WIDTH + border; // EAST past screen border
			yCoordinate = rand() % static_cast<int>(SCREEN_HEIGHT); // random coordinate on the y axis
			break;
		}
		case WEST:
		{
			xCoordinate = 0 - border; // WEST past screen border
			yCoordinate = rand() % static_cast<int>(SCREEN_HEIGHT); // random coordinate on the y axis
			break;
		}
	}

	m_location = sf::Vector2f(xCoordinate, yCoordinate);
	m_sprite.setPosition(m_location);
}
