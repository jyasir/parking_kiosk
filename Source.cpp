#include <conio.h>
#include <iostream>

using namespace std;

void clrscr(void);

int main(void) {

	int arr_c[5] = {};

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

		if (ent_ext_cls == 'A') {

			int total_car = 0;

			for (int i = 0; i < 5; i++) {

				if (arr_c[i] == 0) {

					arr_c[i] = 1;

					cout << "Car is parked at spot " << i << endl;

					break;

				}

				else if (arr_c[i] == 1)	{

					total_car = total_car + 1;

				}

				if (total_car == 5) {

					cout << "Car Parking Full! " << endl;

					break;

				}

			}

		}

		// car exit

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

		for (int i = 0; i < 5; i++) {

			cout << "Parking Spot: " << arr_c[i] << endl;

		}

		cout << "Car Entry: A, Car Exit: B, Close Application: Z = ";

		cin >> ent_ext_cls;

		ent_ext_cls = toupper(ent_ext_cls);


	} // end while

	return 0;
}

void clrscr(void)
{
	system("@cls||clear");
}