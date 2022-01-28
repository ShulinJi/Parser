//
//  parser.cpp
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"
#include "ShapeNode.h"
#include "GroupNode.h"
#include "ShapeList.h"
#include "GroupList.h"

// This is a pointer to the groups list
// The list itseld must be allocated
GroupList* gList;
// check if the argument types pass in is valid
// name is reserved for shape types 
bool validShapeName(string name){
    bool indicator;
    for (int i = 0; i < NUM_KEYWORDS; i++){
        if (name != keyWordsList[i]){
            indicator = true;
            if (i < NUM_TYPES && name == shapeTypesList[i]){
                indicator = false;
                cout << "error: invalid name" << endl;
                break;
            }
        }else {
            indicator = false;
            cout << "error: invalid name" << endl;
            break;
        }        
    }
    return indicator;
}

// check if name already exists in the current database
bool nameExisted(string &name){
    if(gList->getHead() ==  nullptr){
        return true;
    }
    GroupNode* list = gList->getHead();
    while(list != nullptr){
        if(list->getName() == name){
            cout << "error: name " << name << " exists" << endl;
            return false;
        }
        //check myShapeList head is nullptr 
        if(list->getShapeList() == nullptr || list->getShapeList()->getHead() == nullptr){ 
            list = list->getNext();
            continue;
        } 
        ShapeNode* node = list->getShapeList()->getHead(); //first shapeNode
        while(node != nullptr) {
            if(node->getShape()->getName() == name){
                cout << "error: name " << name << " exists" << endl;
                return false;
            }
            if (node->getNext() == nullptr){
                    break;
            }
            node = node->getNext();
        }
        if (list->getNext() == nullptr){
            break;
        }
        list = list->getNext();
    } //while(list != nullptr && list->getNext() != nullptr);
    return true;
}

bool shapeFound(string &shapeName){
    if(gList->getHead() ==  nullptr){
        return false;
    }
    GroupNode* list = gList->getHead();
    while (list != nullptr){
        //check myShapeList head is nullptr 
        if(list->getShapeList() == nullptr || list->getShapeList()->getHead() == nullptr){ 
            list = list->getNext();
            continue;
        } 
        ShapeNode* node = list->getShapeList()->getHead(); //first shapeNode
        while (node != nullptr){
            if(node->getShape()->getName() == shapeName){
                return true;
            }
            if (node->getNext() == nullptr){
                break;
            }
            node = node->getNext(); //shapeNode moves to next
        }
        if (list->getNext() == nullptr){
            break;
        }
        list = list->getNext(); //groupNode moves to next
    }
    return false;
}


bool groupFound(string &groupName){
    if(gList->getHead() ==  nullptr){
        return false; //what if the list is empty?
    }
    GroupNode* list = gList->getHead();

    if (list->getName() == groupName){
        return true;
    }
    while (list != nullptr){
        if(list->getName() == groupName){
            return true;
        }
        if(list->getNext() == nullptr){
            break;
        }
        list = list->getNext();
    }
   /* do {
        if(list->getName() == groupName){
            return true;
        }
        list = list->getNext(); //next groupNode
    } while(list->getNext() != nullptr); */
    return false;
}

bool deleteNameFound(string &name){
    bool indicator1;
    bool indicator2;

    if (shapeFound(name)){
        indicator1 = 1;
    } else {
        indicator1 = 0;
    }

    if (groupFound(name)){
        indicator2 = 1;
    } else {
        indicator2 = 0;
    }
    if ((indicator1 == indicator2) && (indicator1 == 0) && (indicator2 == 0)){
        cout << "error: shape " << name << " not found" << endl;
        return false;
    }
    return true;
}

void shape(string &name, string &type, int x_loc, int y_loc, int x_sz, int y_sz){
   if (gList->getHead()->getShapeList() == nullptr){
        ShapeList* newList = new ShapeList();
        ShapeNode* newNode = new ShapeNode();
        Shape* newShape = new Shape(name, type, x_loc, y_loc, x_sz, y_sz);
        newNode->setShape(newShape);
        newList->setHead(newNode);
        gList->getHead()->setShapeList(newList);
    } else {
        ShapeNode* newNode = new ShapeNode();
        Shape* newShape = new Shape(name, type, x_loc, y_loc, x_sz, y_sz);
        newNode->setShape(newShape);
        gList->getHead()->getShapeList()->insert(newNode);
    }

    cout << name << ": " << type << " " << x_loc << " " << y_loc << " " << x_sz << " " << y_sz << endl;
}

void group(string &name){
    GroupNode* newNode = new GroupNode(name);
    gList->insert(newNode);
    cout << name << ": group" << endl;
}

void move(string &shapeName, string &groupName){
    ShapeNode* tptr;
    GroupNode* ttptr;
    ttptr = gList->getHead();
    while(ttptr !=nullptr) {
        if ((tptr = ttptr->getShapeList()->remove(shapeName)) && (tptr != nullptr)){
            break;
        } else { 
            ttptr = ttptr->getNext();
        }
    }

    tptr->setNext(nullptr);

    ttptr = gList->getHead();

    while(ttptr !=nullptr) {
        if (ttptr->getName() == groupName){
            break;
        } else { 
            ttptr = ttptr->getNext();
        }
    }

    if (ttptr->getShapeList() == nullptr){
        ShapeList* newList = new ShapeList();
        newList->setHead(tptr);
        ttptr->setShapeList(newList);
    } else {
        ttptr->getShapeList()->insert(tptr);
    }
    cout << "moved " << shapeName << " to " << groupName << endl;
}

GroupNode* deleteShape(GroupNode* node, string &name){
    if (node) {
        node->getShapeList()->remove(name);
        GroupNode* next = node->getNext();
        node->setNext(deleteShape(next, name));
        return node;
    }
    return nullptr;
}

void deleteName(string &name){
    if(gList->getHead() ==  nullptr){
        return;
    }
    GroupNode* removed;
    removed = gList->remove(name);

    if(removed != nullptr){
        if(removed->getShapeList() == nullptr){
            delete removed;
            cout << name << ": deleted" << endl;
        } else if (removed->getShapeList()->getHead() != nullptr){
            while (removed->getShapeList()->getHead() != nullptr){
                string shapeName = removed->getShapeList()->getHead()->getShape()->getName();
                ShapeNode* newPtr = removed->getShapeList()->remove(shapeName);
                gList->getHead()->getShapeList()->insert(newPtr);
            }
            removed->getShapeList()->setHead(nullptr);
            removed->setNext(nullptr);
            delete removed;
            cout << name << ": deleted" << endl;
        }
    } else {
        GroupNode* head = gList->getHead();
        gList->setHead(deleteShape(head, name));
        //GroupNode* deleted = deleteShape(head, name);
        //gList->setHead(deleted);
        //delete deleted;
        cout << name << ": deleted" << endl;
    }
}


void drawName(){
    cout << "drawing:" << endl;
    gList->print();
}

bool checkShape(string &shapeName, string &type, int x_loc, int y_loc, int x_sz, int y_sz){
    if (validShapeName(shapeName)){
        if (nameExisted(shapeName)){
            if (x_loc >= 0 && y_loc >= 0 && x_sz >= 0 && y_sz >= 0){
                return true;
            }
        }
    }
    return false;
}

bool checkGroup(string &groupName){
    if (validShapeName(groupName)){
        if (nameExisted(groupName)){
            return true;
        }
    }
    return false;
}

bool checkMove(string &shapeName, string &groupName){
    if (shapeFound(shapeName)){
        if (groupFound(groupName)){
            return true;
        } else{
            cout << "error: group " << groupName << " not found" << endl;
        }
    } else {
        cout << "error: shape " << shapeName << " not found" << endl;
    }
    return false;
}

bool checkDelete(string &name){
    if(validShapeName(name)){
        if (deleteNameFound(name)){
            return true;
        }
    }
    return false;
}

int findCommand(string &s){
    int commandNum = 0;
    for (int i = 0; i < NUM_KEYWORDS; i++){
        if (keyWordsList[i] == s){
            commandNum = i;
            break;
        }
    }
    return commandNum;
}


// ECE244 Student: you may want to add the prototype of
// helper functions you write here

int main() {
    // Create the groups list
    gList = new GroupList();
    
    // Create the poo group and add it to the group list
    GroupNode* poolGroup = new GroupNode(keyWordsList[NUM_KEYWORDS-1]);
    gList->insert(poolGroup);
    
    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while ( !cin.eof () ) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;
        if (command.empty()){
            continue;
        }
        int commandNum;
        commandNum = findCommand(command);

        // Check for the command and act accordingly
        string argument;
        getline(lineStream, argument);
        stringstream alex(argument);

        string name, type, shapeName, groupName;
        int x_loc, y_loc, x_sz, y_sz;
        // ECE244 Student: Insert your code here
        switch(commandNum){
            case 0:
                alex >> shapeName >> type >> x_loc >> y_loc >> x_sz >> y_sz;
                if (checkShape(shapeName, type, x_loc, y_loc, x_sz, y_sz)){
                    shape(shapeName, type, x_loc, y_loc, x_sz, y_sz);
                }
                break;
            case 1:
                alex >> shapeName;
                if (checkGroup(shapeName)){
                    group(shapeName);
                }
                break;
            case 2:
                alex >> shapeName >> groupName;
                if (checkMove(shapeName, groupName)){
                    move(shapeName, groupName);
                }
                break;
            case 3:
                alex >> name;
                if(checkDelete(name)){
                    deleteName(name);
                }
                break;
            case 4:
                    drawName();
                    break;
            case 5:
                cout << "error: invalid name" << endl;
                break;
        }

        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
        
    }  // End input loop until EOF.
    
    return 0;
}

