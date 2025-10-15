#include <CXXGraph/CXXGraph.hpp>
#include <vector>
#include <fstream> 
#include <iostream> 
#include <string>
#include <regex>

using namespace std



void Graph::Graph(string file_path){
	ifstream file(file_path); // open file

	if (!file) { // check if file opened successfully cerr << "Error opening file!" << endl; return 1;
	}

	string line;
	while (getline(file, line)) { // read line by line 
		//Split element by regex
		std::regex re_sep(R"\s*,\s*"); 
		std::sregex_token_iterator it{line.cbegin(), line.end(), re_sep, -1};
		std::sregex_token_iterator end;

		if(*it == "V"){
			
		}
		else if(*it == "E"){
			//Add Ro CxxGraph Attribute an edge
		}
	}
	file.close(); // always close after use

	// And here we have a our graph load
}

