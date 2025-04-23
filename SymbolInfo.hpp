#ifndef SYMBOLINFO_H
#define SYMBOLINFO_H
#include <iostream>
#include <string>
using namespace std;

class SymbolInfo
{
    string name;
    string type;
    SymbolInfo *next;

public:
    SymbolInfo(string name, string type, SymbolInfo *next = nullptr) : name(name), type(type), next(next) {}
    void set_name(const string &name)
    {
        this->name = name;
    }
    void set_type(const string &type)
    {
        this->type = type;
    }
    void set_next(SymbolInfo *next)
    {
        this->next = next;
    }

    string get_name() const
    {
        return name;
    }

    string get_type() const
    {
        return type;
    }
    SymbolInfo *get_next() const
    {
        return next;
    }

    ~SymbolInfo()
    {
    }
};

#endif