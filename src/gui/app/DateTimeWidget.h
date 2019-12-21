#pragma once
#include <SFML/Graphics.hpp>

class DateTimeWidget
{
public:
	DateTimeWidget();
	~DateTimeWidget() = default;

	void setFont(const sf::Font& font);
	void updateAndDrawTo(sf::RenderWindow& window);
	
private:
	sf::Text time_;
	sf::Text date_;

	static sf::String getSystemTime_();
	static sf::String getSystemDate_();
};

