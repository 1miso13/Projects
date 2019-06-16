//
//  Handler.hpp
//  VGE
//
//  Created by Marcel Kiss on 30/03/2019.
//

#ifndef Handler_hpp
#define Handler_hpp

#include "GLDraw.hpp"
#include "Geometry.h"

using namespace std;
using namespace glm;

class Handler {
    bool _editingPolyline = false;
    VertexArray _polyline;
public:
    bool editing() const {
        return _editingPolyline;
    }
    
    void addPoint(vec2 p) {
        if (_editingPolyline) {
            if (_polyline.empty()) {
                _polyline.push_back(p);
            } else {
                _polyline.back() = p;
            }
            _polyline.push_back(p);
        }
    }
    
    void updateGhostPoint(vec2 p) {
        if (_editingPolyline) {
            if (_polyline.empty()) {
                _polyline.push_back(p);
            } else {
                _polyline.back() = p;
            }
        }
    }
    
    void finishPolyline() {
        if (_editingPolyline) {
            _editingPolyline = false;
            if (!_polyline.empty()) {
                if (_polyline.size() > 4) {
                    _polyline.pop_back();
                } else {
                    _polyline.clear();
                }
            }
        }
    }
    
    void finishPolyline(vec2 p) {
        if (_editingPolyline) {
            _editingPolyline = false;
            if (_polyline.size() >= 3) {
                _polyline.back() = p;
            } else {
                _polyline.clear();
            }
        }
    }
    
    void clear() {
        _polyline.clear();
    }
    
    void start() {
        _editingPolyline = true;
    }
    
    const vector<vec2>& polyline() const {
        return _polyline;
    }
};

#endif /* Handler_hpp */
