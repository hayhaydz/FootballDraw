#include "ImportData.h"

void ImportData::import(std::vector<Team> &PlaceholderTeams, int &totalELO) {
	file.open("data.txt");
	// moving through every line in the file
	for (std::string line; getline(file, line);) {
		// assigning an iterating id.
		DefaultTeam.ID = TeamID;
		counter = 0;
		// creating a stringstream to iterate through
		std::stringstream ss(line);
		// whilst the string stream has content it moves through
		while (ss.good()) {
			counter++;
			// seperating content by it's comma
			getline(ss, substr, ',');
			// using the counter to declare which content is currently being set
			switch (counter) {
				case 1:
					DefaultTeam.Name = substr;
					break;
				case 2:
					DefaultTeam.ELO = std::stoi(substr);
					totalELO += std::stoi(substr);
					break;
			}
		}
		// pushing the default team back
		PlaceholderTeams.push_back(DefaultTeam);
		TeamID++;
	}
	file.close();
}