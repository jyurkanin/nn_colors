#include "engine.hpp"
#include <ncurses.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <string>


void Engine::start(int *input, int *output, int _length){
    length = _length;
    double avg;
    count = 0;
    
    dpy = XOpenDisplay(0);
    
    w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, MAP_WIDTH*SCALE, MAP_HEIGHT*SCALE, 0, 0, 0);
    XSelectInput(dpy, w, StructureNotifyMask | ExposureMask);
    XClearWindow(dpy, w);
    XMapWindow(dpy, w);
    gc = XCreateGC(dpy, w, 0, 0);
    
    XEvent e;
    do{
        XNextEvent(dpy, &e);
        printf("type %d\n", e.type);
    } while(e.type != MapNotify);

    for(int i = 0; i < MAP_WIDTH; i++){
        for(int j = 0; j < MAP_HEIGHT; j++){
            nodeMap[i][j] = 0;
        }
    }

    start_node = nodeMap[0][0] = new Node(START_NODE, 0, 0);
//    stop_node = nodeMap[MAP_WIDTH-1][0] = new Node(STOP_NODE, MAP_WIDTH - 1, 0);

    start_node->setStream(input);
    //stop_node->setStream(output);

    for(int i = 0; i < MAP_WIDTH; i++){
        for(int j = 0; j < MAP_HEIGHT; j++){
            if(!nodeMap[i][j])
                nodeMap[i][j] = new Node(TRANS_NODE, i, j);
        }
    }
    
    std::vector<Link*> link_ptrs; //for later deletion
//    start_node->connect(nodeMap, link_ptrs); //recursive boi here. //Let this be a lesson to you kids that recursion is always a stupid fucking idea
    for(int i = 0; i < MAP_WIDTH; i++){
        for(int j = 0; j < MAP_HEIGHT; j++){
            for(int k = 0; k < 3; k++){
                for(int l = 0; l < 3; l++){
                    if(!nodeMap[i][j]->links[k][l] && //no link here boi
                       (i+k-1) >= 0 && (j+l-1) >= 0 && (i+k-1) < MAP_WIDTH && (j+l-1) < MAP_HEIGHT){ //is my boi in bounds
                        nodeMap[i+k-1][j+l-1]->links[2-k][2-l] = nodeMap[i][j]->links[k][l] = new Link(); // i think this is right
                        nodeMap[i][j]->links[k][l]->decay = 0;
                        nodeMap[i][j]->links[k][l]->value = (double) rand() / RAND_MAX;
                    }
                }
            }
        }
    }

    circular();
    
    
    while(count < length){
        tick(&avg);
        redraw(avg);
        XFlush(dpy);
        usleep(100000);
    }

    for(int i = 0; i < MAP_WIDTH; i++){
        for(int j = 0; j < MAP_HEIGHT; j++){
            delete nodeMap[i][j];
        }
    }
    
    for(int i = 0; i < link_ptrs.size(); i++){
        delete link_ptrs[i];
    }
    
    XDestroyWindow(dpy, w);
    XCloseDisplay(dpy);
//    endwin();
}


void Engine::tick(double *average_output){
    if(!num_fired){
        count++;
        updateStreams();   
    }
    
    *average_output = 0;
    num_fired = 0;
    for(int i = 0; i < MAP_WIDTH; i++){ //TODO: activation order might matter. experiment later.
        for(int j = 0; j < MAP_HEIGHT; j++){
            nodeMap[i][j]->transfer(nodeMap);
            if(nodeMap[i][j]->node_state == FIRED){
                *average_output += nodeMap[i][j]->getOutput();
                num_fired++;
            }
            
        }
    }
    
    for(int i = 0; i < MAP_WIDTH; i++){ //TODO: activation order might matter. experiment later.
        for(int j = 0; j < MAP_HEIGHT; j++){
            nodeMap[i][j]->evaluate(stop_node);
        }
    }

     *average_output = *average_output / num_fired;
}

void Engine::updateStreams(){    
    start_node->setOutput((int) *(start_node->stream++));
    start_node->node_state = FIRED;
}

void Engine::redraw(double mean){
    int color = 0;
    for(int i = 0; i < MAP_WIDTH; i++){
        for(int j = 0; j < MAP_HEIGHT; j++){
            color = 256 * nodeMap[i][j]->getColor();
            XSetForeground(dpy, gc, color);
            XFillRectangle(dpy, w, gc, i*SCALE, j*SCALE, SCALE, SCALE);
        }
    }
}

void Engine::circular(){    
    for(int i = MAP_WIDTH/4; i <= 3*MAP_WIDTH/4; i++){       
        for(int k = 0; k < 3; k++){
            for(int l = 0; l < 3; l++){
                nodeMap[i][MAP_WIDTH/4]->links[k][l]->value = 0;
            }
        }
    }
    for(int i = MAP_WIDTH/4; i <= 3*MAP_WIDTH/4; i++){       
        for(int k = 0; k < 3; k++){
            for(int l = 0; l < 3; l++){
                nodeMap[i][3*MAP_WIDTH/4]->links[k][l]->value = 0;
            }
        }
    }
    
    for(int i = MAP_HEIGHT/4; i <= 3*MAP_HEIGHT/4; i++){       
        for(int k = 0; k < 3; k++){
            for(int l = 0; l < 3; l++){
                nodeMap[MAP_HEIGHT/4][i]->links[k][l]->value = 0;
            }
        }
    }
    for(int i = MAP_HEIGHT/4; i <= 3*MAP_HEIGHT/4; i++){       
        for(int k = 0; k < 3; k++){
            for(int l = 0; l < 3; l++){
                nodeMap[3*MAP_HEIGHT/4][i]->links[k][l]->value = 0;
            }
        }
    }

    for(int k = 0; k < 3; k++){
        for(int l = 0; l < 3; l++){
            nodeMap[MAP_WIDTH/4][MAP_HEIGHT/4]->links[k][l]->value = 3;
        }
    }
    for(int k = 0; k < 3; k++){
        for(int l = 0; l < 3; l++){
            nodeMap[3*MAP_WIDTH/4][MAP_HEIGHT/4]->links[k][l]->value = 3;
        }
    }
    for(int k = 0; k < 3; k++){
        for(int l = 0; l < 3; l++){
            nodeMap[MAP_WIDTH/4][3*MAP_HEIGHT/4]->links[k][l]->value = 3;
        }
    }
    for(int k = 0; k < 3; k++){
        for(int l = 0; l < 3; l++){
            nodeMap[3*MAP_WIDTH/4][3*MAP_HEIGHT/4]->links[k][l]->value = 3;
        }
    }
}
