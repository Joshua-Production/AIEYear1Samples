#include "DataFile.h"
#include <fstream>
using namespace std;

// Constuctor  
DataFile::DataFile()
{
	// Sets record count to 0 when a new DataFile is created
	recordCount = 0;
}
// Destructor 
DataFile::~DataFile()
{
	
	Clear();
}

void DataFile::AddRecord(string imageFilename, string name, int age)
{
	// Load the image from specified file
	Image i = LoadImage(imageFilename.c_str());

	// Create a new Record object with the given data
	Record* r = new Record;
	r->image = i;
	r->name = name;
	r->age = age;

	// Increase the record count to track how many records are in the file
	recordCount++;
}

DataFile::Record* DataFile::GetRecord(int index)
{

	return Load(index);
}
// Save the records to a binary file with the samefilename
void DataFile::Save(string filename, Record* record, int length)
{
	// Open the output file stream in binary mode
	ofstream outfile(filename, ios::binary);

	// Write the number of records to the file
	int recordCount = length;
	outfile.write((char*)&recordCount, sizeof(int));

	// Loop through each record and write its data to the file
	for (int i = 0; i < recordCount; i++)
	{
		// Retrieve the image data
		Color* imgdata = GetImageData(record->image);

		// Calculate the sizes of the image data, name, and age
		int imageSize = sizeof(Color) * record->image.width * record->image.height;
		int nameSize = record->name.length();
		int ageSize = sizeof(int);

		// Write the image width and height
		outfile.write((char*)&record->image.width, sizeof(int));
		outfile.write((char*)&record->image.height, sizeof(int));

		// Write the sizes of the name and age fields
		outfile.write((char*)&nameSize, sizeof(int));
		outfile.write((char*)&ageSize, sizeof(int));

		// Write the image data, name, and age to the file
		outfile.write((char*)imgdata, imageSize);
		outfile.write((char*)record->name.c_str(), nameSize);
		outfile.write((char*)&record->age, ageSize);
	}

	// Close the file stream after saving the data
	outfile.close();
}

void DataFile::Load(string filename)
{
	filepath = filename;
}

DataFile::Record* DataFile::Load(int index)
{
	// Open the input file stream in binary mode
	ifstream infile(filepath, ios::binary);
	if (infile.is_open())
	{
		
		infile.seekg(0, ios::beg);
		recordCount = 0;
		infile.read((char*)&recordCount, sizeof(int));

		
			int nameSize = 0;
			int ageSize = 0;
			int width = 0, height = 0, format = 0, imageSize = 0;

			// Skip the records before the index thats being requested 
			for (int i = 0; i < index; i++)
			{

				infile.read((char*)&width, sizeof(int));
				infile.read((char*)&height, sizeof(int));

				imageSize = sizeof(Color) * width * height;

				infile.read((char*)&nameSize, sizeof(int));
				infile.read((char*)&ageSize, sizeof(int));
				infile.seekg(imageSize + ageSize + nameSize, ios::cur);
			}
			// Read the data for the requested record
			infile.read((char*)&width, sizeof(int));
			infile.read((char*)&height, sizeof(int));

			imageSize = sizeof(Color) * width * height;

			infile.read((char*)&nameSize, sizeof(int));
			infile.read((char*)&ageSize, sizeof(int));

			// Allocate memory for the image data and read it from the file
			char* imgdata = new char[imageSize];
			infile.read(imgdata, imageSize);

			// Load the image using the image data
			Image img = LoadImageEx((Color*)imgdata, width, height);

			// Allocate memory for the name and read it from the file
			char* name = new char[nameSize + 1];
			int age = 0;
			infile.read((char*)name, nameSize);
			name[nameSize] = '\0';

			// Read the age data from the file
			infile.read((char*)&age, ageSize);

			// Make a new Record with current data
			Record* r = new Record();
			r->image = img;
			r->name = string(name);
			r->age = age;
			

			delete[] imgdata;
			delete[] name;
			return r;
	}
	
	

	infile.close();

}




void DataFile::Clear()
{

}