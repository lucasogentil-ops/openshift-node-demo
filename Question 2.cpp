#include <cassert>
#include <iostream>
#include <utility>

// 1. Singly linked list from the lecture.
struct Node
{
    int data;
    Node *nextPtr;
};

void InsertAtEnd(Node **ptrToHeadPtr, int newData)
{
    // Walk to the pointer that holds nullptr, then replace it.
    while (*ptrToHeadPtr != nullptr)
    {
        ptrToHeadPtr = &(*ptrToHeadPtr)->nextPtr;
    }
    *ptrToHeadPtr = new Node{newData, nullptr};
}

void DeleteWithValue(Node **ptrToHeadPtr, int value)
{
    while (*ptrToHeadPtr != nullptr && (*ptrToHeadPtr)->data != value)
    {
        ptrToHeadPtr = &(*ptrToHeadPtr)->nextPtr;
    }
    if (*ptrToHeadPtr != nullptr)
    {
        Node *removed = *ptrToHeadPtr;
        *ptrToHeadPtr = removed->nextPtr;
        delete removed;
    }
}

void PrintList(const Node *headPtr)
{
    for (const Node *current = headPtr; current != nullptr; current = current->nextPtr)
    {
        std::cout << current->data << ' ';
    }
    std::cout << '\n';
}

// The lecture's class version owns its nodes and makes independent copies.
class LinkedList
{
private:
    Node *head = nullptr;

public:
    LinkedList() = default;
    ~LinkedList() { clear(); }

    LinkedList(const LinkedList &other) : LinkedList()
    {
        Node **nextLink = &head;
        for (const Node *current = other.head; current != nullptr; current = current->nextPtr)
        {
            *nextLink = new Node{current->data, nullptr};
            nextLink = &(*nextLink)->nextPtr;
        }
    }

    LinkedList &operator=(const LinkedList &other)
    {
        LinkedList copy(other);
        std::swap(head, copy.head);
        return *this;
    }

    void insertAtBeginning(int value) { head = new Node{value, head}; }
    void insertAtEnd(int value) { InsertAtEnd(&head, value); }
    void deleteWithValue(int value) { DeleteWithValue(&head, value); }
    void printList() const { PrintList(head); }
    bool isEmpty() const { return head == nullptr; }

    void clear()
    {
        while (head != nullptr)
        {
            Node *oldHead = head;
            head = head->nextPtr;
            delete oldHead;
        }
    }

    int getSize() const
    {
        int size = 0;
        for (const Node *current = head; current != nullptr; current = current->nextPtr)
        {
            size++;
        }
        return size;
    }

    bool contains(int value) const
    {
        for (const Node *current = head; current != nullptr; current = current->nextPtr)
        {
            if (current->data == value)
            {
                return true;
            }
        }
        return false;
    }
};

// 2. Doubly linked node and list.
class DoublyLinkedNode
{
public:
    int data;
    DoublyLinkedNode *prevPointer;
    DoublyLinkedNode *nextPointer;
};

class DoublyLinkedList
{
private:
    DoublyLinkedNode *head = nullptr;
    DoublyLinkedNode *tail = nullptr;

public:
    DoublyLinkedList() = default;
    DoublyLinkedList(const DoublyLinkedList &) = delete;
    DoublyLinkedList &operator=(const DoublyLinkedList &) = delete;

    ~DoublyLinkedList()
    {
        while (head != nullptr)
        {
            DoublyLinkedNode *oldHead = head;
            head = head->nextPointer;
            delete oldHead;
        }
    }

    // 3a. Insert before the old head.
    void InsertAtBeginning(int value)
    {
        DoublyLinkedNode *node = new DoublyLinkedNode{value, nullptr, head};
        if (head == nullptr)
        {
            tail = node;
        }
        else
        {
            head->prevPointer = node;
        }
        head = node;
    }

    // 3b. Insert after the old tail.
    void InsertAtEnd(int value)
    {
        DoublyLinkedNode *node = new DoublyLinkedNode{value, tail, nullptr};
        if (tail == nullptr)
        {
            head = node;
        }
        else
        {
            tail->nextPointer = node;
        }
        tail = node;
    }

    // 3c. Positions start at 0. Position equal to the length appends.
    // Return false for a position outside that range.
    bool InsertAtPosition(int value, int position)
    {
        if (position < 0)
        {
            return false;
        }
        if (position == 0)
        {
            InsertAtBeginning(value);
            return true;
        }

        DoublyLinkedNode *previous = head;
        for (int i = 1; i < position && previous != nullptr; i++)
        {
            previous = previous->nextPointer;
        }
        if (previous == nullptr)
        {
            return false;
        }
        if (previous == tail)
        {
            InsertAtEnd(value);
            return true;
        }

        DoublyLinkedNode *next = previous->nextPointer;
        DoublyLinkedNode *node = new DoublyLinkedNode{value, previous, next};
        previous->nextPointer = node;
        next->prevPointer = node;
        return true;
    }

    void PrintForward() const
    {
        for (const DoublyLinkedNode *node = head; node != nullptr; node = node->nextPointer)
        {
            std::cout << node->data << ' ';
        }
        std::cout << '\n';
    }

    void PrintBackward() const
    {
        for (const DoublyLinkedNode *node = tail; node != nullptr; node = node->prevPointer)
        {
            std::cout << node->data << ' ';
        }
        std::cout << '\n';
    }
};

int main()
{
    Node *head = nullptr;
    InsertAtEnd(&head, 10);
    InsertAtEnd(&head, 20);
    DeleteWithValue(&head, 10);
    assert(head != nullptr && head->data == 20 && head->nextPtr == nullptr);
    DeleteWithValue(&head, 20);
    assert(head == nullptr);

    LinkedList singly;
    singly.insertAtEnd(10);
    singly.insertAtEnd(20);
    singly.insertAtBeginning(5);
    LinkedList copy(singly);
    LinkedList assigned;
    assigned = copy;
    singly.deleteWithValue(10);
    assert(!singly.contains(10) && singly.getSize() == 2);
    assert(copy.contains(10) && assigned.getSize() == 3);
    std::cout << "Singly linked list: ";
    singly.printList();
    singly.clear();
    assert(singly.isEmpty());

    DoublyLinkedList doubly;
    doubly.InsertAtEnd(20);
    doubly.InsertAtBeginning(10);
    if (!doubly.InsertAtPosition(5, 0) ||
        !doubly.InsertAtPosition(15, 2) ||
        !doubly.InsertAtPosition(30, 4) ||
        doubly.InsertAtPosition(99, -1) ||
        doubly.InsertAtPosition(99, 6))
    {
        std::cerr << "Unexpected insertion result.\n";
        return 1;
    }
    std::cout << "Doubly linked list forward: ";
    doubly.PrintForward();
    std::cout << "Doubly linked list backward: ";
    doubly.PrintBackward();
    return 0;
}
