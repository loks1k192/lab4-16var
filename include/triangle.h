#pragma once

#include "figure.h"

#include <cmath>

template <Scalar T>
class Triangle : public Figure<T> {
public:
    Triangle() {
        for (auto& point : points)
            point = std::make_unique<Point<T>>();
    }

    Triangle(const Point<T>& A, const Point<T>& B, T h) {
        calculatePoints(A, B, h);
    }

    Triangle(const Triangle& other) {
        for (int i = 0; i < 3; ++i)
            points[i] = std::make_unique<Point<T>>(*other.points[i]);
    }

    Triangle& operator=(const Triangle& other) {
        if (this == &other)
            return *this;
        
        for (int i = 0; i < 3; ++i)
            points[i] = std::make_unique<Point<T>>(*other.points[i]);
        
        return *this;
    }

    Triangle(Triangle&& other) noexcept {
        for (int i = 0; i < 3; ++i)
            points[i] = std::move(other.points[i]);
    }

    Triangle& operator=(Triangle&& other) noexcept {
        if (this == &other)
            return *this;
        
        for (int i = 0; i < 3; ++i)
            points[i] = std::move(other.points[i]);
        
        return *this;
    }

    Point<T> center() const override {
        T sumX{0}, sumY{0};

        for (const auto& point : points) {
            sumX += point->x();
            sumY += point->y();
        }

        return Point<T>(sumX / 3, sumY / 3);
    }

    operator double() const override {
        T base = std::hypot(points[1]->x() - points[0]->x(),
                            points[1]->y() - points[0]->y());

        Point<T> middle_of_base = Point<T>((points[0]->x() + points[1]->x()) / 2,
                                           (points[0]->y() + points[1]->y()) / 2);
        
        T height = std::hypot(points[2]->x() - middle_of_base.x(),
                              points[2]->y() - middle_of_base.y());
        
        return static_cast<double>(0.5 * base * height);
    }

    bool equals(const Figure<T>& other) const override {
        const auto* otherTriangle = dynamic_cast<const Triangle<T>*>(&other);
        
        if (!otherTriangle)
            return false;

        for (size_t i = 0; i < 3; ++i)
            if (*(points[i]) != *(otherTriangle->points[i]))
                return false;
        
        return true;
    }

protected:
    void print(std::ostream& os) const override {
        os << "Triangle: ";
        for (const auto& point : points)
            os << *point << " ";
    }

    void read(std::istream& is) override {
        Point<T> A, B;
        T h;
        std::cout << "Enter base points (A and B) and height (h): ";
        is >> A >> B >> h;
        calculatePoints(A, B, h);
    }

private:
    std::array<std::unique_ptr<Point<T>>, 3> points;

    void calculatePoints(const Point<T>& A, const Point<T>& B, T h) {
        if (h <= 0) {
            std::cout << "Height must be positive, resetting to unit triangle.\n";
            return calculatePoints(Point<T>(0,0), Point<T>(1,0), 1);
        }
        
        T dx = B.x() - A.x();
        T dy = B.y() - A.y();
        T length = std::hypot(dx, dy);

        if (!length) {
            std::cout << "Points are identical, resetting to unit triangle.\n";
            return calculatePoints(Point<T>(0,0), Point<T>(1,0), 1);
        }

        T nx = -dy / length;
        T ny = dx / length;

        T midX = (A.x() + B.x()) / 2;
        T midY = (A.y() + B.y()) / 2;

        points[0] = std::make_unique<Point<T>>(A);
        points[1] = std::make_unique<Point<T>>(B);
        points[2] = std::make_unique<Point<T>>(midX + nx * h, midY + ny * h);
    }
};