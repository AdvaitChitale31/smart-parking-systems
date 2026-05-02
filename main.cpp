#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
using namespace std;

// ================= VEHICLE =================
class Vehicle {
public:
    string number;
    string type;
    time_t entryTime;

    Vehicle(string num, string t) {
        number = num;
        type = t;
        entryTime = time(0);
    }
};

// ================= SLOT =================
class ParkingSlot {
public:
    string slotID;
    bool isOccupied;
    int distance;
    Vehicle* vehicle;

    ParkingSlot(string id, int dist) {
        slotID = id;
        distance = dist;
        isOccupied = false;
        vehicle = nullptr;
    }
};

// ================= SYSTEM =================
class ParkingSystem {
private:
    vector<ParkingSlot> slots;

public:
    ParkingSystem() {
        // Create slots (Zone A = nearest)
        slots.push_back(ParkingSlot("A1", 1));
        slots.push_back(ParkingSlot("A2", 1));
        slots.push_back(ParkingSlot("A3", 1));

        slots.push_back(ParkingSlot("B1", 2));
        slots.push_back(ParkingSlot("B2", 2));
        slots.push_back(ParkingSlot("B3", 2));

        slots.push_back(ParkingSlot("C1", 3));
        slots.push_back(ParkingSlot("C2", 3));
        slots.push_back(ParkingSlot("C3", 3));
    }

    // ================= SMART SLOT =================
    int calculateScore(ParkingSlot &slot, string type, bool isVIP) {
        int score = slot.distance;

        if (type == "bike" && slot.slotID[0] == 'A')
            score += 2;

        if (isVIP)
            score -= 2;

        return score;
    }

    ParkingSlot* getBestSlot(string type, bool isVIP) {
        ParkingSlot* best = nullptr;
        int bestScore = 999;

        for (auto &slot : slots) {
            if (!slot.isOccupied) {
                int score = calculateScore(slot, type, isVIP);

                if (best == nullptr || score < bestScore) {
                    best = &slot;
                    bestScore = score;
                }
            }
        }

        return best;
    }

    // ================= PARK =================
    void parkVehicle() {
        string num, type;
        char vipChoice;
        bool isVIP = false;

        cout << "Enter Vehicle Number: ";
        cin >> num;

        cout << "Enter Type (car/bike): ";
        cin >> type;

        cout << "VIP? (y/n): ";
        cin >> vipChoice;

        if (vipChoice == 'y') isVIP = true;

        ParkingSlot* slot = getBestSlot(type, isVIP);

        if (slot == nullptr) {
            cout << "Parking Full!\n";
            return;
        }

        slot->isOccupied = true;
        slot->vehicle = new Vehicle(num, type);

        cout << "✅ Smart Slot Assigned: " << slot->slotID << endl;
    }

    // ================= REMOVE =================
    void removeVehicle() {
        string num;
        cout << "Enter Vehicle Number: ";
        cin >> num;

        for (auto &slot : slots) {
            if (slot.isOccupied && slot.vehicle->number == num) {

                time_t exitTime = time(0);
                double duration = difftime(exitTime, slot.vehicle->entryTime) / 60;

                double fee = duration * 2; // ₹2 per minute

                cout << "Vehicle Found at Slot: " << slot.slotID << endl;
                cout << "Duration: " << duration << " minutes\n";
                cout << "Fee: ₹" << fee << endl;

                // Save to file
                ofstream file("parking_log.txt", ios::app);
                file << num << " " << slot.slotID << " " << fee << endl;
                file.close();

                delete slot.vehicle;
                slot.vehicle = nullptr;
                slot.isOccupied = false;

                cout << "Vehicle Removed ✅\n";
                return;
            }
        }

        cout << "Vehicle Not Found ❌\n";
    }

    // ================= DISPLAY =================
    void display() {
        cout << "\n--- Parking Status ---\n";

        for (auto &slot : slots) {
            cout << slot.slotID << " : ";
            if (slot.isOccupied)
                cout << "Occupied (" << slot.vehicle->number << ")";
            else
                cout << "Free";

            cout << endl;
        }
    }

    // ================= SEARCH =================
    void searchVehicle() {
        string num;
        cout << "Enter Vehicle Number: ";
        cin >> num;

        for (auto &slot : slots) {
            if (slot.isOccupied && slot.vehicle->number == num) {
                cout << "Found at Slot: " << slot.slotID << endl;
                return;
            }
        }

        cout << "Not Found ❌\n";
    }
};

// ================= MAIN =================
int main() {
    ParkingSystem system;
    int choice;

    while (true) {
        cout << "\n==== SMART PARKING SYSTEM ====\n";
        cout << "1. Park Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. View Parking\n";
        cout << "4. Search Vehicle\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: system.parkVehicle(); break;
            case 2: system.removeVehicle(); break;
            case 3: system.display(); break;
            case 4: system.searchVehicle(); break;
            case 5: exit(0);
            default: cout << "Invalid choice\n";
        }
    }

    return 0;
}