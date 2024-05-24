#include "Wave.h"

Wave::Wave()
{
	setupWave();

}

void Wave::setupWave()
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/wave.png"))
	{
		std::cout << "error with wave texture";
	}

	m_sprite.setTexture(m_texture);
	m_location = sf::Vector2f(SCREEN_WIDTH - m_sprite.getGlobalBounds().width, SCREEN_HEIGHT - m_sprite.getGlobalBounds().height);
	m_sprite.setPosition(m_location);
	m_sprite.setOrigin(m_spriteDimensions.x / 2, m_spriteDimensions.y/ 2);
	m_sprite.scale(3.0f, 3.0f);
	m_sprite.setRotation(-90);
	m_sprite.setTextureRect(sf::IntRect(m_currentFrame * m_spriteDimensions.x, 0, m_spriteDimensions.x, m_spriteDimensions.y));

}

sf::Sprite Wave::getSprite()
{
	return m_sprite;
}

void Wave::setupRelease(sf::Vector2f t_playerVelocity, sf::Vector2f t_playerPosition)
{
	float rotationAngle;

	m_currentFrame = 0;
	m_frameCounter = 0.0f;
	m_lastFrame = 0;

	m_velocity = t_playerVelocity;
	m_velocity.x *= 1.5f; // faster than player
	m_velocity.y *= 1.5f;

	// rotate sprite based on movement direction
	rotationAngle = atan2(m_velocity.x, m_velocity.y) * 180 / PI; // angle in degrees
	m_sprite.setRotation(-rotationAngle -90);

	m_location = t_playerPosition;
	m_sprite.setPosition(m_location);
}

void Wave::animateTexture()
{
	m_lastFrame = m_currentFrame;
	m_frameCounter += m_frameIncrement;
	m_currentFrame = static_cast<int>(m_frameCounter) % m_noFrames;

	if (m_lastFrame != m_currentFrame || m_currentFrame == 0) // animate the button
	{
		m_sprite.setTextureRect(sf::IntRect(m_currentFrame * m_spriteDimensions.x, 0, m_spriteDimensions.x, m_spriteDimensions.y));
	}

}

void Wave::move()
{
	if (m_moveCounter <= MAX_MOVE) // limited movement
	{
		m_location += m_velocity;
		m_sprite.setPosition(m_location);
		m_moveCounter++;
	}
	else
	{
		m_release = false; // once limit is hit, stop releasing
		m_moveCounter = 0;
		m_sprite.setPosition(SCREEN_WIDTH - m_sprite.getGlobalBounds().width, SCREEN_HEIGHT - m_sprite.getGlobalBounds().height);
		m_location = m_sprite.getPosition();
	}

}

int Wave::getMoveCounter()
{
	return m_moveCounter;
}

void Wave::setRelease(bool t_release)
{
	m_release = t_release;
}

bool Wave::getRelease()
{
	return m_release;
}

sf::Vector2f Wave::getVelocity()
{
	return m_velocity;
}

void Wave::reset()
{
	m_location = sf::Vector2f(SCREEN_WIDTH - m_sprite.getGlobalBounds().width, SCREEN_HEIGHT - m_sprite.getGlobalBounds().height); // behind the borders so nothing on screen collides it 
	m_sprite.setPosition(m_location);
	m_release = false;
}
