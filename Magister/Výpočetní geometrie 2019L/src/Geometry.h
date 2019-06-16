//
//  Geometry.h
//  VGE
//
//  Created by Marcel Kiss on 25/02/2019.
//

#ifndef Geometry_h
#define Geometry_h

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

typedef vector<vec2> VertexArray; // Polyline
typedef vector<unsigned int> IndexArray; // 3 per trienagle
typedef vector<vec3> ColorArray; // 1 color per triangle

inline float cross(vec2 const& a, vec2 const& b) {
    return a.x * b.y - b.x * a.y;
}

inline float cross(vec2 const& a, vec2 const& b, vec2 const& c) {
    return cross(b - a, c - a);
}

/** Use of machine epsilon to compare floating-point values for equality
 Source: http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon */
template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almost_equal(T x, T y, int ulp=2) {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) <= std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
    // unless the result is subnormal
    || std::abs(x-y) < std::numeric_limits<T>::min();
}

#endif /* Geometry_h */
