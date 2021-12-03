#pragma once
#include <set>
#include <map>

using namespace std;

struct Song { 
	string title;
	int time;  // could also be a string
	string track;
};

struct Album {
	map <string, Song > songs;
	string name;
	int time;
	int nsongs;  // optional variable but makes it easier
};

struct Artist {
	map <string, Album > albums;
	string name;
	int time;
	int nsongs;
};

struct Music {
	void GetString(string);
	void FillInfo();
	void Print();

	multimap<string,Artist> library;
};

int convert_time(string);

string convert_time_back(int);
