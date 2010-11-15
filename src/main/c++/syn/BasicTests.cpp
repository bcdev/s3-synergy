/*
 * File:   BasicTests.cpp
 * Author: thomass
 *
 * Created on November 5, 2010, 1:43 PM
 */

#include <iostream>

using namespace std;

struct Student {
    int id;
    bool isGrad;
};

struct ListNode {
    int data;
    ListNode * next;
};

class BasicTests {
    int classVariable;

public:
    BasicTests(int classVariable);
    void playAroundWithLists();
    void printGradStudentsCount();
    void printEqualityWithoutArgs();
    void printClassVariable();

private:
    void printEquality(int first[], int second[]);
    bool areArraysEqual(int first[], int second[], int size);
    int numGrads(Student students[], int size);
    void printList(ListNode * list);
    void printListReverse(ListNode * list);

};

BasicTests::BasicTests(int _classVariable) {
    classVariable = _classVariable;
}

void BasicTests::printEqualityWithoutArgs() {
    int first[5];
    int second[5];

    int first_wrong[5];
    int second_wrong[5];

    for (int i = 0; i < 5; i++) {
        first[i] = i;
        second[i] = i;

        first_wrong[i] = 0;
        second_wrong[i] = 1;
    }
    printEquality(first, second);
    printEquality(first_wrong, second_wrong);
}

void BasicTests::playAroundWithLists() {
    ListNode * head = NULL; // head points to the list of nodes, initially empty
    int k;
    for (int i = 0; i < 5; i++) {

        k = i;

        // create a new list node containing the value read in
        ListNode * tmp = new ListNode;
        tmp->data = k;

        // attach the new node to the front of the list
        tmp->next = head;
        head = tmp;
    }

    printList(head);
    printListReverse(head);
}

void BasicTests::printGradStudentsCount() {
    int size = 13;
    Student students[size];
    Student * pointer = students;
    for (int i = 0; i < size; i++) {
        pointer->id = i;
        pointer->isGrad = (i % 2 == 0);
        pointer++;
    }

    cout << numGrads(students, size) << "\n\n" << endl;
}

bool BasicTests::areArraysEqual(int first[], int second[], int size) {
    for (int i = 0; i < size; i++) {
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;

}

void BasicTests::printEquality(int first[], int second[]) {
    if (areArraysEqual(first, second, 5)) {
        cout << "Two arrays are equal!" << endl;
    } else {
        cout << "Two arrays are not equal!" << endl;
    }
}

int BasicTests::numGrads(Student student[], int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        if (student[i].isGrad) {
            result++;
        }
    }
    return result;

}

void BasicTests::printList(ListNode * list) {
    while (list != NULL) {
        cout << list->data << endl;
        list = list->next;
    }
}

void BasicTests::printListReverse(ListNode * list) {
    //    cout << list << endl;
    if (list != NULL) {
        printListReverse(list->next);
        cout << list->data << ", ";
    }
}

void BasicTests::printClassVariable() {
    cout << "My class variable is: " << classVariable << "\n";
}