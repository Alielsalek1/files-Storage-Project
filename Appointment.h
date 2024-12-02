#ifndef FILES_ASS_APPOINTMENT_H
#define FILES_ASS_APPOINTMENT_H

#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

const string AppointmentsFile = R"(C:\extras\Projects\Files ass\Data\Appointments.txt)";
const string AppointmentsPI = R"(C:\extras\Projects\Files ass\Indexes\AppointmentsAppointmentID.txt)";
const string AppointmentsSI = R"(C:\extras\Projects\Files ass\Indexes\AppointmentsDoctorID.txt)";

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
