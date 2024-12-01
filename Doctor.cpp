#include <bits/stdc++.h>

using namespace std;

// [Length]DoctorID|DoctorName|Address
class DoctorManager {
    class Doctor {
        string ID;
        string Name;
        string Address;
        bool cmpID (Doctor &other) {
            return this->ID < other.ID;
        }
        bool cmpName (Doctor &other) {
            return this->Name < other.Name;
        }
    };
    struct AvailListNode {
        int position; // Byte position of the available record
        AvailListNode* next;
    };
    vector<Doctor> loadData() { // load the data to a vector if needed

    }
    void addDoctor(Doctor &doctor) {

    }
    void deleteDoctor(int id) {

    }
    void updateDoctorName(int id) {

    }
    void updatePrimaryIndex() {

    }
    void updateSecondaryIndex() {

    }
    void updateFile() {

    }
    void printInfo(int id) {

    }
};