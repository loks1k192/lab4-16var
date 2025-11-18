#pragma once

#include "point.h"

#include <memory>

template <Scalar T>
class Figure {
public:
    virtual ~Figure() noexcept = default;

    virtual Point<T> center() const = 0;
    virtual operator double() const = 0;
    virtual bool equals(const Figure<T>& other) const = 0;

    bool operator==(const Figure<T>& other) const {
        return equals(other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Figure<T>& fig) {
        fig.print(os);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Figure<T>& fig) {
        fig.read(is);
        return is;
    }

protected:
    Figure() = default;

    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;
};