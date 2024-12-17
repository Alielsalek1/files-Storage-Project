#ifndef FILES_ASS_UTILS_H
#define FILES_ASS_UTILS_H

#include <bits/stdc++.h>
using namespace std;

class Doctor {
public:
    string ID;
    string Address;
    string Name;

    bool cmpID(Doctor &other) {
        return this->ID < other.ID;
    }

    bool cmpName(Doctor &other) {
        return this->Name < other.Name;
    }
};

class Appointment {
public:
    string ID;
    string Date;
    string DoctorID;
    bool cmpAppID (Appointment &other) {
        return this->ID < other.ID;
    }
    bool cmpDoctorID (Appointment &other) {
        return this->ID < other.ID;
    }
};

struct AvailListNode {
    int position;
    int size;
    AvailListNode* next;
};

struct Node {
    string data;
    Node* next;

    Node(string value) : data(value), next(nullptr) {}
};

struct LinkedList {
    Node* head;

    LinkedList() : head(nullptr) {}

    void insertBack(string value) {
        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = newNode;
        } else {

            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    deque<string> get() {
        deque<string> result;
        Node* temp = head;

        while (temp != nullptr) {
            result.push_back(temp->data);
            temp = temp->next;
        }

        return result;
    }
};

vector<string> split(string s, char delimiter);

#include "utils.cpp"

#endif