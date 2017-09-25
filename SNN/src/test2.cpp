#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

int main()
{

	json j;
	json o;

    json k = {{"from",0},{"to",1},{"w",30}};
    o.push_back(k);
    k = {{"from",0},{"to",2},{"w",0}};
    o.push_back(k);

    for (json::iterator it = o.begin(); it != o.end(); ++it)
    {
    	std::cout << (*it)["from"] << '\n';

    }



	j["s"] = o;


	// std::cout << j.dump(4) << '\n';

	return 0;
}