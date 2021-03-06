#include "stdafx.h"
#include "RepositoryCore.h"

#ifdef TEST_REPOSITORYCORE

int main() {
	int i = 3;
	Repository::RepositoryCore rpCore;
	while (i>0) {
		std::string author;
		std::string description;
		std::string filePath;
		std::string dependancies;
		std::vector<std::string> categories;
		std::vector<std::string> children;
		std::string checkInStatus;
		std::cout << "Main method in repository\n";

		std::cout << "Enter author name\n";
		std::cin >> author;
		std::cin.ignore();
		std::cout << "Enter Description\n";
		std::getline(std::cin, description);
		std::cout << "Enter file path\n";
		std::cin >> filePath;
		std::cout << "Enter all files your checkin file depends on.Enter end when youre done!\n";
		while (std::cin >> dependancies) {
			if (dependancies.compare("end") == 0)
				break;
			children.push_back(dependancies);
		}
		std::cout << "----------------------------------------------------------------------------------------------------\n";
		std::cout << "\nBelow are the children i.e dependancies of file  " << filePath << std::endl;
		std::cout << "----------------------------------------------------------------------------------------------------\n";
		for (auto& str : children) {
			std::cout << str << "\n";
		}
		std::cout << "Enter check-in status as open or close\n";
		std::cin >> checkInStatus;
		categories.push_back("");
		categories.push_back("cpp file");
		bool checkInType = checkInStatus.compare("open") != 0 ? false : true;
		rpCore.checkIn(author, description, children, categories, filePath, checkInType);
		i--;
	}
	std::string fileToCheckOut;
	int versionToCheckOut;
	std::cout << "Enter file for check out\n";
	std::cin >> fileToCheckOut;
	std::cout << "Enter version number of file you want to check out\n";
	std::cin >> versionToCheckOut;
	rpCore.checkOut(fileToCheckOut,versionToCheckOut);
	while (true) {
		rpCore.browse();
	}
}

#endif