#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <deque>
#include <tuple>
#include "utils.cpp"

using namespace std;

map<string, set<string>> adjacencyList(vector<string> lines) {
    map<string, set<string>> connections;
    for (string l : lines) {
        auto tokens = tokenize(l, '-');
        if (connections.count(tokens[0]) == 0) {
            set<string> nodes;
            connections[tokens[0]] = nodes;
        }
        if (connections.count(tokens[1]) == 0) {
            set<string> nodes;
            connections[tokens[1]] = nodes;
        }
        connections[tokens[0]].insert(tokens[1]);
        connections[tokens[1]].insert(tokens[0]);
    }
    return connections;
}

int countGroupsConnectedWith(string node, map<string, set<string>> connections, set<string> visited) {
    set<string> connectedNodes = connections[node];
    vector<string> nodes(connectedNodes.begin(), connectedNodes.end());
    int count = 0;
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            if (visited.count(nodes[i]) > 0 || visited.count(nodes[j]) > 0) {
                continue;
            }
            if (connections.count(nodes[i]) > 0 && connections[nodes[i]].count(nodes[j]) > 0) {
                count++;
            }
        }
    }
    return count;
}

int solvePart1(vector<string> lines) {
    map<string, set<string>> connections = adjacencyList(lines);
    int count = 0;
    set<string> visited;
    for (const auto &entry : connections) {
        if (entry.first[0] == 't') {
            count += countGroupsConnectedWith(entry.first, connections, visited);
            visited.insert(entry.first);
        }
    }
    return count;
}

struct Node {
    set<string> computers;
    int index;
};

bool isNodeFullConnected(string node, set<string> group, map<string, set<string>> *connections) {
    for (string groupNode : group) {
        if (connections->at(node).count(groupNode) == 0) {
            return false;
        }
    }
    return true;
}

set<string> getGrooupWith(string node, map<string, set<string>> *connections) {
    deque<Node> queue;
    Node init1 = Node();
    init1.computers.insert(node);
    init1.index = 0;
    queue.push_back(init1);
    Node init2 = Node();
    init2.index = 0;
    queue.push_back(init2);
    vector<string> directNodes(connections->at(node).begin(), connections->at(node).end());
    int maxSize = 0;
    set<string> maxGroup;
    while (queue.size() > 0) {
        Node current = queue.front();
        queue.pop_front();
        if (current.index == directNodes.size()) {
            if (current.computers.size() > maxSize) {
                maxSize = current.computers.size();
                maxGroup = current.computers;
            }
            continue;
        }
        if (isNodeFullConnected(directNodes[current.index], current.computers, connections)) {
            Node newNode = Node();
            newNode.computers = set<string>(current.computers.begin(), current.computers.end());
            newNode.computers.insert(directNodes[current.index]);
            newNode.index = current.index + 1;
            queue.push_back(newNode);
        }
        current.index++;
        queue.push_back(current);
    }
    return maxGroup;
}

string solvePart2Old(vector<string> lines) {
    map<string, set<string>> connections = adjacencyList(lines);
    int maxSize = 0;
    set<string> maxGroup;
    for (const auto &entry : connections) {
        set<string> group = getGrooupWith(entry.first, &connections);
        if (group.size() > maxSize) {
            maxSize = group.size();
            maxGroup = group;
        }
    }
    string password = "";
    for (string g : maxGroup) {
        password += g + ",";
    }

    return password.substr(0, password.length() - 1);
}

void findGroups(string node, set<string> currentGroup, map<string, set<string>> *connections, set<string> *groupsFound) {
    string serialized = "";
    for (string n : currentGroup) {
        serialized += n + ",";
    }
    serialized = serialized.substr(0, serialized.length() - 1);
    if (groupsFound->count(serialized) > 0) {
        return;
    }
    groupsFound->insert(serialized);
    for (string neighbor : connections->at(node)) {
        if (currentGroup.count(neighbor) > 0) {
            continue;
        }
        if (isNodeFullConnected(neighbor, currentGroup, connections)) {
            set<string> newGroup = (currentGroup);
            newGroup.insert(neighbor);
            if (newGroup.size() == currentGroup.size()) {
                cout << "ERROR new group is same size" << endl;
            }
            findGroups(neighbor, newGroup, connections, groupsFound);
        }
    }
}

string solvePart2(vector<string> lines) {
    map<string, set<string>> connections = adjacencyList(lines);
    set<string> groupsFound;
    for (const auto &entry : connections) {
        set<string> group;
        group.insert(entry.first);
        findGroups(entry.first, group, &connections, &groupsFound);
    }
    int maxSize = 0;
    string maxGroup;
    for (string g : groupsFound) {
        if (g.length() > maxSize) {
            maxSize = g.length();
            maxGroup = g;
        }
    }

    return maxGroup;
}

int main() {
    vector<string> lines = readLines("data/input23.txt");
    auto t1 = chrono::high_resolution_clock::now();
    int sum = solvePart1(lines);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 1 - sum: " << sum << " Duration (ms): " << ms_int.count() << endl;
    t1 = chrono::high_resolution_clock::now();
    string pass = solvePart2Old(lines);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - password: " << pass << " Duration (ms): " << ms_int.count() << endl;
    t1 = chrono::high_resolution_clock::now();
    pass = solvePart2(lines);
    t2 = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Part 2 - password: " << pass << " Duration (ms): " << ms_int.count() << endl;

    return 0;
}
