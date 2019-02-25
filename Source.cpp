#include <iostream>
#include <cstdio>

using namespace std;

int main(void) {

	// Total Number of Parking Spot

	int num_parking_spot = 10;

	char ent_ext_cls;

	cout << "Enter A for Car Entrance, E for Exit or Z for Closing Program: ";

	/*	Enter or Press 'A' for Car Entry
	Enter or Press 'E' for Car Exit
	Enter or Press 'Z' for Closing Program */

	cin >> ent_ext_cls;

	ent_ext_cls = toupper(ent_ext_cls);

	char(toupper(ent_ext_cls));

	while (ent_ext_cls != 'Z') {
		
		if ((ent_ext_cls == 'A') && (num_parking_spot > 0)) {

			num_parking_spot--;

			cout << endl << "Current Parking Available: " << num_parking_spot << endl;

		}
		else if ((ent_ext_cls == 'E') && (num_parking_spot < 10) && (num_parking_spot >= 0)) {

			num_parking_spot++;

			cout << endl << "Current Parking Available: " << num_parking_spot << endl;

		}
		else if ((ent_ext_cls == 'A') && (num_parking_spot == 0)) {

			cout << "Parking Full" << endl;

		}
		else if ((ent_ext_cls == 'E') && (num_parking_spot == 10)) {

			cout << "Parking Empty" << endl;

		}

		cout << "Awaiting User Feedback...";

		cin >> ent_ext_cls;

		ent_ext_cls = toupper(ent_ext_cls);

		
	} // end while

	cout << endl << " <<< Application Closed >>> " << endl;

	return 0;
}

/*
Revision History:
	First Pass: 02/14/2019 - Initial Release

*/