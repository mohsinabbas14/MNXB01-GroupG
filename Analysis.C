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
// Convert string to Int_t 
Int_t str_to_int(string str) {
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



// Status: Not done
// This one: Merges the entire (mT/lT/fT) file in one goes, but needs a cubic vector. 
// newHist[i][j][k]; i: Title in file, j: x[] y[] sumOfWeights[], k: 
Int_t calcHistmT(string dir, vector<string> &sameJob, vector< vector< vector<Double_t> > > &newHist) 
{
  if(sameJob.size() == 0)
    return 1; 

  Bool_t firstFile = true; 

  // Check if the sameJob-files are mT, lT or fT type. 
  Int_t nrOfFiles; // Number of files. 
  Int_t totalYear;
  if(sameJob[0].at(0) == 'mT') { 
    ...
  } else if(sameJob[0].at(0) == 'lT') {
    ...
  } else if(sameJob[0].at(0) == 'fT') {
    ...
  }

  // For x-type files for sameJob: // [i][k], where k is for x[] 
  vector< vector<double> > numeratorSum(nrOfFiles, vector< Double_t >()), errnumeratorSum(nrOfFiles, vector< Double_t >()), sumSumW(nrOfFiles, vector< Double_t >()); 

  for(Int_t iJob = 0; iJob < sameJob.size(); ) { // Every File
    //   cout << "sameJob " << iJob << ": " << sameJob[iJob] << endl; 
    string direction = dir + sameJob[iJob]; 
    ifstream input(direction.c_str());
    Int_t i = -1; // matrix_j. 
    string line;
    Int_t iLine = 0; // Line i:th in a file 

    for(Int_t in = 0; in < sameJob.size(); ++ in) 

      // Goes through the entire file 
      while (getline(input, line)) 
	{
	  // Check if "line" is a centrality title, if so next matrix and line.   
	  for(Int_t year = 0; year < totalYear; ++year) 
	    for(Int_t month = 0; month < 12; ++month)
	      for(Int_t day = 0; day < 31; ++day) { 
		string date = sameJob[0].substr(1,4) + "distCgen" + int_to_str(cent) + int_to_str(typeBS); 
		if(line.find(centHist) != string::npos) { // Find the search word.
		  getline(input, line); // Get first line of data after title 
		  ++i; // New matrix in newHist; 
		  iLine = 0; 	
		}
	      }  

	  // Converts line to 4 double. 
	  std::istringstream iss(line);
	  Double_t x, y, err, sumOfWeights;
	  iss >> x >> y >> err >> sumOfWeights;	    

	  if(firstFile) {
	    numeratorSum[i].push_back(y);
	    errnumeratorSum[i].push_back(err);
	    sumSumW[i].push_back(sumOfWeights); 

	    newHist[i][0].push_back(x);
	    newHist[i][1].push_back(y);
	    newHist[i][2].push_back(err);
	    newHist[i][3].push_back(sumOfWeights);
	  } else {
	    numeratorSum[i].at(iLine) += y; 
	    errnumeratorSum[i].at(iLine) += err; 
	    sumSumW[i].at(iLine) += sumOfWeights; 
	  }	
	  ++iLine;	
	} // Entire file 

    // Deleting file. 
    if( remove(direction.c_str()) != 0) 
      cout << "Error deleting file" << endl;
    else {
      puts("File successfully deleted"); 
      cout << sameJob[iJob] << endl; ;
      sameJob.erase(sameJob.begin() + iJob); 
    }  
    firstFile = false; // Now to the other files. 
  } // All files 

  // Assign the merged values 
  for( Int_t i = 0; i < newHist.size(); ++i) 
    for( Int_t k = 0; k < newHist[i][0].size(); ++k) {
      newHist[i][1][k] = numeratorSum[i][k]; 
      newHist[i][2][k] = errnumeratorSum[i][k];
      newHist[i][3][k] = sumSumW[i][k]; 
    }
  return 0; 
}



// Status: Not done
// This one: Merges the entire (mT/lT/fT) file in one goes, but needs a cubic vector. 
// newHist[i][j][k]; i: Title in file, j: x[] y[] sumOfWeights[], k: 
Int_t calcHistlh(string dir, vector<string> &sameJob, vector< vector< vector<Double_t> > > &newHist) 
{
  if(sameJob.size() == 0)
    return 1; 

  Bool_t firstFile = true; 

  // Check if the sameJob-files are mT, lT or fT type. 
  Int_t nrOfFiles; // Number of files. 
  Int_t totalYear;
  if(sameJob[0].at(0) == 'mT') { 
    ...
  } else if(sameJob[0].at(0) == 'lT') {
    ...
  } else if(sameJob[0].at(0) == 'fT') {
    ...
  }

  // For x-type files for sameJob: // [i][k], where k is for x[] 
  vector< vector<double> > numeratorSum(nrOfFiles, vector< Double_t >()), errnumeratorSum(nrOfFiles, vector< Double_t >()), sumSumW(nrOfFiles, vector< Double_t >()); 

  for(Int_t iJob = 0; iJob < sameJob.size(); ) { // Every File
    //   cout << "sameJob " << iJob << ": " << sameJob[iJob] << endl; 
    string direction = dir + sameJob[iJob]; 
    ifstream input(direction.c_str());
    Int_t i = -1; // matrix_j. 
    string line;
    Int_t iLine = 0; // Line i:th in a file 

    for(Int_t in = 0; in < sameJob.size(); ++ in) 

      // Goes through the entire file 
      while (getline(input, line)) 
	{
	  // Check if "line" is a centrality title, if so next matrix and line.   
	  for(Int_t year = 0; year < totalYear; ++year) 
	    for(Int_t month = 0; month < 12; ++month)
	      for(Int_t day = 0; day < 31; ++day) { 
		string date = sameJob[0].substr(1,4) + "distCgen" + int_to_str(cent) + int_to_str(typeBS); 
		if(line.find(centHist) != string::npos) { // Find the search word.
		  getline(input, line); // Get first line of data after title 
		  ++i; // New matrix in newHist; 
		  iLine = 0; 	
		}
	      }  

	  // Converts line to 4 double. 
	  std::istringstream iss(line);
	  Double_t x, y, err, sumOfWeights;
	  iss >> x >> y >> err >> sumOfWeights;	    

	  if(firstFile) {
	    numeratorSum[i].push_back(y);
	    errnumeratorSum[i].push_back(err);
	    sumSumW[i].push_back(sumOfWeights); 

	    newHist[i][0].push_back(x);
	    newHist[i][1].push_back(y);
	    newHist[i][2].push_back(err);
	    newHist[i][3].push_back(sumOfWeights);
	  } else {
	    numeratorSum[i].at(iLine) += y; 
	    errnumeratorSum[i].at(iLine) += err; 
	    sumSumW[i].at(iLine) += sumOfWeights; 
	  }	
	  ++iLine;	
	} // Entire file 

    // Deleting file. 
    if( remove(direction.c_str()) != 0) 
      cout << "Error deleting file" << endl;
    else {
      puts("File successfully deleted"); 
      cout << sameJob[iJob] << endl; ;
      sameJob.erase(sameJob.begin() + iJob); 
    }  
    firstFile = false; // Now to the other files. 
  } // All files 

  // Assign the merged values 
  for( Int_t i = 0; i < newHist.size(); ++i) 
    for( Int_t k = 0; k < newHist[i][0].size(); ++k) {
      newHist[i][1][k] = numeratorSum[i][k]; 
      newHist[i][2][k] = errnumeratorSum[i][k];
      newHist[i][3][k] = sumSumW[i][k]; 
    }
  return 0; 
}



// Status: Not done
// This one: Merges the entire (mT/lT/fT) file in one goes, but needs a cubic vector. 
// newHist[i][j][k]; i: Title in file, j: x[] y[] sumOfWeights[], k: 
Int_t calcHistfT(string dir, vector<string> &sameJob, vector< vector< vector<Double_t> > > &newHist) 
{
  if(sameJob.size() == 0)
    return 1; 

  Bool_t firstFile = true; 

  // Check if the sameJob-files are mT, lT or fT type. 
  Int_t nrOfFiles; // Number of files. 
  Int_t totalYear;
  if(sameJob[0].at(0) == 'mT') { 
    ...
  } else if(sameJob[0].at(0) == 'lT') {
    ...
  } else if(sameJob[0].at(0) == 'fT') {
    ...
  }

  // For x-type files for sameJob: // [i][k], where k is for x[] 
  vector< vector<double> > numeratorSum(nrOfFiles, vector< Double_t >()), errnumeratorSum(nrOfFiles, vector< Double_t >()), sumSumW(nrOfFiles, vector< Double_t >()); 

  for(Int_t iJob = 0; iJob < sameJob.size(); ) { // Every File
    //   cout << "sameJob " << iJob << ": " << sameJob[iJob] << endl; 
    string direction = dir + sameJob[iJob]; 
    ifstream input(direction.c_str());
    Int_t i = -1; // matrix_j. 
    string line;
    Int_t iLine = 0; // Line i:th in a file 

    for(Int_t in = 0; in < sameJob.size(); ++ in) 

      // Goes through the entire file 
      while (getline(input, line)) 
	{
	  // Check if "line" is a centrality title, if so next matrix and line.   
	  for(Int_t year = 0; year < totalYear; ++year) 
	    for(Int_t month = 0; month < 12; ++month)
	      for(Int_t day = 0; day < 31; ++day) { 
		string date = sameJob[0].substr(1,4) + "distCgen" + int_to_str(cent) + int_to_str(typeBS); 
		if(line.find(centHist) != string::npos) { // Find the search word.
		  getline(input, line); // Get first line of data after title 
		  ++i; // New matrix in newHist; 
		  iLine = 0; 	
		}
	      }  

	  // Converts line to 4 double. 
	  std::istringstream iss(line);
	  Double_t x, y, err, sumOfWeights;
	  iss >> x >> y >> err >> sumOfWeights;	    

	  if(firstFile) {
	    numeratorSum[i].push_back(y);
	    errnumeratorSum[i].push_back(err);
	    sumSumW[i].push_back(sumOfWeights); 

	    newHist[i][0].push_back(x);
	    newHist[i][1].push_back(y);
	    newHist[i][2].push_back(err);
	    newHist[i][3].push_back(sumOfWeights);
	  } else {
	    numeratorSum[i].at(iLine) += y; 
	    errnumeratorSum[i].at(iLine) += err; 
	    sumSumW[i].at(iLine) += sumOfWeights; 
	  }	
	  ++iLine;	
	} // Entire file 

    // Deleting file. 
    if( remove(direction.c_str()) != 0) 
      cout << "Error deleting file" << endl;
    else {
      puts("File successfully deleted"); 
      cout << sameJob[iJob] << endl; ;
      sameJob.erase(sameJob.begin() + iJob); 
    }  
    firstFile = false; // Now to the other files. 
  } // All files 

  // Assign the merged values 
  for( Int_t i = 0; i < newHist.size(); ++i) 
    for( Int_t k = 0; k < newHist[i][0].size(); ++k) {
      newHist[i][1][k] = numeratorSum[i][k]; 
      newHist[i][2][k] = errnumeratorSum[i][k];
      newHist[i][3][k] = sumSumW[i][k]; 
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
  if(argc > 3) {
    if (string(argv[1]) != "X")
      type = argv[1];
    if (string(argv[2]) != "X")
      givenCity = argv[2];
    if (string(argv[3]) != "X")
      dir = "./" + (string) argv[2] + "/";
  }

  // Returns every file in given folder. 
  vector<string> files = vector<string>(); // All files in that directory. 
  getdir(dir,files); // Give folder name, returns files names. 

  // List of Hist-types: mT, lT and fT.
  vector<string> histTypes;
  histTypes.push_back("mT");
  histTypes.push_back("lT");
  histTypes.push_back("fT");

  // Saves all the datafiles with the given histogram type.
  vector<string> sameJobs; // mT, lT and fT

  // Goes through "files", and returns datafiles (mT/lT/fT) with the given prefix.  
  for (unsigned Int_t ifile = 0; ifile < files.size(); ++ifile) 
    // Find all data files as they are the only ones that begins with mT, lT or fT.
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

    // Lets call the columns x, y, z, w, x   
    vector< vector<Double_t> > newHist(4, vector< Double_t >()); 

    // Assign new file new. 
    string newfileName;

    // Calculate the new hist, depending on histogram type. 
     string name = sameJobs[0].replace(2,1,"A"); // i.e. XXACity.dat
    if(sameJob[0].at(0) == 'mT') { 
      name = sameJobs[0].replace(0,2,"Sweden"); // i.e. mTASweden.dat
      calcNewHistmT(dir, sameJobs, newHist); 
      newfileName = dir + name;
    } else if(sameJob[0].at(0) == 'lT') {
      name = sameJobs[0].replace(0,2,type); // i.e. lTACity.dat
      calcNewHistlT(dir, sameJobs, newHist); 
      newfileName = dir + name;
    } else if(sameJob[0].at(0) == 'fT') {
      name = sameJobs[0].replace(0,2,type); // i.e. fTACity.dat
      calcNewHistfT(dir, sameJobs, newHist); 
      newfileName = dir + name;
    }

    // Create file
    ofstream ofsNewHist(newfileName.c_str());

    // Print the [column][line] data into file. 
    for( Int_t k = 0; k < newHist[0].size(); ++k) { 
      ofsNewHist << fixed << scientific << setprecision(6); // Unnecessary?
      ofsNewHist<< newHist[0][k]<<"  "<< newHist[1][k]<<"  "
		<< newHist[2][k]<<"  "<< newHist[3][k]<< endl;
    } // All subtitles	

  return 0; 
}

