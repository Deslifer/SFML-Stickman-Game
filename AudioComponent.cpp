#include "AudioComponent.h"

AudioComponent::AudioComponent()
{
}

AudioComponent::~AudioComponent()
{
	for (auto& i : this->sounds)
	{
		delete i.second;
	}
	for (auto& i : this->musics)
	{
		delete i.second;
	}
}

void AudioComponent::addSound(const std::string key, std::string path)
{
	this->soundBuffers[key] = new sf::SoundBuffer();
	this->soundBuffers[key]->loadFromFile(path);
	
	this->sounds[key] = new sf::Sound(*this->soundBuffers[key]);
	//this->sounds[key]->setVolume(23);
}

void AudioComponent::addMusic(const std::string key, std::string path)
{
	this->musics[key] = new sf::Music();
	if (!this->musics[key]->openFromFile(path))
		std::cout<<"error";
	this->musics[key]->setVolume(30);
}

void AudioComponent::playSound(const std::string key)
{
	if (sounds[key]->getStatus() == sf::Sound::Stopped)
	{
		sounds[key]->play();
	}
}

void AudioComponent::playMusic(const std::string key)
{
	if (musics[key]->getStatus() == sf::Music::Stopped)
	{
		musics[key]->play();
	}
}

void AudioComponent::stopSound(const std::string key)
{
	if (sounds[key]->getStatus() == sf::Sound::Playing)
		sounds[key]->stop();
}

void AudioComponent::stopMusic(const std::string key)
{
	if (musics[key]->getStatus() == sf::Music::Playing)
		musics[key]->stop();
}
