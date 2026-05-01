#ifndef SHAPE2D_H
#define SHAPE2D_H

#include <stdint.h>
#include "coordinate.h"


typedef struct {
    point2d_t* vertices;
    size_t pointCount;
} polygon_t;

typedef struct {
    double(*perimeter)(const void*);
    double(*area)(const void*);
    polygon_t(*polygon)(const void*, const point2d_t*, const size_t, const double);
    void(*destroy)(void*);
} shape_vtable_t;

typedef struct {
    void* ptr;
    const char* name;
    shape_vtable_t vtable;
} shape_t;

shape_t* make_circle(const double);

shape_t* make_ellipse(const double, const double);

shape_t* make_right_triangle(const double, const double);

shape_t* make_rectangle(const double, const double);

shape_t* make_regular_polygon(const double, const uint32_t);

shape_t* make_polygon(point2d_t*, size_t);

void destroy_shape(shape_t*);

double perimeter(const shape_t*);

double area(const shape_t*);

polygon_t from_shape(const shape_t*, const point2d_t*, const size_t, const double);

double perimeter_of_polygon(const polygon_t*);

double area_of_polygon(const polygon_t*);

void destroy_polygon(polygon_t*);


#endif
