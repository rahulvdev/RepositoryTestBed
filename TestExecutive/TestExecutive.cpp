// TestExecutive.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "../RepositoryCore/RepositoryCore.h"
#include "../FileSystem/FileSystem.h"


typedef std::string Item;
typedef std::vector<std::string> Items;

using namespace FileSystem;

void sendCheckInRequest(Repository::RepositoryCore& rp, Item author, Item descr, Item filePath, Items children, Items categories, bool checkInStatus) {
	rp.checkIn(author, descr, children, categories, filePath, checkInStatus);
}

int main() {
	std::cout << "\n---------------------------CheckIn------------------------------\n";
    Repository::RepositoryCore rpCore;
	Items dep1 = { "DbCore.h","DateTime.h","FileSystem.h","RepositoryUtility.h" };
	Items cat1 = { "CheckIn package","Performs core checkin functionality" };
	sendCheckInRequest(rpCore, "Rajath Joshi", "This is the first commit request for CheckIn package","../CheckIn/CheckIn.h", dep1, cat1,false);
	Items dep2 = { "DbCore.h","CheckIn.h","CheckOut.h","Browse.h" };
	Items cat2 = { "Core repository package","CheckIn,CheckOut,Browse support" };
	sendCheckInRequest(rpCore, "Rajath Joshi", "This is the first commit request for RepositoryCore package","../RepositoryCore/RepositoryCore.h", dep2, cat2, true);
	Items dep3 = { "DbCore.h","RepositoryUtility.h","FileSystem.h"};
	Items cat3 = { "Core repository package","CheckIn,CheckOut,Browse support" };
	sendCheckInRequest(rpCore, "Rajath Joshi", "This is the first commit request for CheckOut package","../CheckOut/CheckOut.h", dep3, cat3, false);
	Items dep4 = { "DateTime.h"};
	Items cat4 = { "Core database","NoSQL","Storage" };
	sendCheckInRequest(rpCore, "Rajath Joshi", "Push DbCore","../DbCore/DbCore.h", dep4, cat4, true);
	Items dep5 = { };
	Items cat5 = { "DateTime","Utility"};
	sendCheckInRequest(rpCore, "Umesh Joshi", "Push DateTime","../DateTime/DateTime.h", dep5, cat5, true);
	Items dep6 = {};
	Items cat6 = { "DateTime","Utility" };
	sendCheckInRequest(rpCore, "Umesh Joshi", "Push DateTime","../DateTime/DateTime.h", dep6, cat6, false);
	Items dep7 = {"DbCore.h"};
	Items cat7 = { "DbCore cpp file","cpp" };
	sendCheckInRequest(rpCore, "Rajath Joshi", "Push DbCore.cpp", "../DbCore/DbCore.cpp", dep7, cat7, true);
	Items dep8 = { "DateTime.h" };
	Items cat8 = { "Core database","NoSQL","Storage" };
	sendCheckInRequest(rpCore, "Rajath Joshi", "Update DbCore public interface","../DbCore/DbCore.h", dep8, cat8, false);
	Items dep9 = { "DateTime.h" };
	Items cat9 = { "Core database","NoSQL","Storage" };
	sendCheckInRequest(rpCore, "Rajath Joshi", "Modify DbCore apis","../DbCore/DbCore.h", dep8, cat8, false);
	std::cout << "\n---------------------------CheckOut------------------------------\n";
	rpCore.checkOut("DbCore.h", 3);
	rpCore.checkOut("DateTime.h", 1);
	rpCore.checkOut("RepositoryCore.h", 2);
	rpCore.checkOut("RepositoryCore.h", 1);
	rpCore.checkOut("CheckIn.h", 1);
	rpCore.checkOut("CheckOut.h", 1);

    rpCore.browse();
}


