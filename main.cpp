#include <iostream>
#include <fstream>
#include <Windows.h>
#include <random>
#include "Slagalica.h"
#include "tbb/tick_count.h"


using namespace tbb;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    string line;
    string kraj;
    vector<string> randomLetters;
    vector<string> allWords;
    vector<string> solutions;
    tick_count startTime;
    tick_count endTime;
    int bestLength;
    bool found;

    ifstream lettersFile("data/Slova.txt");

    if (!lettersFile.is_open())
    {
        cerr << "Greška pri otvaranju fajla!" << endl;
        return 1;
    }

    while (getline(lettersFile, line))
    {
        randomLetters.push_back(line);
    }

    lettersFile.close();


    ifstream inputFile("data/Recnik.txt");

    if (!inputFile.is_open())
    {
        cerr << "Greška pri otvaranju fajla!" << endl;
        return 1;
    }

    while (getline(inputFile, line))
    {
        allWords.push_back(line);
    }

    inputFile.close();


    while (true) {

        solutions.clear();

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, randomLetters.size() - 1);
        string randomWord = "";

        cout << "Slova na raspolaganju: " << endl << endl;

        for (int i = 0; i < 12; i++) {
            string randomLetter = randomLetters[dis(gen)];
            cout << randomLetter << " ";
            randomWord += randomLetter;
        }

        //////////////////////////////////////////////////////////////////////////


        cout << endl << endl << "Koja je vasa rec?" << endl;

        

        wchar_t wstr[255];
        char mb_str[255 * 3 + 1];

        unsigned long read;
        void* con = GetStdHandle(STD_INPUT_HANDLE);

        ReadConsole(con, wstr, 255, &read, NULL);

        int size = WideCharToMultiByte(CP_UTF8, 0, wstr, read, mb_str, sizeof(mb_str), NULL, NULL);
        mb_str[size - 2] = 0;

        kraj = mb_str;

        if (kraj == "kraj") break;

        if (!isSubset(kraj, randomWord)) {
            cout << endl << "Rec nije sastavljena od ponudjenih slova." << endl;
        }
        else {

            startTime = tick_count::now();
            found = findMatchSerial(kraj, allWords);
            endTime = tick_count::now();

            cout << endl << "Serijsko vreme: " << (endTime - startTime).seconds() << endl;

            if (found) {
                cout << "Rec je nadjena u recniku." << endl;
            }

            else {
                cout << "Rec nije nadjena u recniku." << endl;
            }

            startTime = tick_count::now();
            found = findMatchParallel(kraj, allWords);
            endTime = tick_count::now();

            cout << endl << "Paralelno vreme: " << (endTime - startTime).seconds() << endl;

            if (found) {
                cout << "Rec je nadjena u recniku." << endl;
            }

            else {
                cout << "Rec nije nadjena u recniku." << endl;
            }
        }
        
        //////////////////////////////////////////////////////////////////////////


        cout << "A sada da vidimo koju ce rec kompjuter naci:" << endl;

        startTime = tick_count::now();
        bestLength = findBestSolutionsSerial(randomWord, allWords, solutions);
        endTime = tick_count::now();

        cout << endl << "Serijsko vreme: " << (endTime - startTime).seconds() << endl;

        cout << "Serijska duzina: " << bestLength/2 << ", Serijska resenja:" << endl;

        for (int i = 0;i < solutions.size();i++) {
            cout << solutions[i] << endl;
        }
        

        solutions.clear();

        startTime = tick_count::now();
        bestLength = findBestSolutionsParallel(randomWord, allWords, solutions);
        endTime = tick_count::now();

        cout << endl << "Paralelno vreme: " << (endTime - startTime).seconds() << endl;

        cout << "Paralelna duzina: " << bestLength/2 << ", Paralelna resenja:" << endl;

        for (int i = 0;i < solutions.size();i++) {
            cout << solutions[i] << endl;
        }

        cout << endl;
    }
    return 0;
}
