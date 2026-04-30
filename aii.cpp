#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <tuple>
#include <cmath>
#include <algorithm>

using namespace std;

/*====================================================
   STATE CLASS
====================================================*/
class State {
public:
    int x, y, fuel;
    bool c1, c2, c3, c4;

    int g, h, f;
    string path;

    State(int x=1,int y=1,int fuel=20,
          bool c1=false,bool c2=false,
          bool c3=false,bool c4=false) {

        this->x=x;
        this->y=y;
        this->fuel=fuel;
        this->c1=c1;
        this->c2=c2;
        this->c3=c3;
        this->c4=c4;

        g=h=f=0;
        path="";
    }

    bool operator<(const State& other) const {
        return tie(x,y,fuel,c1,c2,c3,c4) <
               tie(other.x,other.y,other.fuel,
                   other.c1,other.c2,other.c3,other.c4);
    }
};

/*====================================================
   GRID WORLD
====================================================*/
class GridWorld {
public:

    bool isBlocked(int x,int y){
        return (x==4 && (y==7 || y==8));
    }

    bool isValid(int x,int y){
        if(x<1 || x>10 || y<1 || y>10) return false;
        if(isBlocked(x,y)) return false;
        return true;
    }

    bool isGoal(State s){
        return (s.x==1 && s.y==1 &&
                s.c1 && s.c2 && s.c3 && s.c4);
    }

    void collectCoins(State &s){
        if(s.x==2 && s.y==2) s.c1=true;
        if(s.x==3 && s.y==3) s.c2=true;
        if(s.x==5 && s.y==7) s.c3=true;
        if(s.x==6 && s.y==5) s.c4=true;
    }

    void refillFuel(State &s){
        if(s.x==4 && s.y==9) s.fuel=20;
    }

    vector<State> getNextStates(State cur){

        vector<State> next;

        int dx[]={1,-1,0,0};
        int dy[]={0,0,1,-1};
        string mv[]={"R ","L ","U ","D "};

        for(int i=0;i<4;i++){

            int nx=cur.x+dx[i];
            int ny=cur.y+dy[i];

            if(!isValid(nx,ny)) continue;
            if(cur.fuel<=0) continue;

            State ns=cur;

            ns.x=nx;
            ns.y=ny;
            ns.fuel--;
            ns.g=cur.g+1;

            collectCoins(ns);
            refillFuel(ns);

            ns.path+=mv[i];

            next.push_back(ns);
        }

        return next;
    }
};

/*====================================================
   HEURISTICS
====================================================*/
class Heuristic {
public:

    static int manhattan(int x1,int y1,int x2,int y2){
        return abs(x1-x2)+abs(y1-y2);
    }

    /* h1 = Nearest Neighbor Sum الحقيقي */
    static int h1(State s){

        vector<pair<int,int>> coins;

        if(!s.c1) coins.push_back({2,2});
        if(!s.c2) coins.push_back({3,3});
        if(!s.c3) coins.push_back({5,7});
        if(!s.c4) coins.push_back({6,5});

        int cx=s.x, cy=s.y;
        int total=0;

        while(!coins.empty()){

            int best=9999, idx=0;

            for(int i=0;i<coins.size();i++){
                int d=manhattan(cx,cy,
                                coins[i].first,
                                coins[i].second);

                if(d<best){
                    best=d;
                    idx=i;
                }
            }

            total+=best;
            cx=coins[idx].first;
            cy=coins[idx].second;

            coins.erase(coins.begin()+idx);
        }

        total+=manhattan(cx,cy,1,1);

        return total;
    }

    static int h2(State s){

        int mx=0;

        if(!s.c1) mx=max(mx,manhattan(s.x,s.y,2,2));
        if(!s.c2) mx=max(mx,manhattan(s.x,s.y,3,3));
        if(!s.c3) mx=max(mx,manhattan(s.x,s.y,5,7));
        if(!s.c4) mx=max(mx,manhattan(s.x,s.y,6,5));

        if(mx==0)
            mx=manhattan(s.x,s.y,1,1);

        return mx;
    }
};


