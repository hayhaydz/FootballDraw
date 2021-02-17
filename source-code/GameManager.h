#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <memory>

#include "ImportData.h"
#include "Group.h"
#include "Team.h"
#include "Match.h"

	//This class handles all class handling, it is the convergance point for interacting classes. It also makes basic program calculations, 
	//calculations such as scores and calls which teams will play against which. This class also stores all important variables.

#pragma once
class GameManager {
	public:
		void setup();

	private:
		void displayData();
		void shuffleTeams();
		void calcTeamStrength();
		void groupPlayoffs();
		void sortGroupPlayoffs();
		void roundSixteen();
		void sortRoundSixteen();

		ImportData ImportDataClass;

		Team DefaultTeam;
		std::vector<Team> PlaceholderTeams;
		int totalELO = 0;
		double averageELO = 0;
		std::vector<Team> MatchTeams;
		int longestNameLength = 0;

		Group DefaultGroup;
		std::vector<Group> Groups;
		char GroupLabels[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		bool isGroupStage = true;
		int groupCounter = 0;
		int groupLabelCounter = 4;

		enum class Stage { GROUP, ROUND, QUARTER, SEMI, FINAL };
		Stage currentStage = Stage::GROUP;

		Match Match;
};

#endif