#include "Appointment.h"
#include "utils.h"

using namespace std;

AppointmentManager::AppointmentManager() {
    updateIndexes();
}

// Function to add a block to the availList
void AppointmentManager::addToAvailList(int position, int size) {
    AvailListNode *newNode = new AvailListNode{position, size, nullptr};

    // If the list is empty or the new block is larger than the head, insert at the head
    if (availList == nullptr || size > availList->size) {
        newNode->next = availList;
        availList = newNode;
        return;
    }

    // Find the correct position to insert the new node to keep the list sorted in descending order
    AvailListNode *current = availList;
    while (current->next != nullptr && current->next->size >= size) {
        current = current->next;
    }

    // Insert the new node
    newNode->next = current->next;
    current->next = newNode;
}

// Function to get the first available block from the availList
int AppointmentManager::getFromAvailList(int stringSize) {
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
        fstream file(AppointmentsFile, ios::in | ios::out | ios::binary);
        file.seekp(availList->position, ios::beg);
        file.write("*", 1);
        file.close();
    }

    AvailListNode *toDelete = availList;
    availList = availList->next;
    addToAvailList(toDelete->position, toDelete->size); //reclaim space
    delete toDelete;

    return byteOffset;
}

void AppointmentManager::updateIndexes() {
    ifstream file(AppointmentsFile, ios::in | ios::out | ios::binary);
    primaryIndex.clear(), secondaryIndex.clear();

    char ch;
    string line;
    vector<string> curRecord;
    int pos = 0, pst = 0;
    while (file.get(ch)) {
        if (line.empty() && curRecord.size()) {
            if (curRecord[0][0] != '*') {
                primaryIndex.emplace_back(curRecord[1], pst);
                secondaryIndex[curRecord[3]].insertBack(curRecord[1]);
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
        secondaryIndex[curRecord[3]].insertBack(curRecord[1]);
    }
    file.close();

    sort(primaryIndex.begin(), primaryIndex.end(),
         [](pair<string, int> &a, pair<string, int> &b) {
             return stoi(a.first) < stoi(b.first);
         });

    fstream primaryFile(AppointmentsPI, ios::in | ios::out | ios::binary | ios::trunc);
    for (auto &entry: primaryIndex)
        primaryFile << entry.first << "|" << entry.second << "\n";
    primaryFile.close();

    fstream secondaryFile(AppointmentsSI, ios::in | ios::out | ios::binary | ios::trunc);
    for (auto &entry: secondaryIndex) {
        secondaryFile << entry.first << "|";
        auto x = entry.second.get();
        for (auto &id: x) secondaryFile << id << "|";
        secondaryFile << "\n";
    }
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

bool AppointmentManager::isAppointmentPresent(string ID) {
    vector<string> entries = loadData();
    for (auto &s: entries) {
        vector<string> cur = split(s, '|');
        if (cur.size() && cur[0][0] != '*' && cur[1] == ID && cur[0][0] != '*') return true;
    }
    return false;
}

void AppointmentManager::addAppointment(Appointment appointment) {
    if (isAppointmentPresent(appointment.ID)) return void(cout << "Appointment already exists\n");

    string record = appointment.ID + "|" + appointment.Date + "|" + appointment.DoctorID;
    string inform = to_string(record.size()) + "|" + record + "\n";

    fstream file(AppointmentsFile, ios::in | ios::out | ios::binary);
    int position = getFromAvailList((int) inform.size());
    if (position == -1)
        file.seekp(0, ios::end);
    else
        file.seekp(position, ios::beg);
    file.write(inform.c_str(), (int) inform.size());
    file.close();

    updateIndexes();
}

void AppointmentManager::updateAppointmentDate(int id) {
    if (!isAppointmentPresent(to_string(id))) return void(cout << "Appointment doesn't exist\n");

    cout << "Enter new Date: ";
    string newDate;
    cin >> newDate;

    vector<string> records = loadData(), v;
    for (string s: records) {
        vector<string> cur = split(s, '|');
        if (cur.size() && cur[0][0] != '*' && cur[1] == to_string(id)) {
            v = cur;
            break;
        }
    }

    // modify doctor
    Appointment appointment;
    appointment.Date = newDate, appointment.ID = to_string(id), appointment.DoctorID = v[3];

    // update
    deleteAppointment(id);
    addAppointment(appointment);
}

void AppointmentManager::deleteAppointment(int id) {
    if (!isAppointmentPresent(to_string(id))) return void(cout << "Appointment doesn't exist\n");

    fstream file(AppointmentsFile, ios::in | ios::out | ios::binary);

    auto lines = loadData();
    int currentByte = 0, pos, sz;
    for (auto &line: lines) {
        auto cur = split(line, '|');

        if (cur.size() && cur[0][0] != '*' && cur[1] == to_string(id)) {
            line[0] = '*';
            pos = currentByte;
            while (line.back() == '\r' || line.back() == '\n') line.pop_back();
            sz = (int) line.size() + 1;
        }
        file << line << endl;

        currentByte += (int) line.size() + 1;
    }
    file.close();

    addToAvailList(pos, sz);

    updateIndexes();
}

void AppointmentManager::printInfo(int id) {
    if (!isAppointmentPresent(to_string(id))) return void(cout << "Appointment doesn't exist\n");

    vector<string> data = loadData();
    for (auto &s: data) {
        vector<string> cur = split(s, '|');
        if (cur.size() && cur[0][0] != '*' && cur[1] == to_string(id)) {
            cout << "Appointment Id:" << cur[1] << endl;
            cout << "Appointment Date:" << cur[2] << endl;
            cout << "Doctor Id:" << cur[3] << endl;
            return;
        }
    }
}
