#pragma once 

#include "figure.h"

#include <array>
#include <cmath>
#include <numbers>

template <Scalar T>
class Octagon : public Figure<T> {
public:
    Octagon() {
        for (auto& point : points)
            point = std::make_unique<Point<T>>();
    }

    Octagon(const Point<T>& center, const Point<T>& vertex) {
        calculatePoints(center, vertex);
    }

    Octagon(const Octagon& other) {
        for (int i = 0; i < 8; ++i)
            points[i] = std::make_unique<Point<T>>(*other.points[i]);
    }

    Octagon& operator=(const Octagon& other) {
        if (this == &other)
            return *this;
        
        for (int i = 0; i < 8; ++i)
            points[i] = std::make_unique<Point<T>>(*other.points[i]);
        
        return *this;
    }

    Octagon(Octagon&& other) noexcept {
        for (int i = 0; i < 8; ++i)
            points[i] = std::move(other.points[i]);
    }

    Octagon& operator=(Octagon&& other) noexcept {
        if (this == &other)
            return *this;
        
        for (int i = 0; i < 8; ++i)
            points[i] = std::move(other.points[i]);
        
        return *this;
    }

    Point<T> center() const override {
        T sumX{0}, sumY{0};

        for (const auto& point : points) {
            sumX += point->x();
            sumY += point->y();
        }

        return Point<T>(sumX / 8, sumY / 8);
    }

    operator double() const override {
        T side = std::hypot(points[1]->x() - points[0]->x(),
                            points[1]->y() - points[0]->y());
        return static_cast<double>(2 * (1 + std::sqrt(2)) * side * side);
    }

    bool equals(const Figure<T>& other) const override {
        const auto* otherOctagon = dynamic_cast<const Octagon<T>*>(&other);
        
        if (!otherOctagon)
            return false;

        for (size_t i = 0; i < 8; ++i)
            if (*(points[i]) != *(otherOctagon->points[i]))
                return false;
        
        return true;
    }

protected:
    void print(std::ostream& os) const override {
        os << "Octagon: ";
        for (const auto& point : points)
            os << *point << " ";
    }

    void read(std::istream& is) override {
        Point<T> center, vertex;
        std::cout << "Enter center and one of the vertices coordinates: ";
        is >> center >> vertex;
        calculatePoints(center, vertex);
    }

private:
    std::array<std::unique_ptr<Point<T>>, 8> points;

    void calculatePoints(const Point<T>& center, const Point<T>& vertex) {
        T dx = vertex.x() - center.x();
        T dy = vertex.y() - center.y();

        T radius = std::hypot(dx, dy);
        if (!radius) {
            std::cout << "Points are identical, resetting to unit octagon.\n";
            return calculatePoints(Point<T>(0, 0), Point<T>(1, 0));
        }

        T baseAngle = std::atan2(dy, dx);
        for (size_t i = 0; i < 8; ++i) {
            T angle = baseAngle + i * (std::numbers::pi_v<T> / 4);
            T x = center.x() + radius * std::cos(angle);
            T y = center.y() + radius * std::sin(angle);
            points[i] = std::make_unique<Point<T>>(x, y);
        }
    }
};