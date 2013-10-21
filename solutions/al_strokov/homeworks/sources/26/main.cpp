#include <iostream>
#include <fstream>
#include <stdint.h>
#include <set>
#include <map>
#include <string>
#include <vector>

struct dataStruct_t {
	uint32_t type;
	uint32_t time;
	std::string msg;
};

int main(int argc, char** argv) {
	std::cout << "*Hometask 25*" << std::endl;

	std::ifstream iFile;
	iFile.open( SOURCE_DIR "/sources/25/input.txt",
			std::ios::in | std::ios::binary);

	if (!iFile.is_open()) {
		std::cout << "cannot open input bin file" << std::endl;
		return false;
	}

	std::string sBuff;
	uint32_t tp;
	uint32_t tm;
	uint32_t ln;
	uint32_t messageSize;
	dataStruct_t dataRecord;
	uint32_t currentTime = 0;
	uint32_t secondsCount = 0;
	static const int buffSize = 2048;
	std::map<int, int> buffersSizes;
	std::map<int, int> messagesCount;

	while (!iFile.eof()) {
		iFile.read((char*) &tp, sizeof tp);
		iFile.read((char*) &tm, sizeof tm);
		iFile.read((char*) &ln, sizeof ln);

		char* buff = new char[ln];
		iFile.read(buff, ln);
		sBuff = buff;
		sBuff = sBuff.substr(0, ln);
		delete[] buff;

		if ((iFile.eof())) {
			continue;
		}

		//new second
		if (currentTime != tm) {
			currentTime = tm;
			secondsCount++;
			buffersSizes.clear();

			std::cout << "--new second--" << std::endl;
		}

		messageSize = sizeof tp + sizeof tm + sizeof ln + (sizeof(char)) * ln;
		std::cout << "SZ[" << messageSize << "]\t";

		std::cout << "tp:" << tp << "\ttm:" << tm << "\tln:" << ln << "\t["
				<< sBuff << "]\t\t";

		if (!buffersSizes.count(tp)) {
			buffersSizes[tp] = buffSize - messageSize;
			messagesCount[tp] += 1;
		} else {
			if (buffersSizes[tp] - messageSize >= 0) {
				buffersSizes[tp] = buffersSizes[tp] - messageSize;
				messagesCount[tp] += 1;
			} else {
				std::cout << "cannot save message" << std::endl;
			}
		}
		std::cout << "buff left [" << buffersSizes[tp] << "] for mt[" << tp << "]"
				<< std::endl;
	}

	iFile.close();

	std::cout << "\n--report--" << std::endl;
	std::cout << "secCount\t" << secondsCount << std::endl;
	std::cout << "--msg stored--" << std::endl;
	std::cout << "[type]\tsaved\tavg" << std::endl;
	for (std::map<int, int>::const_iterator it = messagesCount.begin();
			it != messagesCount.end(); ++it) {
		std::cout << "[" << it->first << "]\t" << it->second << "\t"
				<< (double) it->second / secondsCount << std::endl;
	}

	std::ofstream oFile;
	oFile.open( SOURCE_DIR "/sources/25/output.txt", std::ios::out | std::ios::binary);

	if (!oFile.is_open()){
		std::cout << "cannot open output file" << std::endl;
		return 1;
	}

	double avgMsg;
	for (std::map<int, int>::const_iterator it = messagesCount.begin();
			it != messagesCount.end(); ++it) {
		oFile.write( (char*) &it->first, sizeof it->first);
		avgMsg = (double) it->second / secondsCount;
		oFile.write( (char*) &avgMsg, sizeof avgMsg );
	}
	oFile.close();

	std::cout << "\n--EOF--" << std::endl;

	return 0;
}
