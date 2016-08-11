#include "calc.h"

using namespace std;

int main(int argc, const char* argv[])
{
	if(argc > 1) {
		cout << calc(string(argv[1]));	
	} else {
		std::string input;
		getline(cin, input);
		
		vector<char> whitespaces = {' ', '\r', '\n', '\t'};
		stringstream ss;
		for(char c : input) {
			if(find(whitespaces.begin(), whitespaces.end(), c) == whitespaces.end()) {
				ss << c;
			}
		}
		cout << calc(ss.str());
	}

	return 0;
}