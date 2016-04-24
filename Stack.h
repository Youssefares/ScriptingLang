/*
 * Implementation of a Singly-Linked Earthed Stack ADT
 */

#include "StackInterface.h"

using namespace std;
template<typename item>
Stack<item>::Stack(){
    top = NULL;
}

template<typename item>
bool Stack<item>::isEmpty(){
    return top == NULL;
}
/*
 * returns data at head of linked list (top).
 */

template<typename item>
item Stack<item>::peak(){
    return top->data;
}
/*
 * removes & returns data at head of linked list (top).
 */

template<typename item>
item Stack<item>::pop(){
    Node* doomed = top;
    top = top->next;
    return doomed->data;
}

/*
 * adds at head of linked list (top).
 */
template<typename item>
void Stack<item>::push(item data){
    Node* newTop = new Node();
    newTop->data = data;
    newTop->next = top;
    top = newTop;
    return;
}