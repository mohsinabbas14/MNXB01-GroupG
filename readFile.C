/*
 *     Authors: Frank Johansson & Mohsin Abbas 2018-11-02
 *     Email:   nat13fjo@student.lu.se
 *
 * Description: 
 *		Reads a csv file and writes the useful data to a new file
 * 		with the name "xxr[City].dat"
 *
 * Methods:  
 *
 *     * readFile(string filePath, string begin, string end)
 *       - Description:
 *         * Reads the csv file, filters the data and writes the useful data to a new file
 * 			 with the name "xxr[City].dat"
 *       - Arguments needed: 
 *          * string containing the full path of the csv files that is to be read
 * 			* string containing the first date that is to be read on the form yyyy-mm-dd
 *			* string containing the first date that is to NOT be read on the form yyyy-mm-dd
 *       - Returns: 
 *         * void
 *		
 *
 */
 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void readFile(string filePath, string begin, string end);

void readFile(string filePath, string begin, string end) {
		
	string helpString; // help variable
	
	// open input and output file
	ifstream infile(filePath.c_str());
	string fileHelpString = filePath.substr(filePath.find('_', 0)+1, filePath.find('.', 0)-filePath.find('_', 0));

	cout << fileHelpString << endl;
	fileHelpString = string("xxr" + fileHelpString + "dat");
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
			string helpString1 = helpString.substr(0, 10);
			string helpString2 = helpString.substr(helpString.find(" ", 0), 8);
			string helpString3 = helpString.substr(helpString.find(" ", 19), helpString.find(" G", 20) - helpString3.find(" ", 19));
			//cout << helpString1 << " " << helpString2 << " " << helpString3 << " " << endl;
		
			outfile << helpString1 << " " << helpString2 << " " << helpString3 << endl;
		}	
	}
}
