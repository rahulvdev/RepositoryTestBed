#pragma once
/////////////////////////////////////////////////////////////////////
// RepositoryCore.h - Repository core  supports calls for CheckIn, CheckOut and Browse.                    
// Author : Rajath Umesh Joshi, CSE687 - Object Oriented Design, Spring 2017    
// Dependencies :
// RepositoryUtility.h
// DbCore.h
// FileSystem.h
// Checkin.h
// CheckOut.h
// Browse.h
/////////////////////////////////////////////////////////////////////
#include "../DbCore/DbCore.h"
#include "../CheckIn/CheckIn.h"
#include "../CheckOut/CheckOut.h"
#include "../Browse/Browse.h"
#include <string>


using namespace NoSqlDb;

namespace Repository {

	class RepositoryCore {
	public:
		using Item = std::string;
		using Items = std::vector<Item>;
		using DbCore_ = DbCore<Payload>;
		RepositoryCore();
		bool checkIn(Item author, Item description, Items children, Items categories, Item fileName, bool checkInStatus);
		bool checkOut(Item fileName,int version);
		void browse();

	private:
		Item repoPath = "../FileRepository/";
		DbCore_ db;
	};


	RepositoryCore::RepositoryCore(){}


	// This function initiates the checkin process.
	bool RepositoryCore::checkIn(Item author, Item description, Items children,Items categories, Item fileName, bool checkInStatus)
	{
		std::cout << "Initiating check-in process" << std::endl;
		CheckIn<Payload> chkIn(repoPath,author,description,children, categories, fileName, checkInStatus, db);
		if (!chkIn.commit())
			return false;
		else
			return true;
		
	}
	// This function initiates Checkout process.
	bool RepositoryCore::checkOut(Item fileName, int version)
	{
		std::cout << "Initiating checkout process" << std::endl;
		CheckOut<Payload> chkOut(repoPath, fileName, version, db);
		if (!chkOut.processCheckOut())
			return false;
		else
			return true;
	}
	//  This function initiates browse.
	void RepositoryCore::browse()
	{
		int browseOption;
		std::cout << "\n----------------------------------------------------------------------------------------------\n";
		std::cout << "\nThe following browse types are supported\n";
		std::cout << "1. Display package metadata\n";
		std::cout << "2. Display all files in the repository\n";
		std::cout << "3. Display full package text\n";
		std::cout << "Enter the option number to choose what type of browsing activity you would like to perform\n";
		std::cout << "----------------------------------------------------------------------------------------------\n";
		std::cin >> browseOption;
		Browse<Payload> br(db, repoPath);
		switch (browseOption) {
		case 1: br.showPackageMetadata();
				break;
		case 2:	br.showAllPackages();
				break;
		case 3:	br.showFullPackageText();
				break;
		default:
				std::cout << "Pleas enter any option between 1 and 3\n";
				break;

		}
	}


}
