/***************************************************************************
 * Quaternion class written by BlackAxe / Kolor aka Laurent Schmalen in 1997
 * this class is freeware. you are fully allowed to use this class in non-
 * commercial products. Use in commercial environment is strictly prohibited
 * if you use this, i'll be happy if you greet/credit me and my group
 *
 * Official Version 0.9
 *
 * Laurent aka Blackaxe/KoLoR
 */


#ifndef BA_QUATERNION_H__
#define BA_QUATERNION_H__

#include <iostream>

#ifndef PI
#define PI 3.14159264
#endif

class BAQuat
{
  private:
    float W, X, Y, Z;      // components of a quaternion

  public:
    // default constructor
    BAQuat();
    BAQuat(const float, const float = 0.0, const float = 0.0, const float = 0.0);
  
    // print a quaternion using streams
    void PrintOn(std::ostream& = std::cout) const;

    // quaternion multiplication
    friend BAQuat operator * (const BAQuat&, const BAQuat&);
    const BAQuat& operator *= (const BAQuat&);

    // konjugiertes Quaternion
    const BAQuat& operator ~ ();

    // this function inverts the quaternion
    const BAQuat& operator - ();

    // this normalizes a quaternion 
    const BAQuat& Normalize();
    const BAQuat& FromAxis(const float, float, float, float);
    void ToMatrix(float matrix[3][3]) const;

    void Slerp(const BAQuat&, const BAQuat&, const float);

    // some additional Quaternion functions
    // getting the exponent of a quaternion
    const BAQuat& exp();
    const BAQuat& log();
};

inline std::ostream& operator << (std::ostream &strm, const BAQuat &q)
{
  q.PrintOn(strm);
  return strm;
}

#endif
