#include "MouseDot.h"



MouseDot::MouseDot()
{
	setupDot();
}

void MouseDot::setupDot()
{
	m_dot.setRadius(1.0);
	m_dot.setFillColor(sf::Color::Magenta);
	m_dot.setPosition(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3);

}

sf::CircleShape MouseDot::getDot()
{
	return m_dot;
}

void MouseDot::move(sf::Vector2f t_mousePosition)
{
	m_dot.setPosition(t_mousePosition);
}
