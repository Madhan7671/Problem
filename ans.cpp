#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

struct Point
{
    long long x;
    long long y;
};

long long baseToDec(const string &val, int base)
{
    long long num = 0;
    for (char c : val)
    {
        int digit;
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else
            digit = tolower(c) - 'a' + 10;
        num = num * base + digit;
    }
    return num;
}

double lagrangeAtZero(const vector<Point> &points, int k)
{
    double result = 0;
    for (int i = 0; i < k; i++)
    {
        double term = points[i].y;
        for (int j = 0; j < k; j++)
        {
            if (i == j)
                continue;
            term *= (-points[j].x) / (double)(points[i].x - points[j].x);
        }
        result += term;
    }
    return result;
}

long long processTestCase(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file" << endl;
        return -1;
    }

    string line;
    int n = 0, k = 0;
    vector<Point> points;

    string currentX, base, value;
    while (getline(file, line))
    {
        // remove spaces
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

        // parse n and k
        if (line.find("\"n\"") != string::npos)
        {
            size_t pos = line.find(":");
            n = stoi(line.substr(pos + 1));
        }
        else if (line.find("\"k\"") != string::npos)
        {
            size_t pos = line.find(":");
            k = stoi(line.substr(pos + 1));
        }
        // detect point key x
        else if (line[0] == '"' && line.find(":") != string::npos && line.find("base") == string::npos)
        {
            currentX = line.substr(1, line.find("\"", 1) - 1);
        }
        // detect base
        else if (line.find("\"base\"") != string::npos)
        {
            size_t pos = line.find(":");
            base = line.substr(pos + 2);
            if (base.back() == '"')
                base.pop_back();
        }
        // detect value
        else if (line.find("\"value\"") != string::npos)
        {
            size_t pos = line.find(":");
            value = line.substr(pos + 2);
            if (value.back() == ',')
                value.pop_back();
            if (value.back() == '"')
                value.pop_back();

            long long x = stoll(currentX);
            long long y = baseToDec(value, stoi(base));
            points.push_back({x, y});
        }
    }

    file.close();

    if ((int)points.size() < k)
    {
        cerr << "Error: not enough points" << endl;
        return -1;
    }

    vector<Point> subset(points.begin(), points.begin() + k);
    return (long long)llround(lagrangeAtZero(subset, k));
}

int main()
{
    string filename;
    cout << "Enter JSON filename: ";
    cin >> filename;

    long long secret = processTestCase(filename);
    if (secret != -1)
    {
        cout << "\nSecret (c): " << secret << endl;
    }
    return 0;
}
