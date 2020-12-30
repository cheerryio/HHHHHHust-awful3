#include "pch.h"
#include "Manager.h"
#include "ChildView.h"
#include "CPathSHow.h"

using namespace std;
using namespace boost;

vector<VertexProps> Manager::vertexes;
vector<EdgeProps> Manager::edges;
vector<Organization> Manager::organizations;
vector<vertex_t> Manager::vecs;
vector<vertex_t> Manager::preds;
map<CString, int> Manager::orgMap;

CListBox* Manager::dropMenu;

Graph Manager::g;

bool Manager::bitmapLoaded = false;
bool Manager::pointLoaded = false;
bool Manager::pointFirstLoad = true;
bool Manager::organizationDropMenuOn = false;

int Manager::startVertexId=-1;
int Manager::endVertexId=-1;
bool Manager::isPathShow = true;

CChildView* Manager::childview=nullptr;
HWND Manager::hwnd;
CPathShow* Manager::pathShow=nullptr;

auto isspaceMy = [](unsigned char ch) { return std::isspace(ch); };

string Manager::UTF8ToGB(const char* str)
{
    string result;
    WCHAR* strSrc;
    LPSTR szRes;

    //获得临时变量的大小
    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    //获得临时变量的大小
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[]strSrc;
    delete[]szRes;

    return result;
}

vector<Organization> Manager::loadOrganizations(const std::string organizationFilename, const std::string orglocFilename) {
    std::ifstream infile1(organizationFilename);
    std::ifstream infile2(orglocFilename);
    vector<Organization> locations;
    string line;
    for (int id= 0; std::getline(infile1, line) && (std::find_if_not(line.begin(), line.end(), isspaceMy) != line.end()); id++) {
        line = Manager::UTF8ToGB(line.c_str());
        locations.push_back({id,CString(line.c_str())});
        CString cstr(line.c_str(), line.length());
        Manager::orgMap[cstr] = id;
    }

    for (int vertexId = 0; std::getline(infile2, line);vertexId++) {
        stringstream ss(line);
        string temp;
        int found;
        while (std::getline(ss, temp, ',') && (std::find_if_not(line.begin(), line.end(), isspaceMy) != line.end())) {
            stringstream(temp) >> found;
            ((locations[found]).vertex).push_back(vertexId);
        }
    }

    return locations;
}

vector<VertexProps> Manager::loadVertexes(const std::string locationFilename, const std::string orglocFilename) {
    std::ifstream infile1(locationFilename);
    std::ifstream infile2(orglocFilename);
    string line1,line2;
    vector<VertexProps> vertexes;
    for (int counter = 0; std::getline(infile1, line1) &&
                          std::getline(infile2, line2) &&
                          (std::find_if_not(line1.begin(), line1.end(), isspaceMy) != line1.end()) &&
                          (std::find_if_not(line2.begin(), line2.end(), isspaceMy) != line2.end()) ;
                            
                            counter++)
    {
        stringstream ss1;
        ss1 << line1;
        string temp;
        int found;
        int numbers[2];
        /*读vertex 的 id x y*/
        for (int i = 0; i < 2 && !ss1.eof();i++) {
            ss1 >> temp;
            if (stringstream(temp) >> found) {
                numbers[i] = found;
            }
        }

        stringstream ss2;
        ss2 << line2;
        vector<int> orgs;
        while (std::getline(ss2, temp, ',') && (std::find_if_not(line2.begin(), line2.end(), isspaceMy) != line2.end())) {
            stringstream(temp) >> found;
            orgs.push_back(found);
        }
        vertexes.push_back({counter,numbers[0],numbers[1],orgs});
    }
    return vertexes;
}

vector<EdgeProps> Manager::loadEdges(const std::string filename, std::vector<VertexProps> vertexes) {
    std::ifstream infile(filename);
    string line;
    vector<EdgeProps> edges;
    for (int counter = 0; std::getline(infile, line)&& (std::find_if_not(line.begin(), line.end(), isspaceMy) != line.end()); counter++)
    {
        stringstream ss;
        ss << line;
        string temp;
        int found;
        int numbers[2];
        for (int i = 0; i < 2; i++) {
            std::getline(ss, temp, ',');
            stringstream(temp) >> found;
            numbers[i] = found;
        }
        edges.push_back({ numbers[0],numbers[1],hypot(vertexes[numbers[0]].x - vertexes[numbers[1]].x,vertexes[numbers[0]].y - vertexes[numbers[1]].y) });
    }
    return edges;
}

void Manager::buildGraph() {
    boost::dynamic_properties dp;

    Manager::vertexes = loadVertexes(string("Location.txt"), string("Locorg.txt"));
    Manager::edges = loadEdges(string("Edge.txt"), vertexes);
    Manager::organizations = loadOrganizations(string("Organization.txt"), string("Locorg.txt"));

    for (int i = 0; i < vertexes.size(); i++) {
        vertex_t graphId = add_vertex(Manager::vertexes[i], Manager::g);
        Manager::vecs.push_back(graphId);
        (Manager::vertexes[i]).graphId = graphId;
    }
    for (int i = 0; i < edges.size(); i++)
        add_edge(Manager::vecs[Manager::edges[i].from], Manager::vecs[Manager::edges[i].to], Manager::edges[i], Manager::g);
}

vector<int> Manager::shortestPath(const pair<int,int> startend) {
    vector<vertex_t> preds(40);
    vector<int> ret;
    vertex_t end = startend.second;

    auto wmap = make_transform_value_property_map([](EdgeProps& e) { return e.weight; }, get(boost::edge_bundle, Manager::g));
    boost::dijkstra_shortest_paths(Manager::g, end, boost::predecessor_map(&preds[0]).weight_map(wmap));

    int p = startend.first;
    do
    {
        ret.push_back(p);
        p = preds[p];
    } while (p != end);
    ret.push_back(end);
    return ret;
}

int Manager::longestCommonSubsequence(const CString& A, const CString& B) {
    int m = A.GetLength();
    int n = B.GetLength();
    vector<int> dp(n+1, 0);
    int res = 0;
    for (int i = 1; i <= m; i++) {
        for (int j = n; j >= 1; j--) {
            if (A[i - 1] == B[j - 1]) {
                dp[j] = dp[j - 1] + 1;
            }
            else {
                dp[j] = 0;
            }
            res = max(dp[j], res);
        }
    }
    return res;
}

double Manager::jaccardIndex(const CString& A, const CString& B, const int commonLen) {
    return (double)commonLen / (double)(A.GetLength() + B.GetLength() - commonLen);
}

string Manager::startendString(const int startVertexId, const int endVertexId) {
    string title("");
    if (Manager::startVertexId != -1)
        title = title + "start:" + to_string(Manager::startVertexId);
    if (Manager::endVertexId != -1)
        title = title + "    " + "end:" + to_string(Manager::endVertexId);
    return title;
}

void Manager::pathShowOnMap() {
    if (Manager::startVertexId == -1 || Manager::endVertexId == -1) {
        AfxMessageBox(L"please specify start end point");
        return;
    }
    if (Manager::vertexes.empty())
        return;

    vector<int> pointSequence = Manager::shortestPath(pair<int, int>(Manager::startVertexId, Manager::endVertexId));


    CPathShow* pathshow = new CPathShow(pointSequence);
    if (Manager::pathShow != nullptr) {
        delete Manager::pathShow;
        Manager::isPathShow = true;
        Manager::childview->Invalidate();
    }
    Manager::pathShow = pathshow;
    CRect pointRect(0, 1, 2, 3);
    pathshow->Create(_T("STATIC"), _T("hello"), WS_CHILD | WS_VISIBLE | SS_NOTIFY, pointRect, Manager::childview, 1);


    return;
}