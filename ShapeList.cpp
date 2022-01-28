//
//  ShapeList.cpp
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student:
//  Write the implementation (and only the implementation) of the ShapeList class below
#include "ShapeList.h"

ShapeList::ShapeList(){
    head = nullptr;
} // sets head to nullptr

ShapeList::~ShapeList(){
    ShapeNode* ptr;
    while (head != nullptr) {
        ptr = head;
        head = ptr -> getNext();
        delete ptr;
    }
} // deletes all list nodes starting
  // with head plus associated Shapes
    
ShapeNode* ShapeList::getHead() const{
    return head;
} // returns the head pointer
void ShapeList::setHead(ShapeNode* ptr){
    head = ptr;
} // sets the head pointer to ptr
    
ShapeNode* ShapeList::find(string name) const{
    ShapeNode* tptr = head;
    while (tptr != nullptr){
        if (tptr -> getShape() -> getName() == name) break;
        tptr = tptr -> getNext(); 
    }
    return tptr;
} // finds a shape node with the specified shape name
  // returns a pointer to the node if found, otherwise
  // returns nullptr if the node is not found
void ShapeList::insert(ShapeNode* s){
    if (head == nullptr) {
        head = s;
        s -> setNext(nullptr);
    }
    else {
        ShapeNode* tptr = head;
        while (tptr -> getNext() != nullptr)
            tptr = tptr -> getNext();
            tptr -> setNext(s);
            s -> setNext(nullptr); 
    }
} // inserts the node pointed to by s at the end
                                        // of the list; s should never be nullptr
ShapeNode* ShapeList::remove(string name){
    if (head == nullptr) return nullptr;
    ShapeNode* tptr = head;
    ShapeNode* pptr = nullptr;
    while (tptr != nullptr) {
        if (tptr -> getShape() -> getName() == name) break;
        pptr = tptr;
        tptr = tptr -> getNext();
    }
    if (tptr == nullptr) return nullptr;
    if (tptr == head) {
        head = head -> getNext();
        //delete tptr;
        return tptr;
    }
    pptr -> setNext(tptr -> getNext());
    //delete tptr; 
    return tptr;
} // removes the node with the shape called name
  // returns a pointer to the removed node or
  // nullptr is a node with name does not exist
void ShapeList::print() const{
    if(head != nullptr){
        ShapeNode* tptr = head;
        while (tptr != nullptr) {
            tptr->print();
            tptr = tptr -> getNext();
        }
    }
    return;
} // prints the list


