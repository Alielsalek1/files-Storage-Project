#ifndef HEADER_NAME_H
#define HEADER_NAME_H

#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

vector<string> split(string s, char delimiter) {
    if (s.empty() || s == "\n") return {};
    vector<string> tokens;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delimiter)) tokens.push_back(item);
    return tokens;
}

#endif