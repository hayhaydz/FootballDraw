#ifndef IMPORTDATA_H
#define IMPORTDATA_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Team.h"

	// This class handles importing the necessary data for the program. It stores it in a passed through default vector for the teams. It also adds up the
	// teams elo's.

#pragma once
class ImportData {
	public:
		void import(std::vector<Team> &PlaceholderTeams, int &totalELO);
	private:
		Team DefaultTeam;
		std::ifstream file;
		std::string substr;
		int counter = 0;
		int TeamID = 0;
};

#endif

