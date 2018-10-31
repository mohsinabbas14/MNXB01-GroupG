#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

// Authors: Frank Johansson & Mailk Mohsin 2018-10-30
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class tempTrender {
	public:
	tempTrender(std::string filePath); //Construct using the specified file
	~tempTrender() {} //Destructor
	
	void readFile();
	void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	
	vector<double> vectorFunction();
	
	//void tempOnDay(int monthToCalculate, int dayToCalculate); //Make a histogram of the temperature on this day
	//void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
	
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	protected:
	string its_filePath;
	
};

#endif

