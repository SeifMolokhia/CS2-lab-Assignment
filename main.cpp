//
//  main.cpp
//  Assignment 1 cs2 Lab
//
//  Created by Seif on 20/11/2024.
//
 
#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct timee {
    int hour;
    int minutes;

    int toMinutes() const {
        return hour * 60 + minutes;
    }

    void printTime() const {
        if (hour < 10) {
            cout << "0";
        }
        cout << hour << ":";
        if (minutes < 10) {
            cout << "0";
        }
        cout << minutes;
    }
};

class Patient {
    string id;
    char gender;
    string type;
    timee Time;

public:
    Patient(string i, char g, string t, int h, int m) : id(i), gender(g), type(t), Time{h, m} {}

    string getID() const {
        return id;
    }

    string getType() const {
        return type;
    }

    int getArrivalTimeInMinutes() const {
        return Time.toMinutes();
    }

    void printDetails() const {
        cout << "ID: " << id << ", Gender: " << gender << ", Arrival Time: ";
        Time.printTime();
        cout << ", Type: " << type << endl;
    }
};

class QueueSys {
    queue<Patient> urgentQueue;
    queue<Patient> normalQueue;
    vector<Patient> patientList;
    vector<string> donePatients;
    Patient* inService = nullptr;

public:
    void generatePatients(int count) {
        for (int i = 0; i < count; i++) {
            string id = "2850" + to_string(rand() % 90000000 + 10000000);
            char gender = (rand() % 2 == 0) ? 'M' : 'F';
            string type = (rand() % 2 == 0) ? "Urgent" : "Normal";
            int hour = rand() % 24;
            int minutes = rand() % 60;
            patientList.emplace_back(id, gender, type, hour, minutes);
        }
    }

    void transferToQueues(int currentMinutes) {
        for (auto it = patientList.begin(); it != patientList.end();) {
            if (it->getArrivalTimeInMinutes() <= currentMinutes) {
                if (it->getType() == "Urgent")
                    urgentQueue.push(*it);
                else
                    normalQueue.push(*it);
                it = patientList.erase(it);
            } else {
                ++it;
            }
        }
    }

    void servePatients(int N) {
        int served = 0;
        while (served < N) {
            if (!urgentQueue.empty()) {
                if (inService) {
                    donePatients.push_back(inService->getID());
                    delete inService;
                }
                inService = new Patient(urgentQueue.front());
                urgentQueue.pop();
                served++;
            } else if (!normalQueue.empty()) {
                if (inService) {
                    donePatients.push_back(inService->getID());
                    delete inService;
                }
                inService = new Patient(normalQueue.front());
                normalQueue.pop();
                served++;
            } else {
                break;
            }
        }
        if (served == 0 && inService) {
            donePatients.push_back(inService->getID());
            delete inService;
            inService = nullptr;
        }
    }

    void displayState() const {
        cout << "Current State:  " << endl;
        cout << "Waiting Urgent Patient IDs: " << endl;
        auto tempUrgent = urgentQueue;
        while (!tempUrgent.empty()) {
            cout << tempUrgent.front().getID() << endl;
            tempUrgent.pop();
        }
        cout << "Total number of Urgent Patients is : " << urgentQueue.size() << endl;

        cout << "Waiting Normal Patient IDs: " << endl;
        auto tempNormal = normalQueue;
        while (!tempNormal.empty()) {
            cout << tempNormal.front().getID() << endl;
            tempNormal.pop();
        }
        cout << "Total number of Normal Patients is : " << normalQueue.size() << endl;

        if (inService) {
            cout << "In-Service Patient: " << endl;
            inService->printDetails();
        } else {
            cout << "In-Service Patient: None" << endl;
        }

        cout << "Done Patient IDs: " << endl;
        for (const string& id : donePatients) {
            cout << id << endl;
        }
        cout << "Total number of Done Patients is : " << donePatients.size() << endl;
    }
};

int main() {
    srand(time(0));
    QueueSys queueSystem;
    char scenario;
    cout << "Choose a scenario (S: Simple, M: Moderate, C: Crowded): ";
    cin >> scenario;
    int patientCount;
    switch (scenario) {
        case 'S':
        case 's':
            patientCount = 100;
            break;
        case 'M':
        case 'm':
            patientCount = 300;
            break;
        case 'C':
        case 'c':
            patientCount = 700;
            break;
        default:
            cout << "Invalid scenario selected. Defaulting to Simple (100 patients)." << endl;
            patientCount = 100;
    }

    queueSystem.generatePatients(patientCount);
    cout << patientCount << " patients generated." << endl;

    int currentMinutes = 8 * 60; // Start at 8:00 AM
    cin.ignore();

    while (currentMinutes < 24 * 60) {
        cout << "Current Time: ";
        timee{currentMinutes / 60, currentMinutes % 60}.printTime();
        cout << endl;
        queueSystem.transferToQueues(currentMinutes);

        int N = rand() % 6 + 5; // Random number between 5 and 10
        queueSystem.servePatients(N);
        queueSystem.displayState();

        cout << "Press Enter to advance 1 minute...";
        cin.get();
        currentMinutes++;
    }
    cout << "Simulation ended at midnight." << endl;
    return 0;
}

