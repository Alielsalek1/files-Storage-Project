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
    AvailListNode* next;
};

vector<string> split(string s, char delimiter);

#include "utils.cpp"

#endif
