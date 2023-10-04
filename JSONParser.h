#pragma once
#include <iostream>
#include <fstream>
#include <json/json.h>

using namespace std;

Json::Value ReadFile(string path) {
	ifstream file(path);

	Json::Value obj;
	Json::CharReaderBuilder reader;

	string errors;
	bool parsingSuccessful = Json::parseFromStream(reader, file, &obj, &errors);

	if (!parsingSuccessful) {
		LOG("JSONCPP ERROR : ");
		LOG("%s", errors);
		file.close();
		
	}

	file.close();

	return obj;
}

void WriteFile(string path) {

}
