#include "Match.h"

void Match::setup(std::vector<Team> &Teams, bool isGroupStage) {
	isGroup = isGroupStage;
	calculateResults(Teams);
	// declaring the teams have played against each other
	Teams[0].TeamsPlayed.push_back(Teams[1].ID);
	Teams[1].TeamsPlayed.push_back(Teams[0].ID);
}

void Match::calculateResults(std::vector<Team> &Teams) {
	// 26 shots per game
	// 13 shots per team
	// nigeria - 5 shots
	// spain - 7 shots

	// random number to affect variation of team strength - between 1 - 50 / 10 value added to random number between 1 and strength
	// random number between 1 - strength value / 10
	// 50/50 - even number = score

	for (int i = 0; i < 2; i++) {
		// random number for a variation of shots
		resultVariation = randomNumber(1, 50) / 10;
		// shots determined by a random number between 1 to the variation and the strength divided by ten
		shots = randomNumber((1 + resultVariation), (Teams[i].strength / 10));
		// moving through the amount of shots
		for (int j = 0; j < shots; j++) {
			// if the random number between 1 to 6 is the same as the random number between 1 to 4 a goal is scored, otherwise it's missed
			(randomNumber(1, 6) == randomNumber(1, 4)) ? Teams[i].goals++ : 0;
		}
	}

	// calculating who wins and loses
	if (Teams[0].goals > Teams[1].goals) {
		Teams[0].wins++;
		Teams[1].losses++;
	}
	else if(Teams[0].goals == Teams[1].goals) {
		Teams[0].draws++;
		Teams[1].draws++;

		if (!isGroup) {
			isPenalty = true;
		}
	}
	else {
		Teams[0].losses++;
		Teams[1].wins++;
	}

	// if it's the round of sixteen and there was a draw then penalties are started
	if (isPenalty) {
		for (int i = 0; i < 2; i++) {
			// five attempted shots with a fifty percent chance of landing a goal
			for (int j = 0; j < 5; j++) {
				(randomNumber(1, 2) % 2 == 0) ? Teams[i].penaltyGoals++ : 0;
			}
		}

		// calculating who won and lost
		if (Teams[0].penaltyGoals == Teams[1].penaltyGoals) {
			isSuddenDeath = true;
		}
		else if (Teams[0].penaltyGoals > Teams[1].penaltyGoals) {
			Teams[0].wins++;
			Teams[1].losses++;
			isPenalty = false;
		}
		else {
			Teams[0].losses++;
			Teams[1].wins++;
			isPenalty = false;
		}
	}

	// if it was a draw again then it's moved to sudden death, first to miss loses
	while (isSuddenDeath) {
		suddenDeathCounter++;
		if (randomNumber(1, 2) % 2 != 0) {
			if (suddenDeathCounter % 2 != 0) {
				Teams[0].losses++;
				Teams[1].penaltyGoals++;
				Teams[1].wins++;
			}
			else {
				Teams[0].penaltyGoals++;
				Teams[0].wins++;
				Teams[1].losses++;
			}
			isPenalty = false;
			isSuddenDeath = false;
		}
	}
}

// function for calculating a random number
int Match::randomNumber(int min, int max) {
	return rand() % max + min;
}