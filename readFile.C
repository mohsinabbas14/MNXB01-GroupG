#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// Authors: Frank Johansson & Mailk Mohsin 2018-10-30

// Reads Csv file(stored in its_filePath) and filters the data.

void readFile(string filePath, string begin, string end);

void readFile(string filePath, string begin, string end) // begin and end marks interval on the form yyyy(required)-mm(optional)-dd(optional) - yyyy(required, not including end year)-mm(optional)-dd(optional)
{	
	string helpString; // help variable
	
	// open input and output file
	ifstream infile(filePath.c_str());
	string fileHelpString = filePath.substr(filePath.find('_', 0)+1, filePath.find('.', 0)-4);
	fileHelpString.erase(fileHelpString.find("csv"));
	fileHelpString = string("mTr" + fileHelpString + "dat");
	ofstream outfile(fileHelpString.c_str());
		
	// read infile and skip the first few useless lines		
	while(getline(infile,helpString)) {
		
		if (helpString == "Datum;Tid (UTC);Lufttemperatur;Kvalitet;;Tidsutsnitt:") {
					break;
		}	
	}
	bool isInInterval = false;
	// read and store the data in output file only of good data quality (i,e "G" values)
	while(getline(infile,helpString)) {
		
		if(helpString.find(begin) != string::npos) {
			isInInterval = true;
		}
		if(helpString.find(end) != string::npos) {
			break;
		}
		if (isInInterval) {
			int i = 0;
			for(unsigned int j = 0; j < helpString.size(); j++) {
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
}
