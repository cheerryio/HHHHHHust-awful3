#pragma once

#include "structs.h"
#include "ChildView.h"
#include "CMapPoint.h"
#include "CPathShow.h"

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <map>


#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/transform_value_property_map.hpp>

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, VertexProps, EdgeProps> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

class Manager
{
// Global variables
public:
	static CChildView* childview;		// global CChildView*
	static HWND hwnd;					// global hwnd
	static CPathShow* pathShow;			// global CPathShow control

// Attributes
public:
	static const int mapPointSize = 10;	// point radius

// Member variables
public:
	static std::vector<VertexProps> vertexes;
	static std::vector<EdgeProps> edges;
	static std::vector<Organization> organizations;
	static std::vector<vertex_t> vecs;
	static std::vector<vertex_t> preds;
	static std::map<CString, int> orgMap;

	static Graph g;

	static std::vector<CMapPoint*> mapPoints;
	static CListBox* dropMenu;


// Signals
public:
	static bool bitmapLoaded;				// is background bitmap loaded
	static bool pointLoaded;				// is points loaded
	static bool pointFirstLoad;				// is point first load
	static bool organizationDropMenuOn;		// is organization dropmenu on
	static bool isPathShow;

// 
public:
	static int startVertexId;				// start point id
	static int endVertexId;					// end point id

// Funcs
public:

	/**
	* copyed function that transfer utf8 encoding to gb
	* generally convert file content to show chinese charactor
	* 
	* @param str read content
	* 
	* @return converted string
	*/
	static std::string UTF8ToGB(const char* str);
	/**
	* 根据文件建立organization的vector
	* 需要读Organization.txt和orgloc.txt确定org的id,name,vertex(属于哪个节点)
	* 
	* @param organizationFilename filename of file which stores organizations
	* @param orgloacFilename filename of file which stores location of organizations
	* 
	* @returns organization vector
	*/
	static std::vector<Organization> loadOrganizations(const std::string organizationFilename, const std::string orglocFilename);

	/**
	* 建立vertex的vector
	* 填写id,x,y,orgs(记录此点有哪些org,用org的id,即数字去记录
	* 
	* @param locationFilename
	* @param orglocFilename
	* 
	* @returns
	*/
	static std::vector<VertexProps> loadVertexes(const std::string locationFilename, const std::string orglocFilename);

	/**
	* 得到from,to,weight
	* 
	* @param filename
	* @param 
	* 
	* @returns
	*/
	static std::vector<EdgeProps> loadEdges(const std::string filename,std::vector<VertexProps>);

	/**
	* get a sequence of point locations that need to be lined up
	* 
	* @param startend the pair contains the start point id and end point id
	* 
	* @returns sequence of point id to line up
	*/
	static std::vector<int> shortestPath(const std::pair<int,int> startend);

	/**
	* build graph according to vertexes and edges
	* 
	* 
	*/
	static void buildGraph();

	/**
	* calcaulate the longest common subsequence of two CString
	* 
	* @param A CString A
	* @param B CString B
	* 
	* @returns length of longest common subsequence
	*/
	static int longestCommonSubsequence(const CString& A, const CString& B);

	/**
	* calcaulate similarity index
	* commonLen/(len(A)+len(B))
	* 
	* @param A CString A
	* @param B CString B
	* @param commonLen
	* 
	* @returns jaccard index
	*/
	static double jaccardIndex(const CString& A, const CString& B, const int commonLen);

	/**
	* generate a human understanable sentense that describes where is the start point and where is the end
	* 
	* @param startVertexId the ID of the start point
	* @param endVertexId the ID of the end point
	* 
	* @returns human understandable string
	*/
	static std::string startendString(const int startVertexId, const int endVertexId);

	/**
	* 
	*/
	static void pathShowOnMap();
};

