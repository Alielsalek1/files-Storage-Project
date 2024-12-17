#include "Doctor.h"
#include "utils.h"

DoctorManager::DoctorManager() {
    updateIndexes();
}

// Function to add a block to the availList
void DoctorManager::addToAvailList(int position, int size) {
    AvailListNode* newNode = new AvailListNode{position, size, nullptr};

    // If the list is empty or the new block is larger than the head, insert at the head
    if (availList == nullptr || size > availList->size) {
        newNode->next = availList;
        availList = newNode;
        return;
    }

    // Find the correct position to insert the new node to keep the list sorted in descending order
    AvailListNode* current = availList;
    while (current->next != nullptr && current->next->size >= size) {
        current = current->next;
    }

    // Insert the new node
    newNode->next = current->next;
    current->next = newNode;
}

// Function to get the first available block from the availList
int DoctorManager::getFromAvailList(int stringSize) {
    if (availList == nullptr) return -1; // List is empty

    // Check if the head can accommodate the string
    if (availList->size < stringSize) return -1; // Not enough space

    // Get the byte offset for insertion
    int byteOffset = availList->position;

    // Adjust the head block
    availList->position += stringSize;
    availList->size -= stringSize;

    // Remove the head if it is completely used up
    if (availList->size > 1) {
        fstream file(DoctorsFile, ios::in | ios::out | ios::binary);
        file.seekp(availList->position, ios::beg);
        file.write("*", 1);
        file.close();
    }

    AvailListNode* toDelete = availList;
    availList = availList->next;
    addToAvailList(toDelete->position, toDelete->size); //reclaim space
    delete toDelete;

    return byteOffset;
}

void DoctorManager::updateIndexes() {
    ifstream file(DoctorsFile, ios::in | ios::out | ios::binary);
    primaryIndex.clear(), secondaryIndex.clear();

    char ch;
    string line;
    vector<string> curRecord;
    int pos = 0, pst = 0;
    while (file.get(ch)) {
        if (line.empty() && curRecord.size()) {
            if (curRecord[0][0] != '*') {
                primaryIndex.emplace_back(curRecord[1], pst);
                secondaryIndex[curRecord[2]].insertBack(curRecord[1]);
            }
            pst = pos;
        }
        if (ch == '\n') {
            while (line.size() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
            curRecord = split(line, '|');
            line.clear();
        } else
            line.push_back(ch);
        pos++;
    }
    if (line.empty() && curRecord.size() && curRecord[0][0] != '*') {
        primaryIndex.emplace_back(curRecord[1], pst);
        secondaryIndex[curRecord[2]].insertBack(curRecord[1]);
    }
    file.close();

    sort(primaryIndex.begin(), primaryIndex.end(),
         [](pair<string, int>& a, pair<string, int>& b) {
             return stoi(a.first) < stoi(b.first);
         });

    fstream primaryFile(DoctorsPI, ios::in | ios::out | ios::binary | ios::trunc);
    for (auto &entry : primaryIndex)
        primaryFile << entry.first << "|" << entry.second << "\n";
    primaryFile.close();

    fstream secondaryFile(DoctorsSI, ios::in | ios::out | ios::binary | ios::trunc);
    for (auto &entry: secondaryIndex) {
        secondaryFile << entry.first << "|";
        auto x = entry.second.get();
        for (auto &id: x) secondaryFile << id << "|";
        secondaryFile << "\n";
    }
    secondaryFile.close();
}

vector<string> DoctorManager::loadData() {
    vector<string> ret;
    ifstream file(DoctorsFile);

    string line;
    while (getline(file, line)) ret.push_back(line);

    file.close();
    return ret;
}

bool DoctorManager::isDoctorPresent(string ID) {
    vector<string> entries = loadData();
    for (auto &s : entries) {
        vector<string> cur = split(s, '|');
        if (cur.size() && cur[0][0] != '*' && cur[1] == ID) return true;
    }
    return false;
}

void DoctorManager::addDoctor(Doctor &doctor) {
    if (isDoctorPresent(doctor.ID)) return void(cout << "Doctor already exists\n");

    string record = doctor.ID + "|" + doctor.Name + "|" + doctor.Address;
    string inform = to_string(record.size()) + "|" + record + "\n";

    fstream file(DoctorsFile, ios::in | ios::out | ios::binary);
    int position = getFromAvailList((int)inform.size());
    if (position == -1)
        file.seekp(0, ios::end);
    else
        file.seekp(position, ios::beg);
    file.write(inform.c_str(), (int)inform.size());
    file.close();

    updateIndexes();
}

int DoctorManager::getByteOffset(int id) {
    for (auto &[ID, offset] : primaryIndex)
        if (stoi(ID) == id) return offset;
    return -1;
}

void DoctorManager::deleteDoctor(int id) {
    if (!isDoctorPresent(to_string(id))) return void(cout << "Doctor doesn't exist\n");

    fstream file(DoctorsFile, ios::in | ios::out | ios::binary);

    auto lines = loadData();
    int currentByte = 0, pos, sz;
    for (auto& line : lines) {
        auto cur = split(line, '|');

        if (cur.size() && cur[0][0] != '*' && cur[1] == to_string(id)) {
            line[0] = '*';
            pos = currentByte;
            while (line.back() == '\r' || line.back() == '\n') line.pop_back();
            sz = (int)line.size() + 1;
        }
        file << line << endl;

        currentByte += (int)line.size() + 1;
    }
    file.close();

    addToAvailList(pos, sz);

    updateIndexes();
}

void DoctorManager::updateDoctorName(int id) {
    if (!isDoctorPresent(to_string(id))) return void(cout << "Doctor doesn't exist\n");

    cout << "Enter new name: ";
    string newName; cin >> newName;

    vector<string> records = loadData(), v;
    for (string s : records) {
        vector<string> cur = split(s, '|');
        if (cur.size() && cur[0][0] != '*' && cur[1] == to_string(id)) {
            v = cur;
            break;
        }
    }

    // modify doctor
    Doctor doctor;
    doctor.Name = newName, doctor.ID = to_string(id), doctor.Address = v[3];

    // update
    deleteDoctor(id);
    addDoctor(doctor);
}

void DoctorManager::printInfo(int id) {
    if (!isDoctorPresent(to_string(id))) return void(cout << "Doctor doesn't exist\n");

    vector<string> data = loadData();
    for (auto &s : data) {
        vector<string> cur = split(s, '|');
        if (cur.size() && cur[0][0] != '*' && cur[1] == to_string(id)) {
            cout << "Doctor Id:" << cur[1] << endl;
            cout << "Doctor Name:" << cur[2] << endl;
            cout << "Doctor Address:" << cur[3] << endl;
            return;
        }
    }
}