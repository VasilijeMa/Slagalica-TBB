#include "Slagalica.h"
#include <algorithm>
#include "tbb/task_group.h"

#define SOLUTIONS_CUTOFF 150000
#define MATCH_CUTOFF 50000

using namespace tbb;

bool isSubset(string subset, string set) {

    if (subset.length() > set.length()) {
        return false;
    }

    sort(set.begin(), set.end());
    sort(subset.begin(), subset.end());

    int i = 0;
    int j = 0;

    while (i < subset.length()) {
        if (j == set.length()) {
            return false;
        }
        if (subset[i] == set[j]) {
            ++i;
            ++j;
        }
        else if (subset[i] > set[j]) {
            ++j;
        }
        else {
            return false;
        }
    }
    return true;
}

bool findMatchSerial(string userWord, const vector<string>& allWords) {
    for (int i = 0; i < allWords.size(); i++) {
        if (userWord == allWords[i]) {
            return true;
        }
    }
    return false;
}

bool findMatchParallel(string userWord, const vector<string>& allWords) {
    if (allWords.size() <= MATCH_CUTOFF) {
        return findMatchSerial(userWord, allWords);
    }
    else {
        task_group g;
        bool wasFoundFirst, wasFoundSecond, wasFoundThird;

        vector<string> firstThirdWords(allWords.begin(), allWords.begin() + allWords.size() / 3);
        vector<string> secondThirdWords(allWords.begin() + allWords.size() / 3, allWords.begin() + 2 * allWords.size() / 3);
        vector<string> thirdThirdWords(allWords.begin() + 2 * allWords.size() / 3, allWords.end());

        g.run([&] {wasFoundFirst = findMatchParallel(userWord, firstThirdWords); });
        g.run([&] {wasFoundSecond = findMatchParallel(userWord, secondThirdWords); });
        g.run([&] {wasFoundThird = findMatchParallel(userWord, thirdThirdWords); });

        g.wait();

        return (wasFoundFirst || wasFoundSecond || wasFoundThird);

    }
}

int findBestSolutionsSerial(string randomWord, const vector<string>& allWords, vector<string>& solutions) {
    int bestLength = 0;
    for (int i = 0; i < allWords.size();i++) {
        string word = allWords[i];
        if (word.length() > bestLength) {
            if (isSubset(word, randomWord)) {
                solutions.clear();
                solutions.push_back(word);
                bestLength = word.length();
            }
        }
        else if (word.length() == bestLength) {
            if (isSubset(word, randomWord)) {
                solutions.push_back(word);
            }
        }
    }
    return bestLength;
}

int findBestSolutionsParallel(string randomWord, const vector<string>& allWords, vector<string>& solutions) {
    if (allWords.size() <= SOLUTIONS_CUTOFF) {
        return findBestSolutionsSerial(randomWord, allWords, solutions);
    }
    else {
        task_group g;
        vector<string> solutions2;
        vector<string> solutions3;

        vector<string> firstThirdWords(allWords.begin(), allWords.begin() + allWords.size() / 3);
        vector<string> secondThirdWords(allWords.begin() + allWords.size() / 3, allWords.begin() + 2 * allWords.size() / 3);
        vector<string> thirdThirdWords(allWords.begin() + 2 * allWords.size() / 3, allWords.end());

        int bestLength1, bestLength2, bestLength3;

        g.run([&] {bestLength1 = findBestSolutionsParallel(randomWord, firstThirdWords, solutions); });
        g.run([&] {bestLength2 = findBestSolutionsParallel(randomWord, secondThirdWords, solutions2); });
        g.run([&] {bestLength3 = findBestSolutionsParallel(randomWord, thirdThirdWords, solutions3); });

        g.wait();

        if ((bestLength1 == bestLength2) && (bestLength1 == bestLength3)) {
            solutions.insert(solutions.end(), solutions2.begin(), solutions2.end());
            solutions.insert(solutions.end(), solutions3.begin(), solutions3.end());
        }
        else if ((bestLength2 > bestLength1) && (bestLength2 > bestLength3)) {
            solutions = solutions2;
            return bestLength2;
        }
        else if ((bestLength3 > bestLength1) && (bestLength3 > bestLength2)) {
            solutions = solutions3;
            return bestLength3;
        }
        else if ((bestLength1 > bestLength2) && (bestLength1 > bestLength3)) {
        }
        else if ((bestLength2 == bestLength1)) {
            solutions.insert(solutions.end(), solutions2.begin(), solutions2.end());
        }
        else if ((bestLength3 == bestLength1)) {
            solutions.insert(solutions.end(), solutions3.begin(), solutions3.end());
        }
        else {
            solutions = solutions2;
            solutions.insert(solutions.end(), solutions3.begin(), solutions3.end());
            return bestLength2;
        }

        return bestLength1;
    }
}