#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "konst.h"
#include "kintamieji.h"

using namespace std;
using namespace sf;

int main() {
    // Game window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dino Game");

    // Music
    sf::SoundBuffer buffer;
    buffer.loadFromFile("resources/music.mp3");
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
    sound.setLoop(true);
    
    // Background
    RectangleShape background(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    Texture background_tx;
    background_tx.loadFromFile("resources/background.jpg");
    background.setTexture(&background_tx);
    background.setPosition(0, 0);

    // Ground
    RectangleShape ground(Vector2f(GROUND_WIDTH, GROUND_HEIGHT));
    Texture ground_tx;
    ground_tx.loadFromFile("resources/ground.png");
    ground.setTexture(&ground_tx);
    ground.setPosition(0, 520);

    // Dino
    RectangleShape dino(Vector2f(DINO_WIDTH, DINO_HEIGHT));
    Texture dino_tx;
    dino_tx.loadFromFile("resources/sprite.png");
    dino.setTexture(&dino_tx);
    dino.setPosition(100, WINDOW_HEIGHT - GROUND_HEIGHT - DINO_HEIGHT);

    // Cactus
    RectangleShape cactus(Vector2f(CACTUS_WIDTH, CACTUS_HEIGHT));
    Texture cactus_tx;
    cactus_tx.loadFromFile("resources/cactus.png");
    cactus.setTexture(&cactus_tx);
    cactus.setPosition(WINDOW_WIDTH, WINDOW_HEIGHT - GROUND_HEIGHT - CACTUS_HEIGHT + 5);

    // Score tracking
    int score = 0;
    Font font;
    if (!font.loadFromFile("resources/Pixel Game.otf")) { // Ensure you have a font file in the working directory
        cerr << "Error loading font\n";
        return -1;
    }
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(Color::Yellow);

    // Game over text
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(65);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setString("Game Over! Press R to Restart");
    gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 - 50);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        if (!isGameOver) {
            // Jumping
            if (Keyboard::isKeyPressed(Keyboard::Space) && !isJumping) {
                isJumping = true;
                verticalSpeed = JUMP_SPEED;
            }

            // Dino gravity
            if (isJumping) {
                dino.move(0, verticalSpeed);
                verticalSpeed += GRAVITY;
            }

            // Check if dino is on the ground
            if (dino.getPosition().y >= WINDOW_HEIGHT - GROUND_HEIGHT - DINO_HEIGHT) {
                dino.setPosition(dino.getPosition().x, WINDOW_HEIGHT - GROUND_HEIGHT - DINO_HEIGHT);
                isJumping = false;
            }

            // Move cactus and ground to the left
            cactus.move(-cactusSpeed, 0);
            ground.move(-cactusSpeed, 0);

            // Reset cactus and ground when it goes off-screen
            if (cactus.getPosition().x < -CACTUS_WIDTH) {
                cactus.setPosition(WINDOW_WIDTH, WINDOW_HEIGHT - GROUND_HEIGHT - CACTUS_HEIGHT + 5);
                ground.setPosition(0, 520);
                score++; // Increment score when player dodges a cactus
            }

            // Check for collision
            if (dino.getGlobalBounds().intersects(cactus.getGlobalBounds())) {
                isGameOver = true; // Switch to Game Over state
            }

            // Score text
            scoreText.setString("Score: " + std::to_string(score));
            scoreText.setPosition(10, 10);
        }
        else {
            // Restart game
            if (Keyboard::isKeyPressed(Keyboard::R)) {
                isGameOver = false;
                score = 0;
                dino.setPosition(100, WINDOW_HEIGHT - GROUND_HEIGHT - DINO_HEIGHT);
                cactus.setPosition(WINDOW_WIDTH, WINDOW_HEIGHT - GROUND_HEIGHT - CACTUS_HEIGHT + 5);
                ground.setPosition(0, 520);
            }
        }

        // Exit game when escape pressed
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        // Render
        window.clear(sf::Color::White);
        window.draw(background);
        window.draw(ground);
        window.draw(dino);
        window.draw(cactus);
        window.draw(scoreText);

        if (isGameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
