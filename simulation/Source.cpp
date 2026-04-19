#include <conio.h>
#include <iostream>

using namespace std;

void clrscr(void);

int main(void) {

	int arr_c[6] = {};

	int mode;

	// if mode is eqs to 1, ascending order
	// if mode is eqs to 2, descending order

	cout << "Please enter the Parking Mode: ";
	
	cin >> mode;

	char ent_ext_cls;

	cout << "Enter A for Car Entrance, E for Exit or Z for Closing Program: ";

	/*	Enter or Press 'A' for Car Entry
	Enter or Press 'E' for Car Exit
	Enter or Press 'Z' for Closing Program
	*/

	cin >> ent_ext_cls;

	ent_ext_cls = toupper(ent_ext_cls);

	char(toupper(ent_ext_cls));

	while (ent_ext_cls != 'Z') {

		clrscr();

		// third pass - changes start

		if ((ent_ext_cls == 'A') && (mode == 1)) {

			int total_car = 0;

			for (int i = 0; i < 6; i++) {

				if (arr_c[i] == 0) {

					arr_c[i] = 1;

					cout << "Car is parked at spot " << i << endl;

					break;

				}

				else if (arr_c[i] == 1) {

					total_car = total_car + 1;

				}

				if (total_car == 6) {

					cout << "Car Parking Full! " << endl;

					break;

				}

			}

		}

		if ((ent_ext_cls == 'A') && (mode == 2)) {
		
			int total_car = 0;

			for (int i = 6 ; i > 0 ; i--) {

				if (arr_c[i] == 0) {

					arr_c[i] = 1;

					cout << "Car is parked at spot " << i << endl;

					break;

				}

				else if (arr_c[i] == 1) {

					total_car = total_car + 1;

				}

				if (total_car == 6) {

					cout << "Car Parking Full! " << endl;

					break;

				}

			}

		}

		// third pass - changes ends
		
		// car exit - start

		if (ent_ext_cls == 'E') {

			int car_exit;

			cout << "Determine the position of the car which is leaving: ";

			cin >> car_exit;

			if (arr_c[car_exit] == 0) {

				cout << "This parking spot was vacant, enter a valid entry.";

			}

			else if (arr_c[car_exit] == 1) {

				arr_c[car_exit] = 0;

				cout << "Parking Spot " << car_exit << " now available for Parking! " << endl;

			}

		}

		// car exit - ends

		for (int i = 0; i < 6; i++) {

			if (i == 3) { cout << "Level 2" << endl; }
			if (i == 0) { cout << "Level 1" << endl; }
			cout << "Parking Spot: " << arr_c[i] << endl;

		}

		cout << "Car Entry: A, Car Exit: E, Close Application: Z = ";

		cin >> ent_ext_cls;

		ent_ext_cls = toupper(ent_ext_cls);


	} // end while

	return 0;
}

void clrscr(void)
{
	system("@cls||clear");
}

/* Revision Notes:

	Third pass 03/2019
	
*/