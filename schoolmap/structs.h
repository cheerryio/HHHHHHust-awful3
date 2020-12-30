#pragma once

#include "pch.h"

#include <string>
#include <vector>

struct VertexProps {
	int id;
	int x;
	int y;
	std::vector<int> orgs;
	int graphId;
};


struct EdgeProps {
	int from;
	int to;
	double weight;
};


struct Organization {
	int id;
	CString name;
	std::vector<int> vertex;
};