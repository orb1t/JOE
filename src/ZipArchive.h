/*
 * ZipArchive.h
 *
 *  Created on: Jun 25, 2010
 *      Author: joe
 */

#ifndef ZIPARCHIVE_H_
#define ZIPARCHIVE_H_

#include <map>
#include <string>

#include <stdint.h>

#include "ZipFile.h"

using namespace std;

class ZipArchive {
private:
	string archiveName;
	uint32_t size;
	int debug;
    void processCD(ifstream& in);
    std::map<string, ZipFile>::iterator iter;
public:
	std::map<string, ZipFile> fileTable;
	ZipArchive(char *);
	void iterator();
	bool hasNext();
	ZipFile next();
	~ZipArchive() {};
};

class ZipFileOpenFailure {
public:
	ZipFileOpenFailure() {};
};

#endif /* ZIPARCHIVE_H_ */
