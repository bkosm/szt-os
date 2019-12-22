#include "../Definitions.h"
#include "DateTimeWidget.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

/* Cpp date is a joke */
#pragma warning(disable : 4996)

DateTimeWidget::DateTimeWidget()
{
	time_.setString(getSystemTime_());
	date_.setString(getSystemDate_());

	time_.setCharacterSize(14);
	date_.setCharacterSize(14);

	time_.setFillColor(sf::Color::White);
	date_.setFillColor(sf::Color::White);
	
	time_.setPosition(float(RESOLUTION.width * 96 / 100), float(RESOLUTION.height * 95 / 100));
	date_.setPosition(float(RESOLUTION.width * 96 / 100), float(RESOLUTION.height * 98 / 100));
}

void DateTimeWidget::setFont(const sf::Font& font)
{
	date_.setFont(font);
	time_.setFont(font);
}

void DateTimeWidget::updateAndDrawTo(sf::RenderWindow& window)
{
	const auto timeGB = time_.getGlobalBounds(), dateGB = date_.getGlobalBounds();

	time_.setOrigin(timeGB.width / 2, timeGB.height / 2);
	date_.setOrigin(dateGB.width / 2, dateGB.height / 2);

	time_.setString(getSystemTime_());

	window.draw(time_);
	window.draw(date_);
}

sf::String DateTimeWidget::getSystemTime_()
{
	const auto now = std::time(nullptr);
	const auto dateTime = std::localtime(&now);

	std::ostringstream timeSS;

	timeSS << std::setfill('0') << std::setw(2) << dateTime->tm_hour << ":"
		<< std::setfill('0') << std::setw(2) << dateTime->tm_min << ":"
		<< std::setfill('0') << std::setw(2) << dateTime->tm_sec;

	return sf::String{timeSS.str()};
}

sf::String DateTimeWidget::getSystemDate_()
{
	const auto now = std::time(nullptr);
	const auto dateTime = std::localtime(&now);

	std::ostringstream dateSS;

	dateSS << std::setfill('0') << std::setw(2) << dateTime->tm_mday << " ";

	switch (dateTime->tm_mon)
	{
	case 0:
		{
			dateSS << "JAN ";
			break;
		}
	case 1:
		{
			dateSS << "FEB ";
			break;
		}
	case 2:
		{
			dateSS << "MAR ";
			break;
		}
	case 3:
		{
			dateSS << "APR ";
			break;
		}
	case 4:
		{
			dateSS << "MAY ";
			break;
		}
	case 5:
		{
			dateSS << "JUN ";
			break;
		}
	case 6:
		{
			dateSS << "JUL ";
			break;
		}
	case 7:
		{
			dateSS << "AUG ";
			break;
		}
	case 8:
		{
			dateSS << "SEP ";
			break;
		}
	case 9:
		{
			dateSS << "NOV ";
			break;
		}
	case 10:
		{
			dateSS << "OCT ";
			break;
		}
	case 11:
		{
			dateSS << "DEC ";
			break;
		}
	default:
		{
			dateSS << "UNK ";
		}
	}

	dateSS << std::setfill('0') << std::setw(4) << dateTime->tm_year + 1900;

	return sf::String{dateSS.str()};
}
