#include "Doctor.h"
#include "utils.h"

void DoctorManager::addToAvailList(int position) {
    AvailListNode* newNode = new AvailListNode{position, availList};
    availList = newNode;
}

int DoctorManager::getFromAvailList() {
    if (availList == nullptr) return -1;
    int position = availList->position;
    AvailListNode* toDelete = availList;
    availList = availList->next;
    delete toDelete;
    return position;
}

void DoctorManager::loadIndexes() {
    ifstream file(DoctorsFile, ios::binary);
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

void DoctorManager::updateIndexes() {
    ifstream file(DoctorsFile, ios::binary);
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

    ofstream primaryFile(DoctorsPI, ios::trunc);
    for (auto &entry : primaryIndex)
        primaryFile << entry.first << "|" << entry.second << "\n";
    primaryFile.close();

    ofstream secondaryFile(DoctorsSI, ios::trunc);
    for (auto &entry : secondaryIndex)
        secondaryFile << entry.first << "|" << entry.second << "\n";
    secondaryFile.close();
}

DoctorManager::DoctorManager() {
    loadIndexes();
}

vector<string> DoctorManager::loadData() {
    vector<string> ret;
    ifstream file(DoctorsFile);

    string line;
    while (getline(file, line)) ret.push_back(line);

    file.close();

    return ret;
}

pair<bool, int> DoctorManager::isDoctorPresent(string ID) {
    vector<string> entries = loadData();
    int cnt = 0;
    for (auto &s : entries) {
        vector<string> cur = split(s, '|');
        if (cur[1] == ID && cur[0][0] != '*') return {true, cnt};
        cnt += 51;
    }
    return {false, 0};
}

void DoctorManager::addDoctor(Doctor &doctor) {
    auto isFound = isDoctorPresent(doctor.ID);
    bool found = isFound.first;
    int pos = isFound.second;
    if (found) return void(cout << "Doctor already exists\n");

    string record = doctor.ID + "|" + doctor.Name + "|" + doctor.Address;
    string inform = to_string(record.size()) + "|" + record;

    inform += string(50 - inform.size(), '_');

    fstream file(DoctorsFile, ios::in | ios::out | ios::binary);
    int position = getFromAvailList();
    if (position == -1) {
        file.seekp(0, ios::end);
        file << inform << endl;
        file.close();
    } else {
        file.close();
        vector<string> lines = loadData();
        ofstream file(DoctorsFile, ios::out | ios::trunc);
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
void DoctorManager::deleteDoctor(int id) {
    auto isFound = isDoctorPresent(to_string(id));
    bool found = isFound.first;
    int pos = isFound.second;
    if (!found) return void(cout << "Doctor doesn't exist\n");

    vector<string> lines = loadData();
    ofstream file(DoctorsFile, ios::out | ios::trunc);
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
void DoctorManager::updateDoctorName(int id) {
    auto isFound = isDoctorPresent(to_string(id));
    bool found = isFound.first;
    int pos = isFound.second;
    if (!found) return void(cout << "Doctor doesn't exist\n");

    // get new name
    cout << "Enter new name: ";
    string newName;
    cin >> newName;

    vector<string> records = loadData(), v;
    for (string s : records) {
        vector<string> cur = split(s, '|');
        if (cur[1] == to_string(id)) {
            v = cur;
            break;
        }
    }

    // modify doctor
    Doctor doctor;
    doctor.Name = newName, doctor.ID = to_string(id), doctor.Address = v[3];
    while (doctor.Address.back() == '_') doctor.Address.pop_back();
    // update
    deleteDoctor(id);
    addDoctor(doctor);
}

void DoctorManager::printInfo(int id) {
    vector<string> entries = loadData();
    bool found = false;
    int cnt = 0;
    for (auto &s : entries) {
        vector<string> cur = split(s, '|');
        if (cur[1] == to_string(id)) {
            found = true;
            break;
        }
        cnt += 51;
    }
    if (!found) return void(cout << "No doctor with this ID");

    fstream file(DoctorsFile, ios::in | ios::binary);
    file.seekg(cnt);
    string line;
    getline(file, line);

    vector<string> parts = split(line, '|');
    cout << "Doctor ID: " << parts[1] << "\n"
         << "Name: " << parts[2] << "\n"
         << "Address: " << parts[3] << "\n";

    file.close();
}