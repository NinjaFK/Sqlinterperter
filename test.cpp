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
    int j = 0;
    string temp = "";
    if (q.substr(0, 6) != "select")
    {
        return 1;
    }
    q.erase(0, 7);

    if (q[0] != '*')
    {
        for (int i = 0; i < 36; i++)
        {
            if (q.find(',') != string::npos)
            {
                if (getColNum(q.substr(1, q.find("\",") - 1), db) == -1)
                {
                    return 1;
                }
                q.erase(0, q.find("\",") + 3);
            }
        }
        if (getColNum(q.substr(1, q.find("\" f") - 1), db) == -1)
        {
            return 1;
        }
        q.erase(0, q.find("\" f"));
    }
    if (q.substr(2, 4) != "from")
    {
        return 1;
    }
    q.erase(0, 6);
    if (q.substr(1, q.length()) == "db;")
    {
        return 0;
    }
    if (q.substr(1, 2) != "db")
    {
        return 1;
    }
    q.erase(0, 3);
    if (q.substr(1, 5) != "where")
    {
        return 1;
    }
    q.erase(0, 8);

    do
    {
        if (getColNum(q.substr(0, q.find("\" ")), db) == -1)
        {
            return 1;
        }
        q.erase(0, q.find("\" ") + 1);
        temp = q.substr(1, 2);
        if (temp != "> " && temp != "> ")
        {
            if (temp != "==" && temp != "!=")
            {
                if (temp != ">=" && temp != "<=")
                {
                    return 1;
                }
            }
        }
        q.erase(0, q.find(" \"") + 2);
        if (q.find("and") == string::npos && q.find("or") == string::npos)
        {
            break;
        }
        q.erase(0, q.find(" \""));
        j++;
    } while (j < 4);

    if (q.length() == 0)
    {
        return 1;
    }
    if (q.back() != ';')
    {
        return 1;
    }
    return 0;
}

// Fills struct in with all the query data, neatly organized.
// could have done this when checking syntax, but meh
void parseQuerytoStruct(Query q, string query, string db[ARSIZE][COLMAX])
{
    int numOfCol = 0;

    for (int i = 0; i < 37; i++)
    {
        if (query.find('*') != string::npos)
        {
            q.colList[i] = db[0][i];
        }
        else
        {
            if (query.find(db[0][i]) != string::npos)
            {
                q.colList[numOfCol] = db[0][i];
                numOfCol++;
            }
        }
    }
    // cout << (lower(query)).find("where") << endl;
    if ((lower(query)).find("where") != string::npos)
    {
        query.erase(0, (lower(query)).find("where") + 5);
        cout << '\'' << query << '\'' << endl;
        query.substr(2, query.find("\" "));
        for (int i = 0; i < 4; i++)
        {
            // if (lower(query).find("and") != string::npos || lower(query).find("or") != string::npos)
            //{
            // }
        }
        // cout << '\'' << query.substr(2, query.find("\" ")) << '\'' << endl;
    }
}

string output[ARSIZE][COLMAX];
// Creates output with results
bool generateResults(Query q, string raw[ARSIZE][COLMAX]);
// First we find the matching rows then we print out
// only the rows selected, unless colList is 0 then we print
// all of them out.
// Note if a col name doesn't match we just error out
// if your local output array segfaults, make it global like I put above

void runQuery(string &query, string db[][COLMAX])
{
    if (lower(query) == "q")
    {
        query = "q"; // lowercase it so while in main picks it up
        return;
    } // Exit command so we are done here.
    if (checkSyntax(lower(query), db))
    { // lowercase version so it's ez
        cout << "Error: Invalid Query Syntax. "
             << "Get motivated. Try Again!" << endl;
        return; // if true there is syntax error
    }

    // ok now that we know query is ok we can parse and prepare
    Query q; // the struct with the data to query

    parseQuerytoStruct(q, query, db); // remember to handle separately *
    // now we can actually return the values for it.
    // if (!generateResults(q, db))
    //    cout << "Error: Invalid Query Semantic. "
    //         << "Get motivated. Try Again!" << endl;
}

int main()
{
    string rawData[ARSIZE][COLMAX]; // first row has col names
    readData(rawData);
    writeData(rawData); // output.txt
    cout << "Welcome to the Satellite Database at home" << endl;
    string query = "";
    for (int i = 0; i < ARSIZE; i++) // Clean up output global array
        for (int j = 0; j < COLMAX; j++)
            output[i][j] = "";
    ofstream fout;
    fout.open("queryoutput.txt"); // Cleanup
    fout << "Queries:" << endl;
    fout.close();                                                                                                   // Cleanup output file
    query = "SELECT \"Users\", \" Dry Mass (kg.) \", \"Users\" FROM DB WHERE \" Dry Mass (kg.) \" == \"amongus\";"; // Debug
    runQuery(query, rawData);
    return 0; // Debug
    /*
    while (query != "q")
    {
        cout << "Please enter your query:" << endl;
        getline(cin, query);
        runQuery(query, rawData); // pass by ref
    }
    */
    return 0;
    // readArray(rawData);
    // string query = "";
}