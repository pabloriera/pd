#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

int main()
{

	std::ifstream t("file.txt");
	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
	        std::istreambuf_iterator<char>());

	std::cout << str << std::endl;

	auto j = json::parse(str);

	std::cout << j.dump(4) << std::endl;

	for (json::iterator it = j.begin(); it != j.end(); ++it) {
	  std::cout << it.key() << " : " << it.value() << "\n";
	}

	if (j.find("neurons") != j.end()) {
	
		json o = j["neurons"];
		
		for (json::iterator it = o.begin(); it != o.end(); ++it) {
		  std::cout << it.key() << " : " << it.value() << "\n";
		}

	}
	else 
		std::cout << "missing neurons field" << "\n";


	

	

	for (json::iterator it = j.begin(); it != j.end(); ++it) {
	  std::cout << *it << '\n';
	}

}