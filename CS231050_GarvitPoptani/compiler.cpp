/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

unordered_map<string, int> symTable;
mutex tableMutex;

int getValue(const string& token) {
    if (isdigit(token[0]))
        return stoi(token);

    lock_guard<mutex> lock(tableMutex);
    return symTable[token];
}

void executeStatement(string stmt) {

    stringstream ss(stmt);
    string token;

    ss >> token;

    // int x = 10;
    if (token == "int") {
        string name, eq, value;
        ss >> name >> eq >> value;

        int val = stoi(value);

        lock_guard<mutex> lock(tableMutex);
        symTable[name] = val;
    }
    // x = x + y;
    else {
        string name = token;
        string eq, v1, op, v2;

        ss >> eq >> v1 >> op >> v2;

        int result = getValue(v1);

        if (op == "+")
            result += getValue(v2);

        lock_guard<mutex> lock(tableMutex);
        symTable[name] = result;
    }
}

int main() {

    string code =
        "int x = 10;"
        "int y = 5;"
        "x = x + y;"
        "y = x + 20;";

    // Split by ';'
    vector<string> statements;
    string temp;

    for (char c : code) {
        if (c == ';') {
            statements.push_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }

    vector<thread> threads;

    for (auto& stmt : statements) {
        threads.emplace_back(executeStatement, stmt);
    }

    for (auto& t : threads)
        t.join();

    cout << "Symbol Table:\n";
    for (auto& p : symTable)
        cout << p.first << " = " << p.second << endl;

    return 0;
}