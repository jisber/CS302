/* Dani Shekoury 2/27/20
 * this program takes a superball 
 * game board and prints out the best
 * available move
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <cmath>
#include <time.h>
#include "disjoint.h"
using namespace std;

//template code
#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

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

//end template code 

void SetDS(Superball *, DisjointSet *);

void PrintAnalyze(Superball *, DisjointSet *);

void rc(Superball *, vector<int> &, vector<int> &);

void scores(Superball *, DisjointSet *, vector<int> &);

int max(vector<int> &, vector<int> &);

int random(int);

int colour(Superball *, DisjointSet *, char, vector<int> &, vector<int> &);

main(int argc, char **argv)
{
  Superball *s;
  DisjointSet *ds;
  s = new Superball(argc, argv);
  ds = new DisjointSetByRankWPC(s->r*s->c);

  //calls a fucntion to set up the disjoint set
  SetDS(s, ds);

  //analyses the board and prints out next move
  PrintAnalyze(s, ds);

  //returns memory
  delete s;
  delete ds;

  return 0;
}

//sets up dijoint set by calling union on the parents of two nodes that are the same
void SetDS(Superball *s, DisjointSet *ds){
  for (int i = 0; i < s->r * s->c; i++){
    if(s->board[i] != '.' && s->board[i] != '*'){
      if(((i + 1) % s->c  == (i % s->c) + 1) && i + 1 < s->r * s->c){
        if(s->board[i] == s->board[i + 1]){
          if(ds->Find(i) != -1 || ds->Find(i + 1) != -1){
            if(ds->Find(i) != ds->Find(i + 1)){
              ds->Union(ds->Find(i), ds->Find(i + 1));
            }
          }
        }
      }
      if(i + s->c < s->r * s->c){
        if(s->board[i] == s->board[i + s->c]){
          if(ds->Find(i) != -1 || ds->Find(i + s->c) != -1){
            if(ds->Find(i) != ds->Find(i + s->c)){
              ds->Union(ds->Find(i), ds->Find(i + s->c));
            }
          }
        }
      }
    }
  }
}

//Prints out the next move based on the current board
void PrintAnalyze(Superball *s, DisjointSet *ds){
  list<int> n;
  list<int>::iterator  it;
  vector<int> row;
  vector<int> col;
  vector<int> score(s->r*s->c, -1);
  vector<int> used;
  int num1, num2;

  //sets up vectors of corresponding rows and cols
  rc(s, row, col);

  //finds the block that will give the most points when scored
  scores(s, ds, score);

  int j = max(score, used);

  //will SCORE blocks if available before ending the game
  if(s->empty < 5){
    if(j != -1){
      printf("SCORE %d %d\n", row[j], col[j]);
      return;
    }
  }

  //will SCORE blocks if it will return more than 15 points
  if(score[j] > 15){
    printf("SCORE %d %d\n", row[j], col[j]);
    return;
  }

  
  //finds a block with the color that is worth the most points
  int highest = -1;
  
  for(int i = 0; i < s->r*s->c; i++){
    if(s->board[i] != '.' && s->board[i] != '*'){
      if(highest = -1){
        highest = i;
      }
      else if(s->colors[s->board[i]] > s->colors[s->board[highest]])
        highest = i;
    }
  }

  //attempts to move that block to a group on a score space
  if(j != -1)
    colour(s, ds, s->board[j], row, col);

  //moves a block to a score space or next to a group of blocks that can be scored for all colors starting with the color that will give the most points to the least
  int top = max(s->colors, used);
  do{
    if(top >= 'A' && top <= 'Z'){
      used.push_back(top);
      top += 32;
    }
    if(colour(s, ds, top, row, col))
     return;
    used.push_back(top);
    top = max(s->colors, used);
  }while(s->colors[top] != 0);
  used.clear();

  //picks two random blocks
  srand(time(NULL));
  do{
    do{
      num1 = random(s->r*s->c);
    }while(s->board[num1] == '.' || s->board[num1] == '*');

    srand(time(NULL) + 1);

    do{
      num2 = random(s->r*s->c);
   }while(s->board[num2] == '.' || s->board[num2] == '*');
  }while(num1 == num2);

  //trys to group the first random block with a group on a scoring block
  for(int i = 0; i < s->r*s->c; i++){
    if(s->goals[i] && s->board[i] == s->board[num1]){
      for(int k = 0; k < s->r*s->c; k++){
        if(ds->Find(i) == ds->Find(k)){
            if((k - 1) % s->c  == (k % s->c) - 1 && s->board[k - 1] != '.' && s->board[k - 1] != '*'){
              printf("SWAP %d %d %d %d\n", row[num1], col[num1], row[k - 1], col[k - 1]);
              return;
            }
            else if(k - s->c >= 0 && s->board[k - s->c] != '.' && s->board[k - s->c] != '*'){
              printf("SWAP %d %d %d %d\n", row[num1], col[num1], row[k - s->c], col[k - s->c]);
              return;
            }
            else if(k + s->c < s->c*s->r && s->board[k + s->c] != '.' && s->board[k + s->c] != '*'){
              printf("SWAP %d %d %d %d\n", row[num1], col[num1], row[k + s->c], col[k + s->c]);
              return;
            }
            if((k + 1) % s->c  == (k % s->c) + 1 && s->board[k + 1] != '.' && s->board[k + 1] != '*'){
              printf("SWAP %d %d %d %d\n", row[num1], col[num1], row[k + 1], col[k + 1]);
              return;
            }
          }
        }
      }
    }

  //swaps two random bblocks
  printf("SWAP %d %d %d %d\n", row[num1], col[num1], row[num2], col[num2]);
}

//creates a vector of corresponding row and col
void rc(Superball *s, vector<int> &row, vector<int> &col){
  int r = 0, c = -1;

  for(int i = 0; i < s->r*s->c; i++) {
    if(i % s->c == 0 && i != 0)
      r++;
    c++;
    row.push_back(r);
    col.push_back(c);
    if(c == s->c - 1)
      c = -1;
  }
}

//finds the point value of each possible SCORE
void scores(Superball *s, DisjointSet *ds, vector<int> &score){
  int count = 0;
  for (int i = 0; i < s->r*s->c; i++) {
    if (s->goals[i] && s->board[i] != '*') {
      for(int j = 0; j < s->r*s->c; j++)
        if(ds->Find(i) == ds->Find(j))
          count++;
      if(count >= s->mss)
        score[i] = count * s->colors[(char)s->board[i]];
    } 
    count = 0;
  }
}

//returns the max value in the vector that has not already been used
int max(vector<int> &v, vector<int> &used){
  int slot = 0;
  int max = v[slot];
  vector<int>::iterator it;
  
  for(int i = 0; i < v.size(); i++){
    if(max < v[i]){
      it = find(used.begin(), used.end(), i);
      if(it == used.end()){
        max = v[i];
        slot = i;
      }
    }
  }
  if(max == -1)
    return max;

  return slot;
}

//random number
int random(int cap){
  return rand() % cap;
}

//finds a block that matches the color and moves it on a score block or next to a group of the same color 
int colour(Superball *s, DisjointSet *ds, char color, vector<int> &row, vector<int> &col){
int temp = -1;
  for(int i = 0; i < s->r*s->c; i++){
    if(s->goals[i] && s->board[i] == color){
      for(int k = 0; k < s->r*s->c; k++){
        if(s->board[k] == color)
          if(ds->Find(i) != ds->Find(k))
            temp = k;
      }
      if(temp != -1)
      for(int k = 0; k < s->r*s->c; k++){
          if(ds->Find(i) == ds->Find(k)){
            if((k - 1) % s->c  == (k % s->c) - 1 && s->board[k - 1] != '.' && s->board[k - 1] != '*' && s->board[k - 1]!= s->board[i]){
              printf("SWAP %d %d %d %d\n", row[temp], col[temp], row[k - 1], col[k - 1]);
              return 1;
            }
            else if(k - s->c >= 0 && s->board[k - s->c] != '.' && s->board[k - s->c] != '*' && s->board[k - s->c]!= s->board[i]){
              printf("SWAP %d %d %d %d\n", row[temp], col[temp], row[k - s->c], col[k - s->c]);
              return 1;
            }
            else if(k + s->c < s->c*s->r && s->board[k + s->c] != '.' && s->board[k + s->c] != '*' && s->board[k + s->c]!= s->board[i]){
              printf("SWAP %d %d %d %d\n", row[temp], col[temp], row[k + s->c], col[k + s->c]);
              return 1;
            }
            if((k + 1) % s->c  == (k % s->c) + 1 && s->board[k + 1] != '.' && s->board[k + 1] != '*' && s->board[k + 1]!= s->board[i]){
              printf("SWAP %d %d %d %d\n", row[temp], col[temp], row[k + 1], col[k + 1]);
              return 1;
            }
          }
        }
      }
    }
    return 0;
}