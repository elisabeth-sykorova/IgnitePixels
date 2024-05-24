

#include "Player.h"   // include Player header file

// Player function definitions here

Player::Player()
{
	setupSprite();
	setupHealthBar();
	setupDashBar();
	loadTextures();
	setupWaveIcon();
	loadSounds();
}

void Player::reset() // resetting function
{
	m_location = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	m_health = MAX_HEALTH;
	m_abilityPoints = 3;
	m_boostValue = 0;

	m_sprite.setPosition(m_location);
}

sf::Sprite Player::getSprite()
{
	return m_sprite;
}

void Player::update(sf::Vector2f t_mousePosition) // calculates the distance between mouse and sprite
{
	// drawing movement line

	float length;
	float rotationAngle;

	m_velocity = t_mousePosition - m_sprite.getPosition();

	// rotate sprite
	rotationAngle = atan2(m_velocity.x, m_velocity.y) * 180 / PI; // angle in degrees
	m_sprite.setRotation(-rotationAngle - 90);

	length = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);

	move(length);

	// decaying health

	float decay = SCREEN_WIDTH / 1000; // relevant to the screen because health bar is nearly screen width


	decreaseHealth(decay);
	updateHealthBar();
	updateTexture();
	animateTexture();
	updateDashBar();
	updateWaveIcon();

}

void Player::updateAbilityPoints(int t_amount)
{
	if (t_amount <= MAX_ABILITY_POINTS - m_abilityPoints)
	{
		m_abilityPoints += t_amount;
		if (t_amount > 0) // if gaining points
		{
			m_pointGainSound.play();
		}
	}
	else
	{
		m_abilityPoints = MAX_ABILITY_POINTS;
	}
}

int Player::getAbilityPoints()
{
	return m_abilityPoints;
}

void Player::setSpeed(float t_newSpeed)
{
	if (t_newSpeed == BOOST_SPEED)
	{
		if (m_boostValue > 0 + BOOST_DECREMENT)
		{
			if (m_boostSound.getStatus() != sf::Sound::Playing)
			{
				m_boostSound.play();
			}
			m_speed = t_newSpeed;
		}
		else
		{
			m_speed = DEFAULT_SPEED;
		}
	}
	else
	{
		m_speed = t_newSpeed;
		if (m_boostSound.getStatus() != sf::Sound::Playing)
		{
			m_boostSound.play();
		}
	}
}

float Player::getDefaultSpeed()
{
	return DEFAULT_SPEED;
}

float Player::getSpeed()
{
	return m_speed;
}

float Player::getBoostSpeed()
{
	return BOOST_SPEED;
}

bool Player::getBoosting()
{
	return m_boosting;
}

void Player::incrementBoostValue()
{
	if (m_boostValue <= MAX_BOOST_VALUE - BOOST_INCREMENT)
	{
		m_boostValue += BOOST_INCREMENT;
	}
	else
	{
		m_boostValue = MAX_BOOST_VALUE;
	}
}

void Player::decrementBoostValue()
{
	if (m_boostValue >= 0 + BOOST_DECREMENT)
	{
		m_boostValue -= BOOST_DECREMENT;
		setBoosting(true);
	}
	else
	{
		m_boostValue = 0;
		setBoosting(false);
	}
}

void Player::setBoosting(bool t_boosting)
{
	m_boosting = t_boosting;
}

void Player::setupHealthBar()
{
	healthBar.setSize(sf::Vector2f(MAX_HEALTH, m_barHeight));
	healthBar.setFillColor(sf::Color::Cyan);
	healthBar.setPosition(sf::Vector2f(0 + 40, SCREEN_HEIGHT - 50));

	if (!m_healthBarTexture.loadFromFile("ASSETS/IMAGES/health_bar.png"))
	{
		std::cout << "error with health bar texture";
	}
	m_healthBarSprite.setPosition(sf::Vector2f(0 + 40, SCREEN_HEIGHT - 50));
	m_healthBarSprite.setTexture(m_healthBarTexture);
	m_healthBarSprite.setScale(4.375f,4.375f);

}

void Player::setupDashBar()
{

	if (!m_dashBarTexture.loadFromFile("ASSETS/IMAGES/dash_bar.png"))
	{
		std::cout << "error with dash bar texture";
	}

	m_dashBarSprite.setTexture(m_dashBarTexture);
	m_dashBarSprite.setPosition(sf::Vector2f(SCREEN_WIDTH - 50, 0 + 30));
	m_dashBarSprite.setTextureRect(sf::IntRect(0, 0, m_dashBarDimensions.x, m_dashBarDimensions.y));
	m_dashBarSprite.setScale(2.45f, 2.45f);

}

void Player::setupWaveIcon()
{
	if (!m_waveIconTexture.loadFromFile("ASSETS/IMAGES/wave_indicator.png"))
	{
		std::cout << "error with wave icon";
	}

	m_waveIconSprite.setTexture(m_waveIconTexture);
	m_waveIconSprite.setPosition(sf::Vector2f(SCREEN_WIDTH - 125, 0 + 35));
	m_waveIconSprite.setTextureRect(sf::IntRect(0, 0, m_waveIconDimensions.x, m_waveIconDimensions.y));
	m_waveIconSprite.setScale(3.0f, 3.0f);
}

sf::Sprite Player::getWaveIcon()
{
	return m_waveIconSprite;
}

sf::Sprite Player::getDashBar()
{
	return m_dashBarSprite;
}

sf::Sprite Player::getHealthBar()
{
	return m_healthBarSprite;
}

void Player::decreaseHealth(float t_decreaseAmount) // decreased health by passed amount
{

	if (m_health >= t_decreaseAmount) // only if health is bigger or equal to the equivalent (so it doesnt go into negative values)
	{
		m_health -= t_decreaseAmount;
	}
	else
	{
		m_health = 0.0f; // if decrease amount is more than the health the player currently has, it gets automatically set to zero
	}
}

void Player::increaseHealth()
{
	float increaseAmount = SCREEN_WIDTH/12;

	if (m_health < MAX_HEALTH - increaseAmount) // border checking
	{
		m_health += increaseAmount;
	}
}

float Player::getHealth()
{
	return m_health;
}

sf::Vector2f Player::getVelocity()
{
	return m_velocity;
}

void Player::updateHealthBar() // updating texture based on health value
{
	m_healthBarSprite.setTextureRect(sf::IntRect(0, 0, m_health / 4.375, m_barHeight));
}

void Player::updateTexture()
{
	if (!m_boosting) // different textures during boosting
	{
		if (m_health <= MAX_HEALTH / 4) // if health less than quarted
		{
			m_sprite.setTexture(m_lowTexture);
		}
		else if (m_health <= MAX_HEALTH / 2) // if less than half
		{
			m_sprite.setTexture(m_midTexture);
		}
		else
		{
			m_sprite.setTexture(m_fullTexture); // full health texture
		}
	}
	else // default texture (not boostng)
	{
		if (m_health <= MAX_HEALTH / 4)
		{
			m_sprite.setTexture(m_boostLowTexture);
		}
		else if (m_health <= MAX_HEALTH / 2)
		{
			m_sprite.setTexture(m_boostMidTexture);
		}
		else
		{
			m_sprite.setTexture(m_boostFullTexture);
		}
	}

}

void Player::animateTexture()
{
	m_lastFrame = m_currentFrame;
	m_frameCounter += m_frameIncrement;
	m_currentFrame = static_cast<int>(m_frameCounter) % 6;

	if (m_lastFrame != m_currentFrame) // animate the button
	{
		m_sprite.setTextureRect(sf::IntRect(m_currentFrame * m_spriteDimensions.x, 0, m_spriteDimensions.x, m_spriteDimensions.y));
	}
}

void Player::updateDashBar() // update texture based on boost value
{
	m_dashBarSprite.setTextureRect(sf::IntRect(m_dashBarDimensions.x * static_cast<int>(m_boostValue), 0, m_dashBarDimensions.x, m_dashBarDimensions.y));
}

void Player::updateWaveIcon() 
{
	int currentFrame = m_abilityPoints;
	m_waveIconSprite.setTextureRect(sf::IntRect(m_waveIconDimensions.x * currentFrame, 0, m_waveIconDimensions.x, m_waveIconDimensions.y));
}

void Player::setupSprite()
{
	m_sprite.setTextureRect(sf::IntRect(0,0, 55, 25));
	m_sprite.setOrigin(m_spriteDimensions.x / 4, m_spriteDimensions.y / 4);
	m_sprite.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	m_sprite.scale(2.75,2.75);
	m_sprite.setRotation(-90);
	m_location = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

}

void Player::loadTextures()
{
	if (!m_fullTexture.loadFromFile("ASSETS/IMAGES/fire_full.png"))
	{
		std::cout << "error with full texture";
	}

	if (!m_midTexture.loadFromFile("ASSETS/IMAGES/fire_mid.png"))
	{
		std::cout << "error with mid texture";
	}

	if (!m_lowTexture.loadFromFile("ASSETS/IMAGES/fire_low.png"))
	{
		std::cout << "error with low texture";
	}

	if (!m_boostFullTexture.loadFromFile("ASSETS/IMAGES/fire_full_boost.png"))
	{
		std::cout << "error with full boost texture " << std::endl;
	}

	if (!m_boostMidTexture.loadFromFile("ASSETS/IMAGES/fire_mid_boost.png"))
	{
		std::cout << "error with mid boost texture " << std::endl;
	}

	if (!m_boostLowTexture.loadFromFile("ASSETS/IMAGES/fire_low_boost.png"))
	{
		std::cout << "error with low boost texture " << std::endl;
	}

}

void Player::move(float t_length)
{

	if (t_length > m_speed) // stops jittering
	{
		m_velocity /= t_length; // velocity is then divided by length
		m_velocity *= m_speed; // multiplies the velocity by the speed to extend the vector
		m_location += m_velocity; // changes location of the enemy
		m_sprite.setPosition(m_location); // sprites are set to the position of the location
	}

}

sf::Vector2f Player::getPosition()
{
	return m_sprite.getPosition();
}


void Player::loadSounds()
{

	if (!m_boostBuffer.loadFromFile("ASSETS/SOUNDS/boost_sound.wav"))
	{
		std::cout << "error with gameplay music " << std::endl;
	}

	m_boostSound.setBuffer(m_boostBuffer);

	if (!m_pointGainBuffer.loadFromFile("ASSETS/SOUNDS/point_gain.wav"))
	{
		std::cout << "error with point gain sound " << std::endl;
	}
	m_pointGainSound.setBuffer(m_pointGainBuffer);

}