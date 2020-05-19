//
// Created by sobral on 27/07/19.
//
#include <limits.h>
#include <time.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include "Terminal.h"
#include "gtest/gtest.h"

using namespace std;

#define PROG "./Prog"

struct Result {
    int k, x, y;

    Result(const string & s) {
        istringstream inp(s);

        inp >> k >> x >> y;
    }
    Result() {}
    bool operator==(const Result & o) const {
        return k == o.k and ((x == o.x and y == o.y) or (x == o.y and y == o.x));
    }
    bool operator<(const Result & o) const {
        if (k == o.k) {
            return x < o.x;
        }
        return k < o.k;
    }

    class R_Hash {
    public:
        long operator()(const Result & r) const {
            long h = r.x ^ r.y ^ r.k;
            return h;
        }
    };

    operator string() const {
        string r = to_string(k)+' ';
        if (x < y) r += to_string(x)+' '+to_string(y);
        else r += to_string(y)+' '+to_string(x);
        return r;
    }

};

vector<Result> roda(const string & cmd) {
    Terminal term(cmd);

    auto res = term.leLinhas();
    vector<Result> v;

    for (auto & s: res) v.push_back(Result(s));
    return v;
}

bool operator!=(const vector<Result> & v1, const vector<Result> & v2) {
    unordered_set<Result,Result::R_Hash> set;

    for (auto & r: v2) set.insert(r);
    for (auto & r: v1) {
        if (! set.count(r)) return true;
    }

    return false;
}

void verifica(int n) {
    string arq = "cat ../data/res"+to_string(n)+".txt";
    string arqres = PROG;
    arqres += " ../data/arq"+to_string(n)+".txt";

    auto v = roda(arq);
    auto res = roda(arqres);

    if (v != res) {
        sort(v.begin(), v.end());
        sort(res.begin(), res.end());
        FAIL() << "Esperado: " << v2str(v, ", ") << endl
               << "Obtido: " << v2str(res, ", ");
    }

}

TEST(TermTest, Vazio) {
    verifica(6);
}

TEST(TermTest, Geral) {
    for (int j=0; j < 5; j++) verifica(j);
}

TEST(TermTest, Alguns) {
    verifica(5);
}
