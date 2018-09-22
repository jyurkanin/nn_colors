#include "node.hpp"
#include "const.hpp"
#include <X11/Xlib.h>

#ifndef __ENGINE_H__
#define __ENGINE_H__

class Engine{
private:
    
public:
    Display *dpy;
    Window w;
    GC gc;
    Node *nodeMap[MAP_WIDTH][MAP_HEIGHT];
    Node *start_node;
    Node *stop_node;
    int length;
    int count;
    int num_fired;
    void tick(double *average_output);
    void start(int *input, int *output, int _length);
    void redraw(double mean);
    void updateStreams();
    void circular();
};
#endif
