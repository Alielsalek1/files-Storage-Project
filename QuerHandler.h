#ifndef QUERYHANDLER_H
#define QUERYHANDLER_H

#include "Doctor.h"
#include "Appointment.h"
#include <bits/stdc++.h>

using namespace std;

class QueryHandler {
private:
    DoctorManager doctorManager;
    AppointmentManager appointmentManager;

    map<string, string> parseConditions(const string &whereClause) {
        map<string, string> conditions;
        istringstream ss(whereClause);
        string token;

        while (getline(ss, token, ' ')) {
            int eqPos = token.find('=');
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

    string extractFields(const vector<string> &parts, const vector<int> &indices) {
        string result;
        for (int index: indices) {
            if (!result.empty()) result += "|";
            result += parts[index];
        }
        return result;
    };

    vector<string> executeSelectWithNoConditions(const string &table, const string &fields) {
        vector<string> records, results;
        if (toUpperCase(table) == "DOCTORS") {
            records = doctorManager.loadData();
        } else if (toUpperCase(table) == "APPOINTMENTS") {
            records = appointmentManager.loadData();
        }
        for (auto record: records) {
            if (record.empty() || record[0] == '*') continue;
            vector<string> parts = split(record, '|');
            if (fields == "*" || toUpperCase(fields) == "ALL") {
                auto recordVector = split(record, '|');
                string result;
                for (int i = 1; i < recordVector.size(); i++) {
                    result += recordVector[i] + ' ';
                }
                results.push_back(result);
            } else {
                vector<int> fieldIndices;
                if (toUpperCase(table) == "DOCTORS") {
                    if (toUpperCase(fields) == "DOCTOR ID") fieldIndices.push_back(1);
                    if (toUpperCase(fields) == "DOCTOR NAME") fieldIndices.push_back(2);
                    if (toUpperCase(fields) == "DOCTOR ADDRESS") fieldIndices.push_back(3);
                } else if (toUpperCase(table) == "APPOINTMENTS") {
                    if (toUpperCase(fields) == "APPOINTMENT ID") fieldIndices.push_back(1);
                    if (toUpperCase(fields) == "APPOINTMENT DATE") fieldIndices.push_back(2);
                    if (toUpperCase(fields) == "DOCTOR ID") fieldIndices.push_back(3);
                }
                results.push_back(extractFields(parts, fieldIndices));
            }
        }
        return results;
    }

    vector<string> executeSelect(const string &table, const map<string, string> &conditions, const string &fields) {
        vector<string> results;
        string id;
        string record;
        int offset;
        string min_id = "1";
        deque<string> ids;
        deque<string>::iterator itr;
        if (conditions.empty()) {
            return executeSelectWithNoConditions(table, fields);
        }
        ifstream file;
        if (toUpperCase(table) == "DOCTORS") {
            file = ifstream(DoctorsFile, ios::binary);
            for (pair<string, string> condition: conditions) {
                if (!file.is_open()) return vector<string>() = {"Couldn't open doctors file. "};

                if (toUpperCase(condition.first) == "DOCTORID") {
                    auto found = lower_bound(doctorManager.primaryIndex.begin(), doctorManager.primaryIndex.end(),
                                             make_pair(condition.second, 0));
                    if (found != doctorManager.primaryIndex.end() && found->first != condition.second) return {};
                    offset = found->second;
//                    offset = lower_bound(doctorManager.primaryIndex.begin(), doctorManager.primaryIndex.end(),
//                                         pair(condition.second, 0))->second;
                } else if (toUpperCase(condition.first) == "DOCTORNAME") {
                    ids = doctorManager.secondaryIndex.at(condition.second).get();
                    if (ids.empty()) return {};
                    auto itr = ids.begin();
                    string id = *itr;
                    offset = lower_bound(doctorManager.primaryIndex.begin(), doctorManager.primaryIndex.end(),
                                         pair(id, 0))->second;
                }
            }
        } else if (toUpperCase(table) == "APPOINTMENTS") {
            file = ifstream(AppointmentsFile, ios::binary);
            for (pair<string, string> condition: conditions) {
                if (!file.is_open()) return vector<string>() = {"Couldn't open appointments file. "};

                if (toUpperCase(condition.first) == "APPOINTMENTID") {
                    auto found = lower_bound(appointmentManager.primaryIndex.begin(),
                                             appointmentManager.primaryIndex.end(), make_pair(condition.second, 0));
                    if (found != appointmentManager.primaryIndex.end() && found->first != condition.second) return {};

                    offset = lower_bound(appointmentManager.primaryIndex.begin(),
                                         appointmentManager.primaryIndex.end(),
                                         pair(condition.second, 0))->second;
                } else if (toUpperCase(condition.first) == "DOCTORID") {
                    ids = appointmentManager.secondaryIndex.at(condition.second).get();
                    if (ids.empty()) return {};
                    itr = ids.begin();
                    id = *itr;
                    offset = lower_bound(appointmentManager.primaryIndex.begin(), appointmentManager.primaryIndex.end(),
                                         pair(id, 0))->second;
                }
            }
        }
        bool flag = false;
        do {
            file.seekg(offset);
            getline(file, record);
            if (record == "\n" || record.empty()) {
                getline(file, record);
            }
            vector<string> parts = split(record, '|');
            if (fields == "*" || toUpperCase(fields) == "ALL") {
                auto recordVector = split(record, '|');
                string result;
                for (int i = 1; i < recordVector.size(); i++) {
                    result += recordVector[i] + ' ';
                }
                results.push_back(result);
            } else {
                vector<int> fieldIndices;
                if (toUpperCase(table) == "DOCTORS") {
                    if (toUpperCase(fields) == "DOCTOR ID") fieldIndices.push_back(1);
                    if (toUpperCase(fields) == "DOCTOR NAME") fieldIndices.push_back(2);
                    if (toUpperCase(fields) == "DOCTOR ADDRESS") fieldIndices.push_back(3);
                } else if (toUpperCase(table) == "APPOINTMENTS") {
                    if (toUpperCase(fields) == "APPOINTMENT ID") fieldIndices.push_back(1);
                    if (toUpperCase(fields) == "APPOINTMENT DATE") fieldIndices.push_back(2);
                    if (toUpperCase(fields) == "DOCTOR ID") fieldIndices.push_back(3);
                }
                results.push_back(extractFields(parts, fieldIndices));
            }
            flag = false;
            if (ids.size() > 1) {
                ids.pop_front();
                id = ids.front();
                if (toUpperCase(table) == "DOCTORS") {
                    auto found = lower_bound(doctorManager.primaryIndex.begin(), doctorManager.primaryIndex.end(), make_pair(conditions.begin()->second, 0));
                    if (found != doctorManager.primaryIndex.end() && found->first != conditions.begin()->second) return {};
                    offset = found->second;
//                    offset = lower_bound(doctorManager.primaryIndex.begin(), doctorManager.primaryIndex.end(),
//                                         pair(id, 0))->second;
                }
                else if (toUpperCase(table) == "APPOINTMENTS") {
                    auto found = lower_bound(appointmentManager.primaryIndex.begin(), appointmentManager.primaryIndex.end(), make_pair(conditions.begin()->second, 0));
                    if (found != doctorManager.primaryIndex.end() && found->first != conditions.begin()->second) return {};
                    offset = found->second;
//                    offset = lower_bound(appointmentManager.primaryIndex.begin(), appointmentManager.primaryIndex.end(),
//                                         pair(id, 0))->second;
                }
                flag = true;
            }
        } while (flag);
        file.close();
        return results;
    }

public:
    QueryHandler(DoctorManager doctorMgr, AppointmentManager appointmentMgr)
            : doctorManager(doctorMgr), appointmentManager(appointmentMgr) {}

    string toUpperCase(const string &str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    static inline string trim(const string &s) {
        auto start = s.find_first_not_of(" \t\n\r");
        if (start == string::npos) return "";
        auto end = s.find_last_not_of(" \t\n\r");
        return s.substr(start, end - start + 1);
    }

    void processQuery(const string &query) {

        string trimmedQuery = trim(query);

        string trimmedQueryUpper = toUpperCase(trimmedQuery);

        if (trimmedQueryUpper.find("SELECT") == 0) {

            int fromPos = trimmedQueryUpper.find("FROM");
            if (fromPos == string::npos) {
                return;
            }

            string selectFields = trimmedQuery.substr(6, fromPos - 6);
            selectFields = trim(selectFields);

            int wherePos = trimmedQueryUpper.find("WHERE", fromPos);
            string table;
            if (wherePos != string::npos) {
                table = trimmedQuery.substr(fromPos + 5, wherePos - fromPos - 5);
            } else {
                table = trimmedQuery.substr(fromPos + 5);
            }
            table = trim(table);

            string whereClause;
            if (wherePos != string::npos) {
                whereClause = trimmedQuery.substr(wherePos + 6);
            }
            whereClause = trim(whereClause);

            map<string, string> conditions = parseConditions(whereClause);

            vector<string> results = executeSelect(table, conditions, selectFields);

            if (results.empty()) {
            } else {
                for (const auto &result: results) {
                    cout << result << endl;
                }
            }
        } else {
        }
    }

};

#endif