    #include <bits/stdc++.h>
    #include "Appointment.h"
    #include "Doctor.h"
    #include "QueryHandler.h"

    using namespace std;

    int main() {
        DoctorManager doctorManager;
        AppointmentManager appointmentManager;

        int choice = 0;

        while (choice != 10) {
            // Menu Display
            cout << "1 - Add New Doctor\n"
                 << "2 - Add New Appointment\n"
                 << "3 - Update Doctor Name (Doctor ID)\n"
                 << "4 - Update Appointment Date (Appointment ID)\n"
                 << "5 - Delete Appointment (Appointment ID)\n"
                 << "6 - Delete Doctor (Doctor ID)\n"
                 << "7 - Print Doctor Info (Doctor ID)\n"
                 << "8 - Print Appointment Info (Appointment ID)\n"
                 << "9 - Write Query\n"
                 << "10 - Exit\n";

            cin >> choice;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');


            switch(choice) {
                case 1: {
                    // Add New Doctor
                    Doctor doctor;
                    cout << "Enter Doctor ID: ";
                    cin >> doctor.ID;
                    cin.ignore(); // Clear newline left in buffer
                    cout << "Enter Doctor Name: ";
                    getline(cin, doctor.Name);
                    cout << "Enter Doctor Address: ";
                    getline(cin, doctor.Address);
                    doctorManager.addDoctor(doctor);
                    break;
                }
                case 2: {
                    // Add New Appointment
                    Appointment appointment;
                    cout << "Enter Appointment ID: ";
                    cin >> appointment.ID;
                    cout << "Enter Doctor ID: ";
                    cin >> appointment.DoctorID;
                    cin.ignore(); // Clear newline left in buffer
                    cout << "Enter Appointment Date: ";
                    getline(cin, appointment.Date);
                    appointmentManager.addAppointment(appointment);
                    break;
                }
                case 3: {
                    // Update Doctor Name
                    int id;
                    cout << "Enter Doctor ID to update name: ";
                    cin >> id;
                    doctorManager.updateDoctorName(id);
                    break;
                }
                case 4: {
                    // Update Appointment Date
                    int id;
                    cout << "Enter Appointment ID to update date: ";
                    cin >> id;
                    appointmentManager.updateAppointmentDate(id);
                    break;
                }
                case 5: {
                    // Delete Appointment
                    int id;
                    cout << "Enter Appointment ID to delete: ";
                    cin >> id;
                    appointmentManager.deleteAppointment(id);
                    break;
                }
                case 6: {
                    // Delete Doctor
                    int id;
                    cout << "Enter Doctor ID to delete: ";
                    cin >> id;
                    doctorManager.deleteDoctor(id);
                    break;
                }
                case 7: {
                    // Print Doctor Info
                    int id;
                    cout << "Enter Doctor ID to print info: ";
                    cin >> id;
                    doctorManager.printInfo(id);
                    break;
                }
                case 8: {
                    // Print Appointment Info
                    int id;
                    cout << "Enter Appointment ID to print info: ";
                    cin >> id;
                    appointmentManager.printInfo(id);
                    break;
                }
                case 9: {
                    //write a query
                    QueryHandler queryHandler(&doctorManager, &appointmentManager);
                    string query;
                    cout << "Enter your query: " << endl;
                    getline(cin, query);
                    queryHandler.processQuery(query);
                    break;
                }case 10: {
                    // Exit
                    cout << "Exiting...\n";
                    break;
                }
                default: {
                    cout << "Invalid option. Please try again.\n";
                    break;
                }
            }
        }

        return 0;
    }