#ifndef TEAM_H
#define TEAM_H

#include <iostream>
#include <vector>
#include <string>

	// The team class stores information such as it's ID, wins, strength, elo, name and many more. 
	// It also has an operator for handling the sorting by it's points value.

#pragma once
class Team {
	public:
		void reset();
		int ID = 0;
		std::string Name;
		int ELO = 0;
		int strength = 0;
		int wins = 0;
		int draws = 0;
		int losses = 0;
		int goals = 0;
		int penaltyGoals = 0;
		int points = 0;
		std::vector<int> TeamsPlayed;

		bool operator>(const Team& val) const {
			return points > val.points;
		}

	private:
		
};

#endif