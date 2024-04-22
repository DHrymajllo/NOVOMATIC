#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <cmath>
#include <algorithm>

struct vec2 {
    float x, y;

    vec2 operator-(const vec2& rhs) const { return { x - rhs.x, y - rhs.y }; }
    vec2 operator+(const vec2& rhs) const { return { x + rhs.x, y + rhs.y }; }
    vec2& operator+=(const vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    vec2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
    float dot(const vec2& rhs) const { return x * rhs.x + y * rhs.y; }
};

struct triangle {
    std::array<vec2, 3> points;
};

vec2 getNormal(const vec2& p1, const vec2& p2) {
    return vec2{ p2.y - p1.y, -(p2.x - p1.x) };
}

std::pair<float, float> project(const triangle& tri, const vec2& axis) {
    float min = axis.dot(tri.points[0]);
    float max = min;
    for (int i = 1; i < 3; ++i) {
        float projection = axis.dot(tri.points[i]);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
    return { min, max };
}

bool isSeparated(const std::pair<float, float>& proj1, const std::pair<float, float>& proj2) {
    return proj1.second < proj2.first || proj2.second < proj1.first;
}

bool isColliding(const triangle& triangle1, const triangle& triangle2) {
    std::array<vec2, 4> axes = {
        getNormal(triangle1.points[0], triangle1.points[1]),
        getNormal(triangle1.points[1], triangle1.points[2]),
        getNormal(triangle2.points[0], triangle2.points[1]),
        getNormal(triangle2.points[1], triangle2.points[2])
    };

    for (const auto& axis : axes) {
        auto proj1 = project(triangle1, axis);
        auto proj2 = project(triangle2, axis);
        if (isSeparated(proj1, proj2)) {
            return false;
        }
    }
    return true;
}

void clampTriangle(triangle& tri, const sf::RenderWindow& window) {
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = minX;
    float maxY = maxX;

    for (const auto& point : tri.points) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }

    if (minX < 0.0f) {
        float offset = 0.0f - minX;
        for (auto& point : tri.points) {
            point.x += offset;
        }
    }
    if (maxX > window.getSize().x) {
        float offset = window.getSize().x - maxX;
        for (auto& point : tri.points) {
            point.x += offset;
        }
    }
    if (minY < 0.0f) {
        float offset = 0.0f - minY;
        for (auto& point : tri.points) {
            point.y += offset;
        }
    }
    if (maxY > window.getSize().y) {
        float offset = window.getSize().y - maxY;
        for (auto& point : tri.points) {
            point.y += offset;
        }
    }
}
#ifndef UNIT_TESTING
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Collision Detection");
    sf::Clock clock;

    triangle tri1 = { {{{100, 100}, {150, 50}, {200, 100}}} };
    triangle tri2 = { {{{300, 300}, {350, 250}, {400, 300}}} };

    auto triangleToShape = [](const triangle& tri) {
        sf::ConvexShape shape(3);
        for (size_t i = 0; i < 3; ++i) {
            shape.setPoint(i, sf::Vector2f(tri.points[i].x, tri.points[i].y));
        }
        return shape;
    };

    sf::ConvexShape shape1 = triangleToShape(tri1);
    sf::ConvexShape shape2 = triangleToShape(tri2);

    shape1.setFillColor(sf::Color::Green);
    shape2.setFillColor(sf::Color::Blue);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        vec2 direction;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) direction.y -= 1.0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) direction.y += 1.0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) direction.x -= 1.0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) direction.x += 1.0;

        for (auto& point : tri1.points) {
            point += direction * 0.5f * deltaTime;
        }

        shape1 = triangleToShape(tri1);

        clampTriangle(tri1, window);

        bool colliding = isColliding(tri1, tri2);
        if (colliding) {
            shape1.setFillColor(sf::Color::Red);
            shape2.setFillColor(sf::Color::Red);
        }
        else {
            shape1.setFillColor(sf::Color::Green);
            shape2.setFillColor(sf::Color::Blue);
        }

        window.clear();
        window.draw(shape1);
        window.draw(shape2);
        window.display();
    }

    return 0;
}
#endif
