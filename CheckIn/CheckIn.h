#pragma once
#include <string>
#include <vector>
#include <cstddef>
#include "../DbCore/DbCore.h"
#include "../DateTime/DateTime.h"
#include "../FileSystem/FileSystem.h"
#include "../RepositoryUtility/RepositoryUtility.h"

using namespace NoSqlDb;

template <typename T>
class CheckIn {
public:
	using DbHandle=DbCore<T>;
	using Item = std::string;
	using Items = std::vector<Item>;
	CheckIn(Item dstDir,Item author, Item description,Items children, Items categories, Item filePath, bool checkInStatus,DbHandle& db);
	~CheckIn();
	CheckIn(const CheckIn& chk);
	CheckIn<T>& operator=(const CheckIn<T>& chk);
	DbElement<T> createDbElement(Item fileName);
	bool commit();
	bool checkIfFileExists();
	bool verifyAuthor(Item fileName);
	bool dependancyExistenceCheck(Item fileName);
	std::string getFileNameOnly(std::string fullPath);

private:
	Items categories_;
	Item description_;
	Items children_;
	Item author_;
	Item fileSrc;
	Item repoPath;
	bool open;
	DbHandle& dbH;

};

template <typename T>
CheckIn<T>::CheckIn(Item dstDir,Item author, Item description,Items children, Items categories, Item filePath, bool checkInStatus, DbHandle& db) :
	repoPath(dstDir),author_(author),description_(description),children_(children) ,categories_(categories), fileSrc(filePath), open(checkInStatus), dbH(db) {

}

template <typename T>
CheckIn<T>::~CheckIn() {
	std::cout << "Destructor for CheckInObject" << std::endl;
}


template <typename T>
CheckIn<T>::CheckIn(const CheckIn& chk) {
	fileSrc = chk.fileSrc;
	open = chk.fileSrc;
}

template <typename T>
CheckIn<T>& CheckIn<T>::operator=(const CheckIn& chk) {
	fileSrc = chk.fileSrc;
	open = chk.fileSrc;
}

template <typename T>
DbElement<T> CheckIn<T>::createDbElement(Item fileName)
{
	if (!checkIfFileExists())
		throw(std::exception("Not a valid file path"));
	if (!verifyAuthor(fileName))
		throw(std::exception("Provided author does not have permission to checkIn"));

	DbElement<T> dbElem;
	dbElem.name(author_);
	dbElem.descrip(description_);
	dbElem.dateTime(DateTime().now());
	dbElem.children(children_);
	T pl(categories_, fileSrc, open);
	dbElem.payLoad(pl);

	return dbElem;
}


template <typename T>
bool CheckIn<T>::commit() {
	bool retVal = false;
	try {
		std::string fileName = getFileNameOnly(fileSrc);
		DbElement<T> dbElem = createDbElement(fileName);
		bool isFilePresent = dbH.contains(fileName);
		if (isFilePresent && open == true && dependancyExistenceCheck(fileName)) {
			//Do not change file version
			dbH[fileName] = dbElem;
			size_t currentVersion = dbH.getVersionNumber(fileName);
			size_t extensionIndex = fileName.find('.');
			std::string fileNameWithExtension = RepoUtility::fileNameWithExtension(fileName, currentVersion);
			std::string destination = repoPath + fileNameWithExtension;
			FileSystem::File::remove(destination);
			FileSystem::File::copy(fileSrc,destination);
			retVal = true;
		}
		else if (isFilePresent && open == false && dependancyExistenceCheck(fileName)) {
			dbH[fileName] = dbElem;
			//size_t preVersion = dbH.getVersionNumber(fileName);
			size_t updatedVersion=dbH.updateVersionNumber(fileName);
			//Updated version file
			std::string fileNameWithExtension = RepoUtility::fileNameWithExtension(fileName, updatedVersion);
			//Previous version file
			//std::string  prevFile= RepoUtility::fileNameWithExtension(fileName, preVersion);
			//std::string prevFilePath = repoPath + prevFile;
			std::string destination = repoPath + fileNameWithExtension;
			//FileSystem::File::remove(prevFilePath);
			FileSystem::File::copy(fileSrc,destination);
			retVal = true;
			//Copy updated file to repository directory with updated name to reflect version
		}
		else if(!isFilePresent){
				dbH[fileName] = dbElem;
				dbH.insertFileVersionPair(fileName);
				size_t extensionIndex = fileName.find('.');
				std::string fileNameWithVersion = fileName.substr(0, extensionIndex) + '1' + fileName.substr(extensionIndex);
				std::string destination = repoPath + fileNameWithVersion;
				FileSystem::File::copy(fileSrc, destination);
				retVal = true;
		}
		else {
			retVal = false;;
		}
		return retVal;
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
		return false;
	}
}


template <typename T>
bool CheckIn<T>::verifyAuthor(Item fileName) {
	bool retVal = false;
	if (dbH.contains(fileName) && dbH[fileName].name() == author_)
		retVal = true;
	else if (!dbH.contains(fileName))
		retVal = true;
	else
		retVal = false;

	return retVal;
}

template <typename T>
bool CheckIn<T>::checkIfFileExists() {
	if (FileSystem::File::exists(fileSrc))
		return true;
	else
		return false;
}

template <typename T>
std::string CheckIn<T>::getFileNameOnly(std::string fullPath) {
	std::cout << "The full path of the source file is " << fullPath << std::endl;
	std::size_t found = fullPath.find_last_of("\\");
	return fullPath.substr(found + 1);
}

template <typename T>
bool CheckIn<T>::dependancyExistenceCheck(Item fileName) {
	DbElement<T> dbElem = dbH[fileName];
	typename CheckIn<T>::Items children = dbElem.children();
	for (const auto& child : children) {
		if (!dbH.contains(child))
			return false;
	}
	return true;
	
}

