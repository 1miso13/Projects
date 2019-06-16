//
//  Poly2TriTrinagulator.hpp
//  VGE
//
//  Created by Marcel Kiss on 29/04/2019.
//

#ifndef Poly2TriTrinagulator_hpp
#define Poly2TriTrinagulator_hpp

#include "Triangulator.h"
#include <poly2tri.h>
#include <set>

class Poly2TriTrinagulator: public Triangulator {
public:
    bool usingElements() const;
    bool init(const VertexArray *vertices, IndexArray *indices, VertexArray *output);
    void addHole(const VertexArray *hole);
    void triangulate();
    void reset();
    
private:
    const VertexArray *_vertices = NULL;
    VertexArray *_output = NULL;
    vector<const VertexArray *> _holes;
    
    std::vector<p2t::Point *> _polylineToPoly2TriArray(const VertexArray *polyline);
    void _removeDuplicates(vector<p2t::Point *> &v);
};

#endif /* Poly2TriTrinagulator_hpp */
