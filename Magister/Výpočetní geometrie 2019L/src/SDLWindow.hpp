//
//  SDLWindow.hpp
//
//  Created by Marcel Kiss on 07/01/2019.
//  Copyright © 2019 Marcel Kiss. All rights reserved.
//

#ifndef SDLWindow_hpp
#define SDLWindow_hpp

#ifdef _WIN32
    #include <SDL.h>
#else
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdocumentation"

    #include <SDL2/SDL.h>

    #pragma clang diagnostic pop
#endif


#include <iostream>
#include <functional>
#include <map>
#include <chrono>

using namespace std;

#define GLEWFatal(m,e)  do { std::cerr << m << " GLEW Error: " << glewGetErrorString(e) << std::endl; exit(EXIT_FAILURE); } while(0)
#define SDLFatal(m)     do { std::cerr << m << " SDL Error: " << SDL_GetError() << std::endl; exit(EXIT_FAILURE); } while(0)

class SDLWindow {
    SDL_Window *_window;
    SDL_GLContext _context;
    std::map<SDL_Keycode, bool> _keys;
    int _width, _height;
    int _fwidth, _fheight;
    
    double _frameRate = 0;
    std::chrono::nanoseconds _frameTime;
    std::chrono::nanoseconds _frameTimeCap;
    
public:
    SDLWindow(int width = 500, int height = 500, bool highdpi = true, int glversion = 410);
    
    void mainLoop();
    
    int width() const;
    int height() const;
    int fwidth() const;
    int fheight() const;
    
    void swap() const;
    
    double frameRateCap() const;
    void setFrameRateCap(double frameRateCap);
    double frameRate() const;
    double frameTime() const;
    
    bool key(SDL_Keycode keycode) const;
    
    virtual void init();
    virtual void destroy();
    virtual void draw();
    virtual void shown();
    virtual void hidden();
    virtual void exposed();
    virtual void moved(Sint32 x, Sint32 y);
    virtual void resized(Sint32 width, Sint32 height);
    virtual void sizeChanged(Sint32 width, Sint32 height);
    virtual void minimized();
    virtual void maximized();
    virtual void restored();
    virtual void enter();
    virtual void leave();
    virtual void focusGained();
    virtual void focusLost();
    virtual void close();
    virtual void takeFocus();
    virtual void hitTest();
    virtual void keyDown(Sint32 key);
    virtual void keyUp(Sint32 key);
    
    virtual void mouseMotion(Uint32 state, Sint32 x, Sint32 y, Sint32 xrel, Sint32 yrel);
    virtual void mouseDown(Uint8 button, Uint8 state, Uint8 clicks, Sint32 x, Sint32 y);
    virtual void mouseUp(Uint8 button, Uint8 state, Uint8 clicks, Sint32 x, Sint32 y);
    virtual void mouseWheel(Sint32 x, Sint32 y);
    
    /* Does not work on PC, probably for mobile OS
    virtual void appTerminating();
    virtual void appLowMemory();
    virtual void appWillEnterBackground();
    virtual void appDidEnterBackground();
    virtual void appWillEnterForeground();
    virtual void appDidEnterForeground();
     */
    
    function<void(SDLWindow &window)> initFunc;
    function<void(SDLWindow &window)> destroyFunc;
    function<void(SDLWindow &window)> drawFunc;
    function<void(SDLWindow &window)> shownFunc;
    function<void(SDLWindow &window)> hiddenFunc;
    function<void(SDLWindow &window)> exposedFunc;
    function<void(SDLWindow &window, Sint32 x, Sint32 y)> movedFunc;
    function<void(SDLWindow &window, Sint32 width, Sint32 height)> resizedFunc;
    function<void(SDLWindow &window, Sint32 width, Sint32 height)> sizeChangedFunc;
    function<void(SDLWindow &window)> minimizedFunc;
    function<void(SDLWindow &window)> maximizedFunc;
    function<void(SDLWindow &window)> restoredFunc;
    function<void(SDLWindow &window)> enterFunc;
    function<void(SDLWindow &window)> leaveFunc;
    function<void(SDLWindow &window)> focusGainedFunc;
    function<void(SDLWindow &window)> focusLostFunc;
    function<void(SDLWindow &window)> closeFunc;
    function<void(SDLWindow &window)> takeFocusFunc;
    function<void(SDLWindow &window)> hitTestFunc;
    function<void(SDLWindow &window, Sint32 key)> keyDownFunc;
    function<void(SDLWindow &window, Sint32 key)> keyUpFunc;
    function<void(SDLWindow &window, Uint32 state, Sint32 x, Sint32 y, Sint32 xrel, Sint32 yrel)> mouseMotionFunc;
    function<void(SDLWindow &window, Uint8 button, Uint8 state, Uint8 clicks, Sint32 x, Sint32 y)> mouseDownFunc;
    function<void(SDLWindow &window, Uint8 button, Uint8 state, Uint8 clicks, Sint32 x, Sint32 y)> mouseUpFunc;
    function<void(SDLWindow &window, Sint32 x, Sint32 y)> mouseWheelFunc;
    
    SDL_Window* operator()() const;
};

#endif /* SDLWindow_hpp */
