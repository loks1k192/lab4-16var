/*
    Вариант 16. 
    Фигуры: 8-угольник | Треугольник | Квадрат
*/

#include "array.h"
#include "triangle.h"
#include "square.h"
#include "octagon.h"

#include <climits>
#include <iostream>
#include <memory>

void cinClear() {
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
}

template <typename T>
T readNumer(const std::string& prompt) {
    T value;
    std::cout << prompt;
    if (std::cin >> value) {
        cinClear();
        return value;
    }
    cinClear();
    return -1;
}

int readInt(const std::string& prompt) {
    return readNumer<int>(prompt);
}

double readDouble(const std::string& prompt) {
    return readNumer<double>(prompt);
}

int main() {    
    Array<std::shared_ptr<Figure<double>>> baseFigures;

    baseFigures.add(std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(2, 0)));
    baseFigures.add(std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(1, 0), 1.0));
    baseFigures.add(std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0)));

    std::cout << "\nBase container (Array<std::shared_ptr<Figure<double>>>):\n";
    baseFigures.printAll();

    std::cout << "\nCenters:\n";
    baseFigures.printCenters();

    std::cout << "\n";
    baseFigures.printTotalArea();

    
    Array<Square<double>> squares;

    auto sq = Square<double>(Point<double>(0, 0), Point<double>(2, 0));

    squares.add(sq);
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0)));

    std::cout << "\nThe Legacy Container (Array<Square<double>>):\n";
    squares.printAll();

    std::cout << "\nCenters:\n";
    squares.printCenters();

    std::cout << "\n";
    squares.printTotalArea();

    
    std::cout << "\n\n=== Switching to interactive mode ===\n";

    Array<std::shared_ptr<Figure<double>>> figures;

    while (true) {
        std::cout << "\nMenu:\n"
                  << "1. Add Figure\n"
                  << "2. Remove Figure\n"
                  << "3. Print All Figures\n"
                  << "4. Print Centers\n"
                  << "5. Total Area\n"
                  << "0. Exit\n"
                  << "-> ";

        int choice = readInt("");

        switch (choice) {
            case 0:
                std::cout << "Exiting...\n\n";
                return 0;

            case 1: {
                std::cout << "Select Figure Type:\n"
                          << "  1. Square\n"
                          << "  2. Triangle\n"
                          << "  3. Octagon\n"
                          << "  -> ";

                int figureType = readInt("");

                switch (figureType) {
                    case 1: {
                        auto sq = std::make_shared<Square<double>>();
                        std::cin >> *sq;
                        figures.add(sq);
                        break;
                    }
                    case 2: {
                        auto tri = std::make_shared<Triangle<double>>();
                        std::cin >> *tri;
                        figures.add(tri);
                        break;
                    }
                    case 3: {
                        auto oct = std::make_shared<Octagon<double>>();
                        std::cin >> *oct;
                        figures.add(oct);
                        break;
                    }
                    default:
                        std::cout << "Invalid figure type.\n";
                        break;
                }
                break;
            }

            case 2: {
                if (!figures.getSize()) {
                    std::cout << "Array is empty.\n";
                    break;
                }

                int index = readInt("Enter index of figure to remove: ");
                try {
                    figures.remove(index);
                } catch (const std::out_of_range& e) {
                    std::cout << e.what() << "\n";
                }
                break;
            }

            case 3:
                try {
                    figures.printAll();
                } catch (const std::out_of_range& e) {
                    std::cout << e.what() << "\n";
                }
                break;

            case 4:
                try {
                    figures.printCenters();
                } catch (const std::out_of_range& e) {
                    std::cout << e.what() << "\n";
                }
                break;

            case 5:
                try {
                    figures.printTotalArea();
                } catch (const std::out_of_range& e) {
                    std::cout << e.what() << "\n";
                }
                break;

            default:
                std::cout << "Invalid choice. Try a number between 0 and 5.\n";
                break;
        }
    }

    return 0;
}