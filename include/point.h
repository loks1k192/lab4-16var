#pragma once

#include <type_traits>
#include <concepts>
#include <iostream>

template <typename T>
concept Scalar = std::is_scalar_v<T>;

template <Scalar T>
class Point {
public:
    T _x{}, _y{};

    Point() = default;
    Point(T x, T y) : _x(x), _y(y) {}
    Point(const Point& other) = default;
    Point(Point&& other) noexcept = default;

    Point& operator=(const Point& other) = default;
    Point& operator=(Point&& other) noexcept = default;

    bool operator==(const Point& other) const {
        return _x == other._x && _y == other._y;
    }

    T x() const { return _x; }
    T y() const { return _y; }
    
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x() << ", " << p.y() << ")";
    }

    friend std::istream& operator>>(std::istream& is, Point& p) {
        return is >> p._x >> p._y;
    }
};