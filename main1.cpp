#include "SymbolTable.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cctype>

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
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cerr << "Error opening files." << std::endl;
        return 1;
    }

    std::streambuf *originalCout = std::cout.rdbuf();
    std::cout.rdbuf(outputFile.rdbuf());

    std::string firstLine;
    if (!std::getline(inputFile, firstLine))
    {
        std::cerr << "Input file is empty." << std::endl;
        std::cout.rdbuf(originalCout);
        return 1;
    }

    int numBuckets;
    try
    {
        numBuckets = std::stoi(firstLine);
        if (numBuckets <= 0)
            throw std::invalid_argument("Buckets must be positive");
    }
    catch (...)
    {
        std::cerr << "Invalid number of buckets." << std::endl;
        std::cout.rdbuf(originalCout);
        return 1;
    }

    SymbolTable symbolTable(numBuckets);
    std::string line;
    int cmdCount = 0;

    while (std::getline(inputFile, line))
    {
        cmdCount++;
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string commandStr;
        iss >> commandStr;

        if (commandStr.empty())
            continue;
        char command = std::toupper(commandStr[0]);

        std::cout << "Cmd " << cmdCount << ": " << line << std::endl;

        if (!isValidCommand(command))
        {
            std::cout << "\tInvalid command: " << command << std::endl;
            continue;
        }

        switch (command)
        {
        case 'I':
        {
            std::string name, type;
            if (!(iss >> name >> type))
            {
                std::cout << "\tNumber of parameters mismatch for the command I" << std::endl;
                break;
            }

            std::string remaining;
            std::getline(iss, remaining);
            type += remaining;

            symbolTable.insert(name, type);
            break;
        }

        case 'L':
        {
            std::string name;
            if (!(iss >> name))
            {
                std::cout << "\tNumber of parameters mismatch for the command L" << std::endl;
                break;
            }
            std::string extra;
            if (iss >> extra)
            {
                std::cout << "\tNumber of parameters mismatch for the command L" << std::endl;
            }
            else
            {
                symbolTable.lookup(name);
            }
            break;
        }

        case 'D':
        {
            std::string name;
            if (!(iss >> name))
            {
                std::cout << "\tNumber of parameters mismatch for the command D" << std::endl;
                break;
            }
            std::string extra;
            if (iss >> extra)
            {
                std::cout << "\tNumber of parameters mismatch for the command D" << std::endl;
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
                std::cout << "\tNumber of parameters mismatch for the command P" << std::endl;
                break;
            }
            subCmd = std::toupper(subCmd);

            if (!isValidPrintSubCommand(subCmd))
            {
                std::cout << "\tInvalid subcommand for P: " << subCmd << std::endl;
                break;
            }

            std::string extra;
            if (iss >> extra)
            {
                std::cout << "\tNumber of parameters mismatch for the command P" << std::endl;
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
            std::string extra;
            if (iss >> extra)
            {
                std::cout << "\tNumber of parameters mismatch for the command S" << std::endl;
            }
            else
            {
                symbolTable.Enter_Scope();
            }
            break;
        }

        case 'E':
        {
            std::string extra;
            if (iss >> extra)
            {
                std::cout << "\tNumber of parameters mismatch for the command E" << std::endl;
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
    std::cout.rdbuf(originalCout);
    inputFile.close();
    outputFile.close();
    return 0;
}
