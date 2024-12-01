#include <bits/stdc++.h>

using namespace std;

// [Length]AppointmentID|AppointmentDate|DoctorID
class AppointmentManager {
    class Appointment {
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
        int position; // Byte position of the available record
        AvailListNode* next;

    };
    vector<Appointment> loadData() { // load the data to a vector if needed

    }
    void updateAppointmentDate(int id) {

    }
    void updatePrimaryIndex() {

    }
    void updateSecondaryIndex() {

    }
    void updateFile() {

    }
    void deleteAppointment(int id) {

    }
    void printInfo(int id) {

    }
};