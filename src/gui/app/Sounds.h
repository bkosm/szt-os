#pragma once
#include <SFML/Audio.hpp>

class Sounds
{
public:
	Sounds();
	~Sounds() = default;
	
	sf::Sound boot;
	sf::Sound shutdown;
	sf::Sound error;

	void setVolume(float volume);
	
private:
	sf::SoundBuffer boot_;
	sf::SoundBuffer shutdown_;
	sf::SoundBuffer error_;
};

