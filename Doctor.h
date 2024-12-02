#ifndef FILES_ASS_DOCTOR_H
#define FILES_ASS_DOCTOR_H

#include <bits/stdc++.h>
#include "utils.cpp"

using namespace std;

const string DoctorsFile = R"(C:\extras\Projects\Files ass\Data\Doctors.txt)";
const string DoctorsPI = R"(C:\extras\Projects\Files ass\Indexes\DoctorsDoctorID.txt)";
const string DoctorsSI = R"(C:\extras\Projects\Files ass\Indexes\DoctorsDoctorName.txt)";

// Doctor Format Length|DoctorID|DoctorName|Address
class DoctorManager {
public:
    AvailListNode* availList = nullptr;
    vector<pair<string, int>> primaryIndex;
    vector<pair<string, string>> secondaryIndex;
    void loadIndexes();
    void updateIndexes();
    DoctorManager();
    void addDoctor(Doctor &doctor);
    void deleteDoctor(int id);
    void updateDoctorName(int id);
    void printInfo(int id);
    void addToAvailList(int position);
    int getFromAvailList();
    vector<string> loadData();
    pair<bool, int> isDoctorPresent(string ID);
};

#include "Doctor.cpp"

#endif
