// dijsktras.cpp
#include <iostream>
#include <vector>
#include <sstream>
#include <list>
using namespace std;
// Main Execution

void print(list <char> vec);

int main(int argc, char *argv[]) {
	vector< vector<char>> board;
	list <char> tiletype;
	list <char> tilescore;
	list <char> temp;
	string line;
	int tscore, counter = 1, row, col;
	stringstream ss;

	while(getline(cin, line)){
		ss.str(line);
		ss >> tscore;
		if(counter <= tscore) {
		for(unsigned int i = 0; i < line.size(); i++) {
			if((line[i] <= 'Z' && line[i] >= 'A') || (line[i] >= 'a' && line[i] <= 'z'))  {
				tiletype.push_back(line[i]);
			}
			else if(line[i] <= '9' && line[i]>= '1') {
				tilescore.push_back(line[i]);
			}
		}
			counter++;
		}
		
		if(counter == tscore + (tscore -1)) {
			for(int i = 0; i < line.size(); i++) {
				cout << line[i] << " ";
			}
			cout << endl;
		}
		
		//counter++;
	}
	tilescore.pop_front();
	print(tiletype);
	cout << endl;
	print(tilescore);
	
    return 0;
}
void print(list <char> list) {
	cout << "Size: " << list.size() << endl;
	for (auto const& i: list) {
		cout << i << " ";
	}			 
	cout << endl;
	return;
}

