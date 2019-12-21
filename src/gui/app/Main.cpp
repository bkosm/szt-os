#include "App.h"
#include "Definitions.h"


int main()
{
	App app(RESOLUTION, APP_TITLE);


	/*
	TextBox tB1(15, sf::Color::White, false);
	tB1.setPosition(sf::Vector2f(400, 300));
	tB1.setFont(font);

	//Pêtla aplikacji
	while (window.isOpen())
	{
		//Sprawdzenie wszystkich wydarzeñ okna
		sf::Vector2f MousePosition(sf::Mouse::getPosition(window));
		//dateAndTime(font, window);
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed: window.close();
			case sf::Event::TextEntered:
				{
					tB1.typedOn(event);
				}
			case sf::Event::MouseButtonReleased:
				{
					if (MousePosition.x >= b1.buttonShape.getPosition().x && MousePosition.x <= b1
					                                                                            .buttonShape.
					                                                                            getPosition().x + b1.
						width &&
						MousePosition.y >= b1.buttonShape.getPosition().y && MousePosition.y <= b1
						                                                                        .buttonShape.
						                                                                        getPosition().y + b1.
						height)
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


		/*Wyœwietlanie daty i czsu

	//dateAndTime(font, window);


	//CreateWindow Wb1(140, 20, 200, 100, "Nazwa procesu:");


	sf::String playerInput;
	sf::Text playerText;


	/*	if (event.type == event.MouseButtonReleased && MousePosition.x >= b1.buttonShape.getPosition().x && MousePosition.x <= b1.buttonShape.getPosition().x + b1.width &&
			MousePosition.y >= b1.buttonShape.getPosition().y && MousePosition.y <= b1.buttonShape.getPosition().y + b1.height)
		{
			CreateWindow Wb1(140, 20, 200, 100, "Nazwa procesu:");
			window.draw(Wb1.CWShape);
			window.draw(Wb1.write);
		}
	*/
}
