#include <iostream>
#include <string>
#include <fstream>
#include <sstream> // No additional includes or STL allowed!
using namespace std;

#define ARSIZE 5500 // for changing size on the go
#define COLMAX 37

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
    for (int i = 0; i < 10; i++)
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
    int count = 0;
    for (int i = 0; i < COLMAX; i++)
    {
        if (raw[0][i] == q)
        {
            return count;
        }
        count++;
    }
    return 1;
}

bool checkSyntax(string q, string db[ARSIZE][COLMAX])
{
    int i = 0;
    string line;
    string partLine;
    stringstream ssv(q);
    ssv.ignore(2, '\"');
    ssv.ignore(1, ',');
    ssv >> partLine;
    if (partLine != "select")
    {
        return 1;
    }
    ssv >> partLine;

    if (getColNum(partLine, db) == 1 || partLine != "*")
    {
        return 1;
    }
    ssv >> partLine;
    if (partLine != "from")
    {
        return 1;
    }
    ssv >> partLine;
    if (partLine != "db")
    {
        return 1;
    }
    ssv >> partLine;
    cout << "nice";
    return true;
}

int main()
{
    string rawData[ARSIZE][COLMAX];
    readData(rawData);
    writeData(rawData);
    string query = "";
    query = "SELECT \"Users\" FROM DB";

    cout << checkSyntax(query, rawData) << endl;

    // readArray(rawData);
    // string query = "";
    // query = "SELECT \"Users\" FROM DB;";
    // cout << lower(query) << endl;
    // cout << tolower(';') << endl;
}