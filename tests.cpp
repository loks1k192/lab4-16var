#include <gtest/gtest.h>
#include "array.h"
#include "triangle.h"
#include "square.h"
#include "octagon.h"


// Point

TEST(PointTest, DefaultConstructor) {
    Point<double> p;
    EXPECT_DOUBLE_EQ(p.x(), 0.0);
    EXPECT_DOUBLE_EQ(p.y(), 0.0);
}

TEST(PointTest, ParameterizedConstructor) {
    Point<double> p(3.5, 4.2);
    EXPECT_DOUBLE_EQ(p.x(), 3.5);
    EXPECT_DOUBLE_EQ(p.y(), 4.2);
}

TEST(PointTest, EqualityOperator) {
    Point<double> p1(1.0, 2.0);
    Point<double> p2(1.0, 2.0);
    Point<double> p3(1.0, 3.0);
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}


// Square

TEST(SquareTest, CenterAndArea) {
    Square<double> sq(Point<double>(0, 0), Point<double>(2, 0));
    auto c = sq.center();
    EXPECT_NEAR(c.x(), 1.0, 1e-9);
    EXPECT_NEAR(c.y(), 1.0, 1e-9);
    EXPECT_NEAR(static_cast<double>(sq), 4.0, 1e-9);
}

TEST(SquareTest, EqualityOperator) {
    Square<double> s1(Point<double>(0, 0), Point<double>(1, 0));
    Square<double> s2(Point<double>(0, 0), Point<double>(1, 0));
    const Figure<double>& f1 = s1;
    const Figure<double>& f2 = s2;
    EXPECT_TRUE(f1 == f2);
}


// Triangle 

TEST(TriangleTest, CenterAndArea) {
    Triangle<double> tri(Point<double>(0, 0), Point<double>(2, 0), 2.0);
    auto c = tri.center();
    EXPECT_NEAR(c.x(), 1.0, 1e-9);
    EXPECT_NEAR(c.y(), 2.0 / 3.0, 1e-9);
    EXPECT_NEAR(static_cast<double>(tri), 2.0, 1e-9);
}

TEST(TriangleTest, EqualityOperator) {
    Triangle<double> t1(Point<double>(0, 0), Point<double>(2, 0), 2.0);
    Triangle<double> t2(Point<double>(0, 0), Point<double>(2, 0), 2.0);
    const Figure<double>& f1 = t1;
    const Figure<double>& f2 = t2;
    EXPECT_TRUE(f1 == f2);
}


// Octagon

TEST(OctagonTest, CenterAndArea) {
    Octagon<double> o(Point<double>(0, 0), Point<double>(1, 0));
    auto c = o.center();
    EXPECT_NEAR(c.x(), 0.0, 1e-9);
    EXPECT_NEAR(c.y(), 0.0, 1e-9);
    EXPECT_NEAR(static_cast<double>(o), 2.82842712, 1e-6);
}

TEST(OctagonTest, EqualityOperator) {
    Octagon<double> o1(Point<double>(0, 0), Point<double>(1, 0));
    Octagon<double> o2(Point<double>(0, 0), Point<double>(1, 0));
    const Figure<double>& f1 = o1;
    const Figure<double>& f2 = o2;
    EXPECT_TRUE(f1 == f2);
}


// Array 

// (shared_ptr<Figure>)

class ArrayPolyTest : public ::testing::Test {
protected:
    void SetUp() override {
        sq = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0));
        tri = std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 2.0);
        oct = std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0));
    }

    Array<std::shared_ptr<Figure<double>>> arr;
    std::shared_ptr<Square<double>> sq;
    std::shared_ptr<Triangle<double>> tri;
    std::shared_ptr<Octagon<double>> oct;
};

TEST_F(ArrayPolyTest, AddAndSize) {
    EXPECT_EQ(arr.getSize(), 0);
    arr.add(sq);
    arr.add(tri);
    EXPECT_EQ(arr.getSize(), 2);
}

TEST_F(ArrayPolyTest, RemoveWorks) {
    arr.add(sq);
    arr.add(tri);
    arr.remove(0);
    EXPECT_EQ(arr.getSize(), 1);
}

TEST_F(ArrayPolyTest, RemoveInvalidThrows) {
    EXPECT_THROW(arr.remove(0), std::out_of_range);
}

TEST_F(ArrayPolyTest, PrintEmptyThrows) {
    EXPECT_THROW(arr.printAll(), std::out_of_range);
    EXPECT_THROW(arr.printCenters(), std::out_of_range);
    EXPECT_THROW(arr.printTotalArea(), std::out_of_range);
}

TEST_F(ArrayPolyTest, TotalAreaOutput) {
    arr.add(sq);
    arr.add(tri);
    testing::internal::CaptureStdout();
    arr.printTotalArea();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(out.find("Total Area") != std::string::npos);
}

TEST_F(ArrayPolyTest, MoveSemantics) {
    arr.add(sq);
    arr.add(tri);
    Array<std::shared_ptr<Figure<double>>> moved = std::move(arr);
    EXPECT_EQ(moved.getSize(), 2);
    EXPECT_EQ(arr.getSize(), 0);
}


// (Square<double>)

TEST(ArraySquareDoubleTest, DefaultConstruction) {
    Array<Square<double>> squares;
    EXPECT_EQ(squares.getSize(), 0);
}

TEST(ArraySquareDoubleTest, AddMultipleSquares) {
    Array<Square<double>> squares;
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0)));
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(2, 0)));
    squares.add(Square<double>(Point<double>(1, 1), Point<double>(3, 1)));
    EXPECT_EQ(squares.getSize(), 3);
}

TEST(ArraySquareDoubleTest, RemoveMiddleElement) {
    Array<Square<double>> squares;
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0)));
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(2, 0)));
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(3, 0)));
    
    squares.remove(1);
    EXPECT_EQ(squares.getSize(), 2);
    
    EXPECT_NEAR(static_cast<double>(squares[0]), 1.0, 1e-9);
    EXPECT_NEAR(static_cast<double>(squares[1]), 9.0, 1e-9);
}

TEST(ArraySquareDoubleTest, RemoveLastElement) {
    Array<Square<double>> squares;
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0)));
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(2, 0)));
    
    squares.remove(1);
    EXPECT_EQ(squares.getSize(), 1);
    EXPECT_NEAR(static_cast<double>(squares[0]), 1.0, 1e-9);
}

TEST(ArraySquareDoubleTest, IndexOperator) {
    Array<Square<double>> squares;
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0)));
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(2, 0)));
    
    EXPECT_NEAR(static_cast<double>(squares[0]), 1.0, 1e-9);
    EXPECT_NEAR(static_cast<double>(squares[1]), 4.0, 1e-9);
}

TEST(ArraySquareDoubleTest, IndexOutOfRange) {
    Array<Square<double>> squares;
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0)));
    
    EXPECT_THROW(squares[5], std::out_of_range);
}

TEST(ArraySquareDoubleTest, PrintEmptyArrayThrows) {
    Array<Square<double>> squares;
    EXPECT_THROW(squares.printAll(), std::out_of_range);
    EXPECT_THROW(squares.printCenters(), std::out_of_range);
    EXPECT_THROW(squares.printTotalArea(), std::out_of_range);
}

TEST(ArraySquareDoubleTest, TotalAreaCalculation) {
    Array<Square<double>> squares;
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0))); // area = 1
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(2, 0))); // area = 4
    
    testing::internal::CaptureStdout();
    squares.printTotalArea();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("5") != std::string::npos);
}

TEST(ArraySquareDoubleTest, MoveConstructor) {
    Array<Square<double>> squares;
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0)));
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(2, 0)));
    
    Array<Square<double>> moved(std::move(squares));
    EXPECT_EQ(moved.getSize(), 2);
    EXPECT_EQ(squares.getSize(), 0);
}

TEST(ArraySquareDoubleTest, MoveAssignment) {
    Array<Square<double>> squares;
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(1, 0)));
    squares.add(Square<double>(Point<double>(0, 0), Point<double>(2, 0)));
    
    Array<Square<double>> moved;
    moved = std::move(squares);
    
    EXPECT_EQ(moved.getSize(), 2);
    EXPECT_EQ(squares.getSize(), 0);
}


// (Square<int>)

TEST(ArraySquareIntTest, WorksWithIntType) {
    Array<Square<int>> squares;
    squares.add(Square<int>(Point<int>(0, 0), Point<int>(3, 0)));
    squares.add(Square<int>(Point<int>(0, 0), Point<int>(4, 0)));
    
    EXPECT_EQ(squares.getSize(), 2);
    EXPECT_NO_THROW(squares.printAll());
    EXPECT_NO_THROW(squares.printCenters());
}

TEST(ArraySquareIntTest, IntegerArithmetic) {
    Array<Square<int>> squares;
    squares.add(Square<int>(Point<int>(0, 0), Point<int>(5, 0))); // area = 25
    
    testing::internal::CaptureStdout();
    squares.printTotalArea();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("25") != std::string::npos);
}

TEST(ArraySquareIntTest, RemoveAndSize) {
    Array<Square<int>> squares;
    squares.add(Square<int>(Point<int>(0, 0), Point<int>(1, 0)));
    squares.add(Square<int>(Point<int>(0, 0), Point<int>(2, 0)));
    squares.add(Square<int>(Point<int>(0, 0), Point<int>(3, 0)));
    
    squares.remove(1);
    EXPECT_EQ(squares.getSize(), 2);
}


// (Triangle<double>)

TEST(ArrayTriangleDoubleTest, AddAndRemove) {
    Array<Triangle<double>> triangles;
    triangles.add(Triangle<double>(Point<double>(0, 0), Point<double>(2, 0), 2.0));
    triangles.add(Triangle<double>(Point<double>(0, 0), Point<double>(3, 0), 3.0));
    
    EXPECT_EQ(triangles.getSize(), 2);
    triangles.remove(0);
    EXPECT_EQ(triangles.getSize(), 1);
}

TEST(ArrayTriangleDoubleTest, PrintOperations) {
    Array<Triangle<double>> triangles;
    triangles.add(Triangle<double>(Point<double>(0, 0), Point<double>(2, 0), 2.0));
    
    EXPECT_NO_THROW(triangles.printAll());
    EXPECT_NO_THROW(triangles.printCenters());
    EXPECT_NO_THROW(triangles.printTotalArea());
}

TEST(ArrayTriangleDoubleTest, TotalArea) {
    Array<Triangle<double>> triangles;
    triangles.add(Triangle<double>(Point<double>(0, 0), Point<double>(2, 0), 2.0)); // area = 2
    triangles.add(Triangle<double>(Point<double>(0, 0), Point<double>(4, 0), 2.0)); // area = 4
    
    testing::internal::CaptureStdout();
    triangles.printTotalArea();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("6") != std::string::npos);
}

TEST(ArrayTriangleDoubleTest, MoveSemantics) {
    Array<Triangle<double>> triangles;
    triangles.add(Triangle<double>(Point<double>(0, 0), Point<double>(2, 0), 2.0));
    
    Array<Triangle<double>> moved = std::move(triangles);
    EXPECT_EQ(moved.getSize(), 1);
    EXPECT_EQ(triangles.getSize(), 0);
}


// (Triangle<int>)

TEST(ArrayTriangleIntTest, BasicOperations) {
    Array<Triangle<int>> triangles;
    triangles.add(Triangle<int>(Point<int>(0, 0), Point<int>(4, 0), 3));
    triangles.add(Triangle<int>(Point<int>(0, 0), Point<int>(6, 0), 4));
    
    EXPECT_EQ(triangles.getSize(), 2);
    EXPECT_NO_THROW(triangles.printAll());
}


// (Octagon<double>)

TEST(ArrayOctagonDoubleTest, AddMultipleOctagons) {
    Array<Octagon<double>> octagons;
    octagons.add(Octagon<double>(Point<double>(0, 0), Point<double>(1, 0)));
    octagons.add(Octagon<double>(Point<double>(0, 0), Point<double>(2, 0)));
    octagons.add(Octagon<double>(Point<double>(5, 5), Point<double>(6, 5)));
    
    EXPECT_EQ(octagons.getSize(), 3);
}

TEST(ArrayOctagonDoubleTest, RemoveAll) {
    Array<Octagon<double>> octagons;
    octagons.add(Octagon<double>(Point<double>(0, 0), Point<double>(1, 0)));
    octagons.add(Octagon<double>(Point<double>(0, 0), Point<double>(2, 0)));
    
    octagons.remove(0);
    octagons.remove(0);
    
    EXPECT_EQ(octagons.getSize(), 0);
    EXPECT_THROW(octagons.printAll(), std::out_of_range);
}

TEST(ArrayOctagonDoubleTest, PrintOperations) {
    Array<Octagon<double>> octagons;
    octagons.add(Octagon<double>(Point<double>(0, 0), Point<double>(1, 0)));
    
    EXPECT_NO_THROW(octagons.printAll());
    EXPECT_NO_THROW(octagons.printCenters());
    EXPECT_NO_THROW(octagons.printTotalArea());
}

TEST(ArrayOctagonDoubleTest, IndexAccess) {
    Array<Octagon<double>> octagons;
    octagons.add(Octagon<double>(Point<double>(0, 0), Point<double>(1, 0)));
    
    EXPECT_NO_THROW(octagons[0]);
    EXPECT_THROW(octagons[1], std::out_of_range);
}


// Mixed

TEST(ArrayMixedTest, GrowthBehavior) {
    Array<Square<double>> squares;
    
    for (int i = 1; i <= 50; ++i)
        squares.add(Square<double>(Point<double>(0, 0), Point<double>(i, 0)));
    
    EXPECT_EQ(squares.getSize(), 50);
}

TEST(ArrayMixedTest, RemoveFromEmptyThrows) {
    Array<Triangle<double>> triangles;
    EXPECT_THROW(triangles.remove(0), std::out_of_range);
}


// Integration

TEST(IntegrationTest, AddRemoveAndPrintAll) {
    Array<std::shared_ptr<Figure<double>>> figs;

    auto sq = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0));
    auto tri = std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 2.0);
    auto oct = std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0));

    figs.add(sq);
    figs.add(tri);
    figs.add(oct);
    
    EXPECT_EQ(figs.getSize(), 3);

    EXPECT_NO_THROW(figs.printAll());
    EXPECT_NO_THROW(figs.printCenters());
    EXPECT_NO_THROW(figs.printTotalArea());

    figs.remove(1);
    EXPECT_EQ(figs.getSize(), 2);
}

TEST(IntegrationTest, ComplexScenarioWithMultipleOperations) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    figs.add(std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(2, 0)));
    figs.add(std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(4, 0), 2.0));
    figs.add(std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0)));
    figs.add(std::make_shared<Square<double>>(Point<double>(5, 5), Point<double>(8, 5)));
    
    EXPECT_EQ(figs.getSize(), 4);
    
    figs.remove(1);
    EXPECT_EQ(figs.getSize(), 3);
    
    figs.add(std::make_shared<Triangle<double>>(Point<double>(1, 1), Point<double>(3, 1), 1.5));
    EXPECT_EQ(figs.getSize(), 4);
    
    EXPECT_NO_THROW(figs.printAll());
    EXPECT_NO_THROW(figs.printCenters());
    EXPECT_NO_THROW(figs.printTotalArea());
    
    figs.remove(0);
    EXPECT_EQ(figs.getSize(), 3);
}

TEST(IntegrationTest, TotalAreaCalculationWithMixedFigures) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    figs.add(std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(2, 0)));    
    figs.add(std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(4, 0), 2.0));
    
    testing::internal::CaptureStdout();
    figs.printTotalArea();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("8") != std::string::npos);
}

TEST(IntegrationTest, RemoveAllElementsOneByOne) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    figs.add(std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0)));
    figs.add(std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 2.0));
    figs.add(std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0)));
    
    EXPECT_EQ(figs.getSize(), 3);
    
    figs.remove(2);
    EXPECT_EQ(figs.getSize(), 2);
    
    figs.remove(1);
    EXPECT_EQ(figs.getSize(), 1);
    
    figs.remove(0);
    EXPECT_EQ(figs.getSize(), 0);
    
    EXPECT_THROW(figs.printAll(), std::out_of_range);
    EXPECT_THROW(figs.remove(0), std::out_of_range);
}

TEST(IntegrationTest, LargeCollectionOfFigures) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    for (int i = 0; i < 20; ++i) {
        if (i % 3 == 0) {
            figs.add(std::make_shared<Square<double>>(
                Point<double>(i, 0), Point<double>(i + 1, 0)));
        } else if (i % 3 == 1) {
            figs.add(std::make_shared<Triangle<double>>(
                Point<double>(i, 0), Point<double>(i + 2, 0), 1.0));
        } else {
            figs.add(std::make_shared<Octagon<double>>(
                Point<double>(i, 0), Point<double>(i + 1, 0)));
        }
    }
    
    EXPECT_EQ(figs.getSize(), 20);
    
    for (int i = 9; i >= 0; --i)
        figs.remove(i * 2);
    
    EXPECT_EQ(figs.getSize(), 10);
    EXPECT_NO_THROW(figs.printAll());
}

TEST(IntegrationTest, AccessElementsAfterModifications) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    auto sq1 = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0));
    auto tri = std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 2.0);
    auto sq2 = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(3, 0));
    
    figs.add(sq1);
    figs.add(tri);
    figs.add(sq2);
    
    EXPECT_NEAR(static_cast<double>(*figs[0]), 1.0, 1e-9);  // sq1: area = 1
    EXPECT_NEAR(static_cast<double>(*figs[1]), 2.0, 1e-9);  // tri: area = 2
    EXPECT_NEAR(static_cast<double>(*figs[2]), 9.0, 1e-9);  // sq2: area = 9
    
    figs.remove(1);
    
    EXPECT_NEAR(static_cast<double>(*figs[0]), 1.0, 1e-9);  // sq1
    EXPECT_NEAR(static_cast<double>(*figs[1]), 9.0, 1e-9);  // sq2
}

TEST(IntegrationTest, CentersCalculationForMixedFigures) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    figs.add(std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(2, 0)));
    figs.add(std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 2.0));
    figs.add(std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0)));
    
    auto c1 = figs[0]->center();
    EXPECT_NEAR(c1.x(), 1.0, 1e-9);
    EXPECT_NEAR(c1.y(), 1.0, 1e-9);
    
    auto c2 = figs[1]->center();
    EXPECT_NEAR(c2.x(), 1.0, 1e-9);
    EXPECT_NEAR(c2.y(), 2.0/3.0, 1e-9);
    
    auto c3 = figs[2]->center();
    EXPECT_NEAR(c3.x(), 0.0, 1e-9);
    EXPECT_NEAR(c3.y(), 0.0, 1e-9);
}

TEST(IntegrationTest, MoveEntireArrayWithFigures) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    figs.add(std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0)));
    figs.add(std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 2.0));
    figs.add(std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0)));
    
    EXPECT_EQ(figs.getSize(), 3);
    
    Array<std::shared_ptr<Figure<double>>> moved(std::move(figs));
    
    EXPECT_EQ(moved.getSize(), 3);
    EXPECT_EQ(figs.getSize(), 0);
    
    EXPECT_NO_THROW(moved.printAll());
    EXPECT_THROW(figs.printAll(), std::out_of_range);
}

TEST(IntegrationTest, ComparisonBetweenDifferentFigures) {
    auto sq = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(2, 0));
    auto tri = std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(4, 0), 2.0);
    auto oct = std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0));
    
    EXPECT_FALSE(*sq == *tri);
    EXPECT_FALSE(*sq == *oct);
    EXPECT_FALSE(*tri == *oct);
}

TEST(IntegrationTest, SameFigureTypeButDifferentParameters) {
    auto sq1 = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0));
    auto sq2 = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(2, 0));
    auto sq3 = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0));
    
    EXPECT_FALSE(*sq1 == *sq2);
    EXPECT_TRUE(*sq1 == *sq3);
}

TEST(IntegrationTest, StressTestWithManyOperations) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    for (int i = 0; i < 50; ++i)
        figs.add(std::make_shared<Square<double>>(
            Point<double>(i, 0), Point<double>(i + 1, 0)));
    
    EXPECT_EQ(figs.getSize(), 50);
    
    for (int i = 0; i < 25; ++i)
        figs.remove(0);
    
    EXPECT_EQ(figs.getSize(), 25);
    
    for (int i = 0; i < 25; ++i)
        figs.add(std::make_shared<Triangle<double>>(
            Point<double>(i, 0), Point<double>(i + 2, 0), 1.0));
        
    EXPECT_EQ(figs.getSize(), 50);
    
    EXPECT_NO_THROW(figs.printAll());
    EXPECT_NO_THROW(figs.printCenters());
    EXPECT_NO_THROW(figs.printTotalArea());
}

TEST(IntegrationTest, IndexAccessWithBoundaryChecks) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    figs.add(std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0)));
    figs.add(std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 2.0));
    figs.add(std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0)));
    
    EXPECT_NO_THROW(figs[0]);
    EXPECT_NO_THROW(figs[1]);
    EXPECT_NO_THROW(figs[2]);
    
    EXPECT_THROW(figs[3], std::out_of_range);
    EXPECT_THROW(figs[100], std::out_of_range);
    
    figs.remove(1);
    EXPECT_NO_THROW(figs[0]);
    EXPECT_NO_THROW(figs[1]);
    EXPECT_THROW(figs[2], std::out_of_range);
}

TEST(IntegrationTest, EmptyArrayOperations) {
    Array<std::shared_ptr<Figure<double>>> figs;
    
    EXPECT_THROW(figs.printAll(), std::out_of_range);
    EXPECT_THROW(figs.printCenters(), std::out_of_range);
    EXPECT_THROW(figs.printTotalArea(), std::out_of_range);
    EXPECT_THROW(figs.remove(0), std::out_of_range);
    EXPECT_THROW(figs[0], std::out_of_range);
    
    EXPECT_EQ(figs.getSize(), 0);
}

TEST(IntegrationTest, MixedIntAndDoubleSquares) {
    Array<Square<int>> intSquares;
    Array<Square<double>> doubleSquares;
    
    intSquares.add(Square<int>(Point<int>(0, 0), Point<int>(5, 0)));
    doubleSquares.add(Square<double>(Point<double>(0, 0), Point<double>(5.0, 0)));
    
    EXPECT_NEAR(static_cast<double>(intSquares[0]), 25.0, 1e-9);
    EXPECT_NEAR(static_cast<double>(doubleSquares[0]), 25.0, 1e-9);
}

TEST(IntegrationTest, RealWorldScenario) {
    Array<std::shared_ptr<Figure<double>>> shapes;
    
    shapes.add(std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(10, 0)));
    shapes.add(std::make_shared<Triangle<double>>(Point<double>(0, 10), Point<double>(10, 10), 5.0));
    shapes.add(std::make_shared<Octagon<double>>(Point<double>(5, 5), Point<double>(6, 5)));
    
    EXPECT_EQ(shapes.getSize(), 3);
    
    testing::internal::CaptureStdout();
    shapes.printTotalArea();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Total Area") != std::string::npos);
    
    auto buildingCenter = shapes[0]->center();
    EXPECT_NEAR(buildingCenter.x(), 5.0, 1e-9);
    EXPECT_NEAR(buildingCenter.y(), 5.0, 1e-9);
}


// Type Difference

TEST(FigureTest, DifferentTypesNotEqual) {
    Square<double> s(Point<double>(0, 0), Point<double>(1, 0));
    Triangle<double> t(Point<double>(0, 0), Point<double>(1, 0), 1.0);
    const Figure<double>& f1 = s;
    const Figure<double>& f2 = t;
    EXPECT_FALSE(f1 == f2);
}

TEST(FigureTest, DifferentTypesNotEqual_SameArea) {
    Square<double> sq(Point<double>(0,0), Point<double>(2,0));
    Triangle<double> tri(Point<double>(0,0), Point<double>(4,0), 2);

    const Figure<double>& f1 = sq;
    const Figure<double>& f2 = tri;

    EXPECT_FALSE(f1 == f2);
}

TEST(FigureTest, DifferentTypesNotEqual_SameCenters) {
    Square<double> sq(Point<double>(0,0), Point<double>(2,0));
    Octagon<double> oct(Point<double>(1,1), Point<double>(2,1));

    const Figure<double>& f1 = sq;
    const Figure<double>& f2 = oct;

    EXPECT_FALSE(f1 == f2);
}

TEST(FigureTest, DegenerateFiguresStillNotEqual) {
    Triangle<double> t1(Point<double>(0,0), Point<double>(0,0), 0);
    Square<double> sq(Point<double>(0,0), Point<double>(0,0));

    const Figure<double>& f1 = t1;
    const Figure<double>& f2 = sq;

    EXPECT_FALSE(f1 == f2); 
}

TEST(FigureTest, SameTypeEqualButDifferentParameters) {
    Square<double> s1(Point<double>(0,0), Point<double>(1,0));
    Square<double> s2(Point<double>(0,0), Point<double>(2,0));

    const Figure<double>& f1 = s1;
    const Figure<double>& f2 = s2;

    EXPECT_FALSE(f1 == f2);
}

TEST(FigureTest, OctagonAndTriangleNeverEqual) {
    Octagon<double> o(Point<double>(0,0), Point<double>(3,0));
    Triangle<double> t(Point<double>(0,0), Point<double>(6,0), 3);

    const Figure<double>& f1 = o;
    const Figure<double>& f2 = t;

    EXPECT_FALSE(f1 == f2);
}


// Main

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}