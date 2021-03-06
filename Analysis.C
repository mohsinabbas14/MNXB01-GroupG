/*
 *     Creator: Sebastian Grabowski. 
 *     Email:   nat14sgr@student.lu.se
 *
 *
 * Description: 
 *     Searches for every data-file following a specific name composition, in a given 
 directory and creates files with the necessary information in order plot pre-defined
 histograms (see below): mT, fT and hT. 
 *
 *
 * Methods:  
 *
 *     * str_to_int(string str) 
 *       - Description:
 *         * Convert string to int 
 *       - Arguments needed: 
 *         * string
 *       - Returns: 
 *         * double
 * 
 *     * getNumberOfDaysInMonth(int month, int year) 
 *       - Description:
 *         * Returns number of days in the given month of that year.   
 *       - Arguments needed: 
 *         * int month number 1-12, int year number XXXX (i.e. 1995)  
 *       - Returns: 
 *         * int number of days. 
 *
 *     * dateToInts(string strToSplit)
 *       - Description:
 *         * Returns the date in a vector: {year, month, day}.   
 *       - Arguments needed: 
 *         * string with dates: year-month-day 
 *       - Returns: 
 *         * vector<int>
 *
 *     * getDayNumberInYear(vector<int> date) 
 *       - Description:
 *         * Returns number of days in of that year intil that date.   
 *       - Arguments needed: 
 *         * vector<int>, {year, month, day}
 *       - Returns: 
 *         * int number of days.  
 *
 *     * GetFirstAndLastYear(string dir, string sameJob)
 *       - Description:
 *         * Returns start year and ending year.    
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), 
 *            containing filenames of the same histogram type and 
 *       - Returns: 
 *         * vector<int> 
 *
 *     * getdir(string dir, vector<string> &files)
 *       - Description:
 *         * Adds all the file-names into the "files" vector, from the given directory path. 
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), (empty) vector<string>  
 *       - Returns: 
 *         * int: 0 if done, error if something went wrong. 
 *
 *     * calcHistmT(string dir, vector<string> &sameJob, vector< vector<double> > &newHist)
 *       - Description:
 *         * Calculate the average temperature in the entire country (all given cities) 
 *           of a day (2nd column), for each day, for every year. 
 *           Columns in file: 1:date | 2:<temperature>
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), 
 *           filename and (empty) vector< vector<double> > that will be used to fill the file.  
 *       - Returns: 
 *         * int: 0 if done, 1 if something went wrong.
 *
 *     * calcHistlH(string dir, vector<string> &sameJob, vector< vector<double> > > &newHist) 
 *       - Description:
 *         * For a given city search for the day, in each year, with the highest and 
 *           lowest temperature (mean value during the day).
 *           Columns in file: 1:year | 2:day(lowest) | 3:day(highest)
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), 
 *           filename and (empty) vector< vector<double> > that will be used to fill the file. 
 *       - Returns: 
 *         * int: 0 if done, 1 if something went wrong.
 *
 *     * calcHistfT(string dir, vector<string> &sameJob, vector< vector<double> > > &newHist) 
 *       - Description:
 *         * For a given city, find the highest and lowest temperature during the day for a given year. 
 *           Columns in file: 1:day | 2:H | 3:L | 4:year
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), 
 *           filename and (empty) vector< vector<double> > that will be used to fill the file.
 *       - Returns: 
 *         * int: 0 if done, 1 if something went wrong.
 *
 *     * analyze(int argc, char* argv[])
 *       - Description: 
 *         * Depending on the choosen histogram type (that will need different of arguments), 
 *           this method will need the directory where the data file (following a specific 
 *           template of both data and filename).   
 *       - Arguments needed: 
 *         * string: histogram type: mT, hL, fT. 
 *         * string: city. (X if you want all cities in directory, if using mT.)  
 *         * string: directory 
 *       - Returns: 
 *         * int: 0 if done, 1 if something went wrong.
 *
 *     * Analysis(int argc, char* argv[])
 *       - Description: 
 *         * Calls on analyze 
 *       - Arguments needed: 
 *         * char* argv[1]: histogram type: mT, hL, fT. 
 *         * char* argv[2]: city. (X if you want all cities in directory, if using mT.)  
 *         * char* argv[3]: directory 
 *       - Returns: 
 *         * int: 0 if done.
 * 
 *
 */

 

// Bias: The Mean temperature: Temperature is measured at a interval of 6 hours, however not all measures are included so there be some days where measure measurements are only taken at night. 

// Basic file operations, some are unnecessary (I have used then in old codes)
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>   
#include <cstdio>
#include <stdlib.h>  
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <math.h>

using namespace std;

// Convert string to int 
double str_to_int(string str) {
  stringstream ss(str);
  int num;
  ss >> num;
  return num;
}

//function will return total number of days
int getNumberOfDaysInMonth(int month, int year)
{
  // Leap year condition, if month is 2.
  if( month == 2)
    {
      if((year%400==0) || (year%4==0 && year%100!=0))	
	return 29;
      else	
	return 28;
    }
  // Months which has 31 days.
  else if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8
	  ||month == 10 || month==12)	
    return 31;
  else 		
    return 30;
} 


//function will return total number of days
int getDayNumberInYear(vector<int> date)
{
  int days = date[2]; 
  for(int imonth = 1; imonth < date[1]; ++imonth) 
    days += getNumberOfDaysInMonth(imonth, date[0]);
  return days;
} 


// Returns file names from a given directory. 
int getdir (string dir, vector<string> &files)
{
  DIR *dp;
  struct dirent *dirp;
  if((dp  = opendir(dir.c_str())) == NULL) {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
  }
  string str(".dat"); 
  while ((dirp = readdir(dp)) != NULL) {
    // Something weird happens.  This line prevents it (some if it). 
    if(string(dirp->d_name).find(str) == string::npos || string(dirp->d_name) == str) {
      continue; 
    }
    files.push_back(string(dirp->d_name));
  }
  closedir(dp);

  return 0;
}


vector<int> dateToInts(string strToSplit) {
  char delimeter = '-';
  stringstream ss(strToSplit);
  string item;
  vector<string> splittedStrings;

  // Returns the sentence between the given delimiter
  while (getline(ss, item, delimeter)) {
    splittedStrings.push_back(item);
  }
  // Return the year, month, day in int form. 
  vector<int> k; 
  for(unsigned int i = 0; i < splittedStrings.size(); ++i) 
    k.push_back(str_to_int(splittedStrings[i]));
  return k;
}


int calcHistmT(string dir, string sameJob, vector< vector<double> > &newHist) 
{
  string direction = dir + sameJob; 
  ifstream input(direction.c_str());
  string line;
  string oldDate = "";
  bool firstTime = true; 
  double meanTemp = 0; 
  int nrOfMeasurements = 0;  

  // Goes through the entire file 
  while (getline(input, line)) 
    {
      // Converts line to 3. 
      std::istringstream iss(line);
      string date, time;
      double temp;
      iss >> date >> time >> temp;	 
      vector<int> yearMonthDay = dateToInts(date);

      if(date.compare(oldDate) != 0) { // A new day.
	if(!firstTime) { // One day worth of info has not been recorded get.
	  newHist[0].push_back(dateToInts(oldDate).at(0));
	  newHist[1].push_back(dateToInts(oldDate).at(1));
	  newHist[2].push_back(dateToInts(oldDate).at(2));	
	  newHist[3].push_back(meanTemp/nrOfMeasurements);
	}
	// Reset new day values. 
	oldDate = date; 
	meanTemp = temp; 
	nrOfMeasurements = 1;  
	firstTime = false; 
      } else {
	meanTemp += temp;
	nrOfMeasurements += 1;
      }
    } // Entire file

  // Deleting file. 
  if( remove(direction.c_str()) != 0) { 
  cout << "Error deleting file" << endl;
  return 1;
  } else {
    cout << "File successfully deleted" << sameJob << endl;
  } 

  return 0; 
}


int calcHistlH(string dir, string sameJob, vector< vector<double> > &newHist) 
{ 
  string direction = dir + sameJob;  
  ifstream input(direction.c_str());
  string line;

  bool firstTime = true; 
  int dayH = 0;
  int dayL = 0;
  int tempH = -999; 
  int tempL = 999;
  double meanTemp = 0;  
  int nrOfMeasurements = 0;
  int oldYear = 0; 
  string oldDate = ""; 

  // Goes through the entire file 
  while (getline(input, line)) 
    {
      // Converts line to 3. 
      std::istringstream iss(line);
      string date, time;
      double temp;
      iss >> date >> time >> temp;	 
      vector<int> yearMonthDay = dateToInts(date); 

      if(date.compare(oldDate) != 0) { // New day.
	if(yearMonthDay[0] != oldYear) {	// Start of a new year
	  if(!firstTime) { // One year worth of info has not been recorded get.  
	    newHist[0].push_back(oldYear);
	    newHist[1].push_back(dayL);
	    newHist[2].push_back(dayH);
	  }
	  firstTime = false; 

	  // Reset new year values. 
	  dayH = 0; 
	  dayL = 0;
	  tempH = -999; 
	  tempL = 999; 
	  oldYear = yearMonthDay[0];
	} else { // The old year. 

	  // Calc the mean temp of the day before 
	  //  and search for the highest/lowest temp 
	  meanTemp = meanTemp/nrOfMeasurements;
	  if(meanTemp > tempH) {
	    tempH = meanTemp;
	    dayH = getDayNumberInYear(dateToInts(oldDate));
	  }
	  if(meanTemp < tempL){
	    tempL = meanTemp;
	    dayL = getDayNumberInYear(dateToInts(oldDate));
	  }

	}
	// Reset new day values. 
	oldDate = date; 
	meanTemp = temp; 
	nrOfMeasurements = 1; 
      } else { // The same day.
	meanTemp += temp;
	nrOfMeasurements += 1; 
      }

    } // Entire file 

  // Deleting file. 
  if( remove(direction.c_str()) != 0) {
    cout << "Error deleting file" << endl;
    return 1;
  } else {
    cout << "File successfully deleted " << sameJob << endl;
  }  
  return 0; 
}


int calcHistfT(string dir, string sameJob, vector< vector<double> > &newHist) 
{
  string direction = dir + sameJob; 
  ifstream input(direction.c_str());
  string line;
  string oldDate = "";
  bool firstTime = true; 

  double tempH = -999; 
  double tempL = 999; 
  // Goes through the entire file
  while (getline(input, line)) 
    {
      // Converts line to 3. 
      std::istringstream iss(line);
      string date, time;
      double temp;
      iss >> date >> time >> temp;	 
      vector<int> yearMonthDay = dateToInts(date);

      if(date.compare(oldDate) != 0) { // A new day.
	if(!firstTime) { // One day worth of info has not been recorded get.
	  newHist[0].push_back(getDayNumberInYear(dateToInts(oldDate)));
	  newHist[1].push_back(tempH);
	  newHist[2].push_back(tempL);
	  newHist[3].push_back(yearMonthDay.at(0));
	}

	// Reset new year values.
	tempH = -999; 
	tempL = 999; 
	oldDate = date; 
	firstTime = false; 
      } 
      // Search for highest/lowest temp.
      if(temp > tempH) 
	tempH = temp;
      if(temp < tempL)
	tempL = temp;
    } // Entire file 

  // Deleting file. 
  if( remove(direction.c_str()) != 0) {
    cout << "Error deleting file" << endl;
    return 1;
  } else {
    cout << "File successfully deleted " <<  sameJob << endl; 
  }  
  return 0; 
}

vector<int> GetFirstAndLastYear(string dir, string sameJob) 
{
  string direction = dir + sameJob; 
  ifstream input(direction.c_str());
  string line;
  bool firstTime = true; 
  vector<int> years;
  int maxYear = 0; 
  // Goes through the entire file 
  while (getline(input, line)) 
    {
      // Converts line to 3. 
      std::istringstream iss(line);
      string date, time;
      double temp;
      iss >> date >> time >> temp;	 
      vector<int> yearMonthDay = dateToInts(date);
      if(firstTime)
	years.push_back(yearMonthDay[0]);
      firstTime = false; 
      if(yearMonthDay[0] > maxYear)
	maxYear = yearMonthDay[0];
    } // Entire file
  years.push_back(maxYear);
  return years; 
}


// Status: See if compile in ROOT
// argv[1]: histogram type.
// argv[2]: city. (X if you want all cities in directory.) 
// argv[3]: directory
int analyze(string type, string givenCity, string dir) { 
  
  dir = "./" + dir + "/";
  // Returns every file in given folder. 
  vector<string> files = vector<string>(); // All files in that directory. 
  getdir(dir,files); // Give folder name, returns files names. 

  // List of Hist-types: mT, lH and fT.
  vector<string> histTypes;
  histTypes.push_back("mT");
  histTypes.push_back("lH");
  histTypes.push_back("fT");

  // Saves all the datafiles with the given histogram type.
  vector<string> sameJobs; // mT, lH and fT

  // Goes through "files", and returns datafiles.  
  for (unsigned int ifile = 0; ifile < files.size(); ++ifile) 
    // Find all data files as they are the only ones that begins with XXR
    if(files[ifile].at(2) == 'R') // Only use files that have been read.
      if(files[ifile].at(0) == 'X' && files[ifile].at(1) == 'X') { // Only specific datafile names. 

	// Check the city of data files.            01234567890
	string city = files.at(ifile);      // i.e. XXRCity.dat
	city = city.substr(3,city.size()-7);  // i.e. City

	// If city given, skip any another city. 
	if(givenCity != "X") 
	  if(city != givenCity) 
	    continue; 
    	
	// Add datafile-name to the sameJobs list. 
	string filename = files[ifile];
	sameJobs.push_back(files[ifile]);
      }
  
  if(sameJobs.size() == 0) { // If no data files exists. 
    cout << "No data files found" << endl;
    return 1; 
  }

  // Assign new file name. 
  string newfileName;
  string name = sameJobs[0];
  name.replace(2,1,"A"); // i.e. XXACity.dat
  string city = sameJobs[0];      // i.e. XXRCity.dat
  city = city.substr(3,city.size()-7);  // i.e. City
  name.replace(0,2,type); // i.e. mTASweden.dat
  if(type.at(0) == 'm') 
    name.replace(3,city.size(),"Sweden"); // i.e. mTASweden.dat

  // Create file
  newfileName = dir + name;
  ofstream ofsNewHist(newfileName.c_str());
 
  vector< vector<double> > newHist(4, vector< double >()); 
  // Calculate the mean temperaure over the entire country. 
  vector<int> years = GetFirstAndLastYear(dir, sameJobs[0]);
  int nrOfYear = years.at(1)-years.at(0)+1;
  // Initialize 3D vector with 0, [year,month,day]
  vector< vector< vector<double> > > totalsum(nrOfYear, vector<vector<double> >(12, vector< double >(31,0)));
  vector< vector< vector<double> > > numberofCities(nrOfYear, vector<vector<double> >(12, vector< double >(31,0)));

  switch(type.at(0)) {
  case 'm' : 

    for(unsigned int icity = 0; icity < sameJobs.size(); ++icity) {
      // Calc for the city.
      calcHistmT(dir, sameJobs[icity], newHist);
      // The dates of measurement in a city
      for(unsigned int k = 0; k < newHist[0].size(); ++k) {
	totalsum[newHist[0][k]-years.at(0)][newHist[1][k]-1][newHist[2][k]-1] += newHist[3][k];
	numberofCities[newHist[0][k]-years.at(0)][newHist[1][k]-1][newHist[2][k]-1] += 1;
      } // Going through "newHist"
    } // All cities have been done for. 

    for(unsigned int y = 0; y < nrOfYear; ++y) 
      for(unsigned int m = 0; m < 12; ++m) 
	for(unsigned int d = 0; d < 31; ++d) 
	  if(numberofCities[y][m][d] != 0) 
	    ofsNewHist <<y+years.at(0)<<"-"<<m+1<<"-"<<d+1<<"  "<<totalsum[y][m][d]/numberofCities[y][m][d]<< endl;
    cout << "Done Calculating data for mT histogram" << endl; 

    break;
  case 'l' :
    cout << "Calculating data for lH histogram" << endl; 
    calcHistlH(dir, sameJobs[0], newHist); 
    for(unsigned int k = 0; k < newHist[0].size(); ++k) { 
      ofsNewHist<< newHist[0][k]<<"  "<< newHist[1][k]<<"  "
		<< newHist[2][k]<< endl;
    } // All subtitles
    cout << "Done Calculating data for mT histogram" << endl;
    break;

  case 'f' :
    cout << "Calculating data for fT histogram" << endl; 
    calcHistfT(dir, sameJobs[0], newHist); 
    for(unsigned int k = 0; k < newHist[0].size(); ++k) { 
      ofsNewHist<< newHist[0][k]<<"  "<< newHist[1][k]<<"  "
		<< newHist[2][k]<<"  "<< newHist[3][k]<< endl;
    } // All subtitles
    cout << "Done Calculating data for mT histogram" << endl;
    break;

  default :
    cout << "Incorrect histogram-type typed" << endl;
  }
	
  return 0; 
}


int uppsala(string dir) { 
  string direction = dir + "XXRuppsala.dat"; 
  ifstream input(direction.c_str());
  string line;

  // Create file
  string newfileName = dir + "XXRuppsala2.dat";
  ofstream ofsNewHist(newfileName.c_str());

  // Goes through the entire file
  while (getline(input, line)) 
    {
      // Converts line to 3. 
      std::istringstream iss(line);
      double year, month, day, temp, temp2, id; 
      iss >> year >> month >> day >> temp >> temp2 >> id;	 
      if(id == 1) 
	ofsNewHist<<year<<"-"<<month<<"-"<<day<<"  "<<temp<<"  "<<temp2<< endl;

    } // Entire file 
  return 0;
}


// argv[1]: type of hist 
// argv[2]: city (X if using mT, if you want all cities.) 
// argv[3]: directory. Just the directory name: <foldername>, if it exist in current directory.  
int main(int argc, char* argv[]) { 
  string type;
  string givenCity; 
  string dir = "./";  // Directory where the datafiles exist in.

  // On which prefix word should the work be done on? 
  if(argc > 3) {
    // Setting the path to the Analysis directory.  
    if (string(argv[1]) != "X")
      type = argv[1];
    if (string(argv[2]) != "X")
      givenCity = argv[2];
    if (string(argv[3]) != "X")
      dir = "./" + (string) argv[3] + "/";
  }

  if(givenCity.compare("uppsala") == 0) {
    uppsala(dir);
    analyze(type, "uppsala2", dir);
  } else
    analyze(type, givenCity, dir);

  return 0; 
}
