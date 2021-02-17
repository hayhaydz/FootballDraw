#include "GameManager.h"

void GameManager::setup() {
	ImportDataClass.import(PlaceholderTeams, totalELO);
	shuffleTeams();
	calcTeamStrength();
	groupPlayoffs();
	sortGroupPlayoffs();
	currentStage = static_cast<Stage>(static_cast<int>(currentStage) + 1);
	// for loop to move through round of 16 stages
	for (int i = 0; i < 4; i++) {
		roundSixteen();
		sortRoundSixteen();
		currentStage = static_cast<Stage>(static_cast<int>(currentStage) + 1);
	}
}

void GameManager::shuffleTeams() {
	// creating a random seed and result to use in shuffle
	std::random_device rd;
	auto rng = std::default_random_engine{ rd() };

	// moving through all groups and teams inside groups
	for (int i = 0; i < 8; i++) {
		// asigns a label for the current group
		DefaultGroup.Label = GroupLabels[i];
		// shuffle's the teams around
		//https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
		std::shuffle(std::begin(PlaceholderTeams), std::end(PlaceholderTeams), rng);
		for (int j = 0; j < 4; j++) {
			// pushes four teams into the current group
			DefaultGroup.Teams.push_back(PlaceholderTeams.at(j));
		}
		// erases the pushed teams from the original collection of teams
		PlaceholderTeams.erase(PlaceholderTeams.begin(), PlaceholderTeams.begin() + 4);
		// pushes and resetd the group
		Groups.push_back(DefaultGroup);
		DefaultGroup.Teams.clear();
	}
}

void GameManager::calcTeamStrength() {
	// calculates an average elo based on the total of all team's elos
	averageELO = totalELO / (Groups.size() * 4);

	for (int i = 0; i < Groups.size(); i++) {
		for (int j = 0; j < Groups[i].Teams.size(); j++) {
			if (Groups[i].Teams[j].Name.length() > longestNameLength) {
				// calculates the longest team name
				longestNameLength = Groups[i].Teams[j].Name.length();
			}
			// calculates a team strength perentage based on the average elo
			Groups[i].Teams[j].strength = (Groups[i].Teams[j].ELO / averageELO) * 100;
		}
	}
}

void GameManager::groupPlayoffs() {
	// loops through all groups
	for (int i = 0; i < Groups.size(); i++) {
		// loops through all teams in a group
		for (int j = 0; j < Groups[i].Teams.size(); j++) {
			// a secondary loop going through all teams in a group, but it occurs for every team in a group
			for (int p = 0; p < Groups[i].Teams.size(); p++) {
				// makesure that the current (j) team hasn't played against the (p) team
				if (p != j && std::find(Groups[i].Teams[j].TeamsPlayed.begin(), Groups[i].Teams[j].TeamsPlayed.end(), Groups[i].Teams[p].ID) == Groups[i].Teams[j].TeamsPlayed.end()) {
					// assigns an array to the two teams
					MatchTeams = { Groups[i].Teams[j], Groups[i].Teams[p] };
					// sets up a match for the two teams
					Match.setup(MatchTeams, isGroupStage);
					// re-assigns data back to initial teams
					Groups[i].Teams[j] = MatchTeams[0];
					Groups[i].Teams[p] = MatchTeams[1];
					// resets teams data
					Groups[i].Teams[j].goals = 0;
					Groups[i].Teams[p].goals = 0;
				}
			}
		}
	}
}

void GameManager::sortGroupPlayoffs() {
	// I dont like the excessive use of for loops
	for (int i = 0; i < Groups.size(); i++) {
		for (int j = 0; j < Groups[i].Teams.size(); j++) {
			// calculates the teams points based off of wins and draws
			Groups[i].Teams[j].points = (Groups[i].Teams[j].wins * 3) + Groups[i].Teams[j].draws;
		}
		// sorts teams based on points
		//https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects
		std::sort(Groups[i].Teams.begin(), Groups[i].Teams.end(),std::greater<Team>());
	}
	// calling for current results data to be displayed before it's wiped
	displayData();
	for (int i = 0; i < Groups.size(); i++) {
		// removing the last two teams in a group
		Groups[i].Teams.erase(Groups[i].Teams.end() - 2, Groups[i].Teams.end());
		for (int j = 0; j < Groups[i].Teams.size(); j++) {
			// resetting the remaining teams results
			Groups[i].Teams[j].reset();
		}
	}
	// looping through all groups, handling first place
	for (int i = 0; i < 8; i++) {
		if (groupCounter < 2) {
			if (groupCounter == 0) {
				// checking the current groups iteration to see if the new group will need a label
				DefaultGroup.Label = GroupLabels[i/2];
			}
			// pushing back the current groups first place team
			DefaultGroup.Teams.push_back(Groups[i].Teams[0]);
			// deleting that original team
			Groups[i].Teams.erase(Groups[i].Teams.begin());
			// moving the iterator forward
			groupCounter++;
			if (i == 7) {
				// catching the for loop before it ends to push the current data to the groups
				Groups.push_back(DefaultGroup);
				groupCounter = 0;
				DefaultGroup.Teams.clear();
			}
		}
		else {
			// pushing current group data into the vector of groups and resettng counters, moving i back a position because current loop was used on pushing and not collecting a team
			Groups.push_back(DefaultGroup);
			groupCounter = 0;
			DefaultGroup.Teams.clear();
			i--;
		}
	}
	// same loop as above but pushes second place into new groups
	for (int i = 0; i < 8; i++) {
		if (groupCounter < 2) {
			if (groupCounter == 0) {
				DefaultGroup.Label = GroupLabels[groupLabelCounter];
				groupLabelCounter++;
			}
			DefaultGroup.Teams.push_back(Groups[i].Teams[0]);
			Groups[i].Teams.erase(Groups[i].Teams.begin());
			groupCounter++;
			if (i == 7) {
				Groups.push_back(DefaultGroup);
				groupCounter = 0;
				DefaultGroup.Teams.clear();
				Groups.erase(Groups.begin() + i);
			}
		}
		else {
			Groups.push_back(DefaultGroup);
			groupCounter = 0;
			DefaultGroup.Teams.clear();
			i--;
		}
	}
	//deletes original now empty groups
	Groups.erase(Groups.begin(), Groups.begin() + 7);
}

void GameManager::roundSixteen() {
	isGroupStage = false;
	// playing the teams against each other in their groups
	for (int i = 0; i < Groups.size(); i++) {
		MatchTeams = { Groups[i].Teams[0], Groups[i].Teams[1] };
		Match.setup(MatchTeams, isGroupStage);
		Groups[i].Teams[0] = MatchTeams[0];
		Groups[i].Teams[1] = MatchTeams[1];
	}
}

void GameManager::sortRoundSixteen() {
	// displaying result data before it is reset
	displayData();
	// calculating result points and sorting by that
	for (int i = 0; i < Groups.size(); i++) {
		Groups[i].Teams[0].points = Groups[i].Teams[0].wins * 3;
		Groups[i].Teams[1].points = Groups[i].Teams[1].wins * 3;
		std::sort(Groups[i].Teams.begin(), Groups[i].Teams.end(), std::greater<Team>());
	}
	// resetting current data and erasing certain teams depending on the stage not being the final
	for (int i = 0; i < Groups.size(); i++) {
		if (currentStage != Stage::SEMI && currentStage != Stage::FINAL) { Groups[i].Teams.erase(Groups[i].Teams.end() - 1, Groups[i].Teams.end()); }
		for (int j = 0; j < Groups[i].Teams.size(); j++) {
			Groups[i].Teams[j].reset();
		}
	}
	if (currentStage == Stage::ROUND) {
		// for loop similar to the one in group stage that moves through and calculates the qualifying teams
		for (int i = 0; i < 8; i++) {
			if (groupCounter < 2) {
				if (groupCounter == 0) {
					DefaultGroup.Label = GroupLabels[i / 2];
				}
				DefaultGroup.Teams.push_back(Groups[i].Teams[0]);
				Groups[i].Teams.erase(Groups[i].Teams.begin());
				groupCounter++;
				if (i == 7) {
					Groups.push_back(DefaultGroup);
					groupCounter = 0;
					DefaultGroup.Teams.clear();
				}
			}
			else {
				Groups.push_back(DefaultGroup);
				groupCounter = 0;
				DefaultGroup.Teams.clear();
				i--;
			}
		}
		Groups.erase(Groups.begin(), Groups.begin() + 8);
	}
	else if (currentStage == Stage::QUARTER) {
		for (int i = 0; i < 4; i++) {
			if (groupCounter < 2) {
				if (groupCounter == 0) {
					DefaultGroup.Label = GroupLabels[i / 2];
				}
				DefaultGroup.Teams.push_back(Groups[i].Teams[0]);
				Groups[i].Teams.erase(Groups[i].Teams.begin());
				groupCounter++;
				if (i == 3) {
					Groups.push_back(DefaultGroup);
					groupCounter = 0;
					DefaultGroup.Teams.clear();
				}
			}
			else {
				Groups.push_back(DefaultGroup);
				groupCounter = 0;
				DefaultGroup.Teams.clear();
				i--;
			}
		}
		Groups.erase(Groups.begin(), Groups.begin() + 4);
	}
	else if (currentStage == Stage::SEMI) {
		// first place for loop
		for (int i = 0; i < 2; i++) {
			if (groupCounter < 2) {
				if (groupCounter == 0) {
					DefaultGroup.Label = GroupLabels[i / 2];
				}
				DefaultGroup.Teams.push_back(Groups[i].Teams[0]);
				Groups[i].Teams.erase(Groups[i].Teams.begin());
				groupCounter++;
				if (i == 1) {
					Groups.push_back(DefaultGroup);
					groupCounter = 0;
					DefaultGroup.Teams.clear();
				}
			}
			else {
				Groups.push_back(DefaultGroup);
				groupCounter = 0;
				DefaultGroup.Teams.clear();
				i--;
			}
		}
		// second place for loop
		for (int i = 0; i < 2; i++) {
			if (groupCounter < 2) {
				if (groupCounter == 0) {
					DefaultGroup.Label = GroupLabels[1];
				}
				DefaultGroup.Teams.push_back(Groups[i].Teams[0]);
				Groups[i].Teams.erase(Groups[i].Teams.begin());
				groupCounter++;
				if (i == 1) {
					Groups.push_back(DefaultGroup);
					groupCounter = 0;
					DefaultGroup.Teams.clear();
				}
			}
			else {
				Groups.push_back(DefaultGroup);
				groupCounter = 0;
				DefaultGroup.Teams.clear();
				i--;
			}
		}
		Groups.erase(Groups.begin(), Groups.begin() + 2);
	}
	else if (currentStage == Stage::FINAL) {
		std::cout << "1st: " << Groups[0].Teams[0].Name << std::endl;
		std::cout << "2nd: " << Groups[0].Teams[1].Name << std::endl;
		std::cout << "3rd: " << Groups[1].Teams[0].Name << std::endl;
	}
}

void GameManager::displayData() {
	// display current stage
	switch (currentStage) {
		case Stage::GROUP:
			std::cout << "Group Stage" << std::endl;
			break;
		case Stage::ROUND:
			std::cout << "Round of 16" << std::endl;
			break;
		case Stage::QUARTER:
			std::cout << "Quater-Finals" << std::endl;
			break;
		case Stage::SEMI:
			std::cout << "Semi-Finals" << std::endl;
			break;
		case Stage::FINAL:
			std::cout << "Final" << std::endl;
			break;
	}
	if (currentStage == Stage::GROUP) {
		// displaying group stage grid template for result data
		std::cout << "Group\tTeam name\tTeam Points\tTeam Wins\tTeam Draws\tTeam Losses\n" << std::endl;
		for (int i = 0; i < Groups.size(); i++) {
			std::cout << Groups[i].Label << "    ";
			for (int j = 0; j < Groups[i].Teams.size(); j++) {
				std::cout << "\t" << Groups[i].Teams[j].Name;
				if (Groups[i].Teams[j].Name.length() < longestNameLength) {
					for (int l = 0; l < (longestNameLength - Groups[i].Teams[j].Name.length()); l++) {
						std::cout << " ";
					}
				}
				std::cout << "\t" << Groups[i].Teams[j].points << "           \t" << Groups[i].Teams[j].wins << "         \t" << Groups[i].Teams[j].draws << "          \t" << Groups[i].Teams[j].losses << std::endl;
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}
	else {
		// displaying round of sixteen result template
		for (int i = 0; i < Groups.size(); i++) {
			if (Groups[i].Teams[0].Name.length() < longestNameLength) {
				for (int p = 0; p < (longestNameLength - Groups[i].Teams[0].Name.length()); p++) {
					std::cout << " ";
				}
			}
			std::cout << " " << Groups[i].Teams[0].Name << "    " << Groups[i].Teams[0].goals << "-" << Groups[i].Teams[1].goals << "    ";
			std::cout << Groups[i].Teams[1].Name << std::endl;
			if (Groups[i].Teams[0].goals == Groups[i].Teams[1].goals) {
				for (int l = 0; l < (Groups[i].Teams[0].Name.length() + (longestNameLength - Groups[i].Teams[0].Name.length())); l++) {
					std::cout << " ";
				}
				std::cout << "  Penalties" << std::endl;
				for (int l = 0; l < (Groups[i].Teams[0].Name.length() + (longestNameLength - Groups[i].Teams[0].Name.length())); l++) {
					std::cout << " ";
				}
				std::cout << "     " << Groups[i].Teams[0].penaltyGoals << "-" << Groups[i].Teams[1].penaltyGoals << "    " << std::endl;
			}
			std::cout << std::endl;
		}
	}
}