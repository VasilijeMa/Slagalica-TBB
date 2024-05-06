# Slagalica game
Created by students of the Faculty of technical sciences in Novi Sad, Serbia:
- SV8/2021 Milan Jovkić
- SV15/2021 Vasilije Marković

This project represents a C++ console application which simulates one of the games on the Serbian TV quiz "Slagalica", with the same name.
The game plays similar to Scrabble, in that contestants try find the longest possible anagram of the given set of 12 letters.

The program uses a dataset of acceptable Serbian words from a non-official dictionary, found in the file data/Recnik.txt.
The initial letter set is randomly generated from the file data/Slova.txt, with increased probability for vowels.

Parallelism for finding the optimal anagram was achieved via TBB task_groups.
