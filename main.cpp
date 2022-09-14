/*
    Name: Brian, 5008139573, 1005, 1
    Description: Sql interpeter
    Input: Sql query
    Output: Table of requested sql queries
*/

#include <iostream>
#include <string>
#include <fstream>
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

// reads in our data, data is separated by tabs and new lines
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

// Opens file output.txt and writes output.
void writeData(string raw[ARSIZE][COLMAX])
{
    // create output.txt and add all strings from input array
    ofstream infile("output.txt");
    string str = "";
    for (int i = 0; i < ARSIZE; i++)
    {
        for (int j = 0; j < COLMAX; j++)
        {
            str = raw[i][j];
            infile << str << "*Z*";
        }
        infile << "*N*";
    }
    infile.close();
}

// lowercases a given string, useful for error checking
string lower(string str)
{
    int i = 0;
    // while the character is not \0
    // which denotes end of strings then lowers it
    while (str[i] != '\0')
    {
        str[i] = tolower(str[i]);
        i++;
    }
    return str;
}

// makes our life easier
int getColNum(string q, string raw[ARSIZE][COLMAX])
{
    int count = 0;
    q = lower(q);
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

// Checks the syntax of the query, returns 1 if error, 0 if gucci
// has already been lower case so can check easier. will return
// as soon as error is detected.
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
            if (q.find("\",") != string::npos)
            {
                // if (getColNum(q.substr(1, q.find("\",") - 1), db) == -1)
                //{
                //     return 1;
                // }
                q.erase(0, q.find("\",") + 3);
            }
        }
        if (getColNum(q.substr(1, q.find("\" f") - 1), db) == -1)
        {
            return 1;
        }
        q.erase(0, q.find("\" f") + 2);
    }
    if (q.substr(0, 4) != "from")
    {
        return 1;
    }
    q.erase(0, 5);
    if (q.substr(0, q.length()) == "db;")
    {
        return 0;
    }
    if (q.substr(0, 2) != "db")
    {
        return 1;
    }
    q.erase(0, 3);
    if (q.substr(0, 5) != "where")
    {
        return 1;
    }
    q.erase(0, 7);

    do
    {
        if (getColNum(q.substr(0, q.find("\" ")), db) == -1)
        {
            return 1;
        }
        q.erase(0, q.find("\" ") + 1);
        temp = q.substr(1, 2);
        if (temp != "> " && temp != "< ")
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
        q.erase(0, q.find(" \"") + 2);
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
// q for query so we keep lines short

// Fills struct in with all the query data, neatly organized.
// could have done this when checking syntax, but meh
void parseQuerytoStruct(Query &q, string query, string db[ARSIZE][COLMAX])
{
    int numOfCol = 0;
    int j = 0;
    string lq = lower(query);
    long maxstr = string::npos;

    // redo please without db

    query.erase(0, 8);

    for (int i = 0; i < 37; i++)
    {
        if (query.find('*') != string::npos)
        {
            q.colList[i] = db[0][i];
            q.colCount = 37;
        }
        if (query.find(", \"") != string::npos)
        {
            q.colList[numOfCol] = query.substr(0, query.find("\","));
            query.erase(0, query.find("\",") + 4);
            numOfCol++;
            q.colCount++;
        }
    }
    q.colList[numOfCol] = query.substr(0, query.find("\""));
    q.colCount++;

    if (lq.find("where") != maxstr)
    {
        q.whereCount++;
        lq = lower(query);
        query.erase(0, lq.find("where") + 7);
        for (int i = 0; i < 4; i++)
        {
            if (lq.find("\" and \"") != maxstr || lq.find("\" or \"") != maxstr)
            {
                q.whereLeft[i] = query.substr(0, query.find("\" "));
                query.erase(0, query.find("\" ") + 2);
                q.logicList[i] = query.substr(0, 2);
                query.erase(0, query.find("\"") + 1);
                q.whereRight[i] = query.substr(0, query.find("\""));
                query.erase(0, query.find("\" ") + 2);
                q.ANDORlist[i] = query.substr(0, query.find(" "));
                query.erase(0, query.find("\"") + 1);
                lq = lower(query);
                j++;
                q.whereCount++;
            }
        }
        q.whereLeft[j] = query.substr(0, query.find("\" "));
        query.erase(0, query.find("\" ") + 2);
        q.logicList[j] = query.substr(0, 2);
        query.erase(0, query.find("\"") + 1);
        q.whereRight[j] = query.substr(0, query.find("\""));
        query.erase(0, query.find("\""));
    }
}

string output[ARSIZE][COLMAX];
// Creates output with results
bool generateResults(Query q, string db[ARSIZE][COLMAX])
{
    // First we find the matching rows then we print out
    // only the rows selected, unless colList is 0 then we print
    // all of them out.
    // Note if a col name doesn't match we just error out
    // if your local output array segfaults, make it global like I put above
    // output[j][k] = db[0][getColNum(q.colList[i], db)];
    int colc = 0;
    int relyOp = 0;
    bool p, p2 = false;
    int rowcount = 0;
    for (int i = 0; i < q.colCount; i++)
    {
        if (getColNum(q.colList[i], db) != -1)
        {
            colc++;
        }
    }
    if (colc != q.colCount)
    {
        return 0;
    }

    for (int i = 0; i < q.colCount; i++)
    {
        for (int j = 0; j < ARSIZE; j++)
        {
            if (q.logicList[relyOp] == "> ")
            {
                if (db[j][getColNum(q.whereLeft[relyOp], db)] > q.whereRight[relyOp])
                {
                    p = true;
                }
            }
            if (q.logicList[relyOp] == "< ")
            {
                if (db[j][getColNum(q.whereLeft[relyOp], db)] < q.whereRight[relyOp])
                {
                    p = true;
                }
            }
            if (q.logicList[relyOp] == "==")
            {
                if (db[j][getColNum(q.whereLeft[relyOp], db)] == q.whereRight[relyOp])
                {
                    p = true;
                }
            }
            if (q.logicList[relyOp] == "!=")
            {
                if (db[j][getColNum(q.whereLeft[relyOp], db)] != q.whereRight[relyOp])
                {
                    p = true;
                }
            }
            if (q.logicList[relyOp] == ">=")
            {
                if (db[j][getColNum(q.whereLeft[relyOp], db)] >= q.whereRight[relyOp])
                {
                    p = true;
                }
            }
            if (q.logicList[relyOp] == "<=")
            {
                if (db[j][getColNum(q.whereLeft[relyOp], db)] <= q.whereRight[relyOp])
                {
                    p = true;
                }
            }

            if (q.ANDORlist[relyOp] == "and")
            {
                if (p2 && p)
                {
                    output[relyOp][j] = db[j][getColNum(q.colList[i], db)];
                    relyOp++;
                }
            }
            else if (q.ANDORlist[relyOp] == "or")
            {
                if (p2 || p)
                {
                    output[relyOp][j] = db[j][getColNum(q.colList[i], db)];
                    relyOp++;
                }
            }
            else if (q.whereCount == 1 && p == true)
            {
                // cout << "i: " << i << endl;
                // cout << "relyOp: " << relyOp << endl;
                // cout << "j: " << j << endl;
                // cout << "left of i: " << q.whereLeft[relyOp] << endl;
                // cout << "col of left: " << getColNum(q.whereLeft[relyOp], db) << endl;

                // cout << output[i][j] << endl;
                output[j][i] = db[j][getColNum(q.colList[i], db)];
                rowcount++;
            }
            p2 = p;
            p = false;
        }
    }

    ofstream outfile;
    outfile.open("queryoutput.txt", ofstream::app);
    outfile << "Query:" << endl;
    for (int i = 0; i < q.colCount; i++)
    {
        outfile << q.colList[i] << '\t';
    }
    outfile << '\n';
    for (int i = 0; i < ARSIZE; i++)
    {
        for (int j = 0; j < COLMAX; j++)
        {
            if (output[i][j] != "")
            {
                outfile << output[i][j] << '\t';
            }
        }
        if (output[i][0] != "")
        {
            outfile << '\n';
        }
    }
    outfile << "Total Rows: " << rowcount / 2 << endl;
    outfile.close();
    return 1;
}

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
    if (!generateResults(q, db))
        cout << "Error: Invalid Query Semantic. "
             << "Get motivated. Try Again!" << endl;
}

int main()
{
    string rawData[ARSIZE][COLMAX]; // first row has col names
    readData(rawData);
    writeData(rawData); // output.txt
    cout << "   Welcome to the Satellite Database at home" << endl;
    string query = "";
    for (int i = 0; i < ARSIZE; i++) // Clean up output global array
        for (int j = 0; j < COLMAX; j++)
            output[i][j] = "";
    ofstream fout;
    fout.open("queryoutput.txt"); // Cleanup
    fout << "Queries:" << endl;
    fout.close(); // Cleanup output file
    // query="SELECT \"Users\" FROM DB;";   //Debug
    // runQuery(query, rawData); return 0;//Debug
    while (query != "q")
    {
        cout << "   Please enter your query:" << endl;
        getline(cin, query);
        runQuery(query, rawData); // pass by ref
    }
    return 0;
}
// With documentation expect code to be ~400 lines give or take 100