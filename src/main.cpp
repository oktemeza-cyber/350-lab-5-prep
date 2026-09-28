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

        if(const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()){
            
            if(keyPressed->scancode == sf::Keyboard::Scan::Num1){
                tween = [](float a, float b, float t){
                    float r = 1.0f - std::pow(1.0f - t, 3.0f);
                    return (1 - r) * a + r * b;
                };
            }//ease out cubic

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
}//all functions used here were sampled from https://easings.net, I implemented them in C++ terms myself

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
    //draw circle and do animation w/tween

    sf::VertexArray yAxis(sf::PrimitiveType::Lines, 2);
    yAxis[0].position = sf::Vector2f(50.0f, WINDOW_HEIGHT - 50.0f);
    yAxis[1].position = sf::Vector2f(50.0f, WINDOW_HEIGHT - 350.0f);
    yAxis[0].color = sf::Color::White;
    yAxis[1].color = sf::Color::White;
    window.draw(yAxis);

    sf::VertexArray xAxis(sf::PrimitiveType::Lines, 2);
    xAxis[0].position = sf::Vector2f(50.0f, WINDOW_HEIGHT - 50.0f);
    xAxis[1].position = sf::Vector2f(50.0f + 650.0f, WINDOW_HEIGHT - 50.0f);
    xAxis[0].color = sf::Color::White;
    xAxis[1].color = sf::Color::White;
    window.draw(xAxis);
    //set up the x and y axis for the graph using a vertex array

    int steps = 100;
    sf::VertexArray curve(sf::PrimitiveType::LineStrip, steps + 1);
    for(int i = 0; i <= steps; i++){
        float position = static_cast<float>(i) / steps;
        float tween1 = tween(0.0f, 1.0f, position);
        curve[i].position = sf::Vector2f(std::lerp(50.0f, 50.0f + 650.0f, position), std::lerp(WINDOW_HEIGHT - 50.0f, (WINDOW_HEIGHT - 50.0f) - 350.0f, tween1));
        curve[i].color = sf::Color::Cyan;
    }
    window.draw(curve);
    //Do something similar but with a curve--lerping used to fit points inside the defined graph

    //dot draw
    float current = tween(0.0f, 1.0f, time);
    sf::CircleShape dot(5.0f);
    dot.setOrigin(sf::Vector2f(5.0f, 5.0f));
    dot.setPosition(sf::Vector2f(std::lerp(50.0f, 50.0f + 650.0f, time), std::lerp(WINDOW_HEIGHT - 50.0f, (WINDOW_HEIGHT - 50.0f) - 350.0f, current)));
    dot.setFillColor(sf::Color::Yellow);
    window.draw(dot);
    //lerping used in the same context
    
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
