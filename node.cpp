#include "node.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

Node::Node(){
    node_type = TRANS_NODE;
}

Node::Node(NODE_TYPE n_type, int _x, int _y){
    node_type = n_type;
    node_state = RECOVERY;
    x = _x;
    y = _y;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            links[i][j] = 0;
        }
    }
    threshold = (double) rand() / RAND_MAX;
}

void Node::setStream(int *s){
    stream = s;
}
void Node::setOutput(double o){
    output = o;
}
void Node::setThreshold(double t){
    threshold = t;
}
void Node::evaluate(Node *stopNode){ //sets output and state
    switch(node_type){
    case STOP_NODE:
        break; //stop node will work a lot like a trans node in terms of output
    case START_NODE:
        switch(node_state){
        case RECOVERY:
            output = 0;
            //mvaddstr(x, y+20, "recovery\n");
            break;
        case FIRED:
            //mvaddstr(x, y+20, "switching to recovery\n");
            node_state = RECOVERY;
            break;
        }
        break;
    case TRANS_NODE:
        switch(node_state){
        case FIRED:
            //mvaddstr(x, y+20, "recovering a node\n");
            node_state = RECOVERY;
            lastOutput = output;
            output = 0;
            //TODO: score this neuron here. 
            break;
        case RECOVERY:
            if(output > threshold){
                //mvaddstr(x, y+20, "fired a node\n");
                node_state = FIRED;
            }
            break;
        }
        break;
    }
    

}

void Node::transfer(Node *nodeMap[MAP_WIDTH][MAP_HEIGHT]){ //gets inputs
    std::vector<Node*> unfired_nodes;
    Node *adjacentNodes[3][3];
    
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if((x+i >= 0) && (y+j >= 0) && (x+i < MAP_WIDTH) && (y+j < MAP_HEIGHT))
                adjacentNodes[i+1][j+1] = nodeMap[x+i][y+j];
            else
                adjacentNodes[i+1][j+1] = 0;
        }
    }
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(adjacentNodes[i][j] == 0 || (i == 1 && j == 1)) continue;
            switch(node_type){
            case STOP_NODE:
                switch(adjacentNodes[i][j]->node_state){
                case FIRED:
                    output += (adjacentNodes[i][j]->getOutput() * links[i][j]->value);
                    break;
                case RECOVERY:
                    break;
                }
            case START_NODE:
                break; //start node shouldnt get any input.
            case TRANS_NODE:
                switch(adjacentNodes[i][j]->node_state){
                case FIRED:
                    //mvaddstr(x, y+40, "got fire");
                    output += (adjacentNodes[i][j]->getOutput() * links[i][j]->value * exp(-.1*links[i][j]->decay));
                    links[i][j]->decay += 2;
                    break;
                case RECOVERY:
                    if(links[i][j]->decay > 0)
                        links[i][j]->decay--;
                    break;
                }
                break;
            }
        }
    }
    
    return;
}

double Node::getColor(){
    if(output > 1){
        return output;
    }
    else{
        return lastOutput;
    }
}

double Node::getOutput(){
    return output;
}

void Node::connect(Node* nodeMap[MAP_WIDTH][MAP_HEIGHT], std::vector<Link*> link_ptrs){
    /*//this is the start node.
    Link *link;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(i == 1 && j == 1) continue;
            if(links[i][j]) continue; //this connection has already been made. skip
            if((x+i-1 >= 0) && (y+j-1 >= 0) && (x+i-1 < MAP_WIDTH) && (y+j-1 < MAP_HEIGHT)){
                link = new Link();
                links[i][j] = link;
                link->decay = 0;
                srand(R_SEED);
                link->value = (double)rand() / RAND_MAX; //1 to 0
                link_ptrs.push_back(link);
                nodeMap[i+x-1][j+y-1]->connect(nodeMap, link_ptrs, x, y, link);
            }
        }
    }*/
}

void Node::connect(Node* nodeMap[MAP_WIDTH][MAP_HEIGHT], std::vector<Link*> link_ptrs, int s_x, int s_y, Link *p_link){
    /*links[1 + s_x - x][1 + s_y - y] = p_link;
//    mvaddch(x, y, '!');
//    mvaddstr(50, 0, std::to_string(link_ptrs.size()).c_str());
//    refresh();
//    getch();
    Link *link;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(i == 1 && j == 1) continue;
            if(links[i][j]) continue; //this connection has already been made. skip
            if((x+i-1 >= 0) && (y+j-1 >= 0) && (x+i-1 < MAP_WIDTH) && (y+j-1 < MAP_HEIGHT)){
                link = new Link();
                links[i][j] = link;
                link->decay = 0;
                srand(R_SEED);
                link->value = (double)rand() / RAND_MAX; //1 to 0
                link_ptrs.push_back(link);
                nodeMap[i+x-1][j+y-1]->connect(nodeMap, link_ptrs, x, y, link);
            }
        }
    }*/
}
