/*
 *     Authors: Frank Johansson & Mohsin Abbas 2018-11-02
 *     Email:   nat13fjo@student.lu.se, ma0381ab-s@student.lu.se
 *
 * Description: 
 *		Plots the  different analysis files using root.
 *
 * Methods:  
 *
 *     * plot(string filePath)
 *       - Description:
 *         * Reads the analysis file and plots the data in specific way according to the file name.
 * 			 The filename should have the form "xxA[City].dat" where xx is mT, hL or fT.
 *       - Arguments needed: 
 *          * string containing the full path of the analysis file that is to be read
 *       - Returns: 
 *         * void
 *		
 *
 */
 
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TCanvas.h> // canvas object
#include <TGraph.h>
#include <TLegend.h>
#include <TH3.h>

void plot(string filePath){
	
	// Makes Graph for the Mean Temperature per day over all years
	if (filePath.substr(0,2)== "mT"){
		
		ifstream inFile(filePath.c_str());
		// storing temperatures and days in two vectors	
		vector<double> temperatures;
		double temp;
		string discard;
	
		//int i = 0;
		while(inFile >> discard) {
			inFile >> temp;
			cout << temp << endl;
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
		gr->SetLineColor(kBlack);
		gr->SetLineWidth(1);
		gr->SetMarkerColor(4);
		gr->SetMarkerStyle(8);
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
		// Save the canvas as a picture
		c1->SaveAs("Mean_temperature.png");
	}
	
	
	//  1_D Histogram for a given city shows counts of coldest amd warmest days for all years.
	
	else if (filePath.substr(0,2)== "lH"){
		
		ifstream inFile(filePath.c_str());
		// storing temperatures and days in two vectors	
		double hDay;
		double lDay;
		string year;
		
		// Create Histogram
		
		TH1D* highTemp= new TH1D("highTemp", "Warmest Day;Day;Count",366,0,366);
		highTemp->Sumw2();
	
		TH1D* lowTemp= new TH1D("lowTemp", "Coldest Day;Day;Count",366,0,366);
		lowTemp->Sumw2();
	
		//int i = 0;
		while(inFile >> year) {
			inFile >> lDay;
			inFile >> hDay;
			
			// cout << year << " " << lDay << " " << hDay << endl;
			highTemp->Fill(hDay);	
			if(lDay < 183) lowTemp->Fill(lDay+183);
			else if(lDay > 183) lowTemp->Fill(lDay-183);
			
			//i++;
			//if(i > 20) break; 
		}
		
		// creating canvas
	
		TCanvas *c1 = new TCanvas("c1","Warmest and Coldest Days",200,10,1000,500);
		
		gStyle->SetOptStat(0);
		gStyle->SetOptFit(0);
		gStyle->SetOptTitle(0);
		c1->SetFillColor(16);
		c1->SetGrid();
		
		// create 1d function that we will use to fit our generated data to ensure
		// that the generation works
		TF1* fGaus = new TF1("fGaus", "gaus", 0, 366);
		fGaus->SetParameters(1, 0, 1);
		fGaus->SetLineColor(kBlack);
		
		c1->Divide(2);
		
		c1->cd(1);
		highTemp->Fit(fGaus);
				
		//cout << "Its uncertainty is " << fGaus->GetParError(1) << endl;
		
		TLegend *leg1 = new TLegend(0.65, 0.75, 0.90, 0.95, "", "NDC");
		leg1->SetFillStyle(0); //Hollow fill (transparent)
		leg1->SetBorderSize(0); //Get rid of the border
		leg1->AddEntry(highTemp, "Warmest Day", "l");
		highTemp->Draw();
		leg1->Draw(); //Legends are automatically drawn with "SAME"
		highTemp->SetLineColor(kRed);
		highTemp->GetXaxis()->CenterTitle();
		highTemp->GetYaxis()->CenterTitle();
		
		c1->cd(2);
		lowTemp->Fit(fGaus);
		TLegend *leg2 = new TLegend(0.65, 0.75, 0.90, 0.95, "", "NDC");
		leg2->SetFillStyle(0); //Hollow fill (transparent)
		leg2->SetBorderSize(0); //Get rid of the border
		leg2->AddEntry(lowTemp, "Coldest Day", "l");
		lowTemp->Draw();
		leg2->Draw(); //Legends are automatically drawn with "SAME"
		lowTemp->SetLineColor(kBlue);
		lowTemp->GetXaxis()->CenterTitle();
		lowTemp->GetYaxis()->CenterTitle();
		
		
		// Save the canvas as a picture
		c1->SaveAs("ColdWarm.png");
		
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
			//if(i > 200) break; 
		}
		
		// Create Histogram
		
		TH3D* highTempHist= new TH3D("highTemp", "High Temperature;Year;Temp °C",366,0,366,years.back()-years[0],years[0],years.back(),400,-30,40);
		highTempHist->Sumw2();
		//highTemp->SetMinimum(0);
	
		TH3D* lowTempHist= new TH3D("lowTemp", "Low Temperature;Year;Temp °C",366,0,366,years.back()-years[0],years[0],years.back(),300,-30,40);
		lowTempHist->Sumw2();
		//lowTemp->SetMinimum(0);
		
		for (unsigned int i=0; i <years.size(); i++){
			
			//cout << days[i] << " " << years[i]<< " " << highTemperatures[i] << endl;
			//cout << days[i]<< " "  << years[i]<<  " " <<lowTemperatures[i] << endl;
			
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
		
		highTempHist->Draw("C");
		lowTempHist->Draw("Csame");
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
		c1->SaveAs("Highest_and_Lowest_Temepratues.png");
	}
	
	else {
		
		cout << "Wrong File Name" << endl;
	}
	
}


