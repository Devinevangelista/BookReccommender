#include "BookRecommender.h"


//This function is used to compare the average rated books
bool compareBookRating(pair<string,double> book1, pair<string,double> book2) {
    // Sort by rating high to low
    if (book1.second != book2.second) {
        return book1.second > book2.second;
    } else{
        // If the ratings are the same, sort alphabetically
        return book1.first < book2.first;
    }
}
//This function will sort the dot product
bool SortedDotProduct(const pair<double, string>a, const pair<double, string> b){
    if(a.first!=b.first){
        return a.first > b.first;

    }
    else{
        return a.second < b.second;
    }
}

BookRecommender::BookRecommender(){
// Default constructor initializes data members
    books.clear();
    users.clear();
    ratedBooks.clear();
    average.clear();
};

BookRecommender::BookRecommender(string fileName){
// Constructor that reads data from a file and initializes data members
    //open file
    ifstream inFile(fileName);
    string bookTitle;
    string filler;
    string Bookline;
    set <string> bookSet;
    set <string> userSet;
    string userLine;
    string ratingLine;

    if (inFile.is_open()) {
        //reads in every 2 lines to put books in the set
        getline(inFile, filler);
        while(getline(inFile, Bookline)){
            bookSet.insert(Bookline);
            getline(inFile, filler);
            getline(inFile, filler);
            
        }
        //converts book set to vector
        books.assign(bookSet.begin(), bookSet.end());

        /*TESTING PURPOSES outputs the books
        for (auto it : books){
            std::cout << it << endl;
        }
        */
        
    }

    // Reopen file
    inFile.clear();
    inFile.seekg(0,ios::beg);
    
    //loop through file twice and get usernames
    if(inFile.is_open()){
        while(getline(inFile, userLine)){
            userSet.insert(userLine);
            getline(inFile, filler);
            getline(inFile, filler);
            
        }
        //converts book set to vector
        users.assign(userSet.begin(), userSet.end());
    }

    //empty set
    vector<double> emptySet(books.size(), 0);

    for(int i = 0; i < (int)users.size(); i++){
        ratedBooks.insert(make_pair(users[i], emptySet));
    }
    

    inFile.clear();
    inFile.seekg(0,ios::beg);
    //loop through file 3rd time to assign key and elements to dictionary ratedBooks
    if(inFile.is_open()){

        while (getline(inFile, userLine)){
            getline(inFile, Bookline);
            getline(inFile, ratingLine);

            //find book within the books vector
            auto bookPosition = find(books.begin(), books.end(), Bookline);
            //sets position of book = to rating
            ratedBooks.at(userLine)[bookPosition-books.begin()] = stoi(ratingLine);
            
        }
        /* TESTING PURPOSES
        for (auto it : ratedBooks){
            std::cout << it.first << endl;
            for(int i = 0; i < it.second.size(); i++){
                cout << " " << it.second[i] << endl;
            }
        }
        */

    }

    //find averages of book ratings
    for(int i = 0; i < (int)books.size(); i++){
        double sum = 0;
        int count = 0;
        for(auto it : ratedBooks){
            double rating = it.second[i];
            if(rating != 0){
                sum+= rating;
                count++;
            }
        }
        double avg = count == 0 ? 0 : sum/count;
        average.insert(make_pair(books[i], avg));
    }
  
    
}


void BookRecommender::printRecommend(string userName){
    
    for (auto &user: users){
        user.substr(0, user.length()-1);
    }
    if(find(users.begin(), users.end(), userName)==users.end()){
        //print averages if user input is not in list
        printAverages();
        return;
    }
    else{
        //recommend algorithim
        vector<pair<double,string>> recommend; //holds dot product results
        vector<double>userVector = ratedBooks.at(userName); //create vector with inputted name
        for(auto it = ratedBooks.begin(); it != ratedBooks.end(); it++){//look through each user
            if(it->first != userName) // doesn't compute similarity between itself
            {
                double similar = getSimilarity(userName, it->first);
                recommend.push_back(make_pair(similar, it->first));
            }
        } 
        //sort vector of similarities by value then name
        sort(recommend.begin(), recommend.end(), SortedDotProduct);

        vector<double> averageRatings;
        vector<pair<double, string>> topRatings;
        for(int i = 0; i < 3; i++){
            topRatings.push_back(recommend[i]);
        }
        for(int i = 0; i<(int)books.size(); i++){
            double sum = 0,
                   total = 0,
                   avg = 0;
            if(ratedBooks.at(topRatings[0].second)[i] !=0){
                sum+=ratedBooks.at(topRatings[0].second)[i];
                total++;
            }
            if(ratedBooks.at(topRatings[1].second)[i] !=0){
                sum += ratedBooks.at(topRatings[1].second)[i];
                total++;
            }
            if(ratedBooks.at(topRatings[2].second)[i] !=0){
                sum+=ratedBooks.at(topRatings[2].second)[i];
                total++;
            }
            //take average if a rating was found
            if(total!=0)avg = sum/total;
            if(total==0)avg=0;
            averageRatings.push_back(avg);
        }
        //make list of average and usernames to sort
        vector<pair<double, string>> averageBook;
        for(int i = 0; i < (int)books.size(); i++){
            averageBook.push_back(make_pair(averageRatings[i],books[i]));
        }

        sort(averageBook.begin(),averageBook.end(),SortedDotProduct);

        for(auto it: averageBook){
            if(it.first > 0){
                cout << it.second.erase(it.second.size()-1) << " " << fixed << setprecision(2) << it.first << endl;
            }
        }
    
    }
}

void BookRecommender::printAverages(){

    //sorts alphebetically
    vector<pair<string, double>> sortedAvg(average.begin(), average.end());
    sort(sortedAvg.begin(), sortedAvg.end(), compareBookRating);

    for(auto it : sortedAvg){
        cout << it.first.erase(it.first.size()-1) << " " << fixed << setprecision(2) << it.second << endl;
    }
    
}

double BookRecommender::getSimilarity(string user1, string user2){
    vector<double> userVec1 = ratedBooks.at(user1);
    vector<double> userVec2 = ratedBooks.at(user2);
    double similar = 0.0;
    for(int i = 0; i < (int)userVec1.size(); i++){
        similar += userVec1[i]*userVec2[i];
    }
    return similar;
}



