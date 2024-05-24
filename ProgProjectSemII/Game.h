// Game class declaration
#pragma once

#include "SFML/Graphics.hpp" 
#include "SFML/Audio.hpp" 
#include <iostream>
#include <cstdlib>  // include support for randomizing
#include <ctime>   // supports ctime function
#include <string>
#include <cmath>

#include "Globals.h"   // include Global header file
#include "Player.h"   // include Player header file
#include "MouseDot.h"
#include "ChasingEnemy.h"
#include "FleeingEnemy.h"
#include "Wave.h"


enum class GameStates
{
	Menu,
	Gameplay
};

class Game
{
	// private data members


	sf::RenderWindow window;

	// Active Enemies
	int m_activeChasing = 2;
	int m_activeFleeing = 2;

	// Objects
	Player player;
	MouseDot mouseDot;
	ChasingEnemy chasingEnemies[MAX_CHASING];
	FleeingEnemy fleeingEnemies[MAX_FLEEING];
	Wave wave;
	
	sf::Vector2f m_mouseEndVector; // end of mouse position

	GameStates m_gameState = GameStates::Menu; // starts at menu


	// Booleans
	bool m_gamePlayed = false; // if a game was already played
	bool m_twoGamesPlayed = false; // if two game were played
	bool m_moreGamesPlayed = false; // if more games were played
	bool m_gamesCheckDone = false; // checking all texture/sound changes etc with every new game
	bool m_mouseReleased = false; // mouse pressed or released
	bool m_gameReset = true; // game reset for restarting a game
	bool m_canPressE = true; // stops from rereleasing wave multiple times
	bool m_backgroundSwitched = false; // if background was switched between menu and gameplay state


	// Font
	sf::Font m_font;  // font for writing text


	// Displayed in Menu
	sf::Text m_startGame; // start game button
	sf::Text m_scoreGained; // time elapsed
	sf::Text m_caughtStatistics; // how many were caught
	sf::Text m_healedStatistics; // how many times the player healed (collided with water)
	sf::Text m_timeElapsed;  // text to write on the screen
	sf::Text m_boostExplanation; // text describing usage of boost
	sf::Text m_waveExplanation; // usage of wave
	sf::Text m_openingMessage; // message that only appears before the first game

	sf::Sprite m_lineupSprite; // opening animation sprite
	sf::Texture m_lineupTexture;
	sf::Vector2f m_lineupSpriteDimensions{ 527.0f, 121.0f }; // dimension of one frame of the sprites

	sf::Texture m_runningLineupTexture; // second main screen animation
	sf::Texture m_expiringLineupTexture;

	std::string m_scoreString; // string to store the score message

	// Time tracking
	float m_secondsCounter = 0.0f;
	int m_seconds = 0;
	int m_minutes = 0;
	int m_secondsLastChecked = m_seconds; // for when increasing difficulty over time
	int m_secondsLastCheckedScore = m_seconds; // for score

	// animation
	int m_lastFrame = 0;
	float m_frameCounter = 0.0f;
	int m_currentFrame = 0;
	float m_frameIncrement = 0.15f;
	int m_noFrames = 8;

	// statistics
	int m_timesHealed = 0; // how many times the player healed/collided with fleeing enemies
	int m_timesCaught = 0; // how many times the player collided with the chasing enemies

	// backgrounds
	sf::Texture m_menuBg;
	sf::Texture m_gameplayBg;
	sf::Sprite m_menuBackgroundSprite;
	sf::Sprite m_gameplayBackgroundSprite;
	// animating backgrounds
	int m_bgLastFrame = 0;
	float m_bgFrameCounter = 0.0f;
	int m_bgCurrentFrame = 0;
	float m_bgFrameIncrement = 0.15f;
	int m_bgNoFrames = 6;

	// Menu Icons
	// boost:
	sf::Texture m_boostIconTexture;
	sf::Sprite m_boostIconSprite;
	sf::Vector2f m_boostIconDimensions{ 17.0f,28.0f };
	// wave:
	sf::Texture m_waveIconTexture;
	sf::Sprite m_waveIconSprite;
	sf::Vector2f m_waveIconDimensions{ 20.0f,21.0f };
	// animation of icons
	int m_iconsLastFrame = 0;
	float m_iconsFrameCounter = 0.0f;
	int m_iconsCurrentFrame = 0;
	float m_iconsFrameIncrement = 0.15f;
	int m_iconsNoFrames = 8;

	// Sounds and Music
	sf::Sound m_backgroundMusic;
	sf::SoundBuffer m_menuMusic;
	sf::SoundBuffer m_gameplayMusic;
	sf::Sound m_expireSound;
	sf::SoundBuffer m_expireBuffer;
	sf::Sound m_clickSound;
	sf::SoundBuffer m_clickBuffer;
	sf::Sound m_takingDamageSound;
	sf::SoundBuffer m_takingDamageSoundBuffer;
	sf::Sound m_windWaveSound;
	sf::SoundBuffer m_windWaveSoundBuffer;
	


public:	  // declaration of member functions	
	Game(); // default constructor

	void run();
	void update(); // updates every 1/60th of a second
	void draw(); // rendering w every update
	void loadContent();
	void loadBackgrounds();
	void animateMenuBackground();

	// Processing Keys and Mouse
	void processEvents();
	void processKeysPressed();
	void processMouseMove(sf::Event t_event);

	// Collisions
	void checkCollisions();
	void chasingWithChasing(); // when chasing enemy collides another chasing enemy
	void playerWithChasing(); // when player collides with a chasing enemy
	void playerWithFleeing(); // when player collides with a fleeing enemy

	// Menu
	void setupMenuText();
	void loadMenu();
	void startButtonCollision();
	void setupMenuSprites();
	void animateMenuSprites();
	void switchToMenuBg();
	void randomMenuAnimation();
	// icons
	void setupIcons();
	void animateIcons();

	// Score and Other Game Stats
	void updateScore();
	void increaseEnemies();

	// Reset Game
	void resetGameVariables();

	// Music and Sounds
	void loadSounds();


};
