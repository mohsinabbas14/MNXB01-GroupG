// outfile start with mT
// Authors: Frank Johansson & Mailk Mohsin 2018-10-30
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void readFile()
{	
	string helpString; // help variable
	
	// open input file
	ifstream infile("smhi-opendata_Lund.csv");
	ofstream outfile("mT_Lund.dat");		
	while(getline(infile,helpString)) {
		
		if (helpString == "Datum;Tid (UTC);Lufttemperatur;Kvalitet;;Tidsutsnitt:") {
					break;
		}	
	}
	
	while(getline(infile,helpString)) {
		Int_t i = 0;
		for( Int_t j = 0; j < helpString.size(); j++) {
			if( helpString.at(j) == ';') {
				helpString.at(j) = ' ';
				i++;
			}
			if(i > 3) break; 
		}
		
		helpString = helpString.substr(0, helpString.find(";", 0));
		
		if(helpString.find('Y') != string::npos) {
			continue;
		}
		string helpString1 = helpString.substr(0, helpString.find(" ", 0));
		string helpString2 = helpString.substr(helpString.find(" ", 1), helpString.find(" ", 1));
		string helpString3 = helpString.substr(helpString.find(" ", 11), size_t(12));
		helpString3.erase(helpString3.find(" G"));
		//cout << helpString1 << " " << helpString2 << " " << helpString3 << " " << endl;
		
		outfile << helpString1 << " " << helpString2 << " " << helpString3 << endl;
	}
}
