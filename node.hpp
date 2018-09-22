#include <vector>
#include "const.hpp"
#include <ncurses.h>
#include <math.h>

#ifndef __NODE_H__
#define __NODE_H__

enum NODE_TYPE{
    START_NODE,
    STOP_NODE,
    TRANS_NODE
};

enum NODE_STATE{
    GOT_INPUT,
    FIRED, //ready to give output but not receive input
    RECOVERY //can recieve input but cant generatae output
};

typedef struct{
    double value;
    int decay;
} Link;

class Node{
private:
    double threshold;
	double output;
	double lastOutput;
    int isStop;
    int isStart;
    double score;
public:
    Link *links[3][3];
    int *stream;
    NODE_TYPE node_type;
    int x, y;
	NODE_STATE node_state;
    Node();
    Node(NODE_TYPE node_type, int _x, int _y);
    void evaluate(Node *stopNode);
    void transfer(Node *nodeMap[MAP_WIDTH][MAP_HEIGHT]);
    void connect(Node* nodeMap[MAP_WIDTH][MAP_HEIGHT], std::vector<Link*> link_ptrs);
    void connect(Node* nodeMap[MAP_WIDTH][MAP_HEIGHT], std::vector<Link*> link_ptrs, int s_x, int s_y, Link *link);
	double getOutput();
    double getColor();
    void setOutput(double o);
    void setThreshold(double t);
    void setStream(int *stream);
};

#endif
