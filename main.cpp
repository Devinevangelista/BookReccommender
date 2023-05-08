#include "BookRecommender.h"

using namespace std;

int main(int argc, char *argv[]){
    string in = "";
    string userName;
    BookRecommender BookRecommender(argv[1]);
    //take input in and do requested function
    cin >> in;
    while(in != "quit"){
        if (in == "recommend"){
            string user;
            cin >> user;
            user+='\r';
            BookRecommender.printRecommend(user);
        }
        if(in == "averages"){

            BookRecommender.printAverages();
        }
        cin >> in;
    }


    return 0;

}