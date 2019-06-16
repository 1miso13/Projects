//
//  Poly2TriTrinagulator.cpp
//  VGE
//
//  Created by Marcel Kiss on 29/04/2019.
//

#include "Poly2TriTrinagulator.hpp"


bool Poly2TriTrinagulator::usingElements() const {
    return false;
}

bool Poly2TriTrinagulator::init(const VertexArray *vertices, IndexArray *indices, VertexArray *output) {
    if (!vertices || !output)
        return false;
    _vertices = vertices;
    _output = output;
    return true;
}

void Poly2TriTrinagulator::addHole(const VertexArray *hole) {
    _holes.push_back(hole);
}

void Poly2TriTrinagulator::triangulate() {
    
    if (!_vertices || !_output) return;
    
    std::vector<p2t::Point *> polyline = _polylineToPoly2TriArray(_vertices);
    _removeDuplicates(polyline);
    p2t::CDT cdt = p2t::CDT(polyline);
    
    vector<vector<p2t::Point *>> holeGarbageCollector;
    
    for (const VertexArray *holePolyline : _holes) {
        if (holePolyline) {
            auto hole = _polylineToPoly2TriArray(holePolyline);
            _removeDuplicates(hole);
            cdt.AddHole(hole);
            holeGarbageCollector.push_back(hole);
        }
    }
    
    cdt.Triangulate();
    
    VertexArray &v = *_output;
    const auto triangles = cdt.GetTriangles();
    const size_t beginIndex = v.size();
    v.resize(v.size() + triangles.size() * 3);
    for (int triangleIndex = 0; triangleIndex < triangles.size(); triangleIndex++) {
        p2t::Triangle *t = triangles[triangleIndex];
        v[beginIndex + triangleIndex * 3 + 0] = vec2(t->GetPoint(0)->x, t->GetPoint(0)->y);
        v[beginIndex + triangleIndex * 3 + 1] = vec2(t->GetPoint(1)->x, t->GetPoint(1)->y);
        v[beginIndex + triangleIndex * 3 + 2] = vec2(t->GetPoint(2)->x, t->GetPoint(2)->y);
    }
    
    for (auto &hole : holeGarbageCollector)
        for (auto &p : hole)
            delete p;
    
    for (auto &p : polyline)
        delete p;
}

void Poly2TriTrinagulator::reset() {
    _holes.clear();
}

std::vector<p2t::Point *> Poly2TriTrinagulator::_polylineToPoly2TriArray(const VertexArray *polyline) {
    std::vector<p2t::Point *> points;
    points.resize(polyline->size());
    for (int i = 0; i < polyline->size(); i++) {
        const vec2 p = polyline->at(i);
        points[i] = new p2t::Point(p.x, p.y);
    }
    return points;
}

void Poly2TriTrinagulator::_removeDuplicates(vector<p2t::Point *> &v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v.size(); ) {
            if (i != j) {
                p2t::Point *&v1 = v[i];
                p2t::Point *&v2 = v[j];
                if (almost_equal(v1->x, v2->x) && almost_equal(v1->y, v2->y)) {
                    delete v[j];
                    v.erase(v.begin() + j);
                } else {
                    j++;
                }
            } else {
                j++;
            }
        }
    }
}
