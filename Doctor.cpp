#include "Doctor.h"

AvailListNode* availList = nullptr;

void addToAvailList(int position) {
    AvailListNode* newNode = new AvailListNode{position, availList};
    availList = newNode;
}

int getFromAvailList() {
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

vector<string> loadData() {
    vector<string> ret;
    ifstream file(DoctorsFile);

    string line;
    while (getline(file, line)) ret.push_back(line);

    file.close();

    return ret;
}

void DoctorManager::addDoctor(Doctor &doctor) {
    auto it = lower_bound(primaryIndex.begin(), primaryIndex.end(), make_pair(doctor.ID, 0));
    if (it != primaryIndex.end() && it->first == doctor.ID)
        return void(cout << "Doctor already exists!\n");

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
    cout << "Doctor added successfully.\n";
}
void DoctorManager::deleteDoctor(int id) {
    auto it = lower_bound(primaryIndex.begin(), primaryIndex.end(), make_pair(to_string(id), 0));
    if (it == primaryIndex.end() || it->first != to_string(id))
        return void(cout << "Doctor does not exist!\n");

    vector<string> lines = loadData();
    ofstream file(DoctorsFile, ios::out | ios::trunc);
    size_t currentByte = 0;
    for (auto& line : lines) {

        if (currentByte == it->second) line[0] = '*';
        while (line.back() == '\r' || line.back() == '\n') line.pop_back();
        file << line << endl;

        currentByte += line.size() + 1;
    }
    file.close();

    addToAvailList(it->second);

    updateIndexes();

    cout << "Doctor deleted successfully.\n";
}
void DoctorManager::updateDoctorName(int id) {
    auto it = lower_bound(primaryIndex.begin(), primaryIndex.end(), make_pair(to_string(id), 0));
    if (it == primaryIndex.end() || it->first != to_string(id))
        return void(cout << "Doctor does not exist!\n");

    // get new name
    cout << "Enter new name: ";
    string newName;
    cin.ignore();
    getline(cin, newName);

    // get record from index
    ifstream file(DoctorsFile, ios::binary);
    file.seekg(it->second);
    string line;
    getline(file, line);
    file.close();

    // parse
    vector<string> v = split(line, '|');

    // modify input
    Doctor doctor;
    doctor.Name = newName, doctor.Address = v[3];

    // update
    deleteDoctor(id);
    addDoctor(doctor);

    updateIndexes();
}

void DoctorManager::printInfo(int id) {
    auto it = lower_bound(primaryIndex.begin(), primaryIndex.end(), make_pair(to_string(id), 0));
    if (it == primaryIndex.end() || it->first != to_string(id))
        return void(cout << "Doctor does not exist!\n");

    fstream file(DoctorsFile, ios::in | ios::binary);
    file.seekg(it->second);
    string line;
    getline(file, line);

    vector<string> parts = split(line, '|');
    cout << "Doctor ID: " << parts[1] << "\n"
         << "Name: " << parts[2] << "\n"
         << "Address: " << parts[3] << "\n";

    file.close();
}