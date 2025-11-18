#pragma once

#include "figure.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <iomanip>
#include <type_traits>

template <typename T>
class Array {
public:
    Array() : data(std::make_shared<T[]>(2)), capacity(2), size(0) {}
    ~Array() = default;

    Array(const Array& other) = delete;
    Array& operator=(const Array& other) = delete;

    Array(Array&& other) noexcept
        : data(std::move(other.data)), capacity(other.capacity), size(other.size) {
        other.capacity = 0;
        other.size = 0;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            capacity = other.capacity;
            size = other.size;

            other.capacity = 0;
            other.size = 0;
        }
        return *this;
    }

    template <typename U>
    void add(U&& fig) {
        if (size >= capacity)
            grow();
        data[size++] = std::forward<U>(fig);
    }

    void remove(size_t index) {
        if (!size)
            throw std::out_of_range("Array is empty");
        if (index >= size)
            throw std::out_of_range("Index out of range");
        
        for (size_t i = index; i < size - 1; ++i)
            data[i] = std::move(data[i + 1]);

        if constexpr (requires { data[size - 1].reset(); })
            data[size - 1].reset();

        --size;
        std::cout << "Element at index " << index << " removed.\n";
    }

    void printAll() const {
        if (!size) 
            throw std::out_of_range("Array is empty");

        for (size_t i = 0; i < size; ++i) {
            if constexpr (requires { *data[i]; }) {
                std::cout << i << ": " << *data[i]
                          << " | Area: " << static_cast<double>(*data[i]) << "\n";
            } else {
                std::cout << i << ": " << data[i]
                          << " | Area: " << static_cast<double>(data[i]) << "\n";
            }
        }
    }

    void printCenters() const {
        if (!size) 
            throw std::out_of_range("Array is empty");

        for (size_t i = 0; i < size; ++i) {
            if constexpr (requires { data[i]->center(); }) {
                auto c = data[i]->center();
                std::cout << i << ": Center = (" << c.x() << ", " << c.y() << ")\n";
            } else if constexpr (requires { data[i].center(); }) {
                auto c = data[i].center();
                std::cout << i << ": Center = (" << c.x() << ", " << c.y() << ")\n";
            }
        }
    }

    void printTotalArea() const {
        if (!size) 
            throw std::out_of_range("Array is empty");
        
        double totalArea = 0.0;
        for (size_t i = 0; i < size; ++i) {
            if constexpr (requires { double(data[i]); }) 
                totalArea += double(data[i]);
            else if constexpr (requires { double(*data[i]); }) 
                totalArea += double(*data[i]);
        }

        std::cout << "Total Area: " << totalArea << "\n";
    }

    T& operator[](size_t index) {
        if (index >= size) 
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) 
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    int getSize() const {
        return static_cast<int>(size);
    }

private:
    void grow() {
        capacity = (capacity == 0) ? 2 : capacity * 2;
        auto newData = std::make_shared<T[]>(capacity);

        for (size_t i = 0; i < size; ++i)
            newData[i] = data[i];

        data = newData;
    }

    std::shared_ptr<T[]> data;
    size_t capacity;
    size_t size;
};