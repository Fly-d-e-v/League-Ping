#include <experimental/filesystem>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <fstream>

namespace fs = std::experimental::filesystem;

#include "Ping.h"

#define MAX_DIR_LIST_SIZE 20
#define NETLOG_OFFSET 27
#define PING_VAR_INDEX 8

void Ping::LoadPingData() {
	// Temporary path
	fs::path netLogPath = "C:/Riot Games/League of Legends/League of Legends/Logs/GameLogs";

	std::vector<fs::path> directoryVector;

	fs::directory_iterator dirItr(netLogPath);

	std::stack<fs::directory_entry> dirEntryStack;

	for (auto &i : dirItr) {
		dirEntryStack.push(i);
	}

	size_t listSize = dirEntryStack.size();

	if (listSize > MAX_DIR_LIST_SIZE) {
		listSize = MAX_DIR_LIST_SIZE;
	}

	std::vector<fs::path> dirList;

	for (size_t i = 0; i < listSize; i++) {
		dirList.push_back(dirEntryStack.top().path());
		dirEntryStack.pop();
	}

	dirEntryStack.empty();

	for (auto &j : dirList) {
		std::cout << j << "\n";
	}

	// Process Files from dirList
	if (dirList.size() == 0)
		return;

	std::vector<int> pingList;

	for (int i = 0; i < dirList.size(); i++) {
		fs::directory_iterator dirListDirItr(dirList[i]);

		fs::path netLogPath = "";

		for (auto &j : dirListDirItr) {
			std::string pathToString = j.path().string();
			if (pathToString.substr(pathToString.size() - 10) == "netlog.txt") {
				std::cout << pathToString << "\n";
				netLogPath = j.path();
				break;
			}	
		}

		if (netLogPath == "")
			continue;

		std::ifstream netLogFile(netLogPath);
		std::string line;
		std::vector<std::string> lineList;

		int counter = 0;

		if (netLogFile.is_open()) {
			while (std::getline(netLogFile, line)) {
				if (counter > NETLOG_OFFSET) // Not optimal
					lineList.push_back(line);

				counter++;
			}

			netLogFile.close();
		}
		else {
			continue;
		}

		for (auto &j : lineList) {
			std::string delimiter = ",";

			size_t position = 0;
			std::string token;

			std::vector<std::string> parts;
			while ((position = j.find(delimiter)) != std::string::npos) {
				token = j.substr(0, position);
				parts.push_back(token);
				j.erase(0, position + delimiter.length());
			}

			if (parts.size() < 9)
				continue;

			pingList.push_back(std::stoi(parts[8]));
		}
	}

	std::sort(pingList.begin(), pingList.end());

	if (pingList.size() % 2 == 0) {
		netLogPing = pingList[pingList.size() / 2];
	}
	else {
		int lowerIndex = pingList.size() / 2;

		netLogPing = (pingList[lowerIndex] + pingList[lowerIndex + 1]) / 2;
	}

	std::cout << "Log Ping Median: " << netLogPing << "\n";
}