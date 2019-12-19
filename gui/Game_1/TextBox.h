#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27


class TextBox
{
public:
	TextBox() {}

	TextBox(int size, sf::Color color, bool sel)
	{
		textBox.setCharacterSize(size);
		textBox.setFillColor(color);
		isSelected = sel;
		if(sel)
		{
			textBox.setString("_");
		}
		else
		{
			textBox.setString("");
		}
	}
	void setFont(sf::Font &font)
	{
		textBox.setFont(font);
	}

	void setPosition(sf::Vector2f pos)
	{
		textBox.setPosition(pos);
	}

	void setSelected(bool sel)
	{
		isSelected = sel;
		if(!sel)
		{
			std::string t = text.str();
			std::string newT = "";
			for(int i = 0; i < t.length() - 1; i++)
			{
				newT += t[i];
			}
			textBox.setString(newT);
		}
	}

	std::string getText()
	{
		return text.str();
	}

	void drawTo(sf::RenderWindow &window)
	{
		window.draw(textBox);
	}

	void typedOn(sf::Event input)
	{
		if(isSelected)
		{
			int charTyped = input.text.unicode;
			if(charTyped < 128)
			{
				inputLogic(charTyped);
			}
		}
	}
	
private:
	sf::Text textBox;
	std::ostringstream text;
	bool isSelected = false;
	//bool hasLimit = false;
	//int limit;

	void inputLogic(int charTyped)
	{
		if(charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
		{
			text << static_cast<char>(charTyped);
		}
		else if (charTyped == DELETE_KEY)
		{
			if (text.str().length() > 0)
			{
				std::string t = text.str();
				std::string newT = "";
				for (int i = 0; i < t.length() - 1; i++)
				{
					newT += t[i];
				}
				text.str("");
				text << newT;
				textBox.setString(text.str());
			}
		}
		textBox.setString(text.str() + "_");
	}
	
};

