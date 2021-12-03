#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include "disjoint.h"

using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

int randomnum(int);

class Superball {
  public:
    Superball(int argc, char **argv);
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
};

void usage(const char *s) 
{
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

main(int argc, char **argv)
{
  Superball *s;
  DisjointSet *ds;
  s = new Superball(argc, argv);
  ds = new DisjointSetByRankWPC(s->r * s->c);
	
	int counter = s->c- 1;
	int counterbot = s->c;
		
	for(int i = 0; i < s->board.size(); i++) {
		if(s->board[i] != '.' && s->board[i] != '*') {
			if(s->board[i] == s->board[i + 1] && (i != counter)) {
				if(ds->Find(i) != ds->Find(i + 1)) {
					ds->Union(ds->Find(i), ds->Find(i + 1));
				}
			}
		
			if(s->board[i] == s->board[counterbot] && counterbot < s->r * s-> c) {
				if(ds->Find(i) != ds->Find(counterbot)) {
					ds->Union(ds->Find(i), ds->Find(counterbot));
				}
			}
		}
		
		if(i == counter) {
			counter += s->c;
		}
		counterbot++;
	}
	
	int ran = randomnum(s->board.size());

		while(s->board[ran] == '*' || s->board[ran] == '.') {
			ran = randomnum(s->board.size());	
		}
		
		cout << ran << " TEST " << endl;	
/*
	int count = 0;
	deque <int> hw;
	deque <int>::iterator nit;
	cout << "Scoring sets: " << endl;
	for(int i = 0; i < s->board.size(); i++) {
		if(s->goals[i] && s->board[i] != '*') {
			for(int j = 0; j < s->board.size(); j++) 
				if(ds->Find(i) == ds->Find(j)) 
					count++;
					nit = find(hw.begin(), hw.end(), ds->Find(i));
					if(nit == hw.end()) 
						if(count >= s->mss) 
						cout << "Move is: SWAP " << i / s->c << " " << i % s->c << " " << ran / s->c << " " << ran % s->c << endl;
					//		cout << "Size: " << count << " Char: " << (char)s->board[i] << " Scoring Cell: " << i / s->c << "," << i % s->c << endl;
					hw.push_back(ds->Find(i));
				}
			count = 0;
		}
*/
	return 0;	
}

int randomnum(int size) {
	
	srand(time(nullptr));
	auto rand_val = (rand() % (size + 1)) + 0;

	return rand_val;
}

void tscore(Superball *s, DisjointSet *ds, vector<int> &score) {
	int count = 0;
	for(int i = 0; i < s->board.size(); i++) {
		if(s->goals[i] && s->board[i] != '*') {
			for(int j = 0; j < s->board.size(); j++) 
				if(ds->Find(i) == ds->Find(j)) 
					count++;
						if(count >= s->mss)
							score[i] = count * s->colors[(char)s->board[i]];
					//		cout << "Size: " << count << " Char: " << (char)s->board[i] << " Scoring Cell: " << i / s->c << "," << i % s->c << endl;
				}
			count = 0;
		}


}

