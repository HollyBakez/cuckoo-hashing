// Assignment 3: Cuckoo Hashing algorithm
// Doina Bein
// An open addressing method called Cuckoo Hashing
// INPUT: an input file containing strings of maximum 255 characters,
// one string per line
// OUTPUT: a detailed list of where the strings are inserted.

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

// cuckoo tables' size
const int tablesize = 17;
// combine the two 1-dimensional table into one 2-dimensional table
string  t[tablesize][2];

// define the prime number used in calculating the hash values
const int prime = 41;

// compute the hash functions
size_t f(string, size_t);

// place a string in one of the hash tables
bool place_in_hash_tables (string);

int main() {

  // the strings to be stored in the hash tables
  string s;
  size_t i, len;
  bool placed;

  // clear the tables
  for(i=0; i< tablesize; i++) {
      t[i][0] = "";
      t[i][1] = "";
  }

  char filename[255] = "";

   // display the header
  cout << endl << "CPSC 335.01 - Programming Assignment #3: ";
  cout << "Cuckoo Hashing algorithm" << endl;

  // read the strings from a file
  cout << "Input the file name (no spaces)!" << endl;
  cin >> filename;

  // open the file for reading
  ifstream infile(filename);
  /* read line by line from the file */
  while ( getline(infile,s) ) {
    // place null character at the end of the line instead of <return>
    len = s.size();

    // --TO HOLLAND-- MAKE SURE TO CHANGE line 60 back to s[len-1] from s[len] when you done BOI
     s[len-1]='\0';
    //s[len]='\0'; // you may need to change this line to s[len-1]='\0'


    // insert the string in the cuckoo table
    placed = place_in_hash_tables(s);
    // check whether the placement was successful
    if (!placed) {
      cout << "Placement has failed" << endl;
      return -1;
    }
  }
  infile.close();
  return 0;
}


bool place_in_hash_tables (string s) {

  bool placed;
  size_t pos;
  int index;
  string temp_s, temp;

  temp_s = s;

  // use a counter to detect loops; if counter >= 2*tablesize, then loop
  int counter = 0;

  // start with table T1
  index = 0;

  placed = false;

  pos = f(temp_s, index);

  while((!placed ) && (counter < 2*tablesize)) {
    if (t[pos][index] == "") {
      cout << "String <" << temp_s << "> will be placed at";
      cout  << " t[" << pos <<"][" << index << "]" << endl;
      t[pos][index] = temp_s;
      placed = true;
      return placed;
    }
    else {
      // entry at index <pos> in the <index> hash table is not available
      // so obtain the string stored over there in variable <temp> and place
      // the string <temp_s> there
      // now the string <temp> needs to be placed in the other table
      cout << "String <" << temp_s << "> will be placed at" << " t[" << pos;
      cout <<"][" << index << "]" << " replacing <" << t[pos][index] << ">";
      cout << endl;
      // YOU NEED TO WRITE THE CODE TO STORE IN temp THE STRING STORED AT
      // t[pos][index] AND STORE IN t[pos][index] THE STRING temp_s
      temp = t[pos][index];
      t[pos][index] = temp_s;

      // NOW temp_s CONTAINING THE EVICTED STRING NEEDS TO BE STORED
      // IN THE OTHER TABLE

      // WRITE THE CODE TO SET index TO INDICATE THE OTHER TABLE
      // index = 1 to indicate Table 2
      //index = 1;
      index = index?0:1;
      // WRITE THE CODE TO CALCULATE IN pos THE HASH VALUE FOR temp_s

      temp_s = temp;
      pos = f(temp_s, index);
      counter ++;
    }
  }
  return placed;
};


// compute the hash functions
// TO DO: complete the ELSE brach
size_t f(string s, size_t index) {
  size_t po, len; // position & length
  int i, val=0, temp; // iterator index, value, and a temp holder
  po = 1; // position starting at 1

  len = s.size(); // length the size of the string inputted

  if (index == 0) { // inserting in T1
    val = s[0]; // val = key[0]
    val = val % tablesize; // first character
    if (val < 0) val += tablesize; // value increases

    if (len == 1) 
      return val;

    for (i = 1; i < len; i++)
    {
      temp = s[i]; // Key[i]
      po *= prime; // 41 ^ i
      po = po % tablesize; // to make sure it doesn't exceed tablesize of 17

      if (po < 0) po += tablesize;

      val += temp * po; // summation of val =  key[i] * 41^i
      val = val % tablesize; //  f1 = val % tablesize
      if (val < 0) val += tablesize; // if f1 < 0 then f1 = f1 + tablesize

    }
    return val;
}
  else { // inserting in T2
    // TO DO: YOU NEED TO IMPLEMENT THE STEPS TO CALCULATE THE SECOND
    // HASH FUNCTION in <val>
    val = s[len-1]; // val = key of keysize - 1 

    val = val % tablesize; // f2 = val % tablesize

    if (val < 0) val += tablesize; // if f2 < 0 then f2 = f2 + tablesize

    if (len == 1) 
      return val;

    for (i = 1; i < len; i++)
    {
      temp = s[len-i-1]; // Key[keysize - i - 1]

      // Same as f1 
      po *= prime; // 41^i
      po = po % tablesize; //   f2 = val % tablesize 
      if (po < 0) po += tablesize;

      val += temp * po; // summation of val = key[keyysize-i-1] * 41^i

      val = val % tablesize; // f2 = val % tablesize

      if (val < 0) val += tablesize; // if f2<0 then f2 = f2 + tablesize

    }

    return val;
 }
}
