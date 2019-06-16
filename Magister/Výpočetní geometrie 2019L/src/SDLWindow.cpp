//
//  SDLWindow.cpp
//
//  Created by Marcel Kiss on 07/01/2019.
//  Copyright © 2019 Marcel Kiss. All rights reserved.
//

#ifdef _WIN32
#include <Windows.h>
#include <GL/glew.h>
#endif
#include "SDLWindow.hpp"
#include <thread>

#define TIME_IN_NANO()  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch())
#define NANO_TO_SEC(n)  ((n) / 1000000000.0)
#define SEC_TO_NANO(n)  ((n) * 1000000000.0)

SDLWindow::SDLWindow(int width, int height, bool highdpi, int glversion): _frameTimeCap(0) {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) SDLFatal("SDL could not initialize!");
    
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glversion / 100)) SDLFatal("Unable to set OpenGL major version");
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, (glversion % 100) / 10)) SDLFatal("Unable to set OpenGL minor version");
    if (glversion > 320) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    }
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    /* Some graphic cards does not support 32bit depth, usualy 24bit is enough. */
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    //Create window
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | (highdpi ? SDL_WINDOW_ALLOW_HIGHDPI : 0);
    _window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    
    if (!_window) SDLFatal("Window could not be created!");
    
    //Create context
    _context = SDL_GL_CreateContext(_window);
    if (!_context) SDLFatal("OpenGL context could not be created!");
    
//    int i = 0;
//    SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &i); cout << "Red: " << i << endl;
//    SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &i); cout << "Green: " << i << endl;
//    SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &i); cout << "Blue: " << i << endl;
//    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &i); cout << "Depth: " << i << endl;
//    SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &i); cout << "Stencil: " << i << endl;
    
#ifdef _WIN32
    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) GLEWFatal("Error initializing GLEW!", glewError);
#endif
    
    //Use Vsync
//    if (SDL_GL_SetSwapInterval(1) < 0) SDLFatal("Warning: Unable to set VSync!");
    
    SDL_GetWindowSize(_window, &_width, &_height);
    SDL_GL_GetDrawableSize(_window, &_fwidth, &_fheight);    
}

void SDLWindow::mainLoop() {
    init(); if (initFunc) initFunc(*this);
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_KEYDOWN: {
                    const Sint32 key = e.key.keysym.sym;
                    _keys[key] = true;
                    keyDown(key); if (keyDownFunc) keyDownFunc(*this, key);
                } break;
                case SDL_KEYUP: {
                    const Sint32 key = e.key.keysym.sym;
                    _keys[key] = false;
                    keyUp(key); if (keyUpFunc) keyUpFunc(*this, key);
                } break;
                case SDL_WINDOWEVENT: {
                    switch (e.window.event) {
                        case SDL_WINDOWEVENT_SHOWN: shown(); if (shownFunc) shownFunc(*this); break;
                        case SDL_WINDOWEVENT_HIDDEN: hidden(); if (hiddenFunc) hiddenFunc(*this); break;
                        case SDL_WINDOWEVENT_EXPOSED: exposed(); if (exposedFunc) exposedFunc(*this); break;
                        case SDL_WINDOWEVENT_MOVED: {
                            const Sint32 x = e.window.data1, y = e.window.data2;
                            moved(x, y); if (movedFunc) movedFunc(*this, x, y);
                        } break;
                        case SDL_WINDOWEVENT_RESIZED: {
                            SDL_GL_GetDrawableSize(_window, &_fwidth, &_fheight);
                            _width = e.window.data1; _height = e.window.data2;
                            resized(_width, _height); if (resizedFunc) resizedFunc(*this, _width, _height);
                        } break;
                        case SDL_WINDOWEVENT_SIZE_CHANGED: {
                            const Sint32 w = e.window.data1, h = e.window.data2;
                            sizeChanged(w, h); if (sizeChangedFunc) sizeChangedFunc(*this, w, h);
                        } break;
                        case SDL_WINDOWEVENT_MINIMIZED: minimized(); if (minimizedFunc) minimizedFunc(*this); break;
                        case SDL_WINDOWEVENT_MAXIMIZED: maximized(); if (maximizedFunc) maximizedFunc(*this); break;
                        case SDL_WINDOWEVENT_RESTORED: restored(); if (restoredFunc) restoredFunc(*this); break;
                        case SDL_WINDOWEVENT_ENTER: enter(); if (enterFunc) enterFunc(*this); break;
                        case SDL_WINDOWEVENT_LEAVE: leave(); if (leaveFunc) leaveFunc(*this); break;
                        case SDL_WINDOWEVENT_FOCUS_GAINED: focusGained(); if (focusGainedFunc) focusGainedFunc(*this); break;
                        case SDL_WINDOWEVENT_FOCUS_LOST: focusLost(); if (focusLostFunc) focusLostFunc(*this); break;
                        case SDL_WINDOWEVENT_CLOSE: close(); if (closeFunc) closeFunc(*this); break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
                        case SDL_WINDOWEVENT_TAKE_FOCUS: takeFocus(); if (takeFocusFunc) takeFocusFunc(*this); break;
                        case SDL_WINDOWEVENT_HIT_TEST: hitTest(); if (hitTestFunc) hitTestFunc(*this); break;
#endif
                        default: SDL_Log("Window %d got unknown event %d", e.window.windowID, e.window.event); break;
                    }
                } break;
                    
                case SDL_MOUSEMOTION: {
                    mouseMotion(e.motion.state, e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
                    if (mouseMotionFunc) mouseMotionFunc(*this, e.motion.state, e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
                } break;
                case SDL_MOUSEBUTTONDOWN: {
                    mouseDown(e.button.button, e.button.state, e.button.clicks, e.button.x, e.button.y);
                    if (mouseDownFunc) mouseDownFunc(*this, e.button.button, e.button.state, e.button.clicks, e.button.x, e.button.y);
                } break;
                case SDL_MOUSEBUTTONUP: {
                    mouseUp(e.button.button, e.button.state, e.button.clicks, e.button.x, e.button.y);
                    if (mouseUpFunc) mouseUpFunc(*this, e.button.button, e.button.state, e.button.clicks, e.button.x, e.button.y);
                } break;
                case SDL_MOUSEWHEEL: {
                    mouseWheel(e.wheel.x, e.wheel.y);
                    if (mouseWheelFunc) mouseWheelFunc(*this, e.wheel.x, e.wheel.y);
                } break;
                case SDL_QUIT: quit = true; break;
                
                /* Does not work on PC, probably for mobile OS
                case SDL_APP_TERMINATING: appTerminating(); break;
                case SDL_APP_LOWMEMORY: appLowMemory(); break;
                case SDL_APP_WILLENTERBACKGROUND: appWillEnterBackground(); break;
                case SDL_APP_DIDENTERBACKGROUND: appDidEnterBackground(); break;
                case SDL_APP_WILLENTERFOREGROUND: appWillEnterBackground(); break;
                case SDL_APP_DIDENTERFOREGROUND: appDidEnterForeground(); break;
                 */
            }
        }
        const std::chrono::nanoseconds begin = TIME_IN_NANO();
        draw(); if (drawFunc) drawFunc(*this);
        _frameTime = (TIME_IN_NANO() - begin);
        const std::chrono::nanoseconds overlimit = _frameTimeCap - _frameTime;
        if (overlimit > std::chrono::nanoseconds::zero()) std::this_thread::sleep_for(overlimit);
        _frameRate = 1000000000.0 / (TIME_IN_NANO() - begin).count();
    }
    destroy(); if (destroyFunc) destroyFunc(*this);
}

double SDLWindow::frameRateCap() const {
    return 1.0 / (NANO_TO_SEC(_frameTimeCap.count()));
}

void SDLWindow::setFrameRateCap(double frameRateCap) {
    _frameTimeCap = std::chrono::nanoseconds(frameRateCap > 0 ? (long long)(SEC_TO_NANO(1.0 / frameRateCap)) : 0);
}

double SDLWindow::frameRate() const {
    return _frameRate;
}

double SDLWindow::frameTime() const {
    return NANO_TO_SEC(_frameTime.count());
}

int SDLWindow::width() const {
    return _width;
}

int SDLWindow::height() const {
    return _height;
}

int SDLWindow::fwidth() const {
    return _fwidth;
}

int SDLWindow::fheight() const {
    return _fheight;
}

void SDLWindow::swap() const {
    SDL_GL_SwapWindow(_window);
}

bool SDLWindow::key(SDL_Keycode keycode) const {
    return _keys.count(keycode) ? _keys.at(keycode) : false;
}

void SDLWindow::init() {}
void SDLWindow::destroy() {}
void SDLWindow::draw() {}
void SDLWindow::shown() {}
void SDLWindow::hidden() {}
void SDLWindow::exposed() {}
void SDLWindow::moved(Sint32 x, Sint32 y) {}
void SDLWindow::resized(Sint32 width, Sint32 height) {}
void SDLWindow::sizeChanged(Sint32 width, Sint32 height) {}
void SDLWindow::minimized() {}
void SDLWindow::maximized() {}
void SDLWindow::restored() {}
void SDLWindow::enter() {}
void SDLWindow::leave() {}
void SDLWindow::focusGained() {}
void SDLWindow::focusLost() {}
void SDLWindow::close() {}
void SDLWindow::takeFocus() {}
void SDLWindow::hitTest() {}
void SDLWindow::keyDown(Sint32 key) {}
void SDLWindow::keyUp(Sint32 key) {}
/* Does not work on PC, probably for mobile OS
void SDLWindow::appTerminating() {}
void SDLWindow::appLowMemory() {}
void SDLWindow::appWillEnterBackground() {}
void SDLWindow::appDidEnterBackground() {}
void SDLWindow::appWillEnterForeground() {}
void SDLWindow::appDidEnterForeground() {}
 */
void SDLWindow::mouseMotion(Uint32 state, Sint32 x, Sint32 y, Sint32 xrel, Sint32 yrel) {}
void SDLWindow::mouseDown(Uint8 button, Uint8 state, Uint8 clicks, Sint32 x, Sint32 y) {}
void SDLWindow::mouseUp(Uint8 button, Uint8 state, Uint8 clicks, Sint32 x, Sint32 y) {}
void SDLWindow::mouseWheel(Sint32 x, Sint32 y) {}

SDL_Window* SDLWindow::operator()() const {
    return _window;
}
