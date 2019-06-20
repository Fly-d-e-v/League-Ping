#pragma once

#include <vector>
#include <string>

enum Server {
	EUW,
	NA,
	KR,
	RU
};

class Ping {
public:
	
public:
	void LoadPingData();

	int GetLogPing() { return netLogPing; }
private:
	std::vector<std::string> serverList;

	int netLogPing;
};