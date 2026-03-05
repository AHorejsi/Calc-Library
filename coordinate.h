#ifndef LINE_H
#define LINE_H


typedef struct {
    double xPos;
    double yPos;
} point2d_t;

typedef struct {
    double slope;
    double yIntercept;
} line_t;

point2d_t rotate(const point2d_t, const double);

point2d_t translate(const point2d_t, const point2d_t);

point2d_t rotate_and_translate(point2d_t, const point2d_t, const double);

double distance(const point2d_t, const point2d_t);

line_t between(const point2d_t, const point2d_t);

point2d_t intersection(const line_t, const line_t);


#endif
