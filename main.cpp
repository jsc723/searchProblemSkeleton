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
        /* initialize the initial state */
        s_travel = Concat(n);
        for(int i = 0; i < N; i++) {
            travelled[i] = false;
            b[i] = (s[i] == 's');
        }
        cost = heuristic();
    }

    //arguments passed to generate a successor
    struct SuccessorInfo {
        /* arguments list */
        int n;
        /* constructor */
        SuccessorInfo(int n):n(n){}
    };

    //next state
    State(const State &prev, SuccessorInfo info):n(info.n) {
        /* given previous state, how to generate a succesor state? */
        for(int i = 0; i < N; i++) {
              travelled[i] = prev.travelled[i];
              b[i] = (i > n) ? !prev.b[i] : prev.b[i];
        }
        travelled[n] = true;
        s_travel = Concat(prev.s_travel)(" ")(n + 1);

        //get estimated cost
        getTotalCost(prev);
    }

    //list all successors
    typedef vector<SuccessorInfo> scrList;
    typedef scrList::iterator scrIter;
    scrList getSuccessors() {
        scrList successors;
        //what is a successor?
        for(int i = 0; i < N; i++) {
            if(i != n && b[i]) {
                successors.push_back(SuccessorInfo(i));
            }
        }
        return successors;
    }

    //how to compare two state in priority_queue
    struct Comparator_PQ {
        bool operator()(const State &a, const State &b)
        {
            return a.cost > b.cost; //min heap
        }
    };

    //how to compare two state in set (doesn't matter for tree search)
    struct Comparator_Set {
        bool operator()(const State &a, const State &b)
        {
            if (a.n != b.n)
                return a.n < b.n;
            for(int i = 0; i < N; i++) {
                if(a.b[i] != b.b[i])
                    return a.b[i] < b.b[i];
                else if(a.travelled[i] != b.travelled[i])
                    return a.travelled[i] < b.travelled[i];
            }
            return false; //a == b
        }
    };


    //define what is a goal
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
    //what is the estimated total cost?
    void getTotalCost(const State &prev) {
        cost = prev.cost;
        cost += n < prev.n ? c_b : c_f; //may change this line
        cost += heuristic();
    }
    //what is the estimated cost from current state to goal?
    double heuristic() {
        double h = N;
        for(int i = 0; i < N; i++) {
            if(travelled[i])
                h--;
        }
        return h;
    }

    //how to define a state?
    int n; //should be an index
    bool b[MAX_N], travelled[MAX_N];
    string s_travel;

    //do not modify this
    double cost;

};

struct searchProblemSolver {
    //solve a tree search problem
    static string solveTree(State initState) {
        priority_queue<State, vector<State>, State::Comparator_PQ> pq;
        pq.push(initState);
        while(!pq.empty()) {
            State s = pq.top();
            pq.pop();
            if(s.isGoal())
                return s.showPath();
            State::scrList scr = s.getSuccessors();
            for(State::scrIter i = scr.begin();i != scr.end(); i++) {
                pq.push(State(s, *i));
            }
        }
        return "no solution";
    }

    //solve a graph search problem
    static string solveGraph(State initState) {
        priority_queue<State, vector<State>, State::Comparator_PQ> pq;
        set<State, State::Comparator_Set> closed;
        pq.push(initState);
        while(!pq.empty()) {
            State s = pq.top();
            pq.pop();
            if(s.isGoal())
                return s.showPath();
            if(closed.find(s) == closed.end()) {
                closed.insert(s);
                State::scrList scr = s.getSuccessors();
                for(State::scrIter i = scr.begin();i != scr.end(); i++) {
                    pq.push(State(s, *i));
                }
            }
        }
        return "no solution";
    }
};

int main()
{
    string s;
    cin >> N >> c_f >> c_b >> s;
    cout << searchProblemSolver::solveGraph(State(N, s)) << endl;
    return 0;
}

