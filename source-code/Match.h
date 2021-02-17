#ifndef MATCH_H
#define MATCH_H

#include <iostream>
#include <vector>
#include <time.h>

#include "Team.h"

	// The match class handles matches between two passed in teams. Using randomised values to determine odds and calculating results.

#pragma once
class Match {
	public:
		void setup(std::vector<Team> &Teams, bool isGroupStage);
		void calculateResults(std::vector<Team> &Teams);
		int randomNumber(int min, int max);

	private:
		int resultVariation;
		int shots;
		bool isGroup = true;
		bool isPenalty = false;
		bool isSuddenDeath = false;
		int suddenDeathCounter = 0;
};

#endif