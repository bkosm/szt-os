#include <SFML/Graphics.hpp> 
#include "Button.h"
#include "CreateWindow.h"
#include "TextBox.h"
#include <iostream>
#include <chrono>



#define _CRT_SECURE_NO_WARNINGS

/*	TO DO
		
 */

 /*data i czas*/
void dateAndTime(sf::Font &font, sf::RenderWindow &window)
{
	std::time_t t = std::time(0);   // get time now
	std::tm* now = std::localtime(&t);

	char buf[80];

	strftime(buf, 80, "%Y-%m-%d", now);
	std::string date = buf;
	strftime(buf, 80, "%X", now);
	std::string time = buf;

	

	sf::Text dateT;
	sf::Text timeT;
	dateT.setString(date);
	timeT.setString(time);
	dateT.setFillColor(sf::Color::White);
	timeT.setFillColor(sf::Color::White);
	dateT.setPosition(sf::Vector2f(720, 580));
	timeT.setPosition(sf::Vector2f(730, 570));
	dateT.setCharacterSize(12);
	timeT.setCharacterSize(12);
	dateT.setStyle(sf::Text::Bold);
	timeT.setStyle(sf::Text::Bold);
	dateT.setFont(font);
	timeT.setFont(font);
	
	
	
	window.draw(dateT);
	window.draw(timeT);
}

int main()
{
	//Tworzenie okna aplikacji
	sf::RenderWindow window(sf::VideoMode(800, 600), "SztOS");
	//Ustawnienie limitu klatek
	window.setFramerateLimit(120);
	int i = 0;
	sf::Font font;
	font.loadFromFile("arial.ttf");



	Button b1(20, 20, 100, 50, "CP");
	Button b2(20, 80, 100, 50, "DP");
	Button b3(20, 140, 100, 50, "CF");
	Button b4(20, 200, 100, 50, "DF");
	Button b5(20, 260, 100, 50, "SP");
	TextBox tB1(15, sf::Color::White, false);
	tB1.setPosition(sf::Vector2f(400, 300));
	tB1.setFont(font);
	
	//Pêtla aplikacji
	while (window.isOpen())
	{
		//Sprawdzenie wszystkich wydarzeñ okna
		sf::Vector2f MousePosition(sf::Mouse::getPosition(window));
		dateAndTime(font, window);
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:	window.close();
			case sf::Event::TextEntered:
			{
				tB1.typedOn(event);
			}
			case sf::Event::MouseButtonReleased:
				{
					if(MousePosition.x >= b1.buttonShape.getPosition().x && MousePosition.x <= b1.buttonShape.getPosition().x + b1.width &&
						MousePosition.y >= b1.buttonShape.getPosition().y && MousePosition.y <= b1.buttonShape.getPosition().y + b1.height)
					{
						CreateWindow Wb1(140, 20, 200, 100, "Nazwa procesu:");
						window.draw(Wb1.CWShape);
						window.draw(Wb1.write);
						tB1.setSelected(true);
						tB1.typedOn(event);
					}
				}
			}
		}
		window.clear(sf::Color::Black);


		/*Wyœwietlanie daty i czsu*/
		
		dateAndTime(font, window);
		

		

		

		//CreateWindow Wb1(140, 20, 200, 100, "Nazwa procesu:");


		/*window.draw(b1.drawButton());
		window.draw(b2.drawButton());
		window.draw(b3.drawButton());
		window.draw(b4.drawButton());
		window.draw(b5.drawButton());*/

		b1.drawTo(window);
		b2.drawTo(window);
		b3.drawTo(window);
		b4.drawTo(window);
		b5.drawTo(window);
		

		sf::String playerInput;
		sf::Text playerText;

		
		
	/*	if (event.type == event.MouseButtonReleased && MousePosition.x >= b1.buttonShape.getPosition().x && MousePosition.x <= b1.buttonShape.getPosition().x + b1.width &&
			MousePosition.y >= b1.buttonShape.getPosition().y && MousePosition.y <= b1.buttonShape.getPosition().y + b1.height)
		{
			CreateWindow Wb1(140, 20, 200, 100, "Nazwa procesu:");
			window.draw(Wb1.CWShape);
			window.draw(Wb1.write);

			
		}*/

		

		
		tB1.drawTo(window);
		window.display();

	}


	
	return 0;
	
}