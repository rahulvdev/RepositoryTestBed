#pragma once
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
	Browse(DbHandle db,const Item& repoPath) :dbH(db),repoPath_(repoPath){}
	void showPackageMetadata();
	void showAllPackages();
	void showFullPackageText();
private:
	Item repoPath_;
	DbHandle& dbH;
};

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

template<typename T>
inline void Browse<T>::showAllPackages()
{	
	std::cout << "------------------------------\n";
	std::cout << "\nShowing all packages in the repository\n";
	std::cout << "------------------------------\n";
	RepoUtility::showKeys(dbH);

}

template<typename T>
inline void Browse<T>::showFullPackageText()
{
	Item packageName;
	size_t version = 1;
	std::cout << "Enter the name of the package whose metadata you want to see\n";
		std::cin >> packageName;
	if (!RepoUtility::checkFileExistance(packageName, dbH))
		return;

	std::cout << "Enter the version number of the package\n";
	while (!std::cin >> version)
		std::cout << "Please enter a number for the version\n";

	if (dbH.getVersionNumber(packageName) < version) {
		std::cout << "The version number you have entered is invalid\n";
		std::cout << "Please enter a valid number\n";
		return;
	}

	Item fullPathToPackage = repoPath_ + RepoUtility::fileNameWithExtension(packageName, version);
	Item cmd = "notepad \"" + fullPathToPackage + "\"";
	system(cmd.c_str());
}
