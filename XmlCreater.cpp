// XmlCreater.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>

#include <iostream>
#include <fstream>
#include <string>


int main()
{
	//std::cout << "Hello World!\n"; 

	std::ofstream myfile;
	myfile.open("test-big-1.xml");

	int ilen = 0;
	int jlen = 0;

	//ilen = 10, jlen = 10; // 11 kb
	//ilen = 10000, jlen = 100; // 173 Mb // 1 min 10 sec
	ilen = 10000, jlen = 100; // 173 Mb // 1 min 10 sec

	//for (int i = 0; i < 10; i++)
	//{
	//	myfile << "Writing cthis to a file - " + std::to_string(i);
	//	myfile << "\n";
	//}

	myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	myfile << "\n";
	myfile << "<!DOCTYPE root SYSTEM \"tree2.dtd\">";
	myfile << "\n";
	myfile << "<root>";
	myfile << "\n";
	myfile << "  <node1>content of node 1</node1>";
	myfile << "\n";
	myfile << "  <node2/>";
	myfile << "\n";
	myfile << "  <node3 attribute=\"yes\" foo=\"bar\">this node has attributes</node3>";
	myfile << "\n";
	myfile << "  <node4>other way to create content (which is also a node)</node4>";
	myfile << "\n";

	for (int i = 5; i <= ilen; i++)
	{
		myfile << "  <node" + std::to_string(i) +">";
		myfile << "\n";

		for (int j = 1; j <= jlen; j++)
		{
			myfile << "    <node" + std::to_string(i) + std::to_string(j) 
				+  " odd=\"" + ((j%2 == 0) ? "no" : "yes") 
				+ "\" BandType=\"" + ((j % 2 == 0) ? "one" : "two") 
				+ "\" LowBandRange=\"" + ((j % 2 == 0) ? "treee" : "four") 
				+ "\" HighBandRange=\"" + ((j % 2 == 0) ? "five" : "six") 
				+ "\" HighFrequency=\"" + ((j % 2 == 0) ? "seven" : "eight")
				+ "\" SpectrumType=\"" + ((j % 2 == 0) ? "twentyone" : "seventyhunderd")
				+ "\" HighWarningLevel=\"" + ((j % 2 == 0) ? "fivethousandsixtyeight" : "whysofewwhenweneedmuchmore") 
				+ "\"/>";

			myfile << "\n";
			//myfile << "    <node52 odd=\"yes\" BandType=\"two\" LowBandRange=\"treee\" HighBandRange=\"six\" HighFrequency=\"seven\" SpectrumType=\"seventyhunderd\" HighWarningLevel=\"fivethousandsixtyeight\"/>";
			//myfile << "\n";
		}

		myfile << "  </node" + std::to_string(i) + ">";
		myfile << "\n";

//		myfile << "    <node51 odd=\"no\" BandType=\"one\" LowBandRange=\"four\" HighBandRange=\"five\" HighFrequency=\"eight\" SpectrumType=\"twentyone\" HighWarningLevel=\"whysofewwhenweneedmuchmore\"/>";
//		myfile << "\n";
//		myfile << "    <node52 odd=\"yes\" BandType=\"two\" LowBandRange=\"treee\" HighBandRange=\"six\" HighFrequency=\"seven\" SpectrumType=\"seventyhunderd\" HighWarningLevel=\"fivethousandsixtyeight\"/>";
//		myfile << "\n";
//		myfile << "    <node599 odd=\"no\" BandType=\"one\" LowBandRange=\"four\" HighBandRange=\"five\" HighFrequency=\"eight\" SpectrumType=\"twentyone\" HighWarningLevel=\"whysofewwhenweneedmuchmore\"/>";
//		myfile << "\n";
//		myfile << "  </node" + std::to_string(i) + ">";

/*
		myfile << "  <node5>";
		myfile << "\n";
		myfile << "    <node51 odd=\"no\" BandType=\"one\" LowBandRange=\"four\" HighBandRange=\"five\" HighFrequency=\"eight\" SpectrumType=\"twentyone\" HighWarningLevel=\"whysofewwhenweneedmuchmore\"/>";
		myfile << "\n";
		myfile << "    <node52 odd=\"yes\" BandType=\"two\" LowBandRange=\"treee\" HighBandRange=\"six\" HighFrequency=\"seven\" SpectrumType=\"seventyhunderd\" HighWarningLevel=\"fivethousandsixtyeight\"/>";
		myfile << "\n";
		myfile << "    <node599 odd=\"no\" BandType=\"one\" LowBandRange=\"four\" HighBandRange=\"five\" HighFrequency=\"eight\" SpectrumType=\"twentyone\" HighWarningLevel=\"whysofewwhenweneedmuchmore\"/>";
		myfile << "\n";
		myfile << "  </node5>";
		myfile << "\n";
		myfile << "  <node6>";
		myfile << "\n";
		myfile << "    <node61 odd=\"no\" BandType=\"one\" LowBandRange=\"four\" HighBandRange=\"five\" HighFrequency=\"eight\" SpectrumType=\"twentyone\" HighWarningLevel=\"whysofewwhenweneedmuchmore\"/>";
		myfile << "\n";
		myfile << "    <node62 odd=\"yes\" BandType=\"two\" LowBandRange=\"treee\" HighBandRange=\"six\" HighFrequency=\"seven\" SpectrumType=\"seventyhunderd\" HighWarningLevel=\"fivethousandsixtyeight\"/>";
		myfile << "\n";
		myfile << "  </node6>";
*/
	}

	//    <node999 odd="no" BandType="one" LowBandRange="four" HighBandRange="five" HighFrequency="eight" SpectrumType="twentyone" HighWarningLevel="whysofewwhenweneedmuchmore"/>

	myfile << "</root>";
	myfile << "\n";


	myfile.close();

}
