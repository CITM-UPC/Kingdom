#pragma once
#include <iostream>
#include <fstream>
#include <json/json.h>

using namespace std;

void testing() {
	ifstream file("example.json");

	Json::Value obj;
	Json::CharReaderBuilder reader;

	string errors;
	bool parsingSuccessful = Json::parseFromStream(reader, file, &obj, &errors);

	if (!parsingSuccessful) {
		LOG("ERROR PARSING!!!");
		cout << errors << endl;
		return;
	}

	cout << "JSON DATA: " << obj << endl;

	cout << "Name: " << obj["name"] << endl;
	cout << "Yeet: " << obj["yeet"] << endl;

	file.close();
}

