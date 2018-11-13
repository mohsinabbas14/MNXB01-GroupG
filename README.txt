This code is used to analyze meteorological data recorded at different stations in Sweden in between 1961 and 2015.
Three types of analyzed data is presented in different graphical manners using root (https://root.cern.ch/).
The user can create for a specified time period:
	- a graph showing the mean temperature each day in a specific city or all cities
	- a 1-D histogram with a gaussian fit showing the distribution of the hottest and coldest days in a year, over all years, in a city
	- a 3-D histogram showing the hottest and coldest temperatures each day, every year, in a city
	
There are three methods that conserns the user:
	- readFile() (in readFile.C)
	- analyze()  (in Analysis.C)
	- plot() (in Plot.C)
	
The raw data contains some descriptive text and some bad data points. readFile() reads the raw data and outputs only the useful data to a file with a name on the form XXR[City].dat in the working directory. (R = read)
In turn, analyze() reads this file, perform the calculations specified by the user and outputs the result of those calculations to another file.
The new file follows the namin convention: mTA[City].dat, lHA[City].dat or fTA[City].dat, where the first two characters indicates which type of analysis has been made. (A = analyzed)
Finally plot() reads the analyzed file and depending on the file name, plots the appropriate structure.
It is important to note that when plotting the distribution of the hotest and coldest days, the hottest day is shown counting from the first of January while the coldest day is shown counting from the first of July.

EXAMPLE OF HOW TO RUN THE CODE IN ROOT

Open root in the directory wher the code is kept

// Load the different parts of the code
.L readFile.C+
.L Analysis.C+
.L Plot.C+

// Read the raw data from a specific time period
// readFile("filePath", "starting date" (to be read), "end date" (not to be read) )
// The date is given on the form yyyy-mm-dd

// Example: (Read the data from Lund between 1961-01-01 and 2015-12-31)
readFile("./datasets/smhi-opendata_Lund.csv", "1961-01-01", "2016-01-01")

// Call the analyze function and specify the calculation to be performed
// analyze("type", "city" , "directory" (where the file(s) is(are) stored) )
// The type is "mT" for mean temperature, "lH" for hottest and coldest days and "fT" for hotest and coldest temperatures every day.
// If the city name is "X", then the data from all XXR[City].dat files in the specified directory will be used.
// WARNING: analyze() will delete the read from file XXR[City].dat

// Example: (Calculate the mean temperature in all cities, files in current directory)
analyze("mT", "X", "./")

// Example: (Find the distribution of the hottest and coldest days in Lund, file in current directory)
analyze("lH", "Lund", "./")

// Plot the analyzed data
// plot("filePath")
// If all cities were used during the analysis the analyzed file will be called [XX]ASweden.dat

// Example: (Plot the mean temperature in all cities)
plot("mTASweden.dat")

// Example: (Plot the distribution of the hottest and coldest days in Lund)
plot("lHALund.dat")
