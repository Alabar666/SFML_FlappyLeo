#include "flappy.hpp"

FlappyBird::FlappyBird() {
    window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(1000, 600),
        "Flappy Leo",
        sf::Style::Titlebar | sf::Style::Close
    );
    window->setPosition(sf::Vector2i(0, 0));
    window->setFramerateLimit(60);

    gravity = frame = 0.f;
    space = 220.f;
    count = 0;

    bg.loadFromFile("./assets/background.png");
    flappy.loadFromFile("./assets/flapleo.png");
    pipe.loadFromFile("./assets/pipe.png");

    background = std::make_shared<sf::Sprite>();
    bird = std::make_shared<sf::Sprite>();
    pipeBottom = std::make_shared<sf::Sprite>();
    pipeTop = std::make_shared<sf::Sprite>();

    background->setTexture(bg);
    bird->setTexture(flappy);
    pipeBottom->setTexture(pipe);
    pipeTop->setTexture(pipe);

    bird->setPosition(
        500.f - flappy.getSize().x / 2.f,
        300.f - flappy.getSize().y / 2.f
    );

    bird->setScale(2.f, 2.f);
    bird->setTextureRect(sf::IntRect(0, 0, 30, 26));

    pipeBottom->setScale(1.5f, 1.5f);
    pipeTop->setScale(1.5f, -1.5f);

    gameover = add = false;
    score = 0;
    state = WaitingToStart; 

    font.loadFromFile("./assets/font/flappybird.ttf");

    txt_gameover.setFont(font);
    txt_gameover.setString("Press ENTER to restart");
    txt_gameover.setPosition(200, 300);
    txt_gameover.setCharacterSize(50);
    txt_gameover.setOutlineThickness(3);

    txt_score.setFont(font);
    txt_score.setString(std::to_string(score));
    txt_score.setPosition(10.f, 10.f);
    txt_score.setCharacterSize(50);
    txt_score.setOutlineThickness(3);
}

void FlappyBird::events() {
    auto e = std::make_shared<sf::Event>();
    while (window->pollEvent(*e)) {
        if (e->type == sf::Event::Closed || 
        (e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::Escape)){
            window->close();
        }
    }
    if (state == WaitingToStart && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        state = GameActive; 
        restartGame(); 
    }

    if (gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        restartGame();
    }
}


void FlappyBird::restartGame() {

    score = 0;
    gravity = 0.f;
    frame = 0.f;
    count = 0;
    gameover = false;

    pipes.clear();

    bird->setPosition(
        500.f - flappy.getSize().x / 2.f,
        300.f - flappy.getSize().y / 2.f
    );

    txt_score.setString(std::to_string(score));
}

void FlappyBird::draw() {
    window->clear(sf::Color::Black);
    window->draw(*background);

    for (auto &p : pipes) {
        window->draw(p);
    }

    window->draw(*bird);

    if (state == WaitingToStart) {
        txt_gameover.setString("Press ENTER to start");
        window->draw(txt_gameover);
    }

    if (gameover) {
        window->draw(txt_gameover);
    }

    window->draw(txt_score);

    window->display();
}

void FlappyBird::run() {
    while (window->isOpen()) {
        events();
        if (state == GameActive) { 
            game();
        }
        draw();

        ++count;
        if (count == 300) {
            count = 0;
        }
    }
}

void FlappyBird::movePipes() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        gravity = -8.f;
        bird->setRotation(-frame - 10.f);
    } else {
        bird->setRotation(frame - 10.f);
    }

    if (count % 150 == 0) {
        int pos = std::rand() % 275 + 175;

        pipeBottom->setPosition(1000, pos + space);
        pipeTop->setPosition(1000, pos);

        pipes.push_back(*pipeBottom);
        pipes.push_back(*pipeTop);
    }

    for (std::size_t i{}; i < pipes.size(); ++i) {
        if (pipes[i].getGlobalBounds().intersects(bird->getGlobalBounds())) {
            bird->move(15.f, 0);

            if (pipes[i].getScale().y < 0) {
                bird->move(0, -15.f);
            } else {
                bird->move(0, 15.f);
            }

            gameover = true;
        }

        if (pipes[i].getPosition().x < -100) {
            pipes.erase(pipes.begin() + i);
        }

        pipes[i].move(-4.f, 0);

        if (pipes[i].getPosition().x == 448 && !add) {
            txt_score.setString(std::to_string(++score));
            add = true;
        } else {
            add = false;
        }
    }
}

void FlappyBird::game() {
    if (!gameover) {
        setAnimeBird();
        moveBird();
        movePipes();
    }
}

void FlappyBird::setAnimeBird() {
    frame += 0.15f;

    if (frame >= 3) {
        frame = 0;
    }

    bird->setTextureRect(sf::IntRect(30 * (int)frame, 0, 30, 26));
}

void FlappyBird::moveBird() {
    bird->move(0, gravity);
    gravity += 0.5f;
    if (bird->getPosition().y < 0 || bird->getPosition().y > window->getSize().y) {
        gameover = true;
    }
}
