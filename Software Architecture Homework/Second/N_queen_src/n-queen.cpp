// Project: N-queen problem without optimization
// Author: JHSeng
// Date: 09/30/2019
// Compiler version: GNU G++ 9.1.0

// headers

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>

// using standard library

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;
using std::pair;
using std::make_pair;

// global variables

int __chessboardSize, __numOfSolution;
vector<vector<int>> __map;
set<int> __ansSet;

// function declaration

bool isNumber           (string __str);
int  convertStringToInt (string _str);
void solve              ();
void printChessboard    ();
bool checkPosition      (int __curr_x, int __curr_y);
void dfs                (int __filled);
int  getHash            ();

// function achieve

// check current string is number or not
bool isNumber(string __str) {
    for (auto __i : __str) {
        if (__i < '0' || __i > '9') return false;
    }
    return true;
}

// translate string into integer
int convertStringToInt(string __str) {
    int __ret = 0;
    for (auto __i : __str) {
        __ret = __ret * 10 + __i - '0';
    }
    return __ret;
}

// solve n-queen problem
void solve() {
    // chessboard init
    __ansSet.clear();
    __numOfSolution = 0;

    __map.resize(__chessboardSize + 1);
    for (auto &__i : __map) {
        for (int __j = 0; __j <= __chessboardSize; __j++) {
            __i.emplace_back(0);
        }
    }

    for (int __i = 1; __i <= __chessboardSize; __i++) {
        for (int __j = 1; __j <= __chessboardSize; __j++) {
            if (checkPosition(__i, __j)) {
                __map[__i][__j] = 1;
                dfs(1);
                __map[__i][__j] = 0;
            }
        }
    }
}

// dfs
void dfs(int __filled) {
    // ending condition
    if (__filled == __chessboardSize) {
        // exclude duplicate answers
        int __currHash = getHash();
        if (!__ansSet.count(__currHash)) {
            __ansSet.insert(__currHash);
            printChessboard();
        }
        return;
    }
    // search entire chessboard
    for (int __i = 1; __i <= __chessboardSize; __i++) {
        for (int __j = 1; __j <= __chessboardSize; __j++) {
            if (checkPosition(__i, __j)) {
                __map[__i][__j] = 1;
                dfs(__filled + 1);
                __map[__i][__j] = 0;
            }
        }
    }
}

// check current position is usable or not
bool checkPosition(int __curr_x, int __curr_y) {
    bool __ret = true;

    // current point
    if (__map[__curr_x][__curr_y] == 1) __ret = false;

    // total column
    for (int __i = 1; __i <= __chessboardSize; __i++)
        if (__map[__i][__curr_y] == 1) __ret = false;

    // total row
    for (int __j = 1; __j <= __chessboardSize; __j++)
        if (__map[__curr_x][__j] == 1) __ret = false;

    // diagonal
    int __tmp_x = __curr_x, __tmp_y = __curr_y;
    while (__tmp_x - 1 >= 1 && __tmp_y - 1 >= 1)
        __tmp_x--, __tmp_y--;
    while (__tmp_x <= __chessboardSize && __tmp_y <= __chessboardSize)
        if (__map[__tmp_x++][__tmp_y++] == 1) __ret = false;

    // another diagonal
    __tmp_x = __curr_x, __tmp_y = __curr_y;
    while (__tmp_x - 1 >= 1 && __tmp_y + 1 <= __chessboardSize)
        __tmp_x--, __tmp_y++;
    while (__tmp_x <= __chessboardSize && __tmp_y >= 1)
        if (__map[__tmp_x++][__tmp_y--] == 1) __ret = false;

    return __ret;
}

// print current chessboard
void printChessboard() {
    cout << "No. " << ++__numOfSolution << " solution:" << endl;
    for (int __i = 1; __i <= __chessboardSize; __i++) {
        for (int __j = 1; __j <= __chessboardSize; __j++) {
            cout << __map[__i][__j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// hash current chessboard
int getHash() {
    int __ret = 0;
    for (int __i = 1; __i <= __chessboardSize; __i++) {
        for (int __j = 1; __j <= __chessboardSize; __j++) {
            __ret += __i * __j * __map[__i][__j];
        }
    }
    return __ret;
}

// main logic
int main() {

    // init chessboard size
    string inputString;
    cout << "Please input the number n, n should be smaller or equal then 10: ";
    cin >> inputString;

    // check whether chessboard size is legal
    while (!isNumber(inputString)) {
        cout << "This input is illegal, please input again: ";
        cin >> inputString;
    }
    __chessboardSize = convertStringToInt(inputString);

    // check no solution
    if (__chessboardSize < 4) {
        cout << "The chessboard size is too small, no solution." << endl;
        return 0;
    }

    if (__chessboardSize > 10) {
        cout << "The chessboard size is too large!" << endl;
        return 0;
    }

    if (__chessboardSize >= 7) {
        cout << "The chessboard size is large, please wait for calculating." << endl;
    }

    // solve problem
    cout << "Solving the problem..." << endl << endl;
    solve();
    cout << "All solution have been printed!" << endl;

    return 0;
}