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

bool checkSynCol(string q, string db[ARSIZE][COLMAX])
{
    if (getColNum(q, db) > -1)
    {
        return 0;
    }
    return 1;
}

bool checkSyntax(string q, string db[ARSIZE][COLMAX])
{
    int i = 0;
    string partLine;
    stringstream ssv(q);
    ssv >> partLine;
    if (partLine != "select")
    {
        return 1;
    }

    ssv >> partLine;
    if (q.find('*') != '*')
    {

        getline(ssv, partLine, '\"');
        getline(ssv, partLine, '\"');
        if (checkSynCol(partLine, db) == 1)
        {
            return 1;
        }
        ssv >> partLine;

        // runs if the first col is right

        for (int i = 0; i < 4; i++)
        {
            if (partLine == ",")
            {
                getline(ssv, partLine, '\"');
                getline(ssv, partLine, '\"');
                if (checkSynCol(partLine, db) == 1)
                {
                    return 1;
                }
                ssv >> partLine;
            }
        }
        // checks last col to see if extra comma
        if (partLine == ",")
        {
            return 1;
            cout << "no extra comma at end";
        }
    }

    ssv >> partLine;
    if (partLine != "from")
    {
        return 1;
    }

    ssv >> partLine;
    if (partLine != "db" && partLine != "db;")
    {
        return 1;
    }
    if (partLine.back() == ';')
    {
        return 0;
    }

    ssv >> partLine;

    // cout << "\"" << partLine << "\"" << '\n';

    cout << "return fake zero :";
    return 0;
}

// Fills struct in with all the query data, neatly organized.
// could have done this when checking syntax, but meh
void parseQuerytoStruct(Query q, string query);
{
}

int main()
{
    string rawData[ARSIZE][COLMAX];
    readData(rawData);
    writeData(rawData);
    string query = "";
    query = "SELECT \"users\", \"users\" from db;";

    cout << "Final : " << checkSyntax(lower(query), rawData) << endl;

    // readArray(rawData);
    // string query = "";
}