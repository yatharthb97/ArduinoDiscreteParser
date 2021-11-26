#include "parsar.hpp"


Parser<5> parser;

void setup()
{
	// put your setup code here, to run once:

	//Parsar config
	parser.add("hi", parsar.hi);


	//Other setup ↓
	
}


void loop()
{
	// put your main code here, to run repeatedly:
	
	// Parsar controlled loop
	parsar.check_input();
	parsar.loop();

	//Other loop elements ↓

}