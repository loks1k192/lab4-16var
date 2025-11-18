#pragma once

#include "figure.h"

#include <cmath>
#include <array>

template <Scalar T>
class Square : public Figure<T> {
public:
    Square() {
        for (auto& point : points)
            point = std::make_unique<Point<T>>();
    }

    Square(const Point<T>& A, const Point<T>& B) {
        calculatePoints(A, B);
    }

    Square(const Square& other) {
        for (int i = 0; i < 4; ++i)
            points[i] = std::make_unique<Point<T>>(*other.points[i]);
    }

    Square& operator=(const Square& other) {
        if (this == &other)
            return *this;
        
        for (int i = 0; i < 4; ++i)
            points[i] = std::make_unique<Point<T>>(*other.points[i]);
        
        return *this;
    }

    Square(Square&& other) noexcept {
        for (int i = 0; i < 4; ++i)
            points[i] = std::move(other.points[i]);
    }

    Square& operator=(Square&& other) noexcept {
        if (this == &other)
            return *this;
        
        for (int i = 0; i < 4; ++i)
            points[i] = std::move(other.points[i]);
        
        return *this;
    }

    Point<T> center() const override {
        T sumX{0}, sumY{0};

        for (const auto& point : points) {
            sumX += point->x();
            sumY += point->y();
        }

        return Point<T>(sumX / 4, sumY / 4);
    }

    operator double() const override {
        T side = std::hypot(points[1]->x() - points[0]->x(),
                            points[1]->y() - points[0]->y());
        return static_cast<double>(side * side);
    }

    bool equals(const Figure<T>& other) const override {
        const auto* otherSquare = dynamic_cast<const Square<T>*>(&other);
        
        if (!otherSquare)
            return false;

        for (size_t i = 0; i < 4; ++i)
            if (*(points[i]) != *(otherSquare->points[i]))
                return false;
        
        return true;
    }

protected:
    void print(std::ostream& os) const override {
        os << "Square: ";
        for (const auto& point : points)
            os << *point << " ";
    }

    void read(std::istream& is) override {
        Point<T> A, B;
        std::cout << "Enter bottom side points (A and B): ";
        is >> A >> B;
        calculatePoints(A, B);
    }

private:
    std::array<std::unique_ptr<Point<T>>, 4> points;

    void calculatePoints(const Point<T>& A, const Point<T>& B) {
        T dx = B.x() - A.x();
        T dy = B.y() - A.y();

        if (dx == 0 && dy == 0) {
            std::cout << "Points are identical, resetting to unit square.\n";
            return calculatePoints(Point<T>(0,0), Point<T>(1,0));
        }

        points[0] = std::make_unique<Point<T>>(A);
        points[1] = std::make_unique<Point<T>>(B);
        points[2] = std::make_unique<Point<T>>(B.x() - dy, B.y() + dx);
        points[3] = std::make_unique<Point<T>>(A.x() - dy, A.y() + dx);
    }
};