#pragma once
#include <string>
#include <map>
#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>

class AudioComponent
{
private:
	std::map<std::string, sf::Sound*> sounds;
	std::map<std::string, sf::SoundBuffer*> soundBuffers;
	std::map<std::string, sf::Music*> musics;
public:
	AudioComponent();
	virtual ~AudioComponent();

	void addSound(const std::string key, std::string path);
	void addMusic(const std::string key,std::string path);
	void playSound(const std::string key);
	void playMusic(const std::string key);
	void stopSound(const std::string key);
	void stopMusic(const std::string key);
};

