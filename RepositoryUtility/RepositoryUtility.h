#pragma once
/////////////////////////////////////////////////////////////////////
// RepositoryUtility.h -  Utility methods that assist in the process of file Checkin , Checkout and browse.                   
// Author : Rajath Umesh Joshi, CSE687 - Object Oriented Design, Spring 2017    
// Dependencies :
// DbCore.h
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <sstream>
#include <cstddef>
#include <iomanip>
#include <functional>
#include "../DbCore/DbCore.h"

using namespace NoSqlDb;

namespace RepoUtility {

	typedef std::string Item;
	// It verifies whether package is present in the repository.
	template <typename T>
	bool checkFileExistance(Item fileName,DbCore<T>& dbH) {
		if (!dbH.contains(fileName)) {
			std::cout << "Package not present in the repsoitory\n";
			std::cout << "Please check-in and retry\n";
			return false;
		}
		return true;
	}

	std::string intToString(int val) {
		std::ostringstream os;
		os << val;
		return os.str();
	}

	std::string fileNameWithExtension(Item fileName, int version) {
		size_t extensionIndex = fileName.find('.');
		Item updatedName=fileName.substr(0, extensionIndex) + RepoUtility::intToString(version) + fileName.substr(extensionIndex);
		return updatedName;
	}
	
	std::string fileNameWithoutVersion(Item fileName) {
		size_t extensionIndex = fileName.find('.');
		Item updatedName = fileName.substr(0, extensionIndex-1) + fileName.substr(extensionIndex);
		return updatedName;
	}

	template<typename T>
	void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
	{
		out << "\n  ";
		for (auto key : db.keys())
		{
			out << key << " ";
		}
	}
	//----< show record header items >-----------------------------------

	inline void showHeader(std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(26) << std::left << "DateTime";
		out << std::setw(10) << std::left << "Name";
		out << std::setw(25) << std::left << "Description";
		out << "\n  ";
		out << std::setw(26) << std::left << "------------------------";
		out << std::setw(10) << std::left << "--------";
		out << std::setw(25) << std::left << "-----------------------";
	}
	//----< display DbElements >-----------------------------------------

	template<typename T>
	void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(26) << std::left << std::string(el.dateTime());
		out << std::setw(10) << std::left << el.name();
		out << std::setw(25) << std::left << el.descrip();

		typename DbElement<T>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n    child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
		out << "\n\n--------------Payload-------------------\n\n";
		Payload pl = el.payLoad();
		Item checkInStatus = pl.checkInStatus() ? "open" : "close";
		out << "File Path -> " << pl.filePath() << "\n";
		out << "Check-In Status ->" << checkInStatus.c_str() << "\n";

		typename Payload::Categories catgeories = pl.category();
		for (auto category : catgeories) {
			out << category << "\n";
		}
	}
	//----< display all records in database >----------------------------

	template<typename T>
	void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
	{
		showHeader(out);
		typename DbCore<T>::DbStore dbs = db.dbStore();
		for (auto item : dbs)
		{
			showElem(item.second, out);
		}
	}

}