//
//  main.cpp
//  VGE
//
//  Created by Marcel Kiss on 13/02/2019.
//  Copyright © 2019 Marcel Kiss. All rights reserved.
//

#include "SDLWindow.hpp"
#include "GLDraw.hpp"

#include "Geometry.h"
#include "Handler.hpp"
#include "EarClippingTriangulator.h"
#include "Clip2TriTriangulator.hpp"
#include "Poly2TriTrinagulator.hpp"
#include "Stopwatch.hpp"

using namespace std;
using namespace glm;

#include <poly2tri.h>
#include <clipper.hpp>
#include <thread>

#define RAND_NORM   (rand() % 1000 / 500.0 - 1)
#define RAND_FACTOR (rand() % 1000 / 1000.0)

class Window: public SDLWindow {
    Handler _handler;
    vector<VertexArray> _polygons;
    vector<VertexArray> _holes;
    vector<IndexArray> _indices;
    vector<VertexArray> _triangles;
    bool _editingHole = false;
    
    Stopwatch _sw;
    
    vector<Triangulator *> _triangulators = {
        new EarClippingTriangulator(),
        new Poly2TriTrinagulator(),
        new Clip2TriTriangulator(),
    };
    vector<vec3> _colors = {
        vec3(0.1, 0.05, 0.15),
        vec3(0.15, 0.05, 0.0),
        vec3(0.10, 0.15, 0.05),
    };
    int _triangulatorCurrentIndex = 0;
    
    vec2 _offset = vec2(0, 0);
    
    Triangulator& _triangulator() { return *_triangulators[_triangulatorCurrentIndex]; }
    
    int _genCount = 25;
    std::thread _computeThread;
    bool _cancelThread = false;
    bool _computing = false;
    
    int _stepPolygonIndex = -1;
    
public:
    /* Create window with width and height, using high DPI flag and
       OpenGL 4.1 maximum supported by macOS 10.14.3 (Mojave)  */
    Window(int width, int height): SDLWindow(width, height, true, 410u) {
        
    }
    
    void finishPolyline() {
        _handler.finishPolyline();
        if (!_handler.polyline().empty()) {
            VertexArray v = VertexArray(_handler.polyline().begin(), _handler.polyline().end());
            if (_editingHole) _holes.push_back(v);
            else _polygons.push_back(v);
            _indices.resize(_polygons.size());
            _triangles.resize(_polygons.size());
        }
        _handler.clear();
        _handler.start();
    }
    
    void begin(int i) {
        _triangulator().reset();
        _triangulator().init(&_polygons[i], &_indices[i], &_triangles[i]);
        _triangulator().begin();
    }
    
    void triangulate() {
        _indices.clear();
        _triangles.clear();
        _indices.resize(_polygons.size());
        _triangles.resize(_polygons.size());
        for (int i = 0; i < _polygons.size(); i++) {
            _triangulator().reset();
            _triangulator().init(&_polygons[i], &_indices[i], &_triangles[i]);
            _triangulator().begin();
            for (auto &hole : _holes)
                _triangulator().addHole(&hole);
            _triangulator().triangulate();
        }
    }
    
    void clear() {
        _handler.clear();
        _polygons.clear();
        _holes.clear();
        _indices.clear();
        _triangles.clear();
        _stepPolygonIndex = -1;
    }
    
    vec2 projectPoint(Sint32 x, Sint32 y) {
        return vec2(x / (float)width() * 2.0f - 1.0f, (height() - y) / (float)height() * 2.0f - 1.0f) + _offset;
    }
    
    void mouseDown(Uint8 button, Uint8 state, Uint8 clicks, Sint32 x, Sint32 y) {
        _editingHole = (button == SDL_BUTTON_RIGHT);
        vec2 p = projectPoint(x, y);
        if (clicks > 1) finishPolyline();
        else _handler.addPoint(p);
    }
    
    void mouseMotion(Uint32 state, Sint32 x, Sint32 y, Sint32 xrel, Sint32 yrel) {
        _handler.updateGhostPoint(projectPoint(x, y));
    }

#define GEN_STEP    25
    void keyDown(Sint32 key) {
        switch (key) {
            case SDLK_ESCAPE: {
                finishPolyline();
            } break;
            case 'r': {
                clear();
            } break;
			case ' ': {
                _sw.begin("tri");
                triangulate();
                _sw.end("tri");
                printf("Triangulation time: %g ms\n", _sw.time_in_ms("tri"));
			} break;
			case '1': {
                _triangulatorCurrentIndex = 0; cout << "Triangulator: " << "EarClipping" << endl;
			} break;
            case '2': {
                _triangulatorCurrentIndex = 1; cout << "Triangulator: " << "Poly2Tri" << endl;
            } break;
			case '3': {
                _triangulatorCurrentIndex = 2; cout << "Triangulator: " << "Clip2Tri" << endl;
			} break;
            case 'm': {
                Clip2TriTriangulator *c = dynamic_cast<Clip2TriTriangulator *>(&_triangulator());
                if (c) { c->evenOdd(!c->evenOdd()); cout << "Event Odd: " << (c->evenOdd() ? "Enabled" : "Disabled") << endl; }
            } break;
            case ',': {
                EarClippingTriangulator *e = dynamic_cast<EarClippingTriangulator *>(&_triangulator());
                if (e && !_polygons.empty()) {
                    if (_stepPolygonIndex == -1 || e->finished()) {
                        _stepPolygonIndex++;
                        if (_stepPolygonIndex < _polygons.size())
                            begin(_stepPolygonIndex);
                    }
                    e->Step(); cout << "Step" << endl;
                }
            } break;
            case '.': {
                EarClippingTriangulator *e = dynamic_cast<EarClippingTriangulator *>(&_triangulator());
                if (e && !_polygons.empty()) {
                    if (_stepPolygonIndex == -1 || e->finished()) {
                        _stepPolygonIndex++;
                        if (_stepPolygonIndex < _polygons.size())
                            begin(_stepPolygonIndex);
                    }
                    e->NextTriangle(); cout << "Next Triangle" << endl;
                }
            } break;
            case 'g': {
                generateRandomShape(_genCount);
            } break;
            case 'b': {
                _genCount -= std::max(GEN_STEP, 0); cout << "Gen Vertex Count: " << _genCount << endl;
            } break;
            case 'n': {
                _genCount += std::max(GEN_STEP, 0); cout << "Gen Vertex Count: " << _genCount << endl;
            } break;
            case 'x': {
                if (_computing) {
                    _cancelThread = true;
                    if (_computeThread.joinable())
                        _computeThread.join();
                    _computing = false;
                } else {
                    _cancelThread = false;
                    _computeThread = std::thread([this](){
                        int i = _genCount;
                        while (!_cancelThread) {
                            clear();
                            generateRandomShape(i);
                            _sw.begin("tri");
                            triangulate();
                            _sw.end("tri");
                            printf("%i, %g ms\n", i, _sw.time_in_ms("tri"));
                            i += GEN_STEP;
                        }
                    });
                    _computing = true;
                }
            } break;
            default: break;
        }
    }
    
    void init() {
        /* Init GLEW library for Windows to load OpenGL 3.X functions */
#ifdef _WIN32
        //Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK) GLEWFatal("Error initializing GLEW!", glewError);
#endif
        _handler.start();
    }
    
    void generateRandomShape(int max) {
        _editingHole = false;
        milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        srand(ms.count());
        for (int i = 0; i < max; i++) {
            float a = (i / (float)max) * 3.1417 * 2;
            _handler.addPoint(_offset + vec2(cos(a), sin(a)) * (0.1f + (float)RAND_FACTOR * 0.8f));
        }
        finishPolyline();
    }
    
    void draw() {
        glViewport(0, 0, fwidth(), fheight());
        const vec3 bgColor = _colors[_triangulatorCurrentIndex];
        glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        
        glPointSize(15);
        glLineWidth(1.0f);
        
        mat4 projection = glm::ortho(-1, 1, -1, 1);
        mat4 view = glm::translate(mat4(1.0f), -vec3(_offset, 0));
        mat4 mvp = projection * view;
        glDrawLoadTransform(&(mvp[0][0]));
        float speed = 0.03f;
        if (key(SDLK_w)) _offset += vec2( 0.0f, speed);
        if (key(SDLK_s)) _offset += vec2( 0.0f,-speed);
        if (key(SDLK_a)) _offset += vec2(-speed, 0.0f);
        if (key(SDLK_d)) _offset += vec2( speed, 0.0f);
        
        Triangulator &t = _triangulator();
        
        // Draw triangles one by one
        for (int i = 0; i < _polygons.size(); i++) {
            const VertexArray polyline = _polygons[i];
            const IndexArray indices = _indices[i];
            const VertexArray triangles = _triangles[i];
            glDrawSetMode(GLDrawMode_Color);
            
            glDrawSetColor(0.25, 0.75, 0.25, 1);
            glDraw(GL_LINE_LOOP, polyline);
            
            // Draw polygons
            const VertexArray &vertices = (t.usingElements() ? polyline : triangles);
            if (vertices.size() >= 3) {
                srand(0);
                const size_t triangleCount = (t.usingElements() ? indices.size() / 3 : triangles.size() / 3);
                for (size_t triangleIndex = 0; triangleIndex < triangleCount; triangleIndex++) {
                    VertexArray triangle;
                    for (int i = 0; i < 3; i++) {
                        if (t.usingElements()) {
                            int index = indices[triangleIndex * 3 + i];
                            if (index < vertices.size())
                                triangle.push_back(vertices[index]);
                        } else {
                            triangle.push_back(triangles[triangleIndex * 3 + i]);
                        }
                        
                        if (triangle.size() >= 3) {
                            // Draw triangle
                            const float range = 0.5;
                            const float offset = 0.4;
                            glDrawSetColor(offset + RAND_FACTOR * range, offset + RAND_FACTOR * range, offset + RAND_FACTOR * range, 1);
                            glDraw(GL_TRIANGLES, triangle);
                            
                            glDrawSetColor(1, 1, 1, 1);
                            glDrawLine(triangle[0], triangle[1]);
                            glDrawLine(triangle[1], triangle[2]);
                            glDrawLine(triangle[2], triangle[0]);
                        }
                    }
                }
            }
        }
        
        for (int i = 0; i < _holes.size(); i++) {
            glDrawSetColor(0.25, 0.25, 0.75, 1);
            glDraw(GL_LINE_LOOP, _holes[i]);
        }
        
        if (!_computing) {
            const VertexArray h = _handler.polyline();
            if (_handler.editing()) {
                glDrawSetColor(1, 0.5, 0.5, 1);
                glDraw(GL_LINE_STRIP, h);
                glDrawSetColor(1, 0, 0, 1);
                glDraw(GL_POINTS, h);
            }
        }
        
        EarClippingTriangulator *e = dynamic_cast<EarClippingTriangulator *>(&_triangulator());
        if (e && _stepPolygonIndex >= 0 && _stepPolygonIndex < _polygons.size()) {
            uint a, n, p;
            e->GetActualIndices(a, n, p);
            const vector<vec2> &polyline = _polygons[_stepPolygonIndex];
            const vector<vec2> v = { polyline[a], polyline[n], polyline[p] };
            glDrawSetColor(0, 1, 0, 1);
            glPointSize(20);
            glDraw(GL_POINTS, v);
        }
        
        swap();
    }
    
    
};

/* Error solution for Visual Studio 2017:
   unresolved external symbol main referenced in function "int __cdecl __scrt_common_main_seh(void)"
   Porbably caused by SDL2:
       #define main    SDL_main
 */
#undef main

int main(int argc, const char * argv[]) {
    Window window(500, 500);
    /* Limit frame rate to 60 per seconds to prevent application
       redrawing void function in background (macOS optimization) */
    window.setFrameRateCap(60);
    /* Show window and starts handle SDL events in loop. */
    window.mainLoop();
    return EXIT_SUCCESS;
}
