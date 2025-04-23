#include "SymbolTable.hpp"
#include <bits/stdc++.h>
using namespace std;

bool isValidCommand(char cmd)
{
    return cmd == 'I' || cmd == 'L' || cmd == 'D' ||
           cmd == 'P' || cmd == 'S' || cmd == 'E' || cmd == 'Q';
}

bool isValidPrintSubCommand(char subCmd)
{
    return subCmd == 'A' || subCmd == 'C';
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        cerr << "Error opening files." << endl;
        return 1;
    }

    streambuf *originalCout = cout.rdbuf();
    cout.rdbuf(outputFile.rdbuf());

    string firstLine;
    if (!getline(inputFile, firstLine))
    {
        cerr << "Input file is empty." << endl;
        cout.rdbuf(originalCout);
        return 1;
    }

    int numBuckets;
    try
    {
        numBuckets = stoi(firstLine);
        if (numBuckets <= 0)
            throw invalid_argument("Buckets must be positive");
    }
    catch (...)
    {
        cerr << "Invalid number of buckets." << endl;
        cout.rdbuf(originalCout);
        return 1;
    }

    SymbolTable symbolTable(numBuckets);
    string line;
    int cmdCount = 0;

    while (getline(inputFile, line))
    {
        cmdCount++;
        if (line.empty())
            continue;

        istringstream normalizer(line);
        string token, cleanLine;
        while (normalizer >> token)
        {
            if (!cleanLine.empty())
                cleanLine += " ";
            cleanLine += token;
        }
        istringstream iss(cleanLine);
        string commandStr;
        iss >> commandStr;

        if (commandStr.empty())
            continue;
        char command = toupper(commandStr[0]);

        cout << "Cmd " << cmdCount << ": " << cleanLine << endl;

        if (!isValidCommand(command))
        {
            cout << "\tInvalid command: " << command << endl;
            continue;
        }

        switch (command)
        {
        case 'I':
        {
            string name, type;
            if (!(iss >> name >> type))
            {
                cout << "\tNumber of parameters mismatch for the command I" << endl;
                break;
            }

            string remaining;
            getline(iss, remaining);
            type += remaining;

            symbolTable.insert(name, type);
            break;
        }

        case 'L':
        {
            string name;
            if (!(iss >> name))
            {
                cout << "\tNumber of parameters mismatch for the command L" << endl;
                break;
            }
            string extra;
            if (iss >> extra)
            {
                cout << "\tNumber of parameters mismatch for the command L" << endl;
            }
            else
            {
                symbolTable.lookup(name);
            }
            break;
        }

        case 'D':
        {
            string name;
            if (!(iss >> name))
            {
                cout << "\tNumber of parameters mismatch for the command D" << endl;
                break;
            }
            string extra;
            if (iss >> extra)
            {
                cout << "\tNumber of parameters mismatch for the command D" << endl;
            }
            else
            {
                symbolTable.remove(name);
            }
            break;
        }

        case 'P':
        {
            char subCmd;
            if (!(iss >> subCmd))
            {
                cout << "\tNumber of parameters mismatch for the command P" << endl;
                break;
            }
            subCmd = toupper(subCmd);

            if (!isValidPrintSubCommand(subCmd))
            {
                cout << "\tInvalid subcommand for P: " << subCmd << endl;
                break;
            }

            string extra;
            if (iss >> extra)
            {
                cout << "\tNumber of parameters mismatch for the command P" << endl;
                break;
            }

            if (subCmd == 'A')
            {
                symbolTable.Print_all_scope_table();
            }
            else
            {
                symbolTable.Print_current_scope_table();
            }
            break;
        }

        case 'S':
        {
            string extra;
            if (iss >> extra)
            {
                cout << "\tNumber of parameters mismatch for the command S" << endl;
            }
            else
            {
                symbolTable.Enter_Scope();
            }
            break;
        }

        case 'E':
        {
            string extra;
            if (iss >> extra)
            {
                cout << "\tNumber of parameters mismatch for the command E" << endl;
            }
            else
            {
                symbolTable.Exit_Scope();
            }
            break;
        }

        case 'Q':
        {
            string extra;
            if (iss >> extra)
            {
                cout << "\tNumber of parameters mismatch for the command Q" << endl;
            }
            else
            {
                bool should_exit = symbolTable.Quit();
                if (should_exit)
                {

                    return 0;
                }
            }
            break;
        }
        }
    }

end_execution:
    cout.rdbuf(originalCout);
    inputFile.close();
    outputFile.close();
    return 0;
}
