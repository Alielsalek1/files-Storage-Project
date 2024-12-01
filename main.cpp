#include <bits/stdc++.h>
#include "Appointment.h"
#include "Doctor.h"

using namespace std;

int main() {
    int x = 0;
    DoctorManager doctorManager;
    AppointmentManager appointmentManager;
    while (x != 10) {
        cout << "1 - Add New Doctor\n"
                "2 - Add New Appointment\n"
                "3 - Update Doctor Name (Doctor ID)\n"
                "4 - Update Appointment Date (Appointment ID)\n"
                "5 - Delete Appointment (Appointment ID)\n"
                "6 - Delete Doctor (Doctor ID)\n"
                "7 - Print Doctor Info (Doctor ID)\n"
                "8 - Print Appointment Info (Appointment ID)\n"
                "9 - Write Query\n"
                "10 - Exit\n";
        cin >> x;
        if (x == 1) {
            Doctor doctor;
            cin >> doctor.ID >> doctor.Name >> doctor.Address;
            doctorManager.addDoctor(doctor);
        } else if (x == 2) {
            Appointment appointment;
            cin >> appointment.ID >> appointment.DoctorID >> appointment.Date;
            appointmentManager.addAppointment(appointment);
        } else if (x == 3) {
            int id; cin >> id;
            doctorManager.updateDoctorName(id);
        } else if (x == 4) {
            int id; cin >> id;
            appointmentManager.updateAppointmentDate(id);
        } else if (x == 5) {
            int id; cin >> id;
            appointmentManager.deleteAppointment(id);
        } else if (x == 6) {
            int id; cin >> id;
            doctorManager.deleteDoctor(id);
        } else if (x == 7) {
            int id; cin >> id;
            doctorManager.printInfo(id);
        } else if (x == 8) {
            int id; cin >> id;
            appointmentManager.printInfo(id);
        } else if (x == 9) {
            // TODO
        } else {
            exit(0);
        }
    }
    return 0;
}
