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
    vector<pair<string, string>> secondaryIndex;
    void loadIndexes();
    void updateIndexes();
    AppointmentManager();
    void addAppointment(Appointment appointment);
    void deleteAppointment(int id);
    void updateAppointmentDate(int id);
    void printInfo(int id);
    void addToAvailList(int position);
    int getFromAvailList();
    vector<string> loadData();
    pair<bool, int> isAppointmentPresent(string ID);
};

#include "Appointment.cpp"

#endif
