// ReadStructsConsApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <memory>

#include <windows.h>

using namespace std;



struct Person
{
	int age;
	char first[25];
	char last[25];
	double phone;
};


class BinaryData 
{
private:
	//char age[25];
	int age;
	char first[25];
	char last[25];
	double phone;
public:
	//ctor
	BinaryData() { }
	//destruct
	~BinaryData() { }
	//void SetData(int nAge, string strFirst, string strLast, string phone)
	void SetData(int nAge, string strFirst, string strLast, double phone)
	{
		memcpy(&this->age, &nAge, sizeof(nAge));
		memcpy(this->first, strFirst.c_str(), sizeof(strFirst.size()));
		memcpy(this->last, strLast.c_str(), sizeof(strLast.size()));
		memcpy(&this->phone, &phone, sizeof(phone));

	}
	void Save(ofstream &of)
	{
		of.write(reinterpret_cast<char*>(&age),   sizeof(age));
		of.write(reinterpret_cast<char*>(&first), sizeof(first));
		of.write(reinterpret_cast<char*>(&last),  sizeof(last));
		of.write(reinterpret_cast<char*>(&phone), sizeof(phone));
	}
	void WriteBinaryFile(string strFile)
	{
		ofstream fs;
		fs.open(strFile, ios::out | ios::binary | ios::app);
		if (!fs.is_open())
		{
			cout << "cannot open file" << strFile << endl;
		}
		else
		{
			this->Save(fs);
		}
		fs.close();
	}
	void ReadBinaryFile(string strFile)
	{
		Person p;
		ifstream binaryFile;
		int size = 0;
		binaryFile.open(strFile, ios::in | ios::binary);
		
		binaryFile.seekg(0, ios::end);
		size = (int)binaryFile.tellg();
		binaryFile.seekg(0, ios::end);
		size = (int)binaryFile.tellg();
		binaryFile.seekg(0, ios::beg);

		while (binaryFile.tellg() < size)
		{
			int sz1 = sizeof(p.age);
			int sz2 = sizeof(p.first);
			int sz3 = sizeof(p.last);
			int sz4 = sizeof(p.phone);

			binaryFile.read(reinterpret_cast<char*>(&p.age),   sz1);
			binaryFile.read(reinterpret_cast<char*>(&p.first), sz2);
			binaryFile.read(reinterpret_cast<char*>(&p.last),  sz3);
			binaryFile.read(reinterpret_cast<char*>(&p.phone), sz4);
			
			cout << p.age << "\t" <<p.first << "\t" << p.last << "\t" <<p.phone << endl;
		}
		binaryFile.close();

	}
};


int main(void)
{
	int go = 0;
	cout << "0 = set data to bin file / 1 = print data from bin file: ";
	cin >> go;

	unique_ptr<BinaryData> bd(new BinaryData());

	if(go == 0)
	{
		string strFirst, strLast;
		int age;
		double dblPhone;
		cout << "enter age : ";
		cin >> age;
		cout << "enter first name : ";
		cin >> strFirst;
		cout << "enter last name : ";
		cin >> strLast;
		cout << "enter phone number with dot : ";
		cin >> dblPhone;

		bd->SetData(age, strFirst, strLast, dblPhone);
		bd->WriteBinaryFile("./record.dat");

		bd->ReadBinaryFile("./record.dat");
	}
	else
	{
		bd->ReadBinaryFile("./record.dat");
	}

	//getchar();
	system("pause");
}

