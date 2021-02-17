#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <vector>
#include <memory>

#include "Team.h"

	// This class does no calculating and is purely for storing information in an object. This class stores a vector of teams as well as it's label.

#pragma once
class Group {
	public:
		char Label;
		std::vector<Team> Teams;

	private:
};

#endif