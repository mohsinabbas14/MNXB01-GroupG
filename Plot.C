#include <iostream>
#include <vector>
#include <string>
#include <fstream>
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
#include <TH2.h>

// Makes Graph for the Mean Temperature per day over all years

void plot(string filePath){
	
	if (filePath.substr(0,1)== "mT"){
		
		ifstream inFile(filePath.c_str());
		// storing temperatures and days in two vectors	
		vector<double> temperatures;
		double temp;
		string discard;
	
		//int i = 0;
		while(inFile >> discard) {
			inFile >> temp;
			//cout << temp << endl;
			temperatures.push_back(temp);
			//i++;
			//if(i > 20) break; 
		}
		
		vector<double> days;
		// cout << temperatures.size() << endl;
		for(size_t i = 0 ; i < temperatures.size(); i++) {
			days.push_back(i);
		}
		
		//  cout << days.size() << endl;
	
		// creating canvas
	
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
		gr->GetYaxis()->SetTitle("Temeratures °C");
		gr->GetXaxis()->CenterTitle();
		gr->GetYaxis()->CenterTitle();
		gr->Draw("ACP");
	}
	
	else if (filePath.substr(0,1)== "hL"){
		
		ifstream inFile(filePath.c_str());
		// storing temperatures and days in two vectors	
		vector<double> highTemperatures;
		vector<double> lowTemperatures;
		vector<double> years;
		double hTemp;
		double lTemp;
		string year;
		
	
		//int i = 0;
		while(inFile >> year) {
			inFile >> hTemp;
			inFile >> lTemp;
			
			//cout << temp << endl;
			highTemperatures.push_back(hTemp);
			lowTemperatures.push_back(lTemp);
			double helpYear=atof(year.substr(0,4).c_str());
			years.push_back(helpYear);
			//i++;
			//if(i > 20) break; 
		}
		
		// Create Histogram
		
		TH2D* highTemp= new TH2D("highTemp", "High Temperature;Year;Temp °C",years.size(),years[0],years[0]+years.size(),1,0,40);
		highTemp->Sumw2();
		//highTemp->SetMinimum(0);
	
		TH2D* lowTemp= new TH2D("lowTemp", "Low Temperatures;Year;Temp °C",years.size(),years[0],years[0]+years.size(),1,-30,0);
		lowTemp->Sumw2();
		//lowTemp->SetMinimum(0);
		
		for (unsigned int i=0; i <years.size(); i++){
			
			highTemp->Fill(highTemperatures[i]);
			lowTemp->Fill(lowTemperatures[i]);
			
		}
	
		// creating canvas
	
		TCanvas *c1 = new TCanvas("c1","Mean Temperatures",500,20,1000,500);

		c1->SetFillColor(42);
		c1->SetGrid();
    
		highTemp->Draw();
		lowTemp->Draw("same");
   
		
	}	
		
		
		
}

