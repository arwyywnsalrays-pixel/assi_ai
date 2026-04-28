#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

/*====================================================
   STATE CLASS
====================================================*/
class State {
public:
    int x, y;
    int fuel;
    bool c1, c2, c3, c4;

    int g; // path cost
    int h; // heuristic
    int f; // total cost

    string path;

    State(int x = 1, int y = 1, int fuel = 20,
          bool c1 = false, bool c2 = false,
          bool c3 = false, bool c4 = false) {

        this->x = x;
        this->y = y;
        this->fuel = fuel;
        this->c1 = c1;
        this->c2 = c2;
        this->c3 = c3;
        this->c4 = c4;

        g = h = f = 0;
        path = "";
    }

    bool operator<(const State& other) const {
        return tie(x,y,fuel,c1,c2,c3,c4) <
               tie(other.x,other.y,other.fuel,
               other.c1,other.c2,other.c3,other.c4);
    }
};
