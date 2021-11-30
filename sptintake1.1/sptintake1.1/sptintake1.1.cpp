
// sptintake1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <io.h>
#include <ShObjIdl.h>
#include "sptintake1.1.h"



using  namespace System::IO;
using namespace std;
using namespace System;
using namespace System::Collections;
using namespace System::Windows;
using namespace System::Windows::Forms;


[STAThreadAttribute]

std::string mSys2std(System::String^ S) // converts system::string to std::string - Mikayla
{
	using namespace Runtime::InteropServices;
	std::string a;
	const char* chrs =
		(const char*)(Marshal::StringToHGlobalAnsi(S)).ToPointer();
	a = chrs;
	Marshal::FreeHGlobal(IntPtr((void*)chrs));
	return a;
}

[STAThreadAttribute]

int main()
{
	bool check_first_tab = false;
	vector <double> depth;
	vector <double> bcount;
	String^ filename = "no file";
	Stream^ myStream;
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog();
	openFileDialog1->InitialDirectory = "c:\\";
	openFileDialog1->Filter = "CSV Files|*.csv";
	openFileDialog1->Title = "Please Select a CSV file ONLY, withOUT headers";
	openFileDialog1->RestoreDirectory = true;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		String^ strfilename = /*openFileDialog1->InitialDirectory + */openFileDialog1->FileName; // get the file path from the chosen file

		filename = strfilename;  // set the filepath to the filename variable

	}

	if (filename == "no file") /// makes sure there is a file chosen and presents a warning box if a file is missing
	{
		MessageBox::Show(" You must choose a file");
	}

	string filename2;

	

	filename2 = mSys2std(filename);
	cout << filename2;


	ifstream myFile(filename2, std::ifstream::in); // opens the file using the filename file path
	//ifstream myFile;
	//myFile << filename2;

	int comma_pos, comma_pos2, comma_pos3;

	if (myFile.is_open())
	{
		while (!myFile.eof())
		{

			// This while loop reads in a CSV file and 
			//takes out the commas and changes the string 
			//values it reads in into usable double vectors.
			// The values must be doubles to do the calculaitons with them.


			string aLine;
			getline(myFile, aLine);   // reads in ONE line of the file and stores that line as a string called "aLine"
			comma_pos = aLine.find(',', 0);					// finds the positions of the commas.    
			comma_pos2 = aLine.find(',', comma_pos + 1);      //(cplusplus.com):   string.find( look for this in the line , start at this position )  it returns what position it finds this at   
			comma_pos3 = aLine.find(',', comma_pos2 + 1);
			// check if no pore pressure data added
			if (comma_pos3 < 0 && comma_pos2 > 0)
			{
				MessageBox::Show("error, no pore pressure data in file. CPT data NOT uploaded.!  Please fill 4th column with zeros if no (u) data available");
				break;
			}

			string depths, bcount;										    // the string pieces to each value that will be changed in to usable doubles soon
			depths = aLine.substr(0, comma_pos);								   // (http://www.cplusplus.com/reference/string/string/substr/) string.subtr() clips out a certain part of a string
			bcount = aLine.substr(comma_pos + 1, aLine.length() - comma_pos - 1);    // string.substr( what pos in the string to start reading  , the length of string to read in )
			

			double depthd, bcountd;			// double version of the values read in
			depthd = atof(depths.c_str());			// ".c_str()" changes the c++ string into a c string to be able to use the "atof()" function
			bcountd = atof(bcount.c_str()); // .atof() is a c function that changes c strings into doubles
			
			

			depth.push_back(depthd);	// ".push_back()" puts that value at the end of the vector. Vectors are one dimensional arrays that have dynamic sizes
			bcount.push_back(bcountd);			// Filling in the vectors with the double values. These vectors will be used for the rest of the program to do all needed calculations
			
		}
		// The program always reads in a blank extra line 
		//and this deletes that blank at the end:
		//  "vector.pop_back()" deletes the last position in the vector. In this case it deletes the blank at the end of the vector

		//if error in pore pressure loading then avoid pop backs and exit loop
		if (comma_pos3 < 0 && comma_pos2 > 0)
		{
			// avoid pop backs
			// make sure program knows that soil data was not loaded
			check_first_tab = false;
		}
		else
		{
			depth.pop_back();
			bcount.pop_back();
			
		}

	}
	else
	{
		MessageBox::Show("File Failed to Open. Fix File Name");

	}


	for (int i = 0; i < bcount.size(); i++)
		std::cout << bcount.at(i) << ' ';


	return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
