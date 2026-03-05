#define _USE_MATH_DEFINES

#define NAME_CIRCLE "Circle"
#define NAME_ELLIPSE "Ellipse"
#define NAME_RIGHT_TRIANGLE "Right Triangle"
#define NAME_RECTANGLE "Rectangle"
#define NAME_REGULAR_POLYGON "Regular Polygon"
#define NAME_POLYGON "Polygon"

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "alloc.h"
#include "radian.h"
#include "shape2d.h"


typedef struct {
    double radius;
} circle_t;

typedef struct {
    double vertical;
    double horizontal;
} ellipse_t;

typedef struct {
    double vertical;
    double horizontal;
} right_triangle_t;

typedef struct {
    double length;
    double width;
} rectangle_t;

typedef struct {
    double sideLength;
    uint32_t sideCount;
} regular_polygon_t;

static void default_destroy(void* ptr) {
    shape_t* shape = (shape_t*)ptr;

    free(shape->ptr);
    free(shape);
}

static double circle_perimeter(const void* ptr) {
    circle_t circle = *(circle_t*)ptr;

    return 2 * M_PI * circle.radius;
}

static double circle_area(const void* ptr) {
    circle_t circle = *(circle_t*)ptr;

    return M_PI * circle.radius * circle.radius;
}

static point2d_t circle_point(const circle_t circle, const point2d_t center, const size_t index, const size_t pointCount) {
    double radians = unit_circle(index, pointCount);

    double xPos = circle.radius * cos(radians);
    double yPos = circle.radius * sin(radians);

    point2d_t point = { xPos, yPos };

    return translate(point, center);
}

static polygon_t circle_to_polygon(const void* ptr, point2d_t center, size_t pointCount, double _) {
    circle_t circle = *(circle_t*)ptr;

    point2d_t* vertices = (point2d_t*)malloc(pointCount * sizeof(point2d_t));
    check_alloc(vertices);

    for (size_t index = 0; index < pointCount; ++index) {
        vertices[index] = circle_point(circle, center, index, pointCount);
    }

    polygon_t result = { vertices, pointCount };

    return result;
}

static shape_t* create_shape(const void* shape, size_t byteCount, const char* name, const shape_vtable_t* vtable) {
    void* ptr = malloc(byteCount);
    check_alloc(ptr);

    memcpy(ptr, shape, byteCount);

    shape_t* result = (shape_t*)malloc(sizeof(shape_t));
    check_alloc(result);

    result->ptr = ptr;
    result->name = name;
    result->vtable = *vtable;

    return result;
}

shape_t* make_circle(const double radius) {
    if (radius <= 0) {
        return NULL;
    }

    circle_t circle = { radius };
    static const shape_vtable_t vtable = { circle_perimeter, circle_area, circle_to_polygon, default_destroy };

    return create_shape(&circle, sizeof(circle_t), NAME_CIRCLE, &vtable);
}

static double ellipse_perimeter(const void* ptr) {
    ellipse_t ellipse = *(ellipse_t*)ptr;
    
    double sum = ellipse.horizontal + ellipse.vertical;
    double diff = ellipse.horizontal - ellipse.vertical;

    double param1 = 3 * (diff * diff) / (sum * sum);
    double param2 = 10 + sqrt(4 - param1);
    double result = M_PI * sum * (1 + param1 / param2);

    return result;
}

static double ellipse_area(const void* ptr) {
    ellipse_t ellipse = *(ellipse_t*)ptr;

    return M_PI * ellipse.vertical * ellipse.horizontal;
}

static point2d_t ellipse_point(const ellipse_t ellipse, const point2d_t center, const double rotation, const size_t index, const size_t pointCount) {
    double radians = unit_circle(index, pointCount);

    point2d_t unitCirclePos = { cos(radians), sin(radians) };
    line_t line = between(center, unitCirclePos);

    double semiminorAxis = fmin(ellipse.horizontal, ellipse.vertical);
    double semimajorAxis = fmax(ellipse.horizontal, ellipse.vertical);

    double semiminorSquare = semiminorAxis * semiminorAxis;
    double semimajorSquare = semimajorAxis * semimajorAxis;

    double xPos = -(center.xPos) + sqrt(1 - pow(line.yIntercept - center.yPos, 2) / semimajorSquare - semiminorSquare);
    double yPos = line.slope * xPos + line.yIntercept;

    point2d_t point = { xPos, yPos };

    return rotate_and_translate(point, center, rotation);
}

static polygon_t ellipse_to_polygon(const void* ptr, point2d_t center, size_t pointCount, double rotation) {
    ellipse_t ellipse = *(ellipse_t*)ptr;

    point2d_t* vertices = (point2d_t*)malloc(pointCount * sizeof(point2d_t));
    check_alloc(vertices);

    for (size_t index = 0; index < pointCount; ++index) {
        vertices[index] = ellipse_point(ellipse, center, rotation, index, pointCount);
    }

    polygon_t result = { vertices, pointCount };

    return result;
}

shape_t* make_ellipse(const double vertical, const double horizontal) {
    if (vertical <= 0 || horizontal <= 0) {
        return NULL;
    }

    ellipse_t ellipse = { vertical, horizontal };
    static const shape_vtable_t vtable = { ellipse_perimeter, ellipse_area, ellipse_to_polygon, default_destroy };

    return create_shape(&ellipse, sizeof(ellipse_t), NAME_ELLIPSE, &vtable);
}

static double hypotenuse(const right_triangle_t shape) {
    return sqrt(shape.vertical * shape.vertical + shape.horizontal * shape.horizontal);
}

static double right_triangle_perimeter(const void* ptr) {
    right_triangle_t rightTriangle = *(right_triangle_t*)ptr;

    return rightTriangle.vertical + rightTriangle.horizontal + hypotenuse(rightTriangle);
}

static double right_triangle_area(const void* ptr) {
    right_triangle_t rightTriangle = *(right_triangle_t*)ptr;

    return 0.5 * rightTriangle.vertical * rightTriangle.horizontal;
}

static point2d_t* find_right_triangle_vertices_around_origin(const right_triangle_t triangle, const size_t POINT_COUNT) {
    point2d_t* vertices = (point2d_t*)malloc(POINT_COUNT * sizeof(right_triangle_t));
    check_alloc(vertices);

    double thirdVertical = triangle.vertical / 3;
    double thirdHorizontal = triangle.horizontal / 3;

    point2d_t top = { thirdHorizontal, 2 * thirdVertical };
    point2d_t left = { -thirdHorizontal, -2 * thirdVertical };
    point2d_t bottom = { thirdHorizontal, -thirdVertical };

    vertices[0] = top;
    vertices[1] = left;
    vertices[2] = bottom;

    return vertices;
}

static polygon_t right_triangle_to_polygon(const void* ptr, const point2d_t center, const size_t _, const double rotation) {
    right_triangle_t triangle = *(right_triangle_t*)ptr;

    const size_t POINT_COUNT = 3;
    point2d_t* vertices = find_right_triangle_vertices_around_origin(triangle, POINT_COUNT);

    vertices[0] = rotate_and_translate(vertices[0], center, rotation);
    vertices[1] = rotate_and_translate(vertices[1], center, rotation);
    vertices[2] = rotate_and_translate(vertices[2], center, rotation);

    polygon_t result = { vertices, POINT_COUNT };

    return result;
}

shape_t* make_right_triangle(const double vertical, const double horizontal) {
    if (vertical <= 0 || horizontal <= 0) {
        return NULL;
    }

    right_triangle_t rightTriangle = { vertical, horizontal };
    static const shape_vtable_t vtable = { right_triangle_perimeter, right_triangle_area, right_triangle_to_polygon, default_destroy };

    return create_shape(&rightTriangle, sizeof(right_triangle_t), NAME_RIGHT_TRIANGLE, &vtable);
}

static double rectangle_perimeter(const void* ptr) {
    rectangle_t rectangle = *(rectangle_t*)ptr;

    return rectangle.length + rectangle.length + rectangle.width + rectangle.width;
}

static double rectangle_area(const void* ptr) {
    rectangle_t rectangle = *(rectangle_t*)ptr;

    return rectangle.length * rectangle.width;
}

static polygon_t rectangle_to_polygon(const void* ptr, const point2d_t center, const size_t _, const double rotation) {
    rectangle_t rectangle = *(rectangle_t*)ptr;

    double halfLength = rectangle.length / 2;
    double halfWidth = rectangle.width / 2;

    point2d_t topLeft = { -halfWidth, halfLength };
    point2d_t topRight = { halfWidth, halfLength };
    point2d_t bottomLeft = { -halfWidth, -halfLength };
    point2d_t bottomRight = { halfWidth, -halfLength };

    const size_t POINT_COUNT = 4;

    point2d_t* vertices = (point2d_t*)malloc(POINT_COUNT * sizeof(point2d_t));
    check_alloc(vertices);

    vertices[0] = rotate_and_translate(topLeft, center, rotation);
    vertices[1] = rotate_and_translate(topRight, center, rotation);
    vertices[2] = rotate_and_translate(bottomRight, center, rotation);
    vertices[3] = rotate_and_translate(bottomLeft, center, rotation);

    polygon_t result = { vertices, POINT_COUNT };

    return result;
}

shape_t* make_rectangle(const double length, const double width) {
    if (length <= 0 || width <= 0) {
        return NULL;
    }

    rectangle_t rectangle = { length, width };
    static const shape_vtable_t vtable = { rectangle_perimeter, rectangle_area, rectangle_to_polygon, default_destroy };

    return create_shape(&rectangle, sizeof(rectangle_t), NAME_RECTANGLE, &vtable);
}

static double regular_polygon_perimeter(const void* ptr) {
    regular_polygon_t regularPolygon = *(regular_polygon_t*)ptr;

    return regularPolygon.sideCount * regularPolygon.sideLength;
}

static double regular_polygon_area(const void* ptr) {
    regular_polygon_t regularPolygon = *(regular_polygon_t*)ptr;

    return (regularPolygon.sideLength * regularPolygon.sideLength * regularPolygon.sideCount) / (4 * tan(M_PI / regularPolygon.sideCount));
}

static double circumradius(const regular_polygon_t shape) {
    return shape.sideLength / (2 * sin(M_PI / shape.sideCount));
}

static polygon_t regular_polygon_to_polygon(const void* ptr, const point2d_t center, const size_t _, const double rotation) {
    regular_polygon_t polygon = *(regular_polygon_t*)ptr;

    point2d_t* vertices = (point2d_t*)malloc(polygon.sideCount * sizeof(point2d_t));
    check_alloc(vertices);

    double distanceToVertices = circumradius(polygon);

    for (size_t index = 0; index < polygon.sideCount; ++index) {
        double radians = unit_circle(index, polygon.sideCount);

        double xPos = distanceToVertices * cos(radians);
        double yPos = distanceToVertices * sin(radians);

        point2d_t point = { xPos, yPos };

        vertices[index] = rotate_and_translate(point, center, rotation);
    }

    polygon_t result = { vertices, polygon.sideCount };

    return result;
}

shape_t* make_regular_polygon(const double sideLength, const uint32_t sideCount) {
    if (sideLength <= 0 || sideCount < 3) {
        return NULL;
    }

    regular_polygon_t regularPolygon = { sideLength, sideCount };
    static const shape_vtable_t vtable = { regular_polygon_perimeter, regular_polygon_area, regular_polygon_to_polygon, default_destroy };

    return create_shape(&regularPolygon, sizeof(regular_polygon_t), NAME_REGULAR_POLYGON, &vtable);
}

static double polygon_perimeter(const void* ptr) {
    polygon_t* polygon = (polygon_t*)ptr;

    return perimeter_of_polygon(polygon);
}

static double polygon_area(const void* ptr) {
    polygon_t* polygon = (polygon_t*)ptr;

    return area_of_polygon(polygon);
}

static void polygon_destroy(void* ptr) {
    shape_t* shape = (shape_t*)ptr;
    polygon_t* polygon = (polygon_t*)(shape->ptr);

    destroy_polygon(polygon);
    default_destroy(shape);
}

static polygon_t polygon_to_polygon(const void* ptr, const point2d_t _1, const size_t _2, const double _3) {
    polygon_t polygon = *(polygon_t*)ptr;

    size_t byteCount = polygon.pointCount * sizeof(point2d_t);

    point2d_t* vertices = (point2d_t*)malloc(byteCount);
    check_alloc(vertices);

    memcpy(vertices, polygon.vertices, byteCount);

    polygon_t result = { vertices, polygon.pointCount };

    return result;
}

static bool in_range(const point2d_t pos1, const point2d_t pos2, const point2d_t intersectPoint) {
    double lowerXPos = fmin(pos1.xPos, pos2.xPos);
    double lowerYPos = fmin(pos1.yPos, pos2.yPos);
    double upperXPos = fmax(pos1.xPos, pos2.xPos);
    double upperYPos = fmax(pos1.yPos, pos2.yPos);

    bool inXRange = intersectPoint.xPos < lowerXPos && intersectPoint.xPos > upperXPos;
    bool inYRange = intersectPoint.yPos < lowerYPos && intersectPoint.yPos > upperYPos;
    bool inRange = !(inXRange && inYRange);

    return inRange;
}

static bool has_intersection(const line_t* lines, const size_t lineCount, const line_t connection, const point2d_t limit1, const point2d_t limit2) {
    for (size_t index = 0; index < lineCount; ++index) {
        point2d_t intersectPoint = intersection(lines[index], connection);

        if (isnan(intersectPoint.xPos)) {
            continue;
        }

        if (in_range(limit1, limit2, intersectPoint)) {
            return true;
        }
    }

    return false;
}

static bool is_polygon(const point2d_t* points, const size_t pointCount) {
    if (pointCount < 3) {
        return false;
    }

    line_t* lines = (line_t*)malloc(pointCount * sizeof(line_t));
    check_alloc(lines);

    size_t endIndex = pointCount - 1;

    for (size_t index = 0; index < pointCount; ++index) {
        size_t nextIndex = (index == endIndex) ? 0 : index + 1;

        point2d_t current = points[index];
        point2d_t next = points[nextIndex];

        line_t connection = between(current, next);

        if (has_intersection(lines, index, connection, current, next)) {
            return false;
        }

        lines[index] = connection;
    }

    free(lines);

    return true;
}

shape_t* make_polygon(point2d_t* vertices, size_t pointCount) {
    if (!is_polygon(vertices, pointCount)) {
        return NULL;
    }

    polygon_t polygon = { vertices, pointCount };
    static const shape_vtable_t vtable = { polygon_perimeter, polygon_area, polygon_to_polygon, polygon_destroy };

    return create_shape(&polygon, sizeof(polygon_t), NAME_POLYGON, &vtable);
}

void destroy_shape(shape_t* shape) {
    if (NULL != shape->vtable.destroy && NULL != shape) {
        shape->vtable.destroy(shape);
    }
}

double perimeter(const shape_t* shape) {
    if (NULL == shape->vtable.perimeter || NULL == shape->ptr) {
        return NAN;
    }
    else {
        return shape->vtable.perimeter(shape->ptr);
    }    
}

double area(const shape_t* shape) {
    if (NULL == shape->vtable.area || NULL == shape->ptr) {
        return NAN;
    }
    else {
        return shape->vtable.area(shape->ptr);
    }
}

static polygon_t get_null_polygon(void) {
    static polygon_t NULL_POLYGON = { NULL, 0 };

    return NULL_POLYGON;
}

polygon_t from(const shape_t* shape, const point2d_t center, const size_t pointCount, const double rotation) {
    if (NULL == shape->ptr || NULL == shape->vtable.polygon) {
        return get_null_polygon();
    }
    else {
        return shape->vtable.polygon(shape->ptr, center, pointCount, rotation);
    }
}

double perimeter_of_polygon(const polygon_t* polygon) {
    double totalPerimeter = 0;

    size_t pointCount = polygon->pointCount;
    size_t endIndex = pointCount - 1;

    for (size_t index = 0; index < pointCount; ++index) {
        size_t nextIndex = (index == endIndex) ? 0 : index + 1;

        point2d_t current = polygon->vertices[index];
        point2d_t next = polygon->vertices[nextIndex];

        totalPerimeter += distance(current, next);
    }

    return totalPerimeter;
}

double area_of_polygon(const polygon_t* polygon) {
    double totalArea = 0;

    size_t pointCount = polygon->pointCount;
    size_t endIndex = pointCount - 1;

    for (size_t index = 0; index < pointCount; ++index) {
        size_t nextIndex = (index == endIndex) ? 0 : index + 1;

        point2d_t current = polygon->vertices[index];
        point2d_t next = polygon->vertices[nextIndex];

        double height = (current.yPos + next.yPos) / 2;
        double width = current.xPos - next.xPos;

        totalArea += height * width;
    }

    return fabs(totalArea);
}

void destroy_polygon(polygon_t* polygon) {
    free(polygon->vertices);
}
