#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;

class State {
public:
    int x, y, fuel;
    bool c1, c2, c3, c4;
    int g, h, f;
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
        return tie(x,y,fuel,c1,c2,c3,c4)
             < tie(other.x,other.y,other.fuel,other.c1,other.c2,other.c3,other.c4);
    }
};

class GridWorld {
public:
    bool isBlocked(int x, int y) {
        return (x == 4 && (y == 7 || y == 8));
    }

    bool isValid(int x, int y) {
        return x >= 1 && x <= 10 && y >= 1 && y <= 10 && !isBlocked(x,y);
    }

    bool isGoal(State s) {
        return s.x == 1 && s.y == 1 &&
               s.c1 && s.c2 && s.c3 && s.c4;
    }

    void collect(State &s) {
        if (s.x == 2 && s.y == 2) s.c1 = true;
        if (s.x == 3 && s.y == 3) s.c2 = true;
        if (s.x == 5 && s.y == 7) s.c3 = true;
        if (s.x == 6 && s.y == 5) s.c4 = true;
    }

    void refill(State &s) {
        if (s.x == 4 && s.y == 9) s.fuel = 20;
    }

    vector<State> nextStates(State cur) {
        vector<State> v;

        int dx[4] = {1,-1,0,0};
        int dy[4] = {0,0,1,-1};
        string mv[4] = {"R ","L ","U ","D "};

        for (int i=0;i<4;i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (!isValid(nx,ny) || cur.fuel <= 0) continue;

            State n = cur;
            n.x = nx;
            n.y = ny;
            n.fuel--;
            n.g = cur.g + 1;

            collect(n);
            refill(n);

            n.path += mv[i];

            v.push_back(n);
        }

        return v;
    }
};

class H {
public:
    static int man(int x1,int y1,int x2,int y2) {
        return abs(x1-x2) + abs(y1-y2);
    }

    static int h1(State s) {
        return man(s.x,s.y,1,1);
    }

    static int h2(State s) {
        return h1(s);
    }
};

struct CG {
    bool operator()(State a, State b) {
        return a.h > b.h;
    }
};

struct CA {
    bool operator()(State a, State b) {
        return a.f > b.f;
    }
};

void greedy(State start, GridWorld w) {

    priority_queue<State, vector<State>, CG> pq;
    map<State,bool> vis;

    int expanded = 0;
    int generated = 0;

    start.h = H::h1(start);
    pq.push(start);
    generated++;

    while (!pq.empty()) {

        State cur = pq.top(); pq.pop();

        if (vis[cur]) continue;
        vis[cur] = true;

        expanded++;

        if (w.isGoal(cur)) {
            cout << "Greedy Cost=" << cur.g << endl;
            cout << "Greedy Path=" << cur.path << endl;
            cout << "Greedy Expanded=" << expanded << endl;
            cout << "Greedy Generated=" << generated << endl;
            return;
        }

        for (auto n : w.nextStates(cur)) {
            n.h = H::h1(n);

            if (!vis[n]) {
                pq.push(n);
                generated++;
            }
        }
    }
}

void astar(State start, GridWorld w) {

    priority_queue<State, vector<State>, CA> pq;
    map<State,int> best;

    int expanded = 0;
    int generated = 0;

    start.h = H::h2(start);
    start.f = start.g + start.h;

    pq.push(start);
    best[start] = 0;
    generated++;

    while (!pq.empty()) {

        State cur = pq.top(); pq.pop();

        expanded++;

        if (w.isGoal(cur)) {
            cout << "A* Cost=" << cur.g << endl;
            cout << "A* Path=" << cur.path << endl;
            cout << "A* Expanded=" << expanded << endl;
            cout << "A* Generated=" << generated << endl;
            return;
        }

        for (auto n : w.nextStates(cur)) {

            n.h = H::h2(n);
            n.f = n.g + n.h;
            if (!best.count(n) || n.g < best[n]) {
                best[n] = n.g;
                pq.push(n);
                generated++;
            }
        }
    }
}

int main() {

    GridWorld w;

    vector<State> tests = {
        State(5,8,6),
        State(1,5,10),
        State(7,7,15),
        State(2,9,20),
        State(6,2,12)
    };

    for (int i=0;i<tests.size();i++) {
        cout << "Test " << i+1 << endl;
        greedy(tests[i], w);
        astar(tests[i], w);
        cout << "-----------------\n";
    }
}

