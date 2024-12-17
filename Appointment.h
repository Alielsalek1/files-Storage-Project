#ifndef FILES_ASS_APPOINTMENT_H
#define FILES_ASS_APPOINTMENT_H

#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

const string AppointmentsFile = "../Data/Appointments.txt";
const string AppointmentsPI = "../Indexes/AppointmentsAppointmentID.txt";
const string AppointmentsSI = "../Indexes/AppointmentsDoctorID.txt";

// Length|AppointmentID|AppointmentDate|DoctorID
class AppointmentManager {
public:
    AvailListNode* availList = nullptr;
    vector<pair<string, int>> primaryIndex;
    map<string, LinkedList> secondaryIndex;

    AppointmentManager();
    void updateIndexes();
    void addAppointment(Appointment appointment);
    void deleteAppointment(int id);
    void updateAppointmentDate(int id);
    void printInfo(int id);
    void addToAvailList(int position, int size);
    int getFromAvailList(int stringSize);
    vector<string> loadData();
    bool isAppointmentPresent(string ID);
};

#include "Appointment.cpp"

#endif