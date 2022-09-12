#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream> // No additional includes or STL allowed!
using namespace std;

#define ARSIZE 5500 // for changing size on the go
#define COLMAX 37

struct Query
{
    int colCount = 0;
    int whereCount = 0;
    string colList[COLMAX] = {"", "", "", "", "", "", "", "", "", "",
                              "", "", "", "", "", "", "", "", "", "",
                              "", "", "", "", "", "", "", "", "", "",
                              "", "", "", "", "", "", ""};
    string whereLeft[5] = {"", "", "", "", ""};
    string whereRight[5] = {"", "", "", "", ""};
    string logicList[5] = {"", "", "", "", ""};
    string ANDORlist[4] = {"", "", "", ""};
};

void readData(string raw[ARSIZE][COLMAX])
{
    string line;
    string partLine;
    ifstream infile("UCS-Satellite-Database-5-1-2022.txt");
    getline(infile, line);
    stringstream ss(line);
    // Goes through the amount of rows
    for (int i = 0; i < ARSIZE; i++)
    {
        // Goes through the amount of cols
        for (int j = 0; j < COLMAX; j++)
        {
            // puts a string into partline then
            getline(ss, partLine, '\t');
            raw[i][j] = partLine;
        }
        getline(infile, line);
        ss.str(line);
    }
    // go through each row's individual elements and add
    // it to the array, use tab "/t" as the delimiter
}

// writes to output.txt
void writeData(string raw[ARSIZE][COLMAX])
{
    ofstream infile("output.txt");
    string str = "";
    for (int i = 0; i < ARSIZE; i++)
    {
        for (int j = 0; j < COLMAX; j++)
        {
            str = raw[i][j];
            infile << str << " ";
        }
        infile << endl;
    }
    infile.close();
}

void readArray(string raw[ARSIZE][COLMAX])
{
    string str = "";
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            str = raw[i][j];
            cout << str << " ";
        }
        cout << endl;
    }
}

string lower(string str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        str[i] = tolower(str[i]);
        i++;
    }
    return str;
}

int getColNum(string q, string raw[ARSIZE][COLMAX])
{
    q = lower(q);
    int count = 0;
    for (int i = 0; i < COLMAX; i++)
    {
        if (lower(raw[0][i]) == q)
        {
            return count;
        }
        count++;
    }
    return -1;
}

bool checkSyntax(string q, string db[ARSIZE][COLMAX])
{
    int test = 0;
    if (q.substr(0, 6) != "select")
    {
        return 1;
    }
    q.erase(0, 7);

    if (q[0] != '*')
    {
        for (int i = 0; i < 4; i++)
        {
            if (q.find(',') != string::npos)
            {
                if (getColNum(q.substr(1, q.find("\",") - 1), db) == -1)
                {
                    return 1;
                }
                test = q.find(" \"");
                q.erase(0, q.find(" \"") + 1);
            }
        }
        if (getColNum(q.substr(1, q.find("\" ") - 1), db) == -1)
        {
            return 1;
        }
        q.erase(0, q.find("\" "));
    }
    if (q.substr(2, 4) != "from")
    {
        return 1;
    }
    q.erase(0, 6);
    if (q.substr(2, q.length()) == "db;")
    {
        return 0;
    }

    return 0;
}

// Fills struct in with all the query data, neatly organized.
// could have done this when checking syntax, but meh
void parseQuerytoStruct(Query q, string query, string raw[ARSIZE][COLMAX])
{
    int numOfCol = 0;

    for (int i = 0; i < 37; i++)
    {
        if (query.find('*') != string::npos)
        {
            q.colList[i] = raw[0][i];
        }
        else
        {
            if (query.find(lower(raw[0][i])) != string::npos)
            {
                q.colList[numOfCol] = raw[0][i];
                numOfCol++;
            }
        }
    }
}

int main()
{
    string rawData[ARSIZE][COLMAX];
    readData(rawData);
    writeData(rawData);
    string query = "";
    query = "SELECT \"users\", \"purpose\", \"users\" from db;";

    cout << "Final : " << checkSyntax(lower(query), rawData) << endl;

    // readArray(rawData);
    // string query = "";
}