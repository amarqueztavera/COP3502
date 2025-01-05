// Andrea Marquez Tavera
// COP3502C Section 3
// New Layout

/*------------------------------------------------------------
OBJECTIVE

Give practice with stacks input in C. 
Give practice with trees in C.
------------------------------------------------------------*/

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/////// Struct definitions ///////
// Typedefs to make code cleaner
typedef struct Stack Stack;
typedef struct Node Node;

// Node of the linked list
struct Node {
    int data;
    Node * next;
};

// Stack
struct Stack {
    Node * head;
};

/////// Prototypes ///////
///// Linked List /////
Node * createNode(int data);
Node * insertHead(Node * head, int data);
Node * removeHead(Node * head);

///// Stack /////
Stack * createStack();
int isEmpty(Stack * stack);
void push(Stack * stack, int data);
void pop(Stack * stack);
int top(Stack * stack);
void deleteStack(Stack * stack);

int main() {
    int possQuartersCount = 0;
    int totalDistance = 0;
    int roomID;

    // Create a stack
    Stack * quartersStack = createStack();
    Stack * distanceStack = createStack();

    while(roomID != -1)// get user input of the employee's path
    {
        scanf("%d", &roomID);
        if(roomID != -1)
        {
            push(quartersStack, roomID);// add each entry to the room stack
        }   
    }
    
    // Creates Nodes to keep track of information/test it
    Node * parentOfQuarter = quartersStack->head;
    Node * quarterTest = quartersStack->head->next;
    Node * afterQuarterTest = quarterTest->next;

    while(!(isEmpty(quartersStack))) {// while there are rooms left in the stack
        // first must manipulate the corridor stack
        if(isEmpty(distanceStack) || (distanceStack->head->data != quartersStack->head->data)) {
            // if there are no corridors in its stack or if the head of the corridor stack is not the same as the room stack's
            push(distanceStack, quartersStack->head->data);// add head room to the corridor stack
        }

        // then test if heads of corridor and room stacks are the same
        if(distanceStack->head->next != NULL) {// if there is more than one value in the corridor stack
            if(parentOfQuarter->data == distanceStack->head->data && quarterTest->data == distanceStack->head->next->data) {
                // if the first two values of room stack and the head of corridor stack are the same
                pop(distanceStack);// delete head of corridor stack
            }
        }

        // then test if a room is a quarter
        if(afterQuarterTest != NULL) {// if there is a possible quarter
        // meaning, the possible quarter must be sandwiched by two rooms since the employee must immedaitely leave it to keep counting 
            if(parentOfQuarter->data == afterQuarterTest->data) {// if the employee return to the same room immediately after leaving another one
                possQuartersCount++;// increase quarter count
                Node * temp = distanceStack->head;// temporary Node to manipulate information
                while(temp != NULL)// increase the distance from main entrance to said quarter
                {
                    totalDistance += 1;
                    temp = temp->next;
                }
                pop(quartersStack);// delete head of corridor stack
            }
        }

        pop(quartersStack);// delete head of corridor stack

        parentOfQuarter = quartersStack->head;// update new "parent of quarter"
        if(parentOfQuarter->next == NULL) {// if there are no more values after this
            break;// stop
        } else if(parentOfQuarter->next->next == NULL) {// if there are no more than 1 value after this
            if(isEmpty(distanceStack) || (distanceStack->head->data != quartersStack->head->data)) {
                // if the corridor stack is empty or the head of the corridor stack is not the same as the room stack's
                push(distanceStack, quartersStack->head->data);// add head of room stack to corridor stack
            }
            break;// stop
        }
        quarterTest = quartersStack->head->next;// update new "quarter"
        afterQuarterTest = quarterTest->next;// update new "after quarter"
    }

    printf("%d %d\n", possQuartersCount, totalDistance);// print quarters and total corridors

    // Clean up the memory associated with the stack
    deleteStack(quartersStack);
    deleteStack(distanceStack);

    // Exit
    return 0;
}

/////// Functions ///////
///// Linked List /////
Node * createNode(int data) {
    Node * res = (Node *) malloc(sizeof(Node));
    res->data = data;
    res->next = NULL;
    return res;
}

// Function to insert a node to the head of a linked list
// Return the resulting head
Node * insertHead(Node * head, int data) {
    // Create and link the new head
    Node * newHead = createNode(data);
    newHead->next = head;

    // Return the resulting head
    return newHead;
}

// Function to remove the head of a given linked list
// Return the resulting head
Node * removeHead(Node * head) {
    // Empty list case
    if (head == NULL) {
        return NULL;
    }

    // Prevent Use after free by storing the new head pointer
    Node * newHead = head->next;

    // Delete the old head
    free(head);

    // Return the new head
    return newHead;
}

///// Stack /////
// Function to dynamically create a stack implemented using a Linked List
Stack * createStack() {
    Stack * res = (Stack *) malloc(sizeof(Stack));

    // Make the list empty
    res->head = NULL;

    // Return the resulting stack
    return res;
}

// Function to check if the stack is empty
int isEmpty(Stack * stack) {
    return stack->head == NULL;
}

// Function to add an element to the top of the stack
void push(Stack * stack, int data) {
    stack->head = insertHead(stack->head, data);
}

// Function to remove the top of a stack
void pop(Stack * stack) {
    stack->head = removeHead(stack->head);
}

// Function to access the top element of the stack
int top(Stack * stack) {
    // Ensure that the stack has something on top
    assert(stack->head != NULL);

    // Return the value on top
    return stack->head->data;
}

// Function to clean up the memory associated with a stack
void deleteStack(Stack * stack) {
    // Remove the values in the stack
    while (!isEmpty(stack))
        pop(stack);
    // Clean up the memory of the stack
    free(stack);
}