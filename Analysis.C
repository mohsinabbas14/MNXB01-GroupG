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
 *     * int_to_str(Int_t num)
 *       - Description:
 *         * Convert a Int_t to a string 
 *       - Arguments needed: 
 *         * integer
 *       - Returns: 
 *         * string
 *
 *     * str_to_int(string str) 
 *       - Description:
 *         * Convert string to Int_t 
 *       - Arguments needed: 
 *         * integer
 *       - Returns: 
 *         * string
 *
 *     * getNumberOfDaysInMonth(Int_t month, Int_t year) 
 *       - Description:
 *         * Returns number of days in the given month of that year.   
 *       - Arguments needed: 
 *         * Int_t month number 1-12, Int_t year number XXXX (i.e. 1995)  
 *       - Returns: 
 *         * Int_t number of days. 
 *
 *     * getdir(string dir, vector<string> &files)
 *       - Description:
 *         * Adds all the file-names into the "files" vector, from the given directory path. 
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), (empty) vector<string>  
 *       - Returns: 
 *         * Int_t: 0 if done, 1 if something went wrong. 
 *
 *     * calcHistmT(string dir, vector<string> &sameJob, vector< vector<double> > &newHist)
 *       - Description:
 *         * Calculate the average temperature of a day (2nd column), for each day, for every year. 
             Columns in file: 1:date | 2:<temperature>
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), 
             vector<string> containing filenames of the same histogram type and 
	     (empty) vector< vector<double> > that will be used to fill the file.  
 *       - Returns: 
 *         * Int_t: 0 if done, 1 if something went wrong.
 *
 *     * calcHistlH(string dir, vector<string> &sameJob, vector< vector<double> > > &newHist) 
 *       - Description:
 *         * For a given city search for the day, in each year, with the highest and 
             lowest temperature (mean value during the day).
             Columns in file: 1:year | 2:day(lowest) | 3:day(highest)
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), 
             vector<string> containing filenames of the same histogram type and 
	     (empty) vector< vector<double> > that will be used to fill the file. 
 *       - Returns: 
 *         * Int_t: 0 if done, 1 if something went wrong.
 *
 *     * calcHistfT(string dir, vector<string> &sameJob, vector< vector<double> > > &newHist) 
 *       - Description:
 *         * For a given city, find the highest and lowest temperature during the day for a given year.  
             Columns in file: 1:day | 2:H | 3:L | 4:year
 *       - Arguments needed: 
 *         * string directory path form current place (i.e. here/til/destination), 
             vector<string> containing filenames of the same histogram type and 
	     (empty) vector< vector<double> > that will be used to fill the file.
 *       - Returns: 
 *         * Int_t: 0 if done, 1 if something went wrong.
 *
 *     * Analysis(Int_t argc, char* argv[])
 *       - Description: 
 *         * Depending on the choosen histogram type (that will need different of arguments), 
             this method will need the directory where the data file (following a specific 
	     template of both data and filename.   
 *       - Arguments needed: 
 *         * char* argv[1]: histogram type: mT, hL, fT. 
 *         * char* argv[2]: city. (X for no).  
 *         * char* argv[3]: directory 
 *       - Returns: 
 *         * Int_t: 0 if done, 1 if something went wrong.
 * 
 *
 */

// basic file operations
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
#include <map>
#include <algorithm>
#include <math.h>

// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TH2.h> // 2d histogram classes
#include <TRandom.h> // random generators
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object


using namespace std;



// Status: See if compile in ROOT
// Convert Int_t to string 
string int_to_str(Int_t num) {
  stringstream ss;
  ss << num;
  return ss.str();
}

// Status: See if compile in ROOT
// Convert Int_t to string 
string duo_to_str(Double_t num) {
  stringstream ss;
  ss << num;
  return ss.str();
}


// Status: See if compile in ROOT
// Convert string to Int_t 
Double_t str_to_int(string str) {
  stringstream ss(str);
  Int_t num;
  ss >> num;
  return num;
}

// Status: See if compile in ROOT
//function will return total number of days
Int_t getNumberOfDaysInMonth(Int_t month, Int_t year)
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

// Status: See if compile in ROOT
//function will return total number of days
Int_t getDayNumberInYear(vector<Int_t> date)
{
  Int_t days = date[2]; 
  for(int imonth = 1; imonth <= date[1]; ++imonth) 
    days += getNumberOfDaysInMonth(imonth, date[0]);
  return days;
} 


// Status: See if compile in ROOT
// Returns file names from a given directory. 
Int_t getdir (string dir, vector<string> &files)
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
    if(string(dirp->d_name).find(str) == string::npos || string(dirp->d_name) == str) 
      continue; 
    files.push_back(string(dirp->d_name));
  }
  closedir(dp);
  return 0;
}


vector<Int_t> dateToInts(string strToSplit) {
  char delimeter = '-';
  stringstream ss(strToSplit);
  string item;
  vector<string> splittedStrings;
  while (getline(ss, item, delimeter)) {
      splittedStrings.push_back(item);
    }
  vector<Int_t> k; 
  for(unsigned int i = 0; i < splittedStrings.size(); ++i) 
    k.push_back(str_to_int(splittedStrings[i]));
  return k;
}


// Status: Not done
// This one: Merges the entire  file in one goes, but needs a cubic vector. 
// newHist[i][j][k]; i: Title in file, j: x[] y[] sumOfWeights[], k: 
Int_t calcHistmT(string dir, vector<string> &sameJob, vector< vector<Double_t> > &newHist) 
{
  if(sameJob.size() != 1)
    return 1; 

  // For x-type files for sameJob: // [i][k], where k is for x[] 
  Double_t meanTemp; 
  Int_t nrOfDays;  

  for(unsigned int iJob = 0; iJob < sameJob.size(); ++iJob ) { // Every File

    //   cout << "sameJob " << iJob << ": " << sameJob[iJob] << endl; 
    string direction = dir + sameJob[iJob]; 
    ifstream input(direction.c_str());
    string line;
    string oldDate = "";

    // Goes through the entire file 
    while (getline(input, line)) 
      {
	// Converts line to 3. 
	std::istringstream iss(line);
	string date, time;
	Double_t temp;
	iss >> date >> time >> temp;	 

	if(date.compare(oldDate) != 0) {
	  newHist[0].push_back(date);
	  newHist[1].push_back(meanTemp/nrOfDays);
	  meanTemp = temp;
	  nrOfDays = 1;	
	  oldDate = date; 
	} else {
	  meanTemp += temp; 
	  nrOfDays += 1; 
	}		
      } // Entire file 

    // Deleting file. 
    if( remove(direction.c_str()) != 0) 
      cout << "Error deleting file" << endl;
    else {
      puts("File successfully deleted"); 
      cout << sameJob[iJob] << endl; ;
      sameJob.erase(sameJob.begin() + iJob); 
    }  
  }
  return 0; 
}


// Status: Not done
// This one: Merges the entire file in one goes, but needs a cubic vector. 
// newHist[i][j][k]; i: Title in file, j: x[] y[] sumOfWeights[], k: 
Int_t calcHistlH(string dir, vector<string> &sameJob, vector< vector< vector<Double_t> > > &newHist) 
{
  if(sameJob.size() != 1)
    return 1; 

  // For x-type files for sameJob: // [i][k], where k is for x[] 
  Double_t meanTemp; 
  Int_t nrOfDays;  

  //   cout << "sameJob " << iJob << ": " << sameJob[iJob] << endl; 
  string direction = dir + sameJob[0]; 
  ifstream input(direction.c_str());
  string line;
  string oldDate = "";

  int OldYear = 0; 
  int dayH, dayL = 0; 
  int tempH = -999; 
  int tempL = 999; 
  // Goes through the entire file 
  while (getline(input, line)) 
    {
      // Converts line to 3. 
      std::istringstream iss(line);
      string date, time;
      Double_t temp;
      iss >> date >> time >> temp;	 
      vector<Int_t> yearMonthDay = dateToInts(date);

      if(OldYear != yearMonthDay[0]) { // New year. 
	double x = yearMonthDay.at(0);
	newHist[0].push_back(x);
	newHist[1].push_back(dayL);
	newHist[2].push_back(dayH);
	dayH = 0; 
	dayL = 0;
	tempH = -999; 
	tempL = 999; 
	OldYear = yearMonthDay[0];
      }

      if(date.compare(oldDate) != 0) { // New day.
	if(meanTemp/nrOfDays > tempH) {
	  tempH = meanTemp/nrOfDays;
	  dayH = getDayNumberInYear(dateToInts(oldDate));
	}
	if(meanTemp/nrOfDays < tempL){
	  tempL = meanTemp/nrOfDays;
	  dayL = getDayNumberInYear(dateToInts(oldDate));
	}
	meanTemp = temp;
	nrOfDays = 1;	
	oldDate = date; 
      } else { // Old day. 
	meanTemp += temp; 
	nrOfDays += 1; 
      }		

    } // Entire file 

  // Deleting file. 
  if( remove(direction.c_str()) != 0) 
    cout << "Error deleting file" << endl;
  else {
    puts("File successfully deleted"); 
    cout << sameJob[0] << endl; ;
    sameJob.erase(sameJob.begin()); 
  }  

  return 0; 
}



// Status: Not done
// This one: Merges the entire file in one goes, but needs a cubic vector. 
// newHist[i][j][k]; i: Title in file, j: x[] y[] sumOfWeights[], k: 
Int_t calcHistfT(string dir, vector<string> &sameJob, vector< vector< vector<Double_t> > > &newHist) 
{
  if(sameJob.size() != 1)
    return 1; 

    //   cout << "sameJob " << iJob << ": " << sameJob[iJob] << endl; 
    string direction = dir + sameJob[0]; 
    ifstream input(direction.c_str());
    string line;
    string oldDate = "";

    int dayH, dayL = 0; 
    int tempH = -999; 
    int tempL = 999; 
    // Goes through the entire file 
    while (getline(input, line)) 
      {
	// Converts line to 3. 
	std::istringstream iss(line);
	string date, time;
	Double_t temp;
	iss >> date >> time >> temp;	 
	vector<Double_t> yearMonthDay = dateToInts(date);

	if(date.compare(oldDate) != 0) { // New day.
	  double y = yearMonthDay.at(2);
	  newHist[0].push_back(y);
	  newHist[1].push_back(dayL);
	  newHist[2].push_back(dayH);
	  double x = yearMonthDay.at(0);
	  newHist[3].push_back(x);
	  dayH = 0; 
	  dayL = 0;
	  tempH = -999; 
	  tempL = 999; 
	  oldDate = date; 
	} 
	if(temp > tempH) {
	  tempH = temp;
	  dayH = getDayNumberInYear(dateToInts(oldDate));
	}
	if(temp < tempL){
	  tempL = temp;
	  dayL = getDayNumberInYear(dateToInts(oldDate));
	}
		
      } // Entire file 

    // Deleting file. 
    if( remove(direction.c_str()) != 0) 
      cout << "Error deleting file" << endl;
    else {
      puts("File successfully deleted"); 
      cout << sameJob[0] << endl; ;
      sameJob.erase(sameJob.begin()); 
    }  

  return 0; 
}



// Status: See if compile in ROOT
// argv[1]: histogram type.
// argv[2]: city. (X for not needed) 
// argv[3]: directory
Int_t Analysis(char* argv[]) { 

  string type;
  string givenCity; 
  string dir = "./";  // Directory where the datafiles exist in.
  
  // Setting the path to the Analysis directory.  
  if (string(argv[1]) != "X")
    type = argv[1];
  if (string(argv[2]) != "X")
    givenCity = argv[2];
  if (string(argv[3]) != "X")
    dir = "./" + (string) argv[2] + "/";

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

  // Goes through "files", and returns datafiles (mT/lH/fT) with the given prefix.  
  for (unsigned int ifile = 0; ifile < files.size(); ++ifile) 
    // Find all data files as they are the only ones that begins with mT, lH or fT.
    if(files[ifile].at(2) == 'R') // Only use files that have been read.
      if(files[ifile].at(0) == 'X' && files[ifile].at(1) == 'X') { // Only specific datafile names. 

	// Check the city of data files.            01234567890
	string city = files.at(ifile);      // i.e. XXRCity.dat
	city = city.substr(3,city.size()-4);  // i.e. City

	// If city given, skip any another city. 
	if(givenCity != 'X') 
	  if(city != givenCity) 
	    continue; 
    	
	// Add datafile-name to the sameJobs list. 
	string filename = files.at(ifile);
	sameJobs.push_back(filename);
      }
  
    if(sameJobs.size() == 0) // If no data files exists. 
      return 1; 

    // Lets call the columns x, y, z, w  
    vector< vector<Double_t> > newHist(4, vector< Double_t >()); 

    // Assign new file new. 
    string newfileName;

    // Calculate the new hist, depending on histogram type. 
     string name = sameJobs[0].replace(2,1,"A"); // i.e. XXACity.dat
    if(sameJobs[0].at(0) == 'm') { 
      name = sameJobs[0].replace(0,2,"Sweden"); // i.e. mTASweden.dat
      calcHistmT(dir, sameJobs, newHist); 
    } else if(sameJobs[0].at(0) == 'l') {
      name = sameJobs[0].replace(0,2,type); // i.e. lHACity.dat
      calcHistlH(dir, sameJobs, newHist); 
    } else if(sameJobs[0].at(0) == 'f') {
      name = sameJobs[0].replace(0,2,type); // i.e. fTACity.dat
      calcHistfT(dir, sameJobs, newHist); 
    }
    newfileName = dir + name;

    // Create file
    ofstream ofsNewHist(newfileName.c_str());

    // Print the [column][line] data into file. 
    for(unsigned int k = 0; k < newHist[0].size(); ++k) { 
      ofsNewHist << fixed << scientific << setprecision(6); // Unnecessary?
      ofsNewHist<< newHist[0][k]<<"  "<< newHist[1][k]<<"  "
		<< newHist[2][k]<<"  "<< newHist[3][k]<< endl;
    } // All subtitles	

  return 0; 
}

