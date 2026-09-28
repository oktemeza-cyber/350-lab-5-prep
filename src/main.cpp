#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        if(const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()){
            
            if(keyPressed->scancode == sf::Keyboard::Scan::Num1){
                tween = [](float a, float b, float t){
                    float r = 1.0f - std::pow(1.0f - t, 3.0f);
                    return (1 - r) * a + r * b;
                };
            }//ease out cubic
            break;

            if(keyPressed->scancode == sf::Keyboard::Scan::Num2){
                tween = [](float a, float b, float t){
                    const int constant1 = 1.70158;
                    const int constant3 = constant1 + 1;

                    float r = constant3 * t * t * t - constant1 * t * t;
                    return (1 - r) * a + r * b;
                };
            }//ease In Back

            if(keyPressed->scancode == sf::Keyboard::Scan::Num3){
                tween = [](float a, float b, float t){
                    float pi = 3.141592653;
                    float r = 1.0f - std::cos((t*pi) / 2);
                    return (1 - r) * a + r * b;
                };
            }//ease in sine

            if(keyPressed->scancode == sf::Keyboard::Scan::Num4){
                tween = [](float a, float b, float t){
                    float r = 0 ? 0 : std::pow(2, 10*t - 10);
                    return (1 - r) * a + r * b;
                };
            }//ease in expo

            if(keyPressed->scancode == sf::Keyboard::Scan::Num5){
                tween = [](float a, float b, float t){
                    float pi = 3.141592653;
                    float r = std::sin((t*pi)/2);
                    return (1 - r) * a + r * b;
                };
            }//ease out sine

            if(keyPressed->scancode == sf::Keyboard::Scan::Num6){
                tween = [](float a, float b, float t){
                    float pi = 3.141592653;
                    float r = -(std::cos(pi * t) - 1) / 2;
                    return (1 - r) * a + r * b;
                };
            }//ease in out sine

            if(keyPressed->scancode == sf::Keyboard::Scan::Num7){
                tween = [](float a, float b, float t){
                    float r = t*t*t*t*t;
                    return (1 - r) * a + r * b;
                };
            }//ease in quint

            if(keyPressed->scancode == sf::Keyboard::Scan::Num8){
                tween = [](float a, float b, float t){
                    float r = 1 - std::pow(1 - t, 5);
                    return (1 - r) * a + r * b;
                };
            }//ease out quint

            if(keyPressed->scancode == sf::Keyboard::Scan::Num9){
                tween = [](float a, float b, float t){
                    float r = 1 - (1 - t) * (1 - t);
                    return (1 - r) * a + r * b;
                };
            }//ease out expo
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);

    static int frames = 0;
    int framesWanted = 200;
    float time = static_cast<float>(frames % framesWanted)/framesWanted;
    float x = tween(0.0f, WINDOW_WIDTH, time);
    frames++;

    sf::CircleShape shape;
    shape.setRadius(20);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(sf::Vector2f(5, 5));
    shape.setPosition(sf::Vector2(x, WINDOW_HEIGHT/3.0f));
    window.draw(shape);
    

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
