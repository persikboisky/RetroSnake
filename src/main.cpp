constexpr unsigned int ROWS = 30;
constexpr unsigned int COLUMNS = 30;
constexpr unsigned int SIZE_CELL = 20;
constexpr float SLEEP_SNAKE = 100.0f;

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

constexpr sf::Color COLOR_BACKGROUND = sf::Color(175, 204, 96, 255);
constexpr sf::Color COLOR_SNAKE = sf::Color(42, 52, 25);
constexpr sf::Color COLOR_APPLE = sf::Color(42, 62, 25);

const std::string PATH_TO_FONT = "./font.ttf";
const std::string PATH_TO_RECORD = "./res.bin";

const std::vector<sf::Keyboard::Key> KEY_UP { sf::Keyboard::Key::W, sf::Keyboard::Key::Up };
const std::vector<sf::Keyboard::Key> KEY_DOWN { sf::Keyboard::Key::S, sf::Keyboard::Key::Down };
const std::vector<sf::Keyboard::Key> KEY_LEFT { sf::Keyboard::Key::A, sf::Keyboard::Key::Left };
const std::vector<sf::Keyboard::Key> KEY_RIGHT { sf::Keyboard::Key::D, sf::Keyboard::Key::Right };

unsigned int Score = 0;
bool isRunningGame = true;

class Snake
{
private:
    std::vector<int> x;
    std::vector<int> y;

    int VelX = 0;
    int VelY = 0;

    sf::RectangleShape rect;
    sf::Clock clock;

    void moveSnake()
    {
        if (clock.getElapsedTime().asMilliseconds() > SLEEP_SNAKE)
        {
            for (unsigned index = this->x.size() - 1; index > 0; index--)
            {
                if (this->y[index] == this->y[0] && this->x[index] == this->x[0])
                {
                    isRunningGame = false;
                }

                this->x[index] = this->x[index - 1];
                this->y[index] = this->y[index - 1];
            }

            this->x[0] += this->VelX;
            this->y[0] += this->VelY;
            this->clock.restart();
        }
    }

public:
    Snake(int x, int y)
    {
        this->rect = sf::RectangleShape({ SIZE_CELL - 2.0f, SIZE_CELL - 2.0f });
        this->rect.setFillColor(COLOR_SNAKE);
        this->clock = sf::Clock();
        this->x.push_back(x);
        this->y.push_back(y);
    }

    ~Snake()
    {
        this->x.clear();
        this->y.clear();
    }

    void update()
    {
        for (unsigned int index = 0; index < KEY_UP.size(); index++)
        {
            if (sf::Keyboard::isKeyPressed(KEY_UP[index]) && this->VelY <= 0)
            {
                this->VelX = 0;
                this->VelY = -(int)SIZE_CELL;
            }
            else if (sf::Keyboard::isKeyPressed(KEY_DOWN[index]) && this->VelY >= 0)
            {
                this->VelX = 0;
                this->VelY = SIZE_CELL;
            }
            else if (sf::Keyboard::isKeyPressed(KEY_LEFT[index]) && this->VelX <= 0)
            {
                this->VelX = -(int)SIZE_CELL;
                this->VelY = 0;
            }
            else if (sf::Keyboard::isKeyPressed(KEY_RIGHT[index]) && this->VelX >= 0)
            {
                this->VelX = SIZE_CELL;
                this->VelY = 0;
            }
        }

        if (this->x.size() != Score + 1)
        {
            x.push_back(1000);
            y.push_back(1000);
        }

        if (
            this->x[0] <= SIZE_CELL ||
            this->y[0] <= SIZE_CELL ||
            this->x[0] >= ROWS * SIZE_CELL + SIZE_CELL * 2 ||
            this->y[0] >= ROWS * SIZE_CELL + SIZE_CELL * 2
            )
        {
            isRunningGame = false;
        }

        this->moveSnake();
    }

    void draw(sf::RenderWindow& window)
    {
        for (unsigned int index = 0; index < this->x.size(); index++)
        {
            sf::Vector2f pos = { (float)this->x[index] + 1.0f, (float)this->y[index] + 1.0f };
            this->rect.setPosition(pos);
            window.draw(this->rect);
        }
    }

    sf::Vector2i getCoord()
    {
        return sf::Vector2i(this->x[0], this->y[0]);
    }

    std::vector<int> getCoordX()
    {
        return this->x;
    }

    std::vector<int> getCoordY()
    {
        return this->y;
    }
};

class Apple
{
private:
    int x;
    int y;

    sf::CircleShape circle;

public:
    Apple()
    {
        this->circle = sf::CircleShape(SIZE_CELL / 2);
        this->circle.setFillColor(COLOR_APPLE);
        this->x = rand() % COLUMNS + 2;
        this->y = rand() % ROWS + 2;
    }

    void spawnRandoApple(Snake& snake)
    {
        bool flag = true;
        while (flag)
        {
            this->x = rand() % COLUMNS + 2;
            this->y = rand() % ROWS + 2;

            for (unsigned int index = 0; index < snake.getCoordX().size(); index++)
            {
                if (this->x != snake.getCoordX()[index] && this->y != snake.getCoordY()[index])
                {
                    flag = false;
                }
                else flag = true;
            }
        }
    }

    void update(Snake& snake)
    {
        if (
            snake.getCoord().x == this->x * SIZE_CELL && 
            snake.getCoord().y == this->y * SIZE_CELL
        )
        {
            this->spawnRandoApple(snake);
            Score += 1;
        }
    }

    void draw(sf::RenderWindow& window)
    {
        sf::Vector2f pos = { this->x * (float)SIZE_CELL, this->y * (float)SIZE_CELL };
        this->circle.setPosition(pos);
        window.draw(this->circle);
    }
};

int main()
{
    bool circleProgram = true;
    srand(time(NULL));
    sf::Vector2u sizeWindow = { COLUMNS * SIZE_CELL + SIZE_CELL * 4, ROWS * SIZE_CELL + SIZE_CELL * 4 };
    sf::RenderWindow window(sf::VideoMode(sizeWindow), "Snake Game");
    sf::Font font = sf::Font(PATH_TO_FONT);

    int record = 0;
    std::ifstream file = std::ifstream(PATH_TO_RECORD, std::ios_base::binary);
    file.read((char*)&record, 1);
    file.close();

    while (circleProgram)
    {
        sf::RectangleShape boarderRect = sf::RectangleShape({ COLUMNS * SIZE_CELL, ROWS * SIZE_CELL });
        boarderRect.setPosition({ SIZE_CELL * 2, SIZE_CELL * 2 });
        boarderRect.setOutlineColor(sf::Color::Black);
        boarderRect.setFillColor(COLOR_BACKGROUND);
        boarderRect.setOutlineThickness(1.0f);

        Snake snake = Snake(COLUMNS / 2 * SIZE_CELL + SIZE_CELL, ROWS / 2 * SIZE_CELL + SIZE_CELL);
        Apple apple = Apple();

        sf::Text scoreInput = sf::Text(font, "Score: ", 20);
        scoreInput.setFillColor(COLOR_SNAKE);

        while (isRunningGame)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is < sf::Event::Closed>()) window.close();
            }

            window.clear(COLOR_BACKGROUND);
            snake.update();
            apple.update(snake);
            window.draw(boarderRect);
            snake.draw(window);
            apple.draw(window);

            scoreInput.setPosition({ SIZE_CELL * 2, 0 });
            scoreInput.setString("Score: " + std::to_string(Score));
            window.draw(scoreInput);

            scoreInput.setPosition({ SIZE_CELL * 8, 0 });
            scoreInput.setString("Record: " + std::to_string(record));
            window.draw(scoreInput);

            window.display();

            if (!window.isOpen())
            {
                circleProgram = false;
                isRunningGame = false;
            }

            if (Score > record)
            {
                record = Score;
                std::ofstream f = std::ofstream(PATH_TO_RECORD, std::ios_base::binary);
                f.write((char*)&record, 4);
                f.close();
            }
        }
        Score = 0;
        isRunningGame = true;
    }

	return 0;
}
