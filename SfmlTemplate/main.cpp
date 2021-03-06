// SfmlTemplate.cpp : définit le point d'entrée pour l'application console.
//
#include <SFML/Graphics.hpp>
#include "Car.h"
#include <iostream>
#include <conio.h>
#include <vector>


int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
	window.setFramerateLimit(60);

	///textes
	sf::Font* font = new Font();
	font->loadFromFile("resources/arial.ttf");

	sf::Text instructionsText = sf::Text("Espace pour changer de voiture, fleches pour controller la voiture.\nLe sens de la collision qui sera affiché sera basé sur la position initiale de la voiture.\n(par exemple si collision est a gauche c'est a gauche de la voiture en ayant la voiture avec l'avant vers le bas)", *font, 20);
	sf::Text collisionText;
	collisionText.setCharacterSize(20);
	collisionText.setFont(*font);
	collisionText.setPosition(sf::Vector2f(0, instructionsText.getGlobalBounds().height + 30));
	collisionText.setOutlineColor(sf::Color::Red);
	collisionText.setOutlineThickness(1);
	///

	//variable pour savoir si il y a eu collision pour l'affichage du texte
	bool asCollided = false;

	//vecteur pour stocker les voitures
	std::vector<Car*> carsList;

	///on ajoute 2 voitures
	carsList.push_back(new Car(1, sf::Vector2f(400.0f, 400.0f)));
	carsList.push_back(new Car(2, sf::Vector2f(800.0f, 400.0f)));

	//savoir si une touche du clavier est pressée (pour eviter le spam)
	bool isKeyPressed = false;

	//numéro de la voiture selectionnée
	int currentCarNb = 1;

	//pointeur sur la voiture selectionnée
	Car* currentCar = carsList[currentCarNb - 1];
	currentCar->setSelectedCar(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();

		//si on appuie sur espace on change de voiture selectionnée
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isKeyPressed)
		{
			isKeyPressed = true;
			switch (currentCarNb)
			{
			case 1:
				currentCarNb = 2;
				break;
			case 2:
				currentCarNb = 1;
				break;
			default:
				break;
			}
			currentCar->setSelectedCar(false);
			currentCar = carsList[currentCarNb - 1];
			currentCar->setSelectedCar(true);
			currentCar->resetMovementTimer();
		}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isKeyPressed)
		{
			isKeyPressed = false;
		}
		currentCar->update();

		///Check des collision
		//on boucle une premiére fois sur toutes les voitures pour check si une d'elle entre en collision avec une autre
		for (int i = 0; i < carsList.size(); i++)
		{
			//on boucle une deuxiéme fois sur toutes les voitures pour voir si une autre voiture lui rentre dedans
			for (int j = 0; j < carsList.size(); j++)
			{
				//condition pour ne pas verifier une voiture avec elle même
				if (j != i)
				{
					//on verifie dans un premier temps si les bouding box des 2 voitures s'entrecroisent (pour limiter les calculs)
					if (carsList[i]->getBounds().intersects(carsList[j]->getBounds()))
					{
						//si la voiture va en avant alors on check avec les points de l'avant
						if (carsList[i]->getDrivingDirection() == DrivingDirection::FORWARD)
						{
							//check si le point avant gauche est dans le rectangle
							if (collisionBetweenRotatedRectAndPoint(carsList[j]->getTransform(), carsList[j]->getBoundingBox(), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_LEFT)))
							{
								carsList[i]->setCollisionState(true);
								carsList[j]->setCollisionState(true);
								//si le point avant gauche rentre en collision avec le coté avant de l'autre voiture
								if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(FRONT.first), carsList[j]->getBoundingBoxCornerAtIndex(FRONT.second), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_LEFT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec l'avant de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision devant" << std::endl;
									asCollided = true;
								}
								//si le point avant gauche rentre en collision avec le coté droit de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(RIGHT.first), carsList[j]->getBoundingBoxCornerAtIndex(RIGHT.second), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_LEFT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec la droite de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision droite" << std::endl;
									asCollided = true;
								}
								//si le point avant gauche rentre en collision avec le coté arriere de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(BACK.first), carsList[j]->getBoundingBoxCornerAtIndex(BACK.second), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_LEFT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec l'arriere de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision arriere" << std::endl;
									asCollided = true;
								}
								//si le point avant gauche rentre en collision avec le coté gauche de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(LEFT.first), carsList[j]->getBoundingBoxCornerAtIndex(LEFT.second), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_LEFT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec la gauche de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision gauche" << std::endl;
									asCollided = true;
								}
							}
							//check si le point avant droite est dans le rectangle
							else if (collisionBetweenRotatedRectAndPoint(carsList[j]->getTransform(), carsList[j]->getBoundingBox(), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_RIGHT)))
							{
								carsList[i]->setCollisionState(true);
								carsList[j]->setCollisionState(true);
								//si le point avant droite rentre en collision avec le coté avant de l'autre voiture
								if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(FRONT.first), carsList[j]->getBoundingBoxCornerAtIndex(FRONT.second), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_RIGHT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec l'avant de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision devant" << std::endl;
									asCollided = true;
								}
								//si le point avant droite rentre en collision avec le coté droite de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(RIGHT.first), carsList[j]->getBoundingBoxCornerAtIndex(RIGHT.second), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_RIGHT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec la droite de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision droite" << std::endl;
									asCollided = true;
								}
								//si le point avant droite rentre en collision avec le coté arriere de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(BACK.first), carsList[j]->getBoundingBoxCornerAtIndex(BACK.second), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_RIGHT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec l'arriere de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision arriere" << std::endl;
									asCollided = true;
								}
								//si le point avant droite rentre en collision avec le coté gauche de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(LEFT.first), carsList[j]->getBoundingBoxCornerAtIndex(LEFT.second), carsList[i]->getBoundingBoxCornerAtIndex(FRONT_RIGHT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec la gauche de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision gauche" << std::endl;
									asCollided = true;
								}
							}
						}
						//si la voiture va en arriere alors on check avec les points de l'arriere
						else if (carsList[i]->getDrivingDirection() == DrivingDirection::BACKWARD)
						{
							//check si le point arriere gauche est dans le rectangle
							if (collisionBetweenRotatedRectAndPoint(carsList[j]->getTransform(), carsList[j]->getBoundingBox(), carsList[i]->getBoundingBoxCornerAtIndex(BACK_LEFT)))
							{
								carsList[i]->setCollisionState(true);
								carsList[j]->setCollisionState(true);
								//si le point arriere gauche rentre en collision avec le coté avant de l'autre voiture
								if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(FRONT.first), carsList[j]->getBoundingBoxCornerAtIndex(FRONT.second), carsList[i]->getBoundingBoxCornerAtIndex(BACK_LEFT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec l'avant de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision devant" << std::endl;
									asCollided = true;
								}
								//si le point arriere gauche rentre en collision avec le coté droite de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(RIGHT.first), carsList[j]->getBoundingBoxCornerAtIndex(RIGHT.second), carsList[i]->getBoundingBoxCornerAtIndex(BACK_LEFT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec la droite de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision droite" << std::endl;
									asCollided = true;
								}
								//si le point arriere gauche rentre en collision avec le coté arriere de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(BACK.first), carsList[j]->getBoundingBoxCornerAtIndex(BACK.second), carsList[i]->getBoundingBoxCornerAtIndex(BACK_LEFT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec l'arriere de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision arriere" << std::endl;
									asCollided = true;
								}
								//si le point arriere gauche rentre en collision avec le coté gauche de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(LEFT.first), carsList[j]->getBoundingBoxCornerAtIndex(LEFT.second), carsList[i]->getBoundingBoxCornerAtIndex(BACK_LEFT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec la gauche de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision gauche" << std::endl;
									asCollided = true;
								}
							}
							//check si le point arriere droite est dans le rectangle
							else if (collisionBetweenRotatedRectAndPoint(carsList[j]->getTransform(), carsList[j]->getBoundingBox(), carsList[i]->getBoundingBoxCornerAtIndex(BACK_RIGHT)))
							{
								carsList[i]->setCollisionState(true);
								carsList[j]->setCollisionState(true);
								//si le point arriere droite rentre en collision avec le coté avant de l'autre voiture
								if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(FRONT.first), carsList[j]->getBoundingBoxCornerAtIndex(FRONT.second), carsList[i]->getBoundingBoxCornerAtIndex(BACK_RIGHT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec l'avant de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision devant" << std::endl;
									asCollided = true;
								}
								//si le point arriere droite rentre en collision avec le coté droite de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(RIGHT.first), carsList[j]->getBoundingBoxCornerAtIndex(RIGHT.second), carsList[i]->getBoundingBoxCornerAtIndex(BACK_RIGHT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec la droite de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision droite" << std::endl;
									asCollided = true;
								}
								//si le point arriere droite rentre en collision avec le coté arriere de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(BACK.first), carsList[j]->getBoundingBoxCornerAtIndex(BACK.second), carsList[i]->getBoundingBoxCornerAtIndex(BACK_RIGHT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec l'arriere de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision arriere" << std::endl;
									asCollided = true;
								}
								//si le point arriere droite rentre en collision avec le coté gauche de l'autre voiture
								else if (checkSideCollision(carsList[j]->getBoundingBoxCornerAtIndex(LEFT.first), carsList[j]->getBoundingBoxCornerAtIndex(LEFT.second), carsList[i]->getBoundingBoxCornerAtIndex(BACK_RIGHT)))
								{
									collisionText.setString("La voiture " + std::to_string(carsList[i]->getCarID()) + " entre en collision avec la gauche de la voiture " + std::to_string(carsList[j]->getCarID()) + "\nR pour recommmencer");
									std::cout << "collision gauche" << std::endl;
									asCollided = true;
								}
							}
						}
					}

				}
			}
		}
		///



		//on boucle sur la liste de voitures et change la couleur de la boundingBox pour savoir si c'est celle selectionnée
		for (int i = 0; i < carsList.size(); i++)
		{
			if (carsList[i]->isSelectedCar())
			{
				//vert si c'est celle selectionnée
				carsList[i]->setOutlineColor(sf::Color::Green);
			}
			else
			{
				//rouge dans le cas contraire
				carsList[i]->setOutlineColor(sf::Color::Red);
			}

			window.draw(*carsList[i]);
		}

		//reset si appui sur R et qu'il y a eu collision
		if (asCollided)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				asCollided = false;
				for (int i = 0; i < carsList.size(); i++)
				{
					carsList[i]->setCollisionState(false);
				}
				carsList[0]->setPosition(sf::Vector2f(400.0f, 400.0f));
				carsList[0]->setRotation(0);
				carsList[0]->resetPoints();
				carsList[1]->setPosition(sf::Vector2f(800.0f, 400.0f));
				carsList[1]->setRotation(0);
				carsList[1]->resetPoints();
			}
		}


		window.draw(instructionsText);

		//si il y a collision on affiche le texte
		if (asCollided)
		{
			window.draw(collisionText);
		}

		window.display();
	}

	return 0;
}