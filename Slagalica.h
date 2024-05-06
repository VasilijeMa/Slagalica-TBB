#pragma once

#include <string>
#include <vector>

using namespace std;

bool isSubset(string subset, string set);

bool findMatchSerial(string userWord, const vector<string>& allWords);

bool findMatchParallel(string userWord, const vector<string>& allWords);

int findBestSolutionsSerial(string randomWord, const vector<string>& allWords, vector<string>& solutions);

int findBestSolutionsParallel(string randomWord, const vector<string>& allWords, vector<string>& solutions);