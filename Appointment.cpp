#include "Appointment.h"
#include "utils.h"

using namespace std;

void AppointmentManager::addToAvailList(int position) {
    AvailListNode* newNode = new AvailListNode{position, availList};
    availList = newNode;
}

int AppointmentManager::getFromAvailList() {
    if (availList == nullptr) return -1;
    int position = availList->position;
    AvailListNode* toDelete = availList;
    availList = availList->next;
    delete toDelete;
    return position;
}

void AppointmentManager::loadIndexes() {
    ifstream file(AppointmentsFile, ios::binary);
    if (!file.is_open()) return;

    string line;
    int position = 0;

    while (getline(file, line)) {
        if (line[0] == '*') {
            addToAvailList(position);
        } else {
            vector<string> parts = split(line, '|');
            primaryIndex.emplace_back(parts[1], position);
            secondaryIndex.emplace_back(parts[2], parts[1]);
        }
        position = file.tellg();
    }

    file.close();
}

void AppointmentManager::updateIndexes() {
    ifstream file(AppointmentsFile, ios::binary);
    primaryIndex.clear(), secondaryIndex.clear();

    int pos = 0;
    char ch;
    string line;
    vector<string> curRecord;
    while (file.get(ch)) {
        if (line.empty() && curRecord.size()) {
            if (curRecord[0][0] != '*') {
                primaryIndex.emplace_back(curRecord[1], pos);
                secondaryIndex.emplace_back(curRecord[2], curRecord[1]);
            }
            pos += 51;
        }
        if (ch == '\n') {
            while (line.size() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
            curRecord = split(line, '|');
            line.clear();
        } else
            line.push_back(ch);
    }

    if (line.empty() && curRecord.size() && curRecord[0][0] != '*') {
        primaryIndex.emplace_back(curRecord[1], pos);
        secondaryIndex.emplace_back(curRecord[2], curRecord[1]);
    }

    file.close();

    ofstream primaryFile(AppointmentsPI, ios::trunc);
    for (auto &entry : primaryIndex)
        primaryFile << entry.first << "|" << entry.second << "\n";
    primaryFile.close();

    ofstream secondaryFile(AppointmentsSI, ios::trunc);
    for (auto &entry : secondaryIndex)
        secondaryFile << entry.first << "|" << entry.second << "\n";
    secondaryFile.close();
}

vector<string> AppointmentManager::loadData() {
    vector<string> ret;
    ifstream file(AppointmentsFile);

    string line;
    while (getline(file, line)) ret.push_back(line);

    file.close();

    return ret;
}

AppointmentManager::AppointmentManager() {
    loadIndexes();
}

pair<bool, int> AppointmentManager::isAppointmentPresent(string ID) {
    vector<string> entries = loadData();
    int cnt = 0;
    for (auto &s : entries) {
        vector<string> cur = split(s, '|');
        if (cur[1] == ID && cur[0][0] != '*') return {true, cnt};
        cnt += 51;
    }
    return {false, 0};
}

void AppointmentManager::addAppointment(Appointment appointment) {
    auto isFound = isAppointmentPresent(appointment.ID);
    bool found = isFound.first;
    int pos = isFound.second;
    if (found) return void(cout << "appointment already exists\n");

    string record = appointment.ID + "|" + appointment.DoctorID + "|" + appointment.Date;
    string inform = to_string(record.size()) + "|" + record;

    inform += string(50 - inform.size(), '_');

    fstream file(AppointmentsFile, ios::in | ios::out | ios::binary);
    int position = getFromAvailList();
    if (position == -1) {
        file.seekp(0, ios::end);
        file << inform << endl;
        file.close();
    } else {
        file.close();
        vector<string> lines = loadData();
        ofstream file(AppointmentsFile, ios::out | ios::trunc);
        size_t currentByte = 0;
        for (auto& line : lines) {
            if (currentByte == position)
                file << inform << endl;
            else {
                while (line.back() == '\r' || line.back() == '\n') line.pop_back();
                file << line << endl;
            }
            currentByte += line.size() + 1;
        }
        file.close();
    }
    updateIndexes();
}
void AppointmentManager::updateAppointmentDate(int id) {
    auto isFound = isAppointmentPresent(to_string(id));
    bool found = isFound.first;
    int pos = isFound.second;
    if (!found) return void(cout << "appointment doesn't exist\n");

    cout << "Enter new Date: ";
    string newDate;
    cin >> newDate;

    vector<string> records = loadData(), v;
    for (string s : records) {
        vector<string> cur = split(s, '|');
        if (cur[1] == to_string(id)) {
            v = cur;
            break;
        }
    }

    // modify appointment
    Appointment appointment;
    appointment.ID = to_string(id), appointment.DoctorID = v[2], appointment.Date = newDate;
    while (appointment.Date.back() == '_') appointment.Date.pop_back();
    // update
    deleteAppointment(id);
    addAppointment(appointment);
}

void AppointmentManager::deleteAppointment(int id) {
    auto isFound = isAppointmentPresent(to_string(id));
    bool found = isFound.first;
    int pos = isFound.second;
    if (!found) return void(cout << "Doctor doesn't exist\n");

    vector<string> lines = loadData();
    ofstream file(AppointmentsFile, ios::out | ios::trunc);
    size_t currentByte = 0;
    for (auto& line : lines) {
        if (currentByte == pos) line[0] = '*';
        while (line.back() == '\r' || line.back() == '\n') line.pop_back();
        file << line << endl;

        currentByte += line.size() + 1;
    }
    file.close();

    addToAvailList(pos);

    updateIndexes();
}
void AppointmentManager::printInfo(int id) {
    auto isFound = isAppointmentPresent(to_string(id));
    bool found = isFound.first;
    int pos = isFound.second;
    if (!found) return void(cout << "Appointment doesn't exist\n");

    vector<string> data = loadData();
    for (auto &s : data) {
        vector<string> cur = split(s, '|');
        if (cur[1] == to_string(id)) {
            while (cur[3].back() == '_') cur[3].pop_back();
            cout << cur[1] << " " << cur[2] << " " << cur[3] << endl;
            return;
        }
    }
}