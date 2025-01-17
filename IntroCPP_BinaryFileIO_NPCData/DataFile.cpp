#include "DataFile.h"
#include <fstream>
using namespace std;

DataFile::DataFile()
{
	recordCount = 0;
}

DataFile::~DataFile()
{
	Clear();
}

void DataFile::AddRecord(string imageFilename, string name, int age)
{
	Image i = LoadImage(imageFilename.c_str());

	Record* r = new Record;
	r->image = i;
	r->name = name;
	r->age = age;

	
	recordCount++;
}

DataFile::Record* DataFile::GetRecord(int index)
{
	return Load(index);
}

void DataFile::Save(string filename, Record* record, int length)
{
	ofstream outfile(filename, ios::binary);

	int recordCount = length;
	outfile.write((char*)&recordCount, sizeof(int));

	for (int i = 0; i < recordCount; i++)
	{		
		Color* imgdata = GetImageData(record->image);
				
		int imageSize = sizeof(Color) * record->image.width * record->image.height;
		int nameSize = record->name.length();
		int ageSize = sizeof(int);

		outfile.write((char*)&record->image.width, sizeof(int));
		outfile.write((char*)&record->image.height, sizeof(int));
		
		outfile.write((char*)&nameSize, sizeof(int));
		outfile.write((char*)&ageSize, sizeof(int));

		outfile.write((char*)imgdata, imageSize);
		outfile.write((char*)record->name.c_str(), nameSize);
		outfile.write((char*)&record->age, ageSize);
	}

	outfile.close();
}

void DataFile::Load(string filename)
{
	filepath = filename;
}

DataFile::Record* DataFile::Load(int index)
{
	ifstream infile(filepath, ios::binary);
	if (infile.is_open())
	{
		infile.seekg(0, ios::beg);
		recordCount = 0;
		infile.read((char*)&recordCount, sizeof(int));

			int nameSize = 0;
			int ageSize = 0;
			int width = 0, height = 0, format = 0, imageSize = 0;

			for (int i = 0; i < index; i++)
			{

				infile.read((char*)&width, sizeof(int));
				infile.read((char*)&height, sizeof(int));

				imageSize = sizeof(Color) * width * height;

				infile.read((char*)&nameSize, sizeof(int));
				infile.read((char*)&ageSize, sizeof(int));
				infile.seekg(imageSize + ageSize + nameSize, ios::cur);
			}
			infile.read((char*)&width, sizeof(int));
			infile.read((char*)&height, sizeof(int));

			imageSize = sizeof(Color) * width * height;

			infile.read((char*)&nameSize, sizeof(int));
			infile.read((char*)&ageSize, sizeof(int));

			char* imgdata = new char[imageSize];
			infile.read(imgdata, imageSize);

			Image img = LoadImageEx((Color*)imgdata, width, height);
			char* name = new char[nameSize + 1];
			int age = 0;

			infile.read((char*)name, nameSize);
			name[nameSize] = '\0';


			infile.read((char*)&age, ageSize);

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