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
#include <TH3.h>

// Makes Graph for the Mean Temperature per day over all years
	// Helpfunction

double Gaussian(double* x, double* par) {
	return par[0]*exp(-0.5*(x[0]*x[0] - 2*x[0]*par[1] + par[1]*par[1])/(par[2]*par[2]));
}


void plot(string filePath){
	
	if (filePath.substr(0,2)== "mT"){
		
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
	
		TCanvas *c1 = new TCanvas("c1","Mean Temperatures",200,10,1000,500);

		c1->SetFillColor(16);
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
		gr->GetYaxis()->SetTitle("Temperatures (degrees Celsius)");
		gr->GetXaxis()->CenterTitle();
		gr->GetYaxis()->CenterTitle();
		gr->Draw("ACP");
		gStyle->SetOptTitle(0);
		TLegend *leg = new TLegend(0.75, 0.75, 0.95, 0.95, "", "NDC");
		leg->SetFillStyle(0); //Hollow fill (transparent)
		leg->SetBorderSize(0); //Get rid of the border
		leg->AddEntry(gr, "Mean Temperature", "p"); //Use object title, draw fill
		gr->Draw("ACP");
		leg->Draw(); //Legends are automatically drawn with "SAME
	}
	
	
	//  2-D Histogram for a given city and shows day of Highest amd lowest temperatures every year.
	
	else if (filePath.substr(0,2)== "hL"){
		
		ifstream inFile(filePath.c_str());
		// storing temperatures and days in two vectors	
		double hDay;
		double lDay;
		string year;
		
		// Create Histogram
		
		TH1D* highTemp= new TH1D("highTemp", "Warmest Day;Day;Count",365,0,365);
		highTemp->Sumw2();
		//highTemp->SetMinimum(0);
	
		TH1D* lowTemp= new TH1D("lowTemp", "Coldest Day;Day;Count",365,0,365);
		lowTemp->Sumw2();
		//lowTemp->SetMinimum(0);
	
		//int i = 0;
		while(inFile >> year) {
			inFile >> hDay;
			inFile >> lDay;
			
			//cout << temp << endl;
			highTemp->Fill(hDay);
			lowTemp->Fill(lDay);
			//i++;
			//if(i > 20) break; 
		}
	
		// creating canvas
	
		TCanvas *c1 = new TCanvas("c1","Mean Temperatures",200,10,1000,500);

		c1->SetFillColor(16);
		c1->SetGrid();
		
		
		// create 1d function that we will use to fit our generated data to ensure
		// that the generation works
		TF1* fGaus = new TF1("fGaus", "gaus", -3, 3);
		fGaus->SetParameters(1, 0, 1);
		highTemp->Fit(fGaus);
		
		TF1 *g2    = new TF1("g2","gaus",0,182);
		TF1 *g3    = new TF1("g3","gaus",183,365);
		//TF1 *total = new TF1("total","gaus(0)+gaus(3)+gaus(6)",0,365);
		lowTemp->Fit(g2,"R");
		lowTemp->Fit(g3,"R+");
		
		cout << "Its uncertainty is " << fGaus->GetParError(1) << endl;
		TLegend *leg = new TLegend(0.75, 0.75, 0.95, 0.95, "", "NDC");
		leg->SetFillStyle(0); //Hollow fill (transparent)
		leg->SetBorderSize(0); //Get rid of the border
		leg->AddEntry(highTemp, "Warmest Day", "l"); //Use object title, draw fill
		leg->AddEntry(lowTemp, "Coldest Day", "l"); //Use custom title
		highTemp->Draw();
		lowTemp->Draw("SAME"); //Draw on top of the existing plot
		leg->Draw(); //Legends are automatically drawn with "SAME"
		highTemp->GetXaxis()->CenterTitle();
		highTemp->GetYaxis()->CenterTitle();
		gStyle->SetOptStat(0);
		gStyle->SetOptFit(0);
		gStyle->SetOptTitle(0);
		highTemp->SetLineColor(kRed);
		// Save the canvas as a picture
		c1->SaveAs("ColdWarm.jpg");
		
	}	
	
	
	//  3-D Histogram for a given city and shows Highest amd lowest temperatures of a day over all years
		
	else if (filePath.substr(0,2)== "fT") {
	
		ifstream inFile(filePath.c_str());
		
		// Creating vectors
		vector<double> highTemperatures;
		vector<double> lowTemperatures;
		vector<double> days;
		vector<double> years;
		
		
		// Help variables
		double highTemp;
		double lowTemp;
		double day;
		double year;

	
		//int i = 0;
		while(inFile >> day) {
			inFile >> highTemp;
			inFile >> lowTemp;
			inFile >> year;
			
			//cout << temp << endl;
			highTemperatures.push_back(highTemp);
			lowTemperatures.push_back(lowTemp);
			days.push_back(day);
			years.push_back(year);
			//i++;
			//if(i > 20) break; 
		}
		
		// Create Histogram
		
		TH3D* highTempHist= new TH3D("highTemp", "High Temperature;Year;Temp °C",365,0,365,years.size(),years[0],years[0]+years.size(),400,-30,40);
		highTempHist->Sumw2();
		//highTemp->SetMinimum(0);
	
		TH3D* lowTempHist= new TH3D("lowTemp", "Low Temperature;Year;Temp °C",365,0,365,years.size(),years[0],years[0]+years.size(),300,-30,40);
		lowTempHist->Sumw2();
		//lowTemp->SetMinimum(0);
		
		for (unsigned int i=0; i <years.size(); i++){
			
			cout << days[i] << " " << years[i]<< " " << highTemperatures[i] << endl;
			cout << days[i]<< " "  << years[i]<<  " " <<lowTemperatures[i] << endl;
			
			highTempHist->Fill(days[i],years[i],highTemperatures[i]);
			lowTempHist->Fill(days[i],years[i],lowTemperatures[i]);
			
		}
	
		// creating canvas
	
		TCanvas *c1 = new TCanvas("c1","Temperatures",200,10,1000,500);

		c1->SetFillColor(16);
		c1->SetGrid();
		highTempHist->SetMarkerColor(kRed);
		lowTempHist->SetMarkerColor(kBlue);
		highTempHist->SetMarkerStyle(20);
		lowTempHist->SetMarkerStyle(22);
		
		TLegend *leg = new TLegend(0.85, 0.85, 1, 1, "", "NDC");
		leg->SetFillStyle(0); //Hollow fill (transparent)
		leg->SetBorderSize(0); //Get rid of the border
		leg->AddEntry(highTempHist, "Highest Temp", "p"); //Use object title, draw fill
		leg->AddEntry(lowTempHist, "Lowest Temp", "p"); //Use custom title
		
		highTempHist->Draw("");
		lowTempHist->Draw("same");
		leg->Draw(); //Legends are automatically drawn with "SAME"
		highTempHist->SetTitle("Mean Temperature Per Day (all years)");
		highTempHist->GetXaxis()->SetTitle("Days");
		highTempHist->GetYaxis()->SetTitle("Years");
		highTempHist->GetXaxis()->CenterTitle();
		highTempHist->GetYaxis()->CenterTitle();
		highTempHist->GetZaxis()->SetTitle("Temperature (degrees Celsius)");
		highTempHist->GetZaxis()->CenterTitle();
		gStyle->SetOptStat(0);
		gStyle->SetOptTitle(0);
		gStyle->SetTitleSize(0.05, "x"); //Use bigger text on the axes
		gStyle->SetTitleSize(0.05, "y");
		gStyle->SetLabelSize(0.05, "x"); //Use bigger labels too
		gStyle->SetLabelSize(0.05, "y");
		gStyle->SetTitleSize(0.05, "z");
		gStyle->SetLabelSize(0.05, "z");
		gStyle->SetPadTopMargin(0.05); //Change the margins to fit our new sizes
		gStyle->SetPadRightMargin(0.05);
		gStyle->SetPadBottomMargin(0.16);
		gStyle->SetPadLeftMargin(0.16);
	
		// Save the canvas as a picture
		c1->SaveAs("Highest_and_Lowest_Temepratues.jpg");
	}
	
	else {
		
		cout << "Wrong File Name" << endl;
	}
	
}


