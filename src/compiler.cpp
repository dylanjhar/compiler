//============================================================================
// Name        : compiler.cpp
// Assignment  : #2
// Author      : Dylan Harper
// Description : The first part of a compiler, the lexical analyzer
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class LexAnalyzer{
private:
	vector<string> lexemes;  // source code file lexemes
	vector<string> tokens;   // source code file tokens
	map<string, string> tokenmap;  // valid lexeme/token pairs

public:
	//pre: parameter refers to open data file consisting of token and lexeme pairs i.e.  s_and and t_begin begin t_int 27.
	//	   Each pair appears on its own input line.
	//post: tokenmap has been populated
	LexAnalyzer(istream& infile) {
		string key, value;
		while(infile >> key >> value) {
			tokenmap[key] = value;
		}
//		map<string, string>::iterator mitr;
//		for(mitr = tokenmap.begin(); mitr != tokenmap.end(); ++mitr) {
//			cout << mitr->first << " " << mitr->second << endl;
//		}
	}


	void scanFile(istream& infile, ostream& outfile);
	//pre: 1st parameter refers to an open text file that contains source
	//	   code in the language, 2nd parameter refers to an open empty output file
	//post: If no error, the token and lexeme pairs for the given input
	//		file have been written to the output file and the vectors have been populated.
	// 		If there is an error, incomplete token/lexeme pairs are written to the output file and populated in the vectors.
	//		An error message is also written to the file. A success or fail message has printed to the console.
};

//pre:
//post:
ifstream infilePrompt(string fileName) {
	string file;
	cout << "enter the " << fileName << " file: ";
	cin >> file;
	ifstream infile(file);
		if(!infile) {
			cout << "unable to get file" << endl;
			exit(-1);
		} else {
			return infile;
		}
}

int main() {
	ifstream tlpFile = infilePrompt("token lexeme pair");
	LexAnalyzer lex(tlpFile);

	return 0;
}
