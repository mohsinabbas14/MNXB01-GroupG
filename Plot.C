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
		double hDay;
		double lDay;
		string year;
		
		// Create Histogram
		
		TH1D* highTemp= new TH1D("highTemp", "Warmest Day;Day;Count",365,0,365);
		//highTemp->Sumw2();
		//highTemp->SetMinimum(0);
	
		TH1D* lowTemp= new TH1D("lowTemp", "Coldest Day;Day;Count",365,0,365);
		//lowTemp->Sumw2();
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
	
		TCanvas *c1 = new TCanvas("c1","Mean Temperatures",500,20,1000,500);

		c1->SetFillColor(42);
		c1->SetGrid();
		
		// create 1d function that we will use to fit our generated data to ensure
		// that the generation works
		TF1* fitFunc = new TF1("Gaussian", Gaussian, 1, 366, 3);
		fitFunc->SetParameters(5, 200, 50);
		highTemp->Fit(fitFunc);
		lowTemp->Fit(fitFunc);
		
		cout << "Its uncertainty is " << func->GetParError(1) << endl;
		TLegend *leg = new TLegend(0.65, 0.75, 0.92, 0.92, "", "NDC");
		leg->SetFillStyle(0); //Hollow fill (transparent)
		leg->SetBorderSize(0); //Get rid of the border
		leg->AddEntry(hist, "", "F"); //Use object title, draw fill
		leg->AddEntry(anotherHist, "A title", "F"); //Use custom title
		highTemp->Draw();
		lowTemp->Draw("SAME"); //Draw on top of the existing plot
		leg->Draw(); //Legends are automatically drawn with "SAME"
  
		// Save the canvas as a picture
		c1->SaveAs("highLow.jpg");
		
	}	
		
	else if (filePath.substr(0,1)== "fT") {
	
		ifstream inFile(filePath.c_str());
		// storing temperatures and days in two vectors	
		double highTemperatures;
		double lowTemperatures;
		double day;
		string year;
		
		// Create Histogram
		
		TH2D* highTemp= new TH2D("highTemp", "High Temperature;Year;Temp °C",years.size(),years[0],years[0]+years.size(),1,0,40);
		//highTemp->Sumw2();
		//highTemp->SetMinimum(0);
	
		TH2D* lowTemp= new TH2D("lowTemp", "Low Temperatures;Year;Temp °C",years.size(),years[0],years[0]+years.size(),1,-30,0);
		//lowTemp->Sumw2();
		//lowTemp->SetMinimum(0);
	
		//int i = 0;
		while(inFile >> year) {
			inFile >> day;
			inFile >> temp;
			
			//cout << temp << endl;
			temperatures.push_back(temp);
			days.push_back(lTemp);
			double helpYear=atof(year.substr(0,4).c_str());
			years.push_back(helpYear);
			//i++;
			//if(i > 20) break; 
		}
		
		
		
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

// Helpfunction
double Gaussian(double* x, double* par) {
	return par[0]*exp(-0.5*(x[0]*x[0] - 2*x[0]*par[1] + par[1]*par[1])/(par[2]*par[2]));
}

