#pragma once

#include <iostream>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "SFML/Window.hpp"
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

class AnimationComponent
{
private:
	class Animation
	{
	public:
		//Variables
		sf::Sprite& sprite;
		sf::Texture& textureSheet;
		float animationTimer;
		float timer;
		int width;
		int height;
		sf::IntRect startRect;
		sf::IntRect currentRect;
		sf::IntRect endRect;

		Animation(sf::Sprite& sprite,sf::Texture& texture_sheet, 
			float animation_timer, 
			int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height)
			: sprite(sprite),textureSheet(texture_sheet), 
			animationTimer(animation_timer), 
			width(width), height(height)
		{
			
			this->timer = 0.f;
			this->startRect = sf::IntRect(start_frame_x * width, start_frame_y * height, width, height);
			this->currentRect = this->startRect;
			this->endRect = sf::IntRect(frames_x * width, frames_y * height, width, height);
			
			this->sprite.setTexture(this->textureSheet, true);
			this->sprite.setTextureRect(this->startRect);
		}

		//Functions
		void play(const float& dt)
		{
			//Update timer
			this->timer += 100.f * dt;
			if (this->timer >= this->animationTimer)
			{
				//reset timer
				this->timer = 0.f;

				/*if (this->currentRect != this->endRect)
				{
					this->currentRect.left += this->width;
				}
				else //Reset
				{
					this->currentRect.left = this->startRect.left;
				}*/

				if (this->currentRect != this->endRect)
				{
					if (this->currentRect.left != this->endRect.left)
					{
						this->currentRect.left += this->width;
					}
					else if(this->currentRect.top != this->endRect.top)
					{
						this->currentRect.top += this->height;
						this->currentRect.left = this->startRect.left;
					}
				}
				else //Reset
				{
					this->currentRect = this->startRect;
				}

				this->sprite.setTextureRect(this->currentRect);
			}
		}
		void reset()
		{
			this->timer = 0.f;
			this->currentRect = this->startRect;
		}

	};

	sf::Sprite& sprite;
	sf::Texture& textureSheet;
	std::map<std::string, Animation*> animations;
	Animation* lastAnimation;

public:
	AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet);
	virtual ~AnimationComponent();

	//Functions
	void addAnimation(
		const std::string key, 
		float animation_timer,
		int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height);
	
	void play(const std::string key,const float& dt);
};
