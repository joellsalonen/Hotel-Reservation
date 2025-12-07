// 3 pisteen hotellin lopputyö
// Joel Salonen
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <thread>
using namespace std;

// Struct stores reservation information
struct Reservation {
	int reservationNumber;
	string customerName;
	int roomNumber;
	int nights;
};

// Function prototypes
int getRandomAvailableRoom(const vector<bool>& rooms);
bool isAvailable(const vector <bool> & rooms, int roomNumber);
vector<int> findReservationsByName(const vector<Reservation>& reservations, const string& name);

// Function for making a reservation
void makeReservation(vector<bool>& rooms, vector<Reservation>& reservations, int roomCount, int pricePerNight) {
	// Check if there are free rooms
	int freeRooms = 0;
	for (bool r : rooms) {
		if (!r) freeRooms++;
	}
	if (freeRooms == 0) {
		cout << "All rooms are booked. No more reservations possible." << endl;
		return;
	}

	int chosenRoom;

	// Ask user if they want the program to choose the room
	char autoChoose;
	cout << "Let the system choose a room for you? (y/n)" << endl;
	cin >> autoChoose;

	if (autoChoose == 'y' || autoChoose == 'Y') {
		chosenRoom = getRandomAvailableRoom(rooms);
		if (chosenRoom == -1) { // All rooms booked
			cout << "No available rooms left." << endl;
			return; // Go back to do-while
		}
		cout << "Program chose room number: " << chosenRoom << endl;
	}
	else {
		// Ask user to choose a room manually
		while (true) {
			cout << "Choose a room number: (1-" << roomCount << ") ";

			if (!(cin >> chosenRoom)) {  // User entered something invalid
				cin.clear(); // Clear error flag
				cin.ignore(1000, '\n'); // Discard invalid input
				cout << "invalid input. Please enter a number." << endl;
				continue;
			}
			if (chosenRoom < 1 || chosenRoom > roomCount) {
				cout << "Number out of range. Try again." << endl;
				continue;
			}
			if (!isAvailable(rooms, chosenRoom)) {
				cout << "Room already booked. Choose another." << endl;
				continue;
			}
			break; // Valid input
		}
	}
	// Mark the chosen room reserved
	rooms[chosenRoom - 1] = true;

	// Generate reservation number between 10000 and 99999
	int reservationNumber = rand() % 90000 + 10000;

	// Ask customer's name
	string customerName;
	while (true) {
		cin.ignore(1000, '\n'); // remove leftover newline from previous input
		cout << "Enter your name: ";
		getline(cin, customerName);
		if (customerName.empty()) {
			cout << "Name cannot be empty. Try again." << endl;
			continue;
		}
		break;
	}

	// Get number of nights
	int nights;
	while (true) {
		cout << "How many nights would you like to stay? ";
		if (!(cin >> nights) || nights <= 0) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input. Please enter a positive number of nights." << endl;
			continue;
		}
		break; // Valid input
	}

	// Calculate total price
	int total = nights * pricePerNight;

	// Clear the screen before confirmation
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif

	// small delay before confirmation
	cout << "\nProcessing reservation...\n";
	this_thread::sleep_for(chrono::milliseconds(1000));

	// Print confirmation
	cout << "\n-------------------------------------------" << endl;
	cout << " " << endl;
	cout << "R E S E R V A T I O N   C O N F I R M E D" << endl;
	cout << "\n-------------------------------------------" << endl;
	cout << "Name: " << customerName << endl;
	cout << "Room number: " << chosenRoom << endl;
	cout << "Reservation number: " << reservationNumber << endl;
	cout << "Total price: " << total << " euros." << endl;
	cout << "\n-------------------------------------------" << endl;

	// Save reservation into vector
	Reservation newRes;
	newRes.reservationNumber = reservationNumber;
	newRes.customerName = customerName;
	newRes.roomNumber = chosenRoom;
	newRes.nights = nights;
	reservations.push_back(newRes);
}

// check if room is available
bool isAvailable(const vector<bool>& rooms, int roomNumber) {
	int index = roomNumber - 1;
	return !rooms[index];  // False = free, true = reserved
}

// Function returns a random available room number (1-n), or -1 if none
int getRandomAvailableRoom(const vector<bool>& rooms) {
	vector<int> freeRooms;

	// Check for all free rooms

	for (int i = 0; i < rooms.size(); i++) {
		if (!rooms[i]) { // false = free
			freeRooms.push_back(i + 1); // room number = index + 1
		}
	}
	if (freeRooms.empty()) {
		return -1; // No rooms left
	}

	// Randomly select one free room
	int randomIndex = rand() % freeRooms.size();
	return freeRooms[randomIndex];
}

// Find reservation by reservation number
int findReservationByNumber(const vector<Reservation>& reservations, int number) {
	for (int i = 0; i < reservations.size(); i++) {
		if (reservations[i].reservationNumber == number) {
			return i;
		}
	}
	return -1;
}

// Find all reservations by customer name
vector<int> findReservationsByName(const vector<Reservation>& reservations, const string& name) {
	vector<int> indices;
	for (int i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerName == name) {
			indices.push_back(i);
		}
	}
	return indices;
}

int main() {

	this_thread::sleep_for(chrono::milliseconds(300));

	cout << "\n======================================\n";
	cout << " " << endl;
	cout << "         H O T E L   M E N U       \n";
	cout << "\n======================================\n";
	cout << " " << endl;

	// Number randomizer
	srand(static_cast<unsigned int>(time(0)));


	// Randomize room availability and price
	int roomCount = rand() % 41 + 30;  // 30-70
	int pricePerNight = rand() % 21 + 80; // 80-100
	// Create a vector for the availability of rooms
	// false = free, true = reserved
	vector<bool> rooms(roomCount, false);
	vector<Reservation> reservations;

	cout << "Hotel has " << roomCount << " rooms." << endl;
	cout << "Price per night is " << pricePerNight << " euros." << endl;

	// Main loop
	bool running = true;
	while (running) {
		int choice;
		cout << "\nMenu:\n";
		cout << "1. Make a reservation\n";
		cout << "2. Find reservation by number\n";
		cout << "3. find reservation by Name\n";
		cout << "4. Show free rooms\n";
		cout << "5. Exit\n";
		cout << "Choose an option: ";
		
		if (!(cin >> choice)) { // Validate input
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input. Please enter a number for your choice.\n";
			continue;
		}

			switch (choice) {
			case 1:
				makeReservation(rooms, reservations, roomCount, pricePerNight);
				{
					// Ask user if they want to continue
					char jatko;
					cout << "\n" << endl;
					cout << "Do you want to continue to the main menu? (y/n): ";
					cin >> jatko;
					if (jatko != 'y' && jatko != 'Y') {
						running = false;
					}
				}
				break;

			case 2: {
				// Find reservation by number
				int number;
				cout << "Enter reservation number: ";
				cin >> number;
				int idx = findReservationByNumber(reservations, number);
				if (idx != -1) {
					Reservation& res = reservations[idx];
					cout << "Reservation found:\n";
					cout << "Name: " << res.customerName << endl;
					cout << "Room number: " << res.roomNumber << endl;
					cout << "Nights: " << res.nights << endl;
					cout << "Reservation number: " << res.reservationNumber << endl;
				}
				else {
					cout << "Reservation not found.\n";
				}
				break;
			}

			case 3: {
				// Find reservation by name
				cin.ignore(1000, '\n'); // deletes \n from previous
				string name;
				cout << "Enter customer name: ";
				getline(cin, name);
				vector<int> indices = findReservationsByName(reservations, name);
				if (!indices.empty()) {
					cout << indices.size() << " Reservation(s) found.\n";
					for (int i : indices) {
						Reservation& res = reservations[i];
						cout << "\n--------- RESERVATION DETAILS --------\n";
						cout << "Name: " << res.customerName << endl;
						cout << ", Room: " << res.roomNumber << endl;
						cout << ", Nights: " << res.nights << endl;
						cout	<< ", Reservation #: " << res.reservationNumber << endl;
						cout << "----------------------------------------\n";
					}
				}
				else {
					cout << "No reservations found for that name.\n";
				}
				break;
			}

			case 4: {
				// Show all free rooms
				cout << "Free rooms: " << endl;
				cout << "\n----------------------------------------" << endl;
				int count = 0;
				for (int i = 0; i < rooms.size(); i++) {
					if (!rooms[i]) {
						cout << setw(4) << (i + 1);
						count++;
						if (count % 10 == 0) {
							cout << endl;
						}
					}
				}
				
				cout << "\n----------------------------------------" << endl;
				break;
			}

				case 5:
					running = false; // Exit program
					break;

				default:
					cout << "Invalid choice. Try again. \n";
			}
	}
	return 0;
}