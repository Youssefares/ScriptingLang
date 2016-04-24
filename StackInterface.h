/*
 * Declaration & API of a Singly-Linked Earthed Stack ADT
 */


#include <cstdlib>

template <typename item>
class Stack{
    //Node for singly-linked stack.
    class Node{
        public:
        item data;
        Node* next;
    };
    Node* top;
    public:
    //Stack operations.
    Stack();
    bool isEmpty();
    item peak();
    item pop();
    void push(item);
};

