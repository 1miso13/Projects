//
//  Triangulator.h
//  VGE
//
//  Created by Marcel Kiss on 28/04/2019.
//

#ifndef Triangulator_h
#define Triangulator_h

#include "Geometry.h"

class Triangulator {
public:
    /* Return YES if triangulator is using indices to original vertices.
       Otherwise user must send pointer to emtpy vertex array where are
       new vertices stored. */
    virtual bool usingElements() const = 0;
    
    /* Initialize triangulator with input polyline, index array and output vertex array.
       Returns flag if every required array has been given and it was initialized. */
    virtual bool init(const VertexArray *vertices, IndexArray *indices, VertexArray *output) = 0;
    
    virtual void triangulate() = 0;
    
    virtual void reset() {};
    
    virtual void addHole(const VertexArray *hole) {};
    
    virtual void begin() {};

	virtual bool finished() { return true; };
};

#endif /* Triangulator_h */
