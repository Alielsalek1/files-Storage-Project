#ifndef QUERYHANDLER_H
#define QUERYHANDLER_H

#include "Doctor.h"
#include "Appointment.h"
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <locale>

using namespace std;

class QueryHandler {
private:
    DoctorManager* doctorManager;
    AppointmentManager* appointmentManager;

    map<string, string> parseConditions(const string& whereClause) {
        map<string, string> conditions;
        istringstream ss(whereClause);
        string token;

        while (getline(ss, token, ' ')) {
            size_t eqPos = token.find('=');
            if (eqPos != string::npos) {
                string key = token.substr(0, eqPos);
                string value = token.substr(eqPos + 1);
                key.erase(remove(key.begin(), key.end(), '\''), key.end());
                value.erase(remove(value.begin(), value.end(), '\''), value.end());
                conditions[trim(key)] = trim(value);
            }
        }
        return conditions;
    }

    vector<string> executeSelect(const string& table, const map<string, string>& conditions, const string& fields) {
        vector<string> results;

        auto extractFields = [](const vector<string>& parts, const vector<int>& indices) {
            string result;
            for (int index : indices) {
                if (!result.empty()) result += "|";
                result += parts[index];
            }
            return result;
        };

        if (table == "Doctors") {
            vector<string> records = doctorManager->loadData();
            for (const auto& record : records) {
                if (record.empty() || record[0] == '*') continue;
                vector<string> parts = split(record, '|');
                bool match = true;

                for (const auto& condition : conditions) {
                    if (condition.first == "DoctorID" && parts[1] != condition.second) {
                        match = false;
                        break;
                    }
                }

                if (match) {
                    if (fields == "*" || toUpperCase(fields) == "ALL") {
                        results.push_back(record); // Return full record
                    } else {
                        // Map requested fields to indices
                        vector<int> fieldIndices;
                        if (toUpperCase(fields) == "DOCTOR NAME") fieldIndices.push_back(2); // Example mapping
                        results.push_back(extractFields(parts, fieldIndices));
                    }
                }
            }
        } else if (table == "Appointments") {
            vector<string> records = appointmentManager->loadData();
            for (const auto& record : records) {
                if (record.empty() || record[0] == '*') continue;
                vector<string> parts = split(record, '|');
                bool match = true;

                for (const auto& condition : conditions) {
                    if (condition.first == "DoctorID" && parts[2] != condition.second) {  // DoctorID is in index 2
                        match = false;
                        break;
                    }
                }

                if (match) {
                    if (fields == "*" || toUpperCase(fields) == "ALL") {
                        results.push_back(record); // Return full record
                    } else {
                        // Map requested fields to indices
                        vector<int> fieldIndices;
                        if (toUpperCase(fields) == "APPOINTMENT ID") fieldIndices.push_back(1);
                        if (toUpperCase(fields) == "DOCTOR ID") fieldIndices.push_back(2);
                        if (toUpperCase(fields) == "APPOINTMENT DATE") fieldIndices.push_back(3);
                        results.push_back(extractFields(parts, fieldIndices));
                    }
                }
            }
        }

        return results;
    }




public:
    QueryHandler(DoctorManager* doctorMgr, AppointmentManager* appointmentMgr)
            : doctorManager(doctorMgr), appointmentManager(appointmentMgr) {}

    string toUpperCase(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    static inline string trim(const string& s) {
        auto start = s.find_first_not_of(" \t\n\r");
        if (start == string::npos) return "";
        auto end = s.find_last_not_of(" \t\n\r");
        return s.substr(start, end - start + 1);
    }

    void processQuery(const string& query) {
        cout << "[DEBUG] Received Query: '" << query << "'" << endl;

        // Use the trim function to remove leading/trailing spaces
        string trimmedQuery = trim(query);
        cout << "[DEBUG] Trimmed Query: '" << trimmedQuery << "'" << endl;


        string trimmedQueryUpper = toUpperCase(trimmedQuery);
        cout << "[DEBUG] Trimmed & Uppercased Query: '" << trimmedQueryUpper << "'" << endl;


        if (trimmedQueryUpper.find("SELECT") == 0) {
            cout << "[DEBUG] Query starts with SELECT" << endl;

            size_t fromPos = trimmedQueryUpper.find("FROM");
            if (fromPos == string::npos) {
                cout << "Unsupported query - 'FROM' not found." << endl;
                return;
            }


            string selectFields = trimmedQuery.substr(6, fromPos - 6);
            selectFields = trim(selectFields); // Trim any spaces
            cout << "[DEBUG] Fields to SELECT: '" << selectFields << "'" << endl;


            size_t wherePos = trimmedQueryUpper.find("WHERE", fromPos);
            string table;
            if (wherePos != string::npos) {
                table = trimmedQuery.substr(fromPos + 5, wherePos - fromPos - 5);
            } else {
                table = trimmedQuery.substr(fromPos + 5);
            }
            table = trim(table);
            cout << "[DEBUG] Table Name: '" << table << "'" << endl;

            string whereClause;
            if (wherePos != string::npos) {
                whereClause = trimmedQuery.substr(wherePos + 6);
            }
            whereClause = trim(whereClause);
            cout << "[DEBUG] WHERE Clause: '" << whereClause << "'" << endl;

            map<string, string> conditions = parseConditions(whereClause);

            vector<string> results = executeSelect(table, conditions, selectFields);

            if (results.empty()) {
                cout << "[DEBUG] No results found for the query." << endl;
            } else {
                for (const auto& result : results) {
                    cout << "[DEBUG] Result: " << result << endl;
                    cout << result << endl;
                }
            }
        } else {
            cout << "[DEBUG] Query does NOT start with SELECT or is malformed." << endl;
            cout << "Unsupported query." << endl;
        }
    }

};

#endif // QUERYHANDLER_H
