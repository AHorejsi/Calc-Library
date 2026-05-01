#include <math.h>
#include "coordinate.h"


point2d_t rotate(const point2d_t* point, const double rotation) {
    double sinValue = sin(rotation);
    double cosValue = cos(rotation);

    double newXPos = point->xPos * cosValue - point->yPos * sinValue;
    double newYPos = point->yPos * cosValue + point->xPos * sinValue;

    point2d_t newPoint = { newXPos, newYPos };

    return newPoint;
}

point2d_t translate(const point2d_t* point, const point2d_t* center) {
    double newXPos = point->xPos + center->xPos;
    double newYPos = point->yPos + center->yPos;

    point2d_t newPoint = { newXPos, newYPos };

    return newPoint;
}

point2d_t rotate_and_translate(const point2d_t* point, const point2d_t* center, const double rotation) {
    point2d_t rotationResult = rotate(point, rotation);
    point2d_t rotationAndTranslationResult = translate(&rotationResult, center);

    return rotationAndTranslationResult;
}

double distance(const point2d_t* pos1, const point2d_t* pos2) {
    double xDiff = pos1->xPos - pos2->xPos;
    double yDiff = pos1->yPos - pos2->yPos;

    return sqrt(xDiff * xDiff + yDiff * yDiff);
}

line_t between(const point2d_t* first, const point2d_t* second) {
    double slope = (second->yPos - first->yPos) / (second->xPos - first->xPos);
    double yIntercept = first->yPos - slope * first->xPos;

    line_t result = { slope, yIntercept };

    return result;
}

point2d_t intersection(const line_t* line1, const line_t* line2) {
    double xPos = (line1->yIntercept - line2->yIntercept) / (line1->slope - line2->slope);
    double yPos = line1->slope * xPos + line1->yIntercept;

    if (isinf(xPos) || isinf(yPos)) {
        xPos = NAN;
        yPos = NAN;
    }

    point2d_t result = { xPos, yPos };

    return result;
}
