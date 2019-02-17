#include <bits/stdc++.h>
#define MAX_N 8
using namespace std;
int N, c_f, c_b;

//used to concat arguments into string
struct Concat {
    Concat(){}
    template<class T>
    Concat(T arg) {
        operator()(arg);
    }
    template<class T>
    Concat &operator()(T arg) {
        ss << arg;
        return *this;
    }
    operator string() {
        return ss.str();
    }

    stringstream ss;
};


class State {
public:

    //initial state
    State(int n, string s):n(n - 1){
        s_travel = Concat(n);
        for(int i = 0; i < N; i++) {
            travelled[i] = false;
            b[i] = (s[i] == 's');
        }
        cost = heuristic();
    }

    //arguments passed to generate a successor
    struct SuccessorInfo {
        int n;
        SuccessorInfo(int n):n(n){}
    };

    //next state
    State(const State &prev, SuccessorInfo info):n(info.n) {
        for(int i = 0; i < N; i++) {
              travelled[i] = prev.travelled[i];
              b[i] = (i > n) ? !prev.b[i] : prev.b[i];
        }
        travelled[n] = true;
        s_travel = Concat(prev.s_travel)(" ")(n + 1);

        getTotalCost(prev);
    }

    //list all successors
    typedef vector<SuccessorInfo> scrList;
    typedef scrList::iterator scrIter;
    scrList getSuccessors() {
        scrList successors;
        for(int i = 0; i < N; i++) {
            if(i != n && b[i]) {
                successors.push_back(SuccessorInfo(i));
            }
        }
        return successors;
    }

    //how to compare
    struct Comparator {
        bool operator()(const State &a, const State &b)
        {
            return a.cost > b.cost; //min heap
        }
    };

    //define what is goal
    bool isGoal() {
        for(int i = 0; i < N; i++) {
            if(!travelled[i])
                return false;
        }
        return n + 1 == N;
    }

    //print path that leads to goal
    string showPath() {
        return s_travel;
    }

private:

    void getTotalCost(const State &prev) {
        cost = prev.cost;
        cost += n < prev.n ? c_b : c_f;
        cost += heuristic();
    }
    double heuristic() {
        double h = N;
        for(int i = 0; i < N; i++) {
            if(travelled[i])
                h--;
        }
        return h;
    }

    int n; //should be an index
    bool b[MAX_N], travelled[MAX_N];
    string s_travel;
    double cost;

};

string solveSearchProblem(State initState) {
    priority_queue<State, vector<State>, State::Comparator> pq;
    pq.push(initState);
    while(!pq.empty()) {
        State s = pq.top();
        pq.pop();
        if(s.isGoal()) {
            return s.showPath();
        }
        State::scrList scr = s.getSuccessors();
        for(State::scrIter i = scr.begin();i != scr.end(); i++) {
            pq.push(State(s, *i));
        }
    }
    return "no solution";
}

int main()
{
    string s;
    cin >> N >> c_f >> c_b >> s;
    cout << solveSearchProblem(State(N, s)) << endl;
    return 0;
}

