#ifndef FILES_ASS_DOCTOR_H
#define FILES_ASS_DOCTOR_H

#include <bits/stdc++.h>
#include "utils.cpp"

using namespace std;

const string DoctorsFile = "../Data/Doctors.txt";
const string DoctorsPI = "../Indexes/DoctorsDoctorID.txt";
const string DoctorsSI = "../Indexes/DoctorsDoctorName.txt";

class DoctorManager {
public:
    AvailListNode* availList = nullptr;
    vector<pair<string, int>> primaryIndex;
    map<string, LinkedList> secondaryIndex;
    DoctorManager();
    void updateIndexes();
    void addDoctor(Doctor &doctor);
    void deleteDoctor(int id);
    void updateDoctorName(int id);
    void printInfo(int id);
    void addToAvailList(int position, int size);
    int getFromAvailList(int stringSize);
    vector<string> loadData();
    bool isDoctorPresent(string ID);
    int getByteOffset(int id);
};

#include "Doctor.cpp"

#endif