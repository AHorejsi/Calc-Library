#define OPTION_CIRCLE 1
#define OPTION_ELLIPSE 2
#define OPTION_RIGHT_TRIANGLE 3
#define OPTION_RECTANGLE 4
#define OPTION_REGULAR_POLYGON 5
#define OPTION_POLYGON 6

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "print.h"
#include "error.h"
#include "radian.h"
#include "shape2d.h"


typedef enum {
    CIRCLE,
    ELLIPSE,
    RIGHT_TRIANGLE,
    RECTANGLE,
    REGULAR_POLYGON,
    POLYGON
} shape_option_t;

typedef struct {
    double perimeter;
    double area;
} result_t;

static point2d_t input_center(void) {
    double xPos;
    input("Enter the X-position of the center of the shape: ", "%lf", &xPos);

    double yPos;
    input("Enter the Y-position of the center of the shape: ", "%lf", &yPos);

    new_line();

    point2d_t center = { xPos, yPos };

    return center;
}

static size_t input_point_count(const shape_option_t option) {
    switch (option) {
    case RIGHT_TRIANGLE:
    case RECTANGLE:
    case REGULAR_POLYGON:
    case POLYGON:
        return 0;
    }

    size_t pointCount;
    input("Enter the amount of points to use: ", "%u", &pointCount);
    ensure_greater_than(pointCount, 2, "Point Count");

    new_line();

    return pointCount;
}

static double input_rotation(const shape_option_t option) {
    if (CIRCLE == option) {
        return 0;
    }

    double rotationInDegrees;
    input("Enter the angle of rotation (degrees): ", "%lf", &rotationInDegrees);

    new_line();
    
    return to_radians(rotationInDegrees);
}

static polygon_t enter_polygon_adjustments(const shape_t* shape, const shape_option_t option) {
    point2d_t center = input_center();
    size_t pointCount = input_point_count(option);
    double rotation = input_rotation(option);

    return from(shape, center, pointCount, rotation);
}

static result_t get_polygon_results(const shape_t* shape, const shape_option_t option) {
    polygon_t polygon = enter_polygon_adjustments(shape, option);
    result_t results = { perimeter_of_polygon(&polygon), area_of_polygon(&polygon) };

    destroy_polygon(&polygon);

    return results;
}

static void display_results(const result_t* results, const char* name) {
    printf("%s Perimeter: %lf\n", name, results->perimeter);
    printf("%s Area: %lf\n", name, results->area);

    new_line();
}

static void display_discrepancies(const result_t* shape, const result_t* polygon) {
    double perimeterDiff = fabs(shape->perimeter - polygon->perimeter);
    double areaDiff = fabs(shape->area - polygon->area);

    printf("Perimeter Discrepancy: %lf\n", perimeterDiff);
    printf("Area Discrepancy: %lf\n", areaDiff);

    new_line();
}

static void display_properties(const shape_t* shape, const shape_option_t option) {
    result_t shapeResults = { perimeter(shape), area(shape) };
    result_t polygonResults = get_polygon_results(shape, option);

    display_results(&shapeResults, shape->name);
    display_results(&polygonResults, "Polygon");

    display_discrepancies(&shapeResults, &polygonResults);
}

static shape_t* input_circle(void) {
    double radius;
    input("Enter radius: ", "%lf", &radius);
    ensure_positive(radius, "Radius");

    return make_circle(radius);
}

static shape_t* input_ellipse(void) {
    double vertical;
    input("Enter vertical height: ", "%lf", &vertical);
    ensure_positive(vertical, "Vertical Height");

    double horizontal;
    input("Enter horizontal width: ", "%lf", &horizontal);
    ensure_positive(horizontal, "Horizontal Width");

    return make_ellipse(vertical, horizontal);
}

static shape_t* input_right_triangle(void) {
    double vertical;
    input("Enter the length of the vertical side: ", "%lf", &vertical);
    ensure_positive(vertical, "Vertical Length");

    double horizontal;
    input("Enter the length of the horizontal side: ", "%lf", &horizontal);
    ensure_positive(horizontal, "Horizontal Length");

    return make_right_triangle(vertical, horizontal);
}

static shape_t* input_rectangle(void) {
    double length;
    input("Enter the length: ", "%lf", &length);
    ensure_positive(length, "Length");

    double width;
    input("Enter the width: ", "%lf", &width);
    ensure_positive(width, "Width");

    return make_rectangle(length, width);
}

static shape_t* input_regular_polygon(void) {
    uint32_t sideCount;
    input("Enter the number of sides: ", "%u", &sideCount);
    ensure_greater_than(sideCount, 2, "Number of Sides");

    double sideLength;
    input("Enter the length of the sides: ", "%lf", &sideLength);
    ensure_positive(sideLength, "Side Length");

    return make_regular_polygon(sideLength, sideCount);
}

static point2d_t* enter_polygon_vertices(const size_t pointCount) {
    point2d_t* vertices = (point2d_t*)malloc(pointCount * sizeof(point2d_t));
    check_alloc(vertices);

    const char X_AXIS = 'X';
    const char Y_AXIS = 'Y';

    const char* format = "Enter the %c-position (Point %u): ";
    char prompt[40];

    for (size_t index = 0; index < pointCount; ++index) {
        size_t count = index + 1;

        double xPos;
        sprintf(prompt, format, X_AXIS, count);
        input(prompt, "%lf", &xPos);
        
        double yPos;
        sprintf(prompt, format, Y_AXIS, count);
        input(prompt, "%lf", &yPos);

        point2d_t point = { xPos, yPos };

        vertices[index] = point;
    }

    return vertices;
}

static shape_t* input_polygon(void) {
    size_t pointCount;
    input("Enter the amount of points: ", "%u", &pointCount);
    ensure_greater_than(pointCount, 2, "Amount of Points");

    point2d_t* vertices = enter_polygon_vertices(pointCount);

    return make_polygon(vertices, pointCount);
}

static shape_t* enter_shape_data(const shape_option_t option) {
    shape_t* result;

    switch (option) {
    case CIRCLE:
        result = input_circle();

        break;
    case ELLIPSE:
        result = input_ellipse();

        break;
    case RIGHT_TRIANGLE:
        result = input_right_triangle();

        break;
    case RECTANGLE:
        result = input_rectangle();

        break;
    case REGULAR_POLYGON:
        result = input_regular_polygon();

        break;
    case POLYGON:
        result = input_polygon();

        break;
    }

    new_line();

    return result;
}

static shape_option_t choose_shape_option(void) {
    static const char* prompt =
        "Choose Shape:\n"
        "  Circle(1)\n"
        "  Ellipse(2)\n"
        "  Right Triangle(3)\n"
        "  Rectangle(4)\n"
        "  Regular Polygon(5)\n"
        "  Polygon(6)\n"
        "Enter (1-6): ";
    uint8_t choice;
    input(prompt, "%u", &choice);

    new_line();
    
    switch (choice) {
    case OPTION_CIRCLE:
        return CIRCLE;
    case OPTION_ELLIPSE:
        return ELLIPSE;
    case OPTION_RIGHT_TRIANGLE:
        return RIGHT_TRIANGLE;
    case OPTION_RECTANGLE:
        return RECTANGLE;
    case OPTION_REGULAR_POLYGON:
        return REGULAR_POLYGON;
    case OPTION_POLYGON:
        return POLYGON;
    default:
        fail("ERROR: Input must be between 1 and 6\n");
    }
}

void run_shape2d(void) {
    shape_option_t option = choose_shape_option();
    shape_t* shape = enter_shape_data(option);
    display_properties(shape, option);
    destroy_shape(shape);
}
