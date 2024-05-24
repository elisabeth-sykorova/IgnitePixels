// Name: Elisabeth Sykorova
// Login: C00301316
// Date: 25.04.2024
// Approximate time taken: 30 hours
//---------------------------------------------------------------------------------------------------------
// Project description:
// 
// This is my final first year (2024) project for Interactive Digital Art and Design.
// 
// In this game, the player controls a flame that extinguishes water entities while being chased by earthy-
// leaf enemies that try to stop the fire and restore balance.
// The player's health decays over time and can only be regained by colliding a fleeing enemy. Colliding
// with the chasing enemy results in loss of health.
// The player has two special abilities - a boost, which speeds up the player's movement and makes it
// invulnerable to the chasing enemies. The boost is limited and indicated by a boost meter, which fills
// up with time and then goes down rapidly when being activated.
// The second ability is an air wave, which slows down all chasing enemies that it collides with. This wave
// travels in the direction the player was heading when the wave got released. To use the wave, the player
// needs a wave point; there can be maximum up to 3 wave point held at a time. To gain a wave point, two
// of the chasing enemies need to collide with eachother.
// 
// Player's movement is controlled by the mouse movement,
// Wave can be release by pressing [E],
// Boost is activated by holding [Space]
// 
// The project also involves the game's menu, which includes the start button, instructions, and multiple
// opening animations.
// The whole game is complimented by various background tracks and sound effects.
// --------------------------------------------------------------------------------------------------------
// Known Bugs:
// - Inaccurate boundaries,
// - Enemies can spawn into the boundaries of the player - instant respawn and health gain

//////////////////////////////////////////////////////////// 
// include correct library file for release and debug versions
//////////////////////////////////////////////////////////// 

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib")




#include "Game.h"   // include Game header file


int main()
{
	Game aGame;
	aGame.loadContent();
	aGame.run();

	return 0;
}

Game::Game() : window(sf::VideoMode(static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT)), "Joint Project Game", sf::Style::Default)
// Default constructor
{
	setupMenuSprites();
	setupMenuText();
	setupIcons();
	loadBackgrounds();
	loadSounds();
}

void Game::run()
// This function contains the main game loop which controls the game. 
{

	srand((int)time(nullptr)); // set the seed once

	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	// the clock object keeps the time.
	sf::Clock clock;


	clock.restart();

	while (window.isOpen())
	{
		// check if the close window button is clicked on

		//get the time since last update and restart the clock
		timeSinceLastUpdate += clock.restart();

		//update every 60th of a second
		//only when the time since last update is greater than 1/60 update the world.
		if (timeSinceLastUpdate > timePerFrame)
		{
			processEvents();// process events
			update();
			draw();

			// reset the timeSinceLastUpdate to 0 
			timeSinceLastUpdate = sf::Time::Zero;
		}
	}  // end while loop

}

void Game::update()
{
	// GAME--------------------------------------------------------------
	if (m_gameState == GameStates::Gameplay) // if the game is on
	{
		if (!m_gameReset) // GAME RESETTING (once per game)
		{
			resetGameVariables();
			player.reset();
			wave.reset();

			for (int index = 0; index < m_activeFleeing; index++) // update all fleeing enemies for reset and respawn
			{
				fleeingEnemies[index].update(player.getPosition());
				fleeingEnemies[index].spawn();
			}

			for (int index = 0; index < m_activeChasing; index++) // update all chasing enemies for reset and respawn
			{
				chasingEnemies[index].update(player.getPosition());
				chasingEnemies[index].spawn();
			}

			m_gameReset = true; // reset happens only once
		}

		if (!m_gamesCheckDone) // check once per game
		{
			if (m_gamePlayed != true) // sets to true when a game is played (once) - controls the change of animation textures in menu
			{
				m_gamePlayed = true;
			}
			else if (m_gamePlayed && !m_twoGamesPlayed) // when two games are played
			{
				m_twoGamesPlayed = true;
			}
			else if (m_twoGamesPlayed && !m_moreGamesPlayed) // when more games are played
			{
				m_moreGamesPlayed = true;
			}

			// background switch
			m_menuBackgroundSprite.setTexture(m_gameplayBg);
			m_bgFrameIncrement /= 2;
			m_backgroundSwitched = false;

			// music switch
			m_backgroundMusic.setBuffer(m_gameplayMusic);
			m_backgroundMusic.setVolume(15);
			m_backgroundMusic.play();

			m_gamesCheckDone = true;

		}

		updateScore(); // updating score with time
		player.incrementBoostValue(); // progressive gain of boost ability
		checkCollisions();
		increaseEnemies(); // increases difficulty over time (checking every update)
		processKeysPressed(); // get keyboard input

		if (wave.getRelease()) // move wave if released
		{
			wave.move();
			wave.animateTexture();
		}

		if (player.getHealth() == 0)
		{
			m_gameState = GameStates::Menu;
		}

		player.update(m_mouseEndVector);

		// Chasing Enemies Movement/Update
		if (!wave.getRelease())
		{
			for (int index = 0; index < m_activeChasing; index++)
			{
				chasingEnemies[index].update(player.getPosition()); // moves away from the position passed to the function - player's location by default
			}
		}
		else
		{
			for (int index = 0; index < m_activeChasing; index++)
			{
				if (wave.getSprite().getGlobalBounds().intersects(chasingEnemies[index].getSprite().getGlobalBounds()))
				{
					chasingEnemies[index].waveUpdate(wave.getVelocity()); // if the wave is released, moves away from the wave's position instead
				}
				else
				{
					chasingEnemies[index].update(player.getPosition());
				}
			}
		}

		// Fleeing Enemies Movement/Update
		for (int index = 0; index < m_activeFleeing; index++) // update all feeling enemies
		{
			fleeingEnemies[index].update(player.getPosition());
		}

	}

	// MENU--------------------------------------------------------------
	else if (m_gameState == GameStates::Menu)
	{
		if (!m_backgroundSwitched)
		{
			switchToMenuBg();
		}

		if (m_moreGamesPlayed && m_gameReset) // random animation after more games played
		{
			randomMenuAnimation();
			m_gameReset = false;
			m_gamesCheckDone = false;
		}

		else if (m_twoGamesPlayed && m_gameReset) // third animation showed after second game
		{
			m_lineupSprite.setTexture(m_expiringLineupTexture);
			m_lineupSprite.setPosition(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 50);
			m_gameReset = false;
			m_gamesCheckDone = false;
		}
		else if (m_gamePlayed && m_gameReset) // second animation showed after first game
		{
			m_lineupSprite.setTexture(m_runningLineupTexture);
			m_lineupSprite.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50);
			m_gameReset = false;
			m_gamesCheckDone = false;
		}

		loadMenu(); // updating always
		startButtonCollision();
		animateMenuSprites();
		animateIcons();
		animateMenuBackground();
	}

}

void Game::draw()
// This function draws the game world
{
	// Clear the screen and draw your game sprites
	window.clear(sf::Color(53, 48, 43));

	if (m_gameState == GameStates::Gameplay) // sprites in gameplay
	{
		window.draw(m_gameplayBackgroundSprite);
		window.draw(m_timeElapsed);  // write message to the screen
		window.draw(player.getSprite());

		if (wave.getRelease())
		{
			window.draw(wave.getSprite());
		}

		for (int index = 0; index < m_activeChasing; index++)
		{
			window.draw(chasingEnemies[index].getSprite());
		}

		for (int index = 0; index < m_activeFleeing; index++)
		{
			window.draw(fleeingEnemies[index].getSprite());
		}

		window.draw(player.getHealthBar());
		window.draw(player.getDashBar());
		window.draw(player.getWaveIcon());
	}
	//------------------------------------------------
	else if (m_gameState == GameStates::Menu) // sprites in menu
	{
		window.draw(m_menuBackgroundSprite);
		window.draw(m_startGame);

		if (m_gamePlayed)
		{
			window.draw(m_scoreGained);
			window.draw(m_healedStatistics);
		}
		else
		{
			window.draw(m_openingMessage); // only show before the first game starts
		}
		window.draw(m_lineupSprite);
		window.draw(m_boostIconSprite);
		window.draw(m_boostExplanation);
		window.draw(m_waveIconSprite);
		window.draw(m_waveExplanation);
	}

	window.display();
}

void Game::processEvents()
{

	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	
		if (sf::Event::MouseMoved == event.type) // user moved the mouse
		{
			processMouseMove(event);
		}

		if (sf::Event::MouseButtonReleased == event.type)
		{
			m_mouseReleased = true;
		}
		else
		{
			m_mouseReleased = false;
		}

	}
}

void Game::processKeysPressed()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		if (m_canPressE) // stops from accidentally double releasing a wave
		{
			m_canPressE = false;
			if (player.getAbilityPoints() >= 1)
			{
				wave.setRelease(true);
				wave.setupRelease(player.getVelocity(), player.getPosition());
				player.updateAbilityPoints(-1);
				m_windWaveSound.play();
			}
		}
	}
	else
	{
		m_canPressE = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		player.setSpeed(player.getBoostSpeed()); // change player's speed to boosted speed
		player.decrementBoostValue();
	}
	else if (player.getSpeed() != player.getDefaultSpeed())
	{
		player.setSpeed(player.getDefaultSpeed()); // sets speed back to default once after boost is over
		player.setBoosting(false);

	}
}

void Game::processMouseMove(sf::Event t_event) // saves the location of the mouse into a vector
{
	m_mouseEndVector.x = static_cast<float>(t_event.mouseMove.x);
	m_mouseEndVector.y = static_cast<float>(t_event.mouseMove.y);

	mouseDot.move(m_mouseEndVector);

}

void Game::increaseEnemies() // increasing over time
{
	if (m_secondsLastChecked != m_seconds)
	{
		if (m_seconds % 10 == 0 && m_activeChasing < MAX_CHASING && m_activeChasing < 4) // first three spawn in every 10 seconds each
		{
			m_activeChasing++;
		}
		else if (m_seconds % 30 == 0 && m_activeChasing < MAX_CHASING) // later increasing slows down to every 30 seconds
		{
			m_activeChasing++;
		}

		if (m_seconds % 15 == 0 && m_activeFleeing < MAX_FLEEING) // one more spawns every 15 seconds
		{
			m_activeFleeing++;
		}

		m_secondsLastChecked = m_seconds;
	}

}

void Game::resetGameVariables() // set all game variables back to default before next game starts
{
	m_secondsCounter = 0.0f;
	m_seconds = 0;
	m_secondsLastChecked = 0;
	m_secondsLastCheckedScore = 0;
	m_minutes = 0;
	m_activeChasing = 2;
	m_activeFleeing = 2;
	m_timesCaught = 0;
	m_timesHealed = 0;
}

void Game::checkCollisions() // all collision checks
{
	chasingWithChasing();
	playerWithFleeing();
	playerWithChasing();
}

void Game::chasingWithChasing()
{
	for (int firstIndex = 0; firstIndex < m_activeChasing; firstIndex++)
	{
		for (int secondIndex = 0; secondIndex < m_activeChasing; secondIndex++)
		{
			if (firstIndex != secondIndex &&
				chasingEnemies[firstIndex].getSprite().getGlobalBounds().intersects // enemy at first index boundaries
				(chasingEnemies[secondIndex].getSprite().getGlobalBounds())) // enemy at second index boundaries
			{
				chasingEnemies[secondIndex].spawn(); // if two chasing enemies intersect, respawn one of them 
				player.updateAbilityPoints(1);
			}
		}
	}
}

void Game::playerWithChasing()
{
	for (int index = 0; index < m_activeChasing; index++)
	{
		if (chasingEnemies[index].getSprite().getGlobalBounds().intersects( // if enemy at index intersects the player
			player.getSprite().getGlobalBounds()))
		{
			chasingEnemies[index].spawn();

			if (!player.getBoosting()) // boosting = invulnerability, doesnt count as damage taken (or times caught)
			{
				player.decreaseHealth(SCREEN_WIDTH / 12);
				m_timesCaught++;
				m_takingDamageSound.play();
			}
		}
	}
}

void Game::playerWithFleeing()
{
	for (int index = 0; index < m_activeFleeing; index++)
	{
		if (fleeingEnemies[index].getSprite().getGlobalBounds().intersects // enemies boundaries
						  (player.getSprite().getGlobalBounds()))  // player boundaries
		{
			fleeingEnemies[index].spawn(); // respawn after collision
			m_expireSound.play();

			player.increaseHealth(); // increase health
			m_timesHealed++;
		}
	}
}


void Game::setupMenuText()
{
	// start game button
	m_startGame.setFont(m_font);
	m_startGame.setCharacterSize(40);

	// score message
	m_scoreGained.setFont(m_font);
	m_scoreGained.setCharacterSize(12);


	// statistics
	m_healedStatistics.setFont(m_font);
	m_healedStatistics.setCharacterSize(12);

	// opening message
	m_openingMessage.setFont(m_font);
	m_openingMessage.setCharacterSize(12);

}

void Game::loadMenu() // update based on changed statistics
{
	// play button
	m_startGame.setString("Ignite");
	m_startGame.setOrigin(m_startGame.getGlobalBounds().width / 2, (m_startGame.getGlobalBounds().height / 2)); // centre of the text 
	m_startGame.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50);
	m_startGame.setFillColor(sf::Color(232, 224, 220));

	// score message update and reposition
	m_scoreGained.setString(m_scoreString);
	m_scoreGained.setOrigin(m_scoreGained.getGlobalBounds().width / 2, (m_scoreGained.getGlobalBounds().height / 2)); // centre of the text 
	m_scoreGained.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 70 - 50);
	m_scoreGained.setFillColor(sf::Color(232, 224, 220));

	// statistics reposition
	m_healedStatistics.setLetterSpacing(2);
	m_healedStatistics.setString("You  evaporated   " + std::to_string(m_timesHealed) + "   water  entities  and  got  caught  " + std::to_string(m_timesCaught) + "   times.");
	m_healedStatistics.setOrigin(m_healedStatistics.getGlobalBounds().width / 2, (m_healedStatistics.getGlobalBounds().height / 2)); // centre of the text 
	m_healedStatistics.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 260 - 50);
	m_healedStatistics.setFillColor(sf::Color(232, 224, 220));

	// opening message
	m_openingMessage.setLetterSpacing(2);
	m_openingMessage.setString("Evaporate   as   many   water   entities   to   keep   your   flame   alive\n                                                                                   and  don't  get  caught!  ");
	m_openingMessage.setOrigin(m_openingMessage.getGlobalBounds().width / 2, (m_openingMessage.getGlobalBounds().height / 2)); // centre of the text 
	m_openingMessage.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 260 - 50);
	m_openingMessage.setFillColor(sf::Color(232, 224, 220));


}

void Game::startButtonCollision() // checks whether the button is being collided with and clicked
{
	if (mouseDot.getDot().getGlobalBounds().intersects( // if mouse intersects start game text/button
		m_startGame.getGlobalBounds())) 
	{
		m_startGame.setFillColor(sf::Color(223,160,58)); // change color of text (hover effect)

		if (m_mouseReleased) // if mouse intersects the button and is clicked
		{
			m_gameState = GameStates::Gameplay; // change game state to gameplay
			m_clickSound.play(); // sound effect
		}
	}
	else
	{
		m_startGame.setFillColor(sf::Color(232, 224, 220)); // if mouse does not interesct (anymore), change the color back
	}
}

void Game::setupMenuSprites() // loading textures and setting up the menu animation sprite
{
	// Menu Screen Art
	if (!m_lineupTexture.loadFromFile("ASSETS/IMAGES/opening_animation1.png"))
	{
		std::cout << "error with lineup file " << std::endl;
	}

	if (!m_runningLineupTexture.loadFromFile("ASSETS/IMAGES/opening_animation2.png"))
	{
		std::cout << "error with third lineup file " << std::endl;
	}

	if (!m_expiringLineupTexture.loadFromFile("ASSETS/IMAGES/opening_animation3.png"))
	{
		std::cout << "error with third lineup file " << std::endl;
	}

	m_lineupSprite.setTexture(m_lineupTexture);
	m_lineupSprite.setOrigin(m_lineupSpriteDimensions.x / 2,m_lineupSpriteDimensions.y / 2);
	m_lineupSprite.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50);
	m_lineupSprite.setTextureRect(sf::IntRect(0, 0, m_lineupSpriteDimensions.x, m_lineupSpriteDimensions.y));
	m_lineupSprite.setScale(2.2, 2.2);

}

void Game::animateMenuSprites() // menu animation
{
	m_lastFrame = m_currentFrame;
	m_frameCounter += m_frameIncrement;
	m_currentFrame = static_cast<int>(m_frameCounter) % m_noFrames;

	if (m_lastFrame != m_currentFrame) // animate the button
	{
		m_lineupSprite.setTextureRect(sf::IntRect(m_currentFrame * m_lineupSpriteDimensions.x, 0, m_lineupSpriteDimensions.x, m_lineupSpriteDimensions.y));
	}
}

void Game::switchToMenuBg() // switches the texture and music to menu mode
{
	m_menuBackgroundSprite.setTexture(m_menuBg);
	m_menuBackgroundSprite.setScale(3.0f, 3.0f);
	m_bgFrameIncrement *= 2;
	m_backgroundSwitched = true;

	m_backgroundMusic.setBuffer(m_menuMusic);
	m_backgroundMusic.setVolume(30);
	m_backgroundMusic.play();
}

void Game::randomMenuAnimation() // picks a number between 0-2 and changes the texture based on the result
{
	int randomTexture = rand() % 3;

	switch (randomTexture)
	{
	case 0:
	{
		m_lineupSprite.setTexture(m_runningLineupTexture);
		m_lineupSprite.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50);
		break;
	}
	case 1:
	{
		m_lineupSprite.setTexture(m_expiringLineupTexture);
		m_lineupSprite.setPosition(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 50);
		break;
	}
	case 2:
	{
		m_lineupSprite.setTexture(m_lineupTexture);
		m_lineupSprite.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50);
		break;
	}
	}
}

void Game::setupIcons() // loads the textures and sets up the menu icons
{
	// boost icon:
	if (!m_boostIconTexture.loadFromFile("ASSETS/IMAGES/boost_icon.png"))
	{
		std::cout << "error with boost icon " << std::endl;
	}
	m_boostIconSprite.setTexture(m_boostIconTexture);
	m_boostIconSprite.setScale(2.2f, 2.2f);
	m_boostIconSprite.setPosition(0 + 15, 0 + 15);
	m_boostIconSprite.setTextureRect(sf::IntRect(m_iconsCurrentFrame * m_boostIconDimensions.x, 0, m_boostIconDimensions.x, m_boostIconDimensions.y));

	// text description
	m_boostExplanation.setFont(m_font);
	m_boostExplanation.setFillColor(sf::Color(166, 153, 148));
	m_boostExplanation.setCharacterSize(8);
	m_boostExplanation.setPosition(0 + 70, 0 + 45);
	m_boostExplanation.setLetterSpacing(3);
	m_boostExplanation.setString(":   Press   //SPACE//   to   speed   up   and   become   invulnerable   (until   the   boost   meter   is   empty).");
	

	// wave icon:
	if (!m_waveIconTexture.loadFromFile("ASSETS/IMAGES/wave_icon.png"))
	{
		std::cout << "error with wave icon " << std::endl;
	}
	m_waveIconSprite.setTexture(m_waveIconTexture);
	m_waveIconSprite.setScale(2.2f, 2.2f);
	m_waveIconSprite.setPosition(0 + 15, 0 + 95);
	m_waveIconSprite.setTextureRect(sf::IntRect(m_iconsCurrentFrame * m_waveIconDimensions.x, 0, m_waveIconDimensions.x, m_waveIconDimensions.y));

	// text description
	m_waveExplanation.setFont(m_font);
	m_waveExplanation.setFillColor(sf::Color(166, 153, 148));
	m_waveExplanation.setCharacterSize(8);
	m_waveExplanation.setLetterSpacing(3);
	m_waveExplanation.setLineSpacing(1.5);
	m_waveExplanation.setPosition(0 + 70, 0 + 95);
	m_waveExplanation.setString(":   Press   //E//   to   release   a   wave   to   push   away   enemies.   \n     You   can   have   up   to   3   charges   at   a   time   gained   when   two   enemies   collide. ");
}

void Game::animateIcons() // icons animation
{
	m_iconsLastFrame = m_iconsCurrentFrame;
	m_iconsFrameCounter += m_iconsFrameIncrement;
	m_iconsCurrentFrame = static_cast<int>(m_iconsFrameCounter) % m_iconsNoFrames;

	if (m_iconsLastFrame != m_iconsCurrentFrame) // animate the icon
	{
		m_waveIconSprite.setTextureRect(sf::IntRect(m_iconsCurrentFrame * m_waveIconDimensions.x, 0, m_waveIconDimensions.x, m_waveIconDimensions.y));
	}

	if (m_iconsLastFrame != m_iconsCurrentFrame) // animate the icon
	{
		m_boostIconSprite.setTextureRect(sf::IntRect(m_iconsCurrentFrame * m_boostIconDimensions.x, 0, m_boostIconDimensions.x, m_boostIconDimensions.y));
	}


}

void Game::updateScore()
{
	std::string null = ""; // placeholder for double digit seconds
	std::string divider = ":"; // divider between minutes and seconds
	m_secondsCounter += 1 / 60.0f; // update every 1/60th of a second
	m_seconds = static_cast<int>(m_secondsCounter);

	if (m_seconds % 60 == 0 && m_seconds != m_secondsLastCheckedScore) // if the seconds are divisible by 60, it means a minute has passed - this has to be updated once only
	{
		m_minutes++;
		m_secondsCounter = 0.0f;
		m_seconds = 0;
		m_secondsLastCheckedScore = m_seconds;
	}

	if (m_seconds < 10) // placeholder zero
	{
		null = "0";
	}

	m_scoreString = "Time  Elapsed:   " + std::to_string(m_minutes) + divider + null + std::to_string(m_seconds);
	m_timeElapsed.setString(m_scoreString);

}


void Game::loadContent()
// Load the font file & setup the time elapsed string
{
	if (!m_font.loadFromFile("ASSETS/FONTS/IgnitePixel.ttf"))
	{
		std::cout << "error with font file " << std::endl;
	}

	// set up the message string 
	m_timeElapsed.setFont(m_font);  // set the font for the text
	m_timeElapsed.setCharacterSize(12); // set the text size
	m_timeElapsed.setFillColor(sf::Color::White); // set the text colour
	m_timeElapsed.setPosition(10, 10);  // its position on the screen

}

void Game::loadBackgrounds()
{
	if (!m_menuBg.loadFromFile("ASSETS/IMAGES/menu_background.png"))
	{
		std::cout << "error with menu bg " << std::endl;
	}

	m_menuBackgroundSprite.setTexture(m_menuBg);
	m_menuBackgroundSprite.setScale(3.0f, 3.0f);

	if (!m_gameplayBg.loadFromFile("ASSETS/IMAGES/gameplay_background.png"))
	{
		std::cout << "error with gameplay bg " << std::endl;
	}

	m_gameplayBackgroundSprite.setTexture(m_gameplayBg);
	m_gameplayBackgroundSprite.setScale(3.0f, 3.0f);

}

void Game::animateMenuBackground()
{
	m_bgLastFrame = m_bgCurrentFrame;
	m_bgFrameCounter += m_bgFrameIncrement;
	m_bgCurrentFrame = static_cast<int>(m_bgFrameCounter) % m_bgNoFrames;

	if (m_bgLastFrame != m_bgCurrentFrame) // animate the icon
	{
		m_menuBackgroundSprite.setTextureRect(sf::IntRect(m_bgCurrentFrame * SCREEN_WIDTH/3, 0, SCREEN_WIDTH/3, SCREEN_HEIGHT/3));
	}

}

void Game::loadSounds() // sets up and loads the sound and background music
{
	if (!m_gameplayMusic.loadFromFile("ASSETS/SOUNDS/gameplay_music.wav"))
	{
		std::cout << "error with gameplay music " << std::endl;
	}

	if (!m_menuMusic.loadFromFile("ASSETS/SOUNDS/menu_music.wav"))
	{
		std::cout << "error with menu music " << std::endl;
	}

	if (!m_expireBuffer.loadFromFile("ASSETS/SOUNDS/evaporate_sound.wav"))
	{
		std::cout << "error with evaporate sound effect";
	}

	m_expireSound.setBuffer(m_expireBuffer);
	m_expireSound.setVolume(3);

	if (!m_clickBuffer.loadFromFile("ASSETS/SOUNDS/button_click_sound.wav"))
	{
		std::cout << "error with click sound effect";
	}
	m_clickSound.setBuffer(m_clickBuffer);
	m_clickSound.setVolume(100);

	if (!m_takingDamageSoundBuffer.loadFromFile("ASSETS/SOUNDS/taking_damage.wav"))
	{
		std::cout << "error with taking damage sound effect";
	}
	m_takingDamageSound.setBuffer(m_takingDamageSoundBuffer);
	m_takingDamageSound.setVolume(40);


	if (!m_windWaveSoundBuffer.loadFromFile("ASSETS/SOUNDS/wind_wave.wav"))
	{
		std::cout << "error with taking wind effect";
	}
	m_windWaveSound.setBuffer(m_windWaveSoundBuffer);
	m_windWaveSound.setVolume(40);

}



