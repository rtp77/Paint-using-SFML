#include <SFML/Graphics.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"

int loopCount = 0;

const int width = 1500;
const int height = 800;

int main()
{
	//Setup Window
	sf::RenderWindow window(sf::VideoMode(width,height), "Paint");
	ImGui::SFML::Init(window);
	window.setVerticalSyncEnabled(true);

	// create an empty 800x600 texture
	sf::Texture texture;
	if (!texture.create(width, height))
	{
		std::cout << "Fail to create the texture";
	}

	// create an black image
	sf::Image image;
	image.create(width, height, sf::Color::Black);

	//variables
	bool brush = false, blue = false, yellow = false;
	bool clear = false;
	int brushSize = 20;
	sf::Color color(255, 255, 255);
	int redC = 255, greenC = 255, blueC = 255;

	// Set the texture to be the image and create the Sprite to render
	texture.update(image);
	sf::Sprite sprite;
	sprite.setTexture(texture);

	//ui
	sf::RectangleShape uiBox(sf::Vector2f(315, 250));
	uiBox.setFillColor(sf::Color::White);
	uiBox.setOrigin(0,0);
	uiBox.setPosition(0, 0);

	//Red box
	sf::RectangleShape redBox(sf::Vector2f(105, 50));
	redBox.setOrigin(0, 0);
	redBox.setPosition(0, 180);
	redBox.setFillColor(sf::Color::Red);

	//Green box
	sf::RectangleShape greenBox(sf::Vector2f(105, 50));
	greenBox.setOrigin(0, 0);
	greenBox.setPosition(105, 180);
	greenBox.setFillColor(sf::Color::Green);

	//Blue box
	sf::RectangleShape blueBox(sf::Vector2f(105, 50));
	blueBox.setOrigin(0, 0);
	blueBox.setPosition(210, 180);
	blueBox.setFillColor(sf::Color::Blue);

	//fill box
	sf::RectangleShape fillBox(sf::Vector2f(315, 60));
	fillBox.setFillColor(sf::Color::Magenta);
	fillBox.setOrigin(0, 0);
	fillBox.setPosition(0, 230);

	//cursor
	sf::RectangleShape ver(sf::Vector2f(1, 20));
	ver.setOrigin(1, 10);
	ver.setFillColor(sf::Color::White);

	sf::RectangleShape hor(sf::Vector2f(20,1));
	hor.setOrigin(10, 1);
	ver.setFillColor(sf::Color::White);



	sf::Clock deltaClock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				int Mx = sf::Mouse::getPosition(window).x;
				int My = sf::Mouse::getPosition(window).y;
					if (brush == true && Mx>1 && Mx<width-1 && My>1 && My<height-1) {
							int x = Mx, y = My, radius = brushSize;
							float sinus = 0.70710678118;
							int range = radius / (2 * sinus);
							for (int i = radius; i >= range; --i) {
								int j = sqrt(radius * radius - i * i);
								for (int k = -j; k <= j; k++) {
									image.setPixel(x - k, y + i,color);
									image.setPixel(x - k, y - i, color);
									image.setPixel(x + i, y + k,color);
									image.setPixel(x - i, y - k, color);
								}
							}
							range = brushSize * sinus;
							for (int i = x - range + 1; i < x + range; i++) {
								for (int j = y - range + 1; j < y + range; j++) {
									image.setPixel(i, j, color);
								}
							}

					}//Brush Size	
			}
			
		}//poll event
		ImGui::SFML::Update(window, deltaClock.restart());

		//Selected color display
		color.r = redC; color.g = greenC; color.b = blueC;
		sf::Color colR(redC, 0,0); sf::Color colG(0, greenC, 0); sf::Color colB(0, 0, blueC); sf::Color colFill(redC, greenC, blueC);
		redBox.setFillColor(colR);
		greenBox.setFillColor(colG);
		blueBox.setFillColor(colB);
		fillBox.setFillColor(colFill);

		//clear display
		if (clear == true) {
			sf::Color colClear(0, 0, 0);
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					image.setPixel(i, j, colClear);
				}
			}
			clear = false;
		}

		//Brush display
		if (brush == true) {
			int Mx = sf::Mouse::getPosition(window).x;
			int My = sf::Mouse::getPosition(window).y;
			ver.setPosition(Mx, My);
			hor.setPosition(Mx, My);
			sf::Color colFill(redC, greenC, blueC);
			ver.setFillColor(colFill);
			hor.setFillColor(colFill);
		}
		if (brush == false) {
			ver.setPosition(0, 0);
			hor.setPosition(0, 0);
		}



		ImGui::Begin("Painter");
		ImGui::Text("Painter");
		ImGui::Checkbox("Brush", &brush);
		ImGui::SliderInt("Brush Size", &brushSize,0,60);
		ImGui::SliderInt("Red", &redC, 0, 255);
		ImGui::SliderInt("Green", &greenC, 0, 255);
		ImGui::SliderInt("Blue", &blueC, 0, 255);
		ImGui::Checkbox("CLEAR", &clear);
		ImGui::End();


		//The actual Rendering
		window.clear();
		texture.update(image);
		window.draw(sprite);
		window.draw(uiBox);
		ImGui::SFML::Render(window);
		window.draw(redBox); window.draw(greenBox); window.draw(blueBox); window.draw(fillBox);
		window.draw(ver); window.draw(hor);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;

}


