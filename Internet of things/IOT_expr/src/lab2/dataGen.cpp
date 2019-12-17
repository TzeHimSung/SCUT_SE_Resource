#include <bits/stdc++.h>

using namespace std;

struct Package {
    // attribute
    int from_x, from_y, to_x, to_y, length;
    double startTime;
    // build function
    Package() {}
    Package(double _a, int _b, int _c, int _d, int _e, int _f) {
        startTime = _a;
        from_x = _b; from_y = _c;
        to_x = _d; to_y = _e;
        length = _f;
    }
    bool operator<(const Package &rhs)const {
        return startTime - rhs.startTime < 0;
    }
};

vector<Package>v;
const int matrixSize = 9;
const int numOfPackage = 100000;
const double maxi = 4294967295; // the maxinum of mt19937
const int lengthOfPackage = 5;
string sumFileName = "./data/bench";
string fileName = "./data/bench.0.0";

int main() {
    // maxinum is 4294967295 and mininum is 0
    mt19937 rnd(time(0));
    // start generate package
    for (int total = 1; total <= numOfPackage; total++) {
        double currStartTime = (double)rnd() / maxi;
        int currStartX = rnd() % matrixSize, currStartY = rnd() % matrixSize;
        // module 1
        // int currEndX = matrixSize - currStartX - 1, currEndY = matrixSize - currStartY - 1;
        // module 2
        // int currEndX = currStartX, currEndY = currStartY - 1;
        // if (currEndY == -1) {
        //     currEndY = 8, currEndX--;
        // }
        // if (currEndX == -1) currEndX = 8;
        // module 3
        // int currEndX = currStartX, currEndY = currStartY;
        // if ((currEndX == 0 && currEndY == 0) || (currEndX == 8 && currEndY == 8)) {
        //     currEndX = 8 - currEndX, currEndY = 8 - currEndY;
        // }
        // module 4
        int currEndX = (currStartX + 4) % matrixSize, currEndY = (currStartY + 4) % matrixSize;
        v.emplace_back(Package(currStartTime, currStartX, currStartY, currEndX, currEndY, lengthOfPackage));
    }
    // sort package by time
    sort(v.begin(), v.end());
    // output all package
    freopen(sumFileName.c_str(), "a", stdout);
    for (auto curr : v) {
        cout << curr.startTime << " " << curr.from_x << " " << curr.from_y << " "
             << curr.to_x << " " << curr.to_y << " " << curr.length << endl;
    }
    // output package by source coordinate
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fileName[13] = char(i + 48); fileName[15] = char(j + 48);
            freopen(fileName.c_str(), "a", stdout);
            for (auto curr : v) {
                if (curr.from_x == i && curr.from_y == j) {
                    cout << curr.startTime << " " << curr.from_x << " " << curr.from_y << " "
                         << curr.to_x << " " << curr.to_y << " " << curr.length << endl;
                }
            }
        }
    }
    return 0;
}