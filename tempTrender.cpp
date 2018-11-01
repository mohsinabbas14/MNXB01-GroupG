#include <iostream>
#include <vector>
#include "tempTrender.h"
// Authors: Frank Johansson & Mailk Mohsin 2018-10-30
// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <TRandom.h>
#include <TGraph.h>
#include <TLegend.h>

tempTrender::tempTrender(string filePath) {
	its_filePath = filePath;
	cout << "The user supplied " << filePath << " as the path to the data file." << endl;
	cout << "You should probably store this information in a member variable of the class. Good luck with the project! :)" << endl;
}

// Reads Csv file(stored in its_filePath) and filters the data.

void tempTrender::readFile()
{	
	string helpString; // help variable
	
	// open input and output file
	ifstream infile(its_filePath.c_str());
	string fileHelpString = its_filePath.substr(its_filePath.find("_", 0), its_filePath.find(".", 0));
	fileHelpString.erase(fileHelpString.find("csv"));
	fileHelpString = string("mT" + fileHelpString + "dat");
	ofstream outfile(fileHelpString.c_str());
	
	// read infile and skip the first few useless lines		
	while(getline(infile,helpString)) {
		
		if (helpString == "Datum;Tid (UTC);Lufttemperatur;Kvalitet;;Tidsutsnitt:") {
					break;
		}	
	}
	
	// read and store the data in output file only of good data quality (i,e "G" values)
	
	while(getline(infile,helpString)) {
		int i = 0;
		for( int j = 0; j < helpString.size(); j++) {
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


// Makes Graph for the Mean Temperature per day over all years

void tempTrender::tempPerDay() {
	
	// storing temperatures and days in two vectors
	
	vector<double> temperatures = vectorFunction();
	vector<double> days;
	// cout << temperatures.size() << endl;
	for(size_t i = 0 ; i < temperatures.size(); i++) {
		days.push_back(i);
	}
	cout << days.size() << endl;
	
	//creating canvas
	
	TCanvas *c1 = new TCanvas("c1","Mean Temperatures",500,20,1000,500);

    c1->SetFillColor(42);
    c1->SetGrid();
    
    double* daysA = &days[0];
    double* temperaturesA = &temperatures[0];
   
   // Plotting the days and temperature
   
    TGraph *gr = new TGraph(int(temperatures.size()),daysA,temperaturesA);
    gr->SetLineColor(2);
    gr->SetLineWidth(4);
    gr->SetMarkerColor(4);
    gr->SetMarkerStyle(21);
    gr->SetTitle("Mean Temperature Per Day (all years)");
    gr->GetXaxis()->SetTitle("Days");
    gr->GetYaxis()->SetTitle("Temeratures");
    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->CenterTitle();
    gr->Draw("ACP");
}

vector<double> tempTrender::vectorFunction() {
	readFile();
	vector<double> temperatures;
	double temp;
	string discard;
	ifstream infile("/home/courseuser/FrankJohanssonProject/MNXB01-project/code/mT_Lund.dat");
	int i = 0;
	while(infile >> discard) {
		
		
		
		infile >> discard;
		infile >> temp;
		//cout << temp << endl;
		temperatures.push_back(temp);
		
		i++;
		
		//if(i > 20) break; 
	}
	return temperatures;
}
