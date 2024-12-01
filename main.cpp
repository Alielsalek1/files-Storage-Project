#include <bits/stdc++.h>
#include "Appointment.cpp"
#include "Doctor.cpp"

using namespace std;

int main() {
    int x = 0;
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

        } else if (x == 2) {

        } else if (x == 3) {

        } else if (x == 4) {

        } else if (x == 5) {

        } else if (x == 6) {

        } else if (x == 7) {

        } else if (x == 8) {

        } else if (x == 9) {

        } else {
            exit(0);
        }
    }
    return 0;
}
