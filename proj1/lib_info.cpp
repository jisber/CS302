//Jacob Isber and Dani Shekoury
//Project 1
//Revision 1.0
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include "user.hpp"
#include <sstream>

 

using namespace std;

int main(int argc, char* argv[]) {

	string line;
	ifstream fin;
	Music libr;

//	if(argc > 2) { cout << "Too many arguments" << endl; return -1;}

	//Opens and checks the file.
	fin.open(argv[1]);
	if(fin.fail()) cout << "Could not open file" << endl;
	
	int tc= 0;
	while(getline(fin, line)) { //Reads in each line from the text file.
		
		libr.GetString(line);
		
	//	for(int i = 0; i < line.size(); i++) if(line[i] == '_') line[i] = ' ';	//Converts all "_" to a " " in a text file.
			
	}
	
	libr.FillInfo();
	libr.Print();


	return 0;
}

void Music::GetString(string temp) {

	istringstream str(temp);
	Song tSong;
	Artist tArtist;
	Album tAlbum;
	string time;
//	Music libr;
	bool found = 0;
	multimap<string,Artist>::iterator it;
	multimap<string,Album>::iterator ita;
	multimap<string,Song>::iterator its;

	int counter = 0;

	do{
		string word;
		str >> word;

		for(int i = 0; i < word.size(); i++){
			if(word[i] == '_')
				word[i] = ' ';
		}
		
		if(counter == 0) tSong.title = word;
		else if(counter == 1) time = word;
		else if(counter == 2) tArtist.name = word;
		else if(counter == 3) tAlbum.name = word;
		else if(counter == 5) tSong.track = word;
		
		

		counter++;
	}while(str);
	

	it = library.find(tArtist.name);
	if(it == library.end()) {
		library.insert(pair<string,Artist>(tArtist.name, tArtist));
	}

	it = library.find(tArtist.name);
	it->second.name = tArtist.name;

	ita = it->second.albums.find(tAlbum.name);
	if(ita == it->second.albums.end()){
		it->second.albums.insert(pair<string,Album>(tAlbum.name, tAlbum));
	}
	
	ita = it->second.albums.find(tAlbum.name);
	ita->second.name = tAlbum.name;

	its = ita->second.songs.find(tSong.track);
	if(its == ita->second.songs.end()){
		ita->second.songs.insert(pair<string,Song>(tSong.track, tSong));
	}

	its = ita->second.songs.find(tSong.track);
	its->second.title = tSong.title;
	its->second.time = convert_time(time);
	its->second.track = tSong.track;
		
/*	
	for(it = library.begin(); it != library.end(); it++) {
		cout << "First: " << it->first << endl;
		for(ita = it->second.albums.begin(); ita != it->second.albums.end(); ita++) {
			cout << "Second: \t" << ita->first << endl;
			for(its = ita->second.songs.begin(); its != ita->second.songs.end(); its++) {
				cout << "Third: \t\t" << its->first << endl;
			}
		}
	}


	
		for(it = library.begin(); it != library.end(); ++it) {
		cout << "First: " << it->first << endl;// "Second: " << it->second << endl;
	//	cout << tc << endl;
	//	tc++;
	}
	*/
	
	

	return;
}

void Music::FillInfo(){
	multimap<string,Artist>::iterator it;
	multimap<string,Album>::iterator ita;
	multimap<string,Song>::iterator its;
	int sTime = 0;
	int aTime = 0;
	int nSong = 0;

	for(it = library.begin(); it != library.end(); it++){
		aTime = 0;
		nSong = 0;
		for(ita = it->second.albums.begin(); ita != it->second.albums.end(); ita++) {
			sTime = 0;
			for(its = ita->second.songs.begin(); its != ita->second.songs.end(); its++) {
				sTime += its->second.time;
			}
			ita->second.time = sTime;
			ita->second.nsongs = ita->second.songs.size();
			nSong += ita->second.nsongs;
			aTime += ita->second.time;
		}
		it->second.time = aTime;
		it->second.nsongs = nSong;
	}


}

void Music::Print(){
	multimap<string,Artist>::iterator it;
	multimap<string,Album>::iterator ita;
	multimap<string,Song>::iterator its;

	for(it = library.begin(); it != library.end(); it++){
		printf("%s: %d, %s\n", it->second.name.c_str(), it->second.nsongs, convert_time_back(it->second.time).c_str());
		for(ita = it->second.albums.begin(); ita != it->second.albums.end(); ita++) {
			printf("\t%s: %d, %s\n", ita->second.name.c_str(), ita->second.nsongs, convert_time_back(ita->second.time).c_str());
			for(its = ita->second.songs.begin(); its != ita->second.songs.end(); its++) {
				printf("\t\t%s. %s: %s\n", its->second.track.c_str(), its->second.title.c_str(), convert_time_back(its->second.time).c_str());
			}
		}
	}
}


int convert_time(string str) {
	stringstream ss(str);
	string hh,mm;
	getline(ss, hh, ':');
	getline(ss, mm, ';');

	int h,m;
	stringstream ssh(hh);
	ssh >> h;
	stringstream ssm(mm);
	ssm >> m;
	int minutes = ((h * 60) + m);;
	return minutes;
}

string convert_time_back(int time){
	string str, hh, mm;
	int h, m;

	h = (time / 60);
	m = (time % 60);

	stringstream ssh;
	stringstream ssm;

	ssh << h;
	ssm << m;

	str = ssh.str() + ":" + ssm.str();

	return str;
}
