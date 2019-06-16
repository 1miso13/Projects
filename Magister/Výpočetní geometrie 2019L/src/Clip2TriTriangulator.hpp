//
//  Clip2TriTriangulator.hpp
//  VGE
//
//  Created by Marcel Kiss on 28/04/2019.
//

#ifndef Clip2TriTriangulator_hpp
#define Clip2TriTriangulator_hpp

#include "Triangulator.h"

#include <poly2tri.h>
#include <clipper.hpp>

class Clip2TriTriangulator: public Triangulator {
    
public:
    bool usingElements() const;
    bool init(const VertexArray *vertices, IndexArray *indices, VertexArray *output);
    void addHole(const VertexArray *hole);
    void triangulate();
    void reset();
    
    bool evenOdd() const;
    void evenOdd(bool evenOdd);
    
private:
    const VertexArray *_polyline = NULL;
    VertexArray *_output = NULL;
    vector<const VertexArray *> _holes;
    
    bool _evenOdd = true;
    double _precisionMultiplier = 10000000;
    
    void _vec2ToClipper(const glm::vec2 &v, ClipperLib::IntPoint &p);
    p2t::Point* _clipperToPoly2Tri(ClipperLib::IntPoint &p);
    std::vector<p2t::Point *> _clipperPathToPoly2TriArray(ClipperLib::Path &path);
    std::vector<p2t::Point *> _polylineToPoly2TriArray(const VertexArray *polyline);
    void _removeDuplicates(vector<p2t::Point *> &v);
    void _triangulateNode(ClipperLib::PolyNode *node);
};

#endif /* Clip2TriTriangulator_hpp */
