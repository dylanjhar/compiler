//============================================================================
// Name        : compiler.cpp
// Assignment  : #2
// Author      : Dylan Harper
// Description : The first part of a compiler, the lexical analyzer which scans
//				 source code for validity and outputs the scan results
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

	//pre: param is an open output file
	//post: lexemes and tokens vectors are written to output file
	void print(ostream& outfile) {
		vector<string>::iterator vitr;
		int i = 0;
		for(vitr = tokens.begin(); vitr != tokens.end(); ++vitr) {
			outfile << *vitr << " : " << lexemes[i] << endl;
			i++;
		}
	}

	//pre: first param is open input file with source code, second param is open output file,
	//	   third param is the current character in the input file, fourth param contains
	//	   the supposed lexeme in the input file
	//post: if valid int lexeme, vectors are populated with token/lexeme and if not, error message
	//		is printed to console as well as the output file, print() is called
	void intDetect(istream& infile, ostream& outfile, char& ch, string& lexeme) {
		lexeme.push_back(ch);
		infile.get(ch);
		while(!infile.eof() && isdigit(ch)) {
			lexeme.push_back(ch);
			infile.get(ch);
		}
		if(isalpha(ch)) {
			outfile << "error - integers can only consist of numbers/IDs cannot begin with a number" << endl;
			print(outfile);
			cout << "error - program scan not completed" << endl;
			exit(-1);
		} else {
			tokens.push_back("t_int");
			lexemes.push_back(lexeme);
			lexeme = "";
		}
	}

	//pre: first param is open input file with source code, second param is open output file,
	//	   third param is the current character in the input file, fourth param contains
	//	   the supposed lexeme in the input file
	//post: if valid string, vectors are populated with token/lexeme and if not, error message
	//		is printed to console as well as the output file, print() is called
	void strDetect(istream& infile, ostream& outfile, char& ch, string& lexeme) {
		lexeme.push_back(ch);
		infile.get(ch);
		while(!infile.eof() && ch != '"') {
			lexeme.push_back(ch);
			infile.get(ch);
		}
		if(ch != '"') {
			outfile << "error - string must be enclosed in double quotes" << endl;
			print(outfile);
			cout << "error - program scan not completed" << endl;
			exit(-1);
		} else {
			lexeme.push_back(ch);
			tokens.push_back("t_str");
			lexemes.push_back(lexeme);
			lexeme = "";
			infile.get(ch);
		}
	}

	//pre: first param is open input file with source code, second param is open output file,
	//	   third param is the current character in the input file, fourth param contains
	//	   the supposed lexeme in the input file
	//post: if valid token, vectors are populated with token/lexeme
	void lexemeDetect(istream& infile, ostream& outfile, char& ch, string& lexeme) {
		map<string, string>::iterator mitr;

		lexeme.push_back(ch);
		infile.get(ch);
		if(!isalpha(lexeme[0]) && ch == '=') {
			lexeme.push_back(ch);
			infile.get(ch);
		}
		for(mitr = tokenmap.begin(); mitr != tokenmap.end() && lexeme != ""; ++mitr) {
			if(lexeme == mitr->second) {
				tokens.push_back(mitr->first);
				lexemes.push_back(mitr->second);
				lexeme = "";
			}
		}
		if(lexeme != "") {
			while(!infile.eof() && lexeme != "" && ch != '=' && ch != ' ' && ch != '\n' && ch != '\t') {
				lexeme.push_back(ch);
				for(mitr = tokenmap.begin(); mitr != tokenmap.end() && lexeme != ""; ++mitr) {
					if(lexeme == mitr->second) {
						tokens.push_back(mitr->first);
						lexemes.push_back(mitr->second);
						lexeme = "";
					}
				}
				infile.get(ch);
			}
		}
	}

	//pre: first param is open input file with source code, second param is open output file,
	//	   third param is the current character in the input file, fourth param contains
	//	   the supposed lexeme in the input file
	//post: if valid id, vectors are populated with token/lexeme and if not, error message
	//		is printed to console as well as the output file, print() is called
	void idDetect(istream& infile, ostream& outfile, char& ch, string& lexeme) {
		if(lexeme != "" && isalpha(lexeme[0])) {
			while(!infile.eof() && (isalpha(ch) || isdigit(ch)) && ch != ' ' && ch != '\n' && ch != '\t') {
				lexeme.push_back(ch);
				infile.get(ch);
			}
			for(int i = 1; i < lexeme.length(); i++) {
				if(!isdigit(lexeme[i]) && !isalpha(lexeme[i])) {
					outfile << "error - IDs must consist of only letters and numbers" << endl;
					print(outfile);
					cout << "error - program scan not completed" << endl;
					exit(-1);
				}
			}
			tokens.push_back("t_id");
			lexemes.push_back(lexeme);
			lexeme = "";
		} else if(lexeme != "") {
			outfile << "error - not a valid lexeme" << endl;
			print(outfile);
			cout << "error - program scan not completed" << endl;
			exit(-1);
		}
	}

public:
	//pre: parameter refers to open data file consisting of token and lexeme pairs i.e.  s_and and t_begin begin t_int 27.
	//	   Each pair appears on its own input line.
	//post: tokenmap has been populated
	LexAnalyzer(istream& infile) {
		string key, value;
		infile >> key >> value;
		tokenmap[key] = value;
		while(!infile.eof()) {
			infile >> key >> value;
			tokenmap[key] = value;
		}
	}

	//pre: 1st parameter refers to an open text file that contains source
	//	   code in the language, 2nd parameter refers to an open empty output file
	//post: If no error, the token and lexeme pairs for the given input file
	//		have been written to the output file and the vectors have been populated.
	// 		If there is an error, incomplete token/lexeme pairs are written to the output file and populated in the vectors.
	//		An error message is also written to the file. A success or fail message has printed to the console.
	void scanFile(istream& infile, ostream& outfile) {
		char ch;
		string lexeme = "";

		infile.get(ch);
		while(ch == ' ' || ch == '\n' || ch == '\t') {
			infile.get(ch);
		}

		while(!infile.eof()) {
			if(isdigit(ch)) {
				intDetect(infile, outfile, ch, lexeme);
			} else if(ch == '"') {
				strDetect(infile, outfile, ch, lexeme);
			} else {
				lexemeDetect(infile, outfile, ch, lexeme);
				idDetect(infile, outfile, ch, lexeme);
			}
			while(!infile.eof() && (ch == ' ' || ch == '\n' || ch == '\t')) {
				infile.get(ch);
			}
		}
		print(outfile);
		cout << "program scan complete" << endl;
	}

};

//pre: param is apart of the prompt telling user what file to enter
//post: infile is returned with file that user entered
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

	ifstream srcFile = infilePrompt("source code");

	string file;
	cout << "enter the output file: ";
	cin >> file;
	ofstream outFile(file);
		if(!outFile) {
			cout << "unable to get file" << endl;
			exit(-1);
		}

	lex.scanFile(srcFile, outFile);

	return 0;
}
