#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stack>
#include <vector>
#include <limits.h>
#include <vector>
#include <list>
#include <time.h>
#include <algorithm>

using namespace std;

class Board{//to avoid using global variables I made class
public:
    Board(int _n, bool ib);//constructor
    void printall();//printer function
    void backtrack();//main recursive function
    void solve();//just activator

private:
    vector<int> table;  // table[i] means that queen at column i is at (i, table[i]) coordinates
    vector<int> forward_diagonals; // to keep used or unused diagonals
    vector<int> back_diagonals; //to keep used or unused diagonals
    vector<int> rows; //to keep used or unused rows
    vector<int> for_shuffling; // to random iterate for possible values of queen
    vector<int> random_iterating; // to random iterate between choosing next queen
    int n; //size
    int index; // it is index of current queen (at which column it is)
    bool status; //status instead of result to indicate did we finished our task
    vector<bool> tip; // to print only assigned queen
    bool isverb; // isVerbose
    int cnt_node;

};

Board::Board(int _n, bool ib): n(_n), isverb(ib){//constructor like initializer
    table.resize(n + 1);//resizing all needed vector to work as arrays
    tip.resize(n + 1);
    random_iterating.resize(n);
    for_shuffling.resize(n);
    for(int i = 0; i < n; i++){
        table[i] = 0;
        tip[i] = 0;
        for_shuffling[i] = i;
        random_iterating[i] = i;
    }
    forward_diagonals.resize(2 * n + 10);//here 2 * n + 10 because i - table[i] which defines diagonal can be negative so to avoid we use this
    back_diagonals.resize(2 * n + 10);
    rows.resize(2 * n + 10);
    for(int i = 0; i < 2 * n + 9; i++){//-1 it means all diagonals and rows are unused
        forward_diagonals[i] = -1;
        back_diagonals[i]= -1;
        rows[i] = -1;
    }
    index = 0;//starting queen's index in random shuffle
    status = 0;//global status of task
    cnt_node = 0;
}

void Board::printall(){//simple printing
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(tip[j] && table[j] == i)
                cout << "Q";
            else
                cout << "*";
        }
        cout << endl;
    }
}

void Board::backtrack(){//main recursive algorithm
    int t = random_iterating[index]; // t means current real index of queen
    if(index == n)//terminating case
        status = 1;
    else{
        int i; // will be current considering value also in random_shuffle
        random_shuffle(for_shuffling.begin(), for_shuffling.end());
        for(int k = 0; k < n; k++){
            i = for_shuffling[k];
            if(rows[i] != -1)//check whether rows is used
                continue;
            if(forward_diagonals[n + 4 + t - i] != -1)// check whether forward diagonal is used
                continue;
            if(back_diagonals[t + i] != -1)// check whether back diagonal is used
                continue;
            table[t] = i;//so cell i not under attack let assign this cell to queen
            tip[t] = 1;//means that that queen was assigned
            rows[i] = t;// i's rows is used by queen t
            forward_diagonals[n + 4 + t - i] = t; //same
            back_diagonals[t + i] = t; // same
            if(isverb) {
                for(int i = 0; i < n; i++) {
                    cout <<"-";
                }
                cout << " " <<cnt_node << endl;
                cnt_node++;
                printall();
            }
            index++;//lets choose next unused queen
            backtrack(); //call recursion
            if(status == 1)//if result != failure return result
                return;
            index--;//coming again
            rows[i] = -1;//unassigning queen
            forward_diagonals[n + 4 + t - i] = -1;//same
            back_diagonals[t + i] = -1;//same
            tip[t] = 0;//unassigning queen
        }
    }
}

void Board::solve(){
    random_shuffle(random_iterating.begin(), random_iterating.begin());
    backtrack();
    if(!isverb)
        printall();
}

void solve_nqueen(int size, bool isVerbose) {
    //srand(time(NULL));
    srand(23); //random seed for random shuffles
    Board bd(size, isVerbose); // creating our Board
    bd.solve(); //main solver function in Board

}

/*
 * The main function.
 */
int main(int argc, char** argv) {

    int size;
    bool isVerbose = false, isArgValid = true;

    if (argc==2) {
        size = atoi(argv[1]);
        isArgValid = (size>0);
    } else if (argc==3) {
        if (strcmp(argv[1], "-v")==0) {
            isVerbose = true;
            size = atoi(argv[2]);
            isArgValid = (size>0);
        } else {
            isArgValid = false;
        }
    } else {
        isArgValid = false;
    }

    if (!isArgValid) {
        cout << "Error in command line arguments." << endl;
        cout << "Usage: " << argv[0] << " [-v] n" << endl;
        cout << "where n is the number of queens and the size of the board (nxn)." << endl;
        cout << "The option -v enables the output of the intermediate states of" << endl;
        cout << "the search procedure." << endl;
        return 1;
    }


    solve_nqueen(size, isVerbose);

    return 0;
}

