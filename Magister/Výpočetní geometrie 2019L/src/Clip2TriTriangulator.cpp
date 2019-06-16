//
//  Clip2TriTriangulator.cpp
//  VGE
//
//  Created by Marcel Kiss on 28/04/2019.
//

#include "Clip2TriTriangulator.hpp"

bool Clip2TriTriangulator::init(const VertexArray *polyline, IndexArray *indices, VertexArray *output) {
    if (!polyline || !output)
        return false;
    _polyline = polyline;
    _output = output;
    return true;
}

bool Clip2TriTriangulator::usingElements() const {
    return false;
}

void Clip2TriTriangulator::addHole(const VertexArray *hole) {
    _holes.push_back(hole);
}

void Clip2TriTriangulator::reset() {
    _holes.clear();
}

/** Shrink hole polyline by 1 to center to prevent duplicate points */
void HoleEdgeShrink(ClipperLib::Path& path) {
    unsigned prev = (int)(path.size() - 1);
    for (unsigned i = 0; i < path.size(); i++) {
        path[i].X - path[prev].X > 0 ? path[i].X-- : path[i].X++;
        path[i].Y - path[prev].Y > 0 ? path[i].Y-- : path[i].Y++;
        prev = i;
    }
}

void Clip2TriTriangulator::_triangulateNode(ClipperLib::PolyNode *node) {
    while (node) {
        if (!node->IsHole()) {
            auto polyline = _clipperPathToPoly2TriArray(node->Contour);
            _removeDuplicates(polyline);
            
            // Create triangulation context
            p2t::CDT cdt = p2t::CDT(polyline);
            
            vector<vector<p2t::Point *>> holeGarbageCollector;
            
            for (auto &child : node->Childs) {
                _triangulateNode(child);
                if (child->IsHole()) {
                    ClipperLib::Path holePath = child->Contour;
                    HoleEdgeShrink(holePath);
                    auto hole = _clipperPathToPoly2TriArray(holePath);
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
        node = node->GetNext();
    }
}

void Clip2TriTriangulator::triangulate() {
    
    if (!_polyline) return;
    
    ClipperLib::Path path;
    path.resize(_polyline->size());
    for (int i = 0; i < _polyline->size(); i++)
        _vec2ToClipper(_polyline->at(i), path[i]);
    
    ClipperLib::Paths holes;
    for (const VertexArray *holePolyline : _holes) {
        if (holePolyline) {
            ClipperLib::Path holePath;
            holePath.resize(holePolyline->size());
            for (int i = 0; i < holePolyline->size(); i++)
                _vec2ToClipper(holePolyline->at(i), holePath[i]);
            holes.push_back(holePath);
        }
    }
    
    ClipperLib::PolyTree solution;
    ClipperLib::Clipper c;
    c.StrictlySimple(true);
    c.AddPath(path, ClipperLib::ptSubject, true);
    c.AddPaths(holes, ClipperLib::ptClip, true);
    c.Execute(ClipperLib::ctDifference, solution, _evenOdd ? ClipperLib::pftEvenOdd : ClipperLib::pftNonZero, ClipperLib::pftNonZero);
    
    _output->clear();
    ClipperLib::PolyNode *node = solution.GetFirst();
    _triangulateNode(node);
}

bool Clip2TriTriangulator::evenOdd() const {
    return _evenOdd;
}

void Clip2TriTriangulator::evenOdd(bool evenOdd) {
    _evenOdd = evenOdd;
}

void Clip2TriTriangulator::_vec2ToClipper(const glm::vec2 &v, ClipperLib::IntPoint &p) {
    p = ClipperLib::IntPoint(v.x * _precisionMultiplier, v.y * _precisionMultiplier);
}

p2t::Point* Clip2TriTriangulator::_clipperToPoly2Tri(ClipperLib::IntPoint &p) {
    return new p2t::Point(p.X / _precisionMultiplier, p.Y / _precisionMultiplier);
}

std::vector<p2t::Point *> Clip2TriTriangulator::_clipperPathToPoly2TriArray(ClipperLib::Path &path) {
    std::vector<p2t::Point *> points;
    points.resize(path.size());
    for (int i = 0; i < path.size(); i++)
        points[i] = _clipperToPoly2Tri(path[i]);
    return points;
}

std::vector<p2t::Point *> Clip2TriTriangulator::_polylineToPoly2TriArray(const VertexArray *polyline) {
    std::vector<p2t::Point *> points;
    points.resize(polyline->size());
    for (int i = 0; i < polyline->size(); i++) {
        const vec2 p = polyline->at(i);
        points[i] = new p2t::Point(p.x, p.y);
    }
    return points;
}

void Clip2TriTriangulator::_removeDuplicates(vector<p2t::Point *> &v) {
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

