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

class Board1{
public:
    Board1(){};
    Board1(int _n, bool ib);
    void update_conflicts();//updates conflict list
    void initialize();//giving initial values
    void modify_conflicts(vector<bool>& a1, vector<int>& al, vector<int>& ar, int exp, int var);//modifies conflicts
    void modify_confs(vector<int>& al, vector<int>& ar, int exp, int var, int& cnt);//modifies during steps
    void solver();//main solver
    void printall();//printer
    void start1();//starter
private:
    vector<int> forward_diagonals_left; //for finding most left used queen in diagonal
    vector<int> forward_diagonals_right; //same for the most right
    vector<bool> fd;
    vector<int> back_diagonals_left;//sane here
    vector<int> back_diagonals_right;
    vector<bool> bd;
    vector<int> rows_left;
    vector<int> rows_right;
    vector<bool> rw;
    vector<int> table;
    vector<int> conflicts;
    vector<int> forsuffle;
    int n;
    int con;
    int cnt;
    bool isverb;
    int cnt_node;
};

Board1::Board1(int _n, bool ib): n(_n), isverb(ib){//constructor resizing all needed vectors
        conflicts.resize(n + 1);
        table.resize(n + 1);
        forward_diagonals_left.resize(2 * n + 10);
        forward_diagonals_right.resize(2 * n + 10);
        back_diagonals_left.resize(2 * n + 10);
        back_diagonals_right.resize(2 * n + 10);
        rows_left.resize(2 * n + 10);
        rows_right.resize(2 * n + 10);
        fd.resize(2 * n + 10);
        bd.resize(2 * n + 10);
        rw.resize(2 * n + 10);
        forsuffle.resize(n);
        for(int i = 0; i < n; i++){
            forsuffle[i] = i;
        }
        cnt_node = 0;
    }

void Board1::solver(){//main solver we need timer for terminating
    time_t tmer = time(NULL) + 100;
    bool status = 0;
    srand(time(NULL));
    start1();
    update_conflicts();
    int timer = 0;
    while(con && timer < 20000){//our main loop
        if(time(NULL) > tmer)
            break;
        vector<int> conflicted;
        for(int i = 0; i < n; i++){
            if(conflicts[i])
                conflicted.push_back(i);//to choose random queen
        }
        int randvar = rand() % conflicted.size();
        randvar = conflicted[randvar];
        int min_conf = INT_MAX;//initializing min_conf
        int minindex;
        random_shuffle(forsuffle.begin(), forsuffle.end());//reshuffling
        for(int k = 0; k < n; k++){
            int i = forsuffle[k];
            if(i != table[randvar]){
                cnt = 6;
                modify_confs(forward_diagonals_left, forward_diagonals_right, n + 4 + randvar - i, randvar, cnt);//finding potential conflicts
                modify_confs(back_diagonals_left, back_diagonals_right, randvar + i, randvar, cnt);
                modify_confs(rows_left, rows_right, i, randvar, cnt);
                if(min_conf > cnt){//if found less saving it
                    min_conf = cnt;
                    minindex = i;
                }
            }
        }
        table[randvar] = minindex;//giving to queen new value
        update_conflicts();//
        if(isverb) {
            for(int i = 0; i < n; i++) {
                cout <<"-";
            }
            cout << " " <<cnt_node << endl;
            cnt_node++;
            printall();
        }
        timer++;//nextloop
    }
    if(con == 0){
        status = 1;
        printall();
    }
    if(!status){
        cout << "No solution.";
    }
}

void Board1::start1(){//start nights table)
    int night = 0;
    for(int i = 0; i < n; i++){
        table[i] = night;
        night += 2;
        if(night >= n)
            night = 1;
    }
}

void Board1::printall(){//simple printer
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(table[j] == i)
                cout << "Q";
            else
                cout << "*";
        }
        cout << endl;
    }
}

void Board1::update_conflicts(){//calculates all tables
    initialize();
    for(int i = 0; i < n; i++){
        modify_conflicts(fd, forward_diagonals_left, forward_diagonals_right, n + 4 + i - table[i], i);
        modify_conflicts(bd, back_diagonals_left, back_diagonals_right, i + table[i], i);
        modify_conflicts(rw, rows_left, rows_right, table[i], i);
    }
    con = 0;
    for(int i = 0; i < n; i++){
        cnt = 6;
        modify_confs(forward_diagonals_left, forward_diagonals_right, n + 4 + i - table[i], i, cnt);
        modify_confs(back_diagonals_left, back_diagonals_right, i + table[i], i, cnt);
        modify_confs(rows_left, rows_right, table[i], i, cnt);
        conflicts[i] = cnt;
        con += cnt;
    }
}

void Board1::initialize(){
    for(int i = 0; i < 2 * n + 10; i++){//giving initial values to vectors
        forward_diagonals_left[i] = INT_MAX;
        forward_diagonals_right[i] = INT_MIN;
        back_diagonals_left[i] = INT_MAX;
        back_diagonals_right[i] = INT_MIN;
        rows_left[i] = INT_MAX;
        rows_right[i] = INT_MIN;
        fd[i] = 0;
        bd[i] = 0;
        rw[i] = 0;
    }
}

void Board1::modify_conflicts(vector<bool>& a1, vector<int>& al, vector<int>& ar, int exp, int var){//to modify tables on each potential step
    if(a1[exp]){
        if(al[exp] > var)
            al[exp] = var;
        if(ar[exp] < var)
            ar[exp] = var;
    }
    else{
        a1[exp] = 1;
        al[exp] = var;
        ar[exp] = var;
    }
}

void Board1::modify_confs(vector<int>& al, vector<int>& ar, int exp, int var, int& cnt){//simple updater of the edges
    if(al[exp] >= var)
        cnt--;
    if(ar[exp] <= var)
        cnt--;
}

class Board{//second board
public:
    Board(){};
    Board(int _n, bool ib): n(_n), isverb(ib){
        table.resize(n + 1);
        forward_diagonals.resize(2 * n + 10);
        back_diagonals.resize(2 * n + 10);
        forsuffle.resize(n);
        shuffle_iterating.resize(n);
        for(int i = 0; i < n; i++){
            forsuffle[i] = i;
            shuffle_iterating[i] = i;
        }
        cnt_node = 0;
    }
    void initialize();
    int expectconf(int y, int x);
    void modify_confs(int y, int x);
    void solver();
    void printall();
    void start1();
private:
    vector<int> forward_diagonals;//whether used diagonals or not
    vector<int> back_diagonals;//same
    vector<int> table;
    vector<int> forsuffle;
    vector<int> shuffle_iterating;//private is the same
    int n;
    int con;
    int cnt;
    bool isverb;
    int cnt_node;
};

void Board::solver(){

    bool status = 0;
    for(int q = 0; q < 100; q++){
        con = 0;
        srand(time(NULL));
        start1();
        initialize();
        int timer = 0;
        int max_steps;
        if(q & 1)
            max_steps = 10000;
        else
            max_steps = 200;
        while(con && timer < max_steps){
            vector<int> conflicted;
            for(int i = 0; i < n; i++){
                if(forward_diagonals[n + 4 + i - table[i]] || back_diagonals[i + table[i]])
                    conflicted.push_back(i);
            }
            int randvar = rand() % conflicted.size();
            randvar = conflicted[randvar];
            int min_conf = INT_MAX;
            int minindex;
            random_shuffle(forsuffle.begin(), forsuffle.end());
            for(int k = 0; k < n; k++){
                int i = forsuffle[k];
                if(i != randvar){
                    int expected = expectconf(i, randvar);
                    if(expected < min_conf){
                        min_conf = expected;
                        minindex = i;
                    }
                }
            }
            modify_confs(minindex, randvar);
            if(isverb) {
                for(int i = 0; i < n; i++)
                    cout <<"-";
                cout << " " <<cnt_node << endl;
                cnt_node++;
                printall();
            }
            timer++;
        }
        if(con == 0){
            status = 1;
            if(!isverb)
                printall();
            break;
        }
    }
    if(!status){
        cout << "No solution.";
    }
}

void Board::start1(){
    random_shuffle(shuffle_iterating.begin(),shuffle_iterating.end());
    for(int i = 0; i < n; i++){
        table[i] = shuffle_iterating[i];
    }
}

void Board::printall(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(table[j] == i)
                cout << "Q";
            else
                cout << "*";
        }
        cout << endl;
    }
}

void Board::initialize(){
    for(int i = 0; i < 2 * n + 10; i++){
        forward_diagonals[i] = 0;
        back_diagonals[i] = 0;
    }
    for(int i = 0; i < n; i++){
        con += forward_diagonals[n + 4 + i - table[i]];
        con += back_diagonals[i + table[i]];
        forward_diagonals[n + 4 + i - table[i]]++;
        back_diagonals[i + table[i]]++;
    }
}

int Board::expectconf(int y, int x){//testing expected
    int expnumber = con;
    expnumber -= (forward_diagonals[n + 4 + x - table[x]] - 1);
    expnumber -= (back_diagonals[x + table[x]] - 1);
    expnumber += forward_diagonals[n + 4 + x - table[y]];
    expnumber += back_diagonals[x + table[y]];
    forward_diagonals[n + 4 + x - table[x]]--;
    forward_diagonals[n + 4 + x - table[y]]++;
    back_diagonals[x + table[x]]--;
    back_diagonals[x + table[y]]++;
    expnumber -= (forward_diagonals[n + 4 + y - table[y]] - 1);
    expnumber -= (back_diagonals[y + table[y]] - 1);
    expnumber += forward_diagonals[n + 4 + y - table[x]];
    expnumber += back_diagonals[y + table[x]];
    forward_diagonals[n + 4 + x - table[x]]++;
    forward_diagonals[n + 4 + x - table[y]]--;
    back_diagonals[x + table[x]]++;
    back_diagonals[x + table[y]]--;
    return expnumber;
}

void Board::modify_confs(int y, int x){//swap operation
    con -= (forward_diagonals[n + 4 + x - table[x]] - 1);
    con -= (back_diagonals[x + table[x]] - 1);
    con += forward_diagonals[n + 4 + x - table[y]];
    con += back_diagonals[x + table[y]];
    forward_diagonals[n + 4 + x - table[x]]--;
    forward_diagonals[n + 4 + x - table[y]]++;
    back_diagonals[x + table[x]]--;
    back_diagonals[x + table[y]]++;
    con -= (forward_diagonals[n + 4 + y - table[y]] - 1);
    con -= (back_diagonals[y + table[y]] - 1);
    con += forward_diagonals[n + 4 + y - table[x]];
    con += back_diagonals[y + table[x]];
    forward_diagonals[n + 4 + y - table[y]]--;
    forward_diagonals[n + 4 + y - table[x]]++;
    back_diagonals[y + table[y]]--;
    back_diagonals[y + table[x]]++;
    swap(table[x], table[y]);
}




void solve_nqueen(int size, bool isVerbose) {
    if(size % 6 == 3 || size % 6 == 4){
        Board temp(size,isVerbose);
        temp.solver();
    }
    else{
        Board1 st(size,isVerbose);
        st.solver();
    }
}

/*
 * The main function.
 */
int main(int argc, char** argv) {

    int size;
    bool isVerbose, isArgValid = true;

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

