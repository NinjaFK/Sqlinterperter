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
    // go through each row's individual elements and add
    // it to the array, use tab "/t" as the delimiter
    for (int i = 0; i < ARSIZE; i++)
    {
        for (int j = 0; j < COLMAX; j++)
        {
            getline(ss, partLine, '\t');
            raw[i][j] = partLine;
        }
        getline(infile, line);
        ss.str(line);
    }
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

// Checks the syntax of the query, returns 1 if error, 0 if gucci
// has already been lower case so can check easier. will return
// as soon as error is detected.
bool checkSyntax(string q, string db[ARSIZE][COLMAX])
{
    int i = 0;
    string line;
    string partLine;
    stringstream ssv;
    ssv.ignore(2, '"');
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
    return 0;
}
// q for query so we keep lines short

// Fills struct in with all the query data, neatly organized.
// could have done this when checking syntax, but meh
void parseQuerytoStruct(Query q, string query);

// makes our life easier
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

    parseQuerytoStruct(q, query); // remember to handle separately *
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
    cout << "Welcome to the Satellite Database at home" << endl;
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
        cout << "Please enter your query:" << endl;
        getline(cin, query);
        runQuery(query, rawData); // pass by ref
    }
    return 0;
}
// With documentation expect code to be ~400 lines give or take 100