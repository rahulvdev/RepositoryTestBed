#pragma once
/////////////////////////////////////////////////////////////////////
// Browse.h - Supports browsing feature for the Repository                        
// Author : Rajath Umesh Joshi, CSE687 - Object Oriented Design, Spring 2017    
// Dependencies :
// RepositoryUtility.h
// DbCore.h
/////////////////////////////////////////////////////////////////////

#include <string>
#include <cstdlib>
#include "../DbCore/DbCore.h"
#include "../RepositoryUtility/RepositoryUtility.h"


using namespace NoSqlDb;

template <typename T>
class Browse {
public:
	using Item = std::string;
	using DbHandle = DbCore<T>;
	Browse(DbHandle& db,const Item& repoPath) :dbH(db),repoPath_(repoPath){}                   
	void showPackageMetadata();
	void showAllPackages();
	void showFullPackageText(Item packageName,int version);
private:
	Item repoPath_;
	DbHandle& dbH;
};

// This function shows the details of each package in the repository.
template<typename T>
inline void Browse<T>::showPackageMetadata()
{
	Item packageName;
	size_t version = 1;
	std::cout << "Enter the name of the package whose metadata you want to see\n";
		std::cin >> packageName;
	if (!RepoUtility::checkFileExistance(packageName,dbH))
		return;

	std::cout << "Enter the version number of the package\n";
	while (!std::cin >> version)
		std::cout << "Please enter a number for the version\n";

	if (dbH.getVersionNumber(packageName) < version) {
		std::cout << "The version number you have entered is invalid\n";
		std::cout << "Please enter a valid number\n";
		return;
	}
	std::cout << "------------------------------\n";
	std::cout << "Showing packagae metadata\n";
	std::cout << "------------------------------\n";
	DbElement<T> dbElem = dbH[packageName];
	RepoUtility::showHeader();
	RepoUtility::showElem(dbElem);
}

// This function shows all the packages in the repository.
template<typename T>
inline void Browse<T>::showAllPackages()
{	
	std::cout << "\n------------------------------\n";
	std::cout << "\nShowing all packages in the repository\n";
	std::cout << "------------------------------\n";
	RepoUtility::showKeys(dbH);

}
// This function shows full package text.
template<typename T>
inline void Browse<T>::showFullPackageText(Item packageName,int version)
{
	Item packageName;
	size_t version = 1;
	std::cout << "\nEnter the name of the package whose text you want to see\n";
		std::cin >> packageName;
		std::cin.ignore(INT_MAX,'\n');
	if (!RepoUtility::checkFileExistance(packageName, dbH))
		return;

	std::cout << "Enter the version number of the package\n";
	while (!(std::cin >> version)) {
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Invalid input.Try again: ";
	}

	if (dbH.getVersionNumber(packageName) < version) {
		std::cout << "The version number you have entered is invalid\n";
		std::cout << "Please enter a valid number\n";
		return;
	}

	Item fullPathToPackage = repoPath_ + RepoUtility::fileNameWithExtension(packageName, version);
	Item cmd = "notepad \"" + fullPathToPackage + "\"";
	system(cmd.c_str());
}
