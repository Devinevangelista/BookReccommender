#ifndef BOOKRECCOMENDER_H
#define BOOKRECCOMENDER_H
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

class BookRecommender{
    private:
        vector<string> books;
        vector<string> users;
        map<string, vector<double>> ratedBooks;
        map<string, double> average;
    public:
        BookRecommender();
        BookRecommender(string fileName);
        void printRecommend(string userName);
        void printAverages();
        map<string, double> getAverages() const;
        double getSimilarity(string userName1, string userName2);

};



#endif