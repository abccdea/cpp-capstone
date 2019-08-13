/* File:            CVector.h
 * Author:          Vish Potnis
 * Description:     - Vector object
 *                  - Initialize as either polar or x/y
 *                  - Get x/y projects and add 2 vectors
 */

#include <cmath>
#include <iostream>
#include "constants.h"

enum class VectorType
{
    XY,
    POLAR
};

class CVector
{
    public:
        CVector();
        CVector(double mag, double angle, VectorType type);

        void calculateProjection();     // calculate x and y projection
        void calculatePolar();          // calculate polar form of vector

        CVector operator+(const CVector &vec);                                      // add 2 vectors and calculate polar form
        friend std::ostream& operator<<(std::ostream &out, const CVector &vec);

        // getters
        double getXProjection() const;
        double getYProjection() const;
        double getMag() const;
        double getAngle() const;

    private:
        double _mag;
        double _angle;
        double _x;
        double _y;
};

