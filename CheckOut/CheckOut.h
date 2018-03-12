#pragma once
#include <string>
#include "../DbCore/DbCore.h"
#include "../RepositoryUtility/RepositoryUtility.h"
#include "../FileSystem/FileSystem.h"

using namespace NoSqlDb;

template <typename T>
class CheckOut {
public:
	using DbHandle = DbCore<T>;
	using Item = std::string;
	CheckOut(Item dstDir, Item fileName, int version,DbHandle& db) :dstDir_(dstDir),fileName_(fileName),version_(version),dbH(db){}
	bool processCheckOut();
private:
	DbHandle & dbH;
	Item clientDir_ { "../ClientStorage/" };
	Item dstDir_;
	Item fileName_;
	int version_;
};

template <typename T>
inline bool CheckOut<T>::processCheckOut() {
	int retVal = false;
	if (!dbH.contains(fileName_)) {
		std::cout << "\nFile not present in the repository\n";
		std::cout << "Please check the file in first\n";
		return false;
	}
	int currentVersion = dbH.getVersionNumber(fileName_);
	if (currentVersion < version_) {
		std::cout << "You have entered a version number higher than the most updated version of the file\n";
		std::cout << "Please enter a lower version number\n";
		return false;
	}
	std::string fileToCheckOut = RepoUtility::fileNameWithExtension(fileName_, currentVersion);
	std::string filePath = dstDir_ + fileToCheckOut;
	std::string nameWithoutVersion = RepoUtility::fileNameWithoutVersion(fileToCheckOut);
	if (FileSystem::Directory::exists(clientDir_ + "version" + RepoUtility::intToString(currentVersion))) {
		std::string fullCopyPath = clientDir_ + "version" + RepoUtility::intToString(currentVersion) + "/" + nameWithoutVersion;
		FileSystem::File::copy(filePath, fullCopyPath);
		retVal = true;
	}
	else {
		std::string newSubDir = clientDir_ + "version" + RepoUtility::intToString(currentVersion);
		std::string fullCopyPath = newSubDir + "/" + nameWithoutVersion;
		FileSystem::Directory::create(newSubDir);
		FileSystem::File::copy(filePath, fullCopyPath);
		retVal = true;
	}
	return retVal;
}