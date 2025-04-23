#ifndef SCOPETABLE_HEADER
#define SCOPETABLE_HEADER

#include "SymbolInfo.hpp"
#include <bits/stdc++.h>
using namespace std;

class ScopeTable
{
    int unique_table_number, number_of_buckets;
    ScopeTable *parent_scope;
    SymbolInfo **hash_table;

public:
    ScopeTable(int number_of_buckets, int id)
    {
        this->number_of_buckets = number_of_buckets;
        this->unique_table_number = id;
        this->parent_scope = nullptr;
        hash_table = new SymbolInfo *[number_of_buckets];

        for (int i = 0; i < number_of_buckets; i++)
        {
            hash_table[i] = nullptr;
        }
    }

    ~ScopeTable()
    {
        for (int i = 0; i < number_of_buckets; i++)
        {
            SymbolInfo *current = hash_table[i];
            while (current != nullptr)
            {
                SymbolInfo *temp = current;
                current = current->get_next();
                delete temp;
            }
        }
        delete[] hash_table;
    }

    unsigned long SDBMHash(const string &str) const
    {
        unsigned long hash = 0;

        for (char ch : str)
        {
            hash = (ch + (hash << 6) + (hash << 16) - hash) % number_of_buckets;
        }

        return hash;
    }

    bool Insert(const string &name, const string &type)
    {
        unsigned long bucket_no = SDBMHash(name);

        SymbolInfo *current = hash_table[bucket_no];
        int position = 1;

        while (current != nullptr)
        {
            if (current->get_name() == name)
            {
                cout << "\t" << "'" << name << "'" << " already exists in the current ScopeTable" << endl;
                return false;
            }
            current = current->get_next();
            position++;
        }

        current = hash_table[bucket_no];
        position = 1;
        SymbolInfo *insert_new_symbol = new SymbolInfo(name, type);
        if (hash_table[bucket_no] == nullptr)
        {
            hash_table[bucket_no] = insert_new_symbol;
        }
        else
        {
            current = hash_table[bucket_no];
            while (current->get_next() != nullptr)
            {
                current = current->get_next();
                position++;
            }
            current->set_next(insert_new_symbol);
            position++;
        }
        cout << "\t" << "Inserted in ScopeTable# " << unique_table_number << " at position " << bucket_no + 1 << ", " << position << endl;
        return true;
    }

    SymbolInfo *LookUp(const string &name)
    {
        unsigned long bucket_no = SDBMHash(name);
        SymbolInfo *current = hash_table[bucket_no];
        int position = 1;
        while (current != nullptr)
        {
            if (current->get_name() == name)
            {
                cout << "\t" << "'" << name << "'" << " found in ScopeTable# " << unique_table_number << " at position " << bucket_no + 1 << ", " << position << endl;
                return current;
            }
            current = current->get_next();
            position++;
        }
        return nullptr;
    }
    bool Delete(const string &name)
    {
        unsigned long bucket_no = SDBMHash(name);
        SymbolInfo *current = hash_table[bucket_no];
        SymbolInfo *prev = nullptr;
        int position = 1;
        while (current != nullptr)
        {
            if (current->get_name() == name)
            {
                if (prev == nullptr)
                {
                    hash_table[bucket_no] = current->get_next();
                }
                else
                {
                    prev->set_next(current->get_next());
                }
                cout << "\tDeleted " << "'" << name << "'" << " from ScopeTable# " << unique_table_number << " at position " << bucket_no + 1 << ", " << position << endl;
                delete current;
                return true;
            }
            prev = current;
            current = current->get_next();
            position++;
        }
        cout << "\tNot found in the current ScopeTable" << endl;
        return false;
    }

    void Print(int indent_level)
    {
        string indent(indent_level, '\t');

        cout << indent << "ScopeTable# " << unique_table_number << endl;
        for (int i = 0; i < number_of_buckets; i++)
        {
            cout << indent << i + 1 << "--> ";
            SymbolInfo *current = hash_table[i];

            while (current != nullptr)
            {
                string name = current->get_name();
                string type = current->get_type();

                cout << "<" << name << ",";

                if (type.find("FUNCTION") == 0)
                {
                    istringstream iss(type);
                    string temp, return_type;
                    iss >> temp >> return_type;

                    int max_params = 100;
                    string *params = new string[max_params];
                    int param_count = 0;

                    string param;
                    while (iss >> param && param_count < max_params)
                        params[param_count++] = param;

                    cout << "FUNCTION," << return_type << "<==(";
                    for (int j = 0; j < param_count; j++)
                    {
                        cout << params[j];
                        if (j != param_count - 1)
                            cout << ",";
                    }
                    cout << ")";

                    delete[] params;
                }

                else if (type.find("STRUCT") == 0 || type.find("UNION") == 0)
                {
                    istringstream iss(type);
                    string tag;
                    iss >> tag;

                    int max_members = 100;
                    string *member_types = new string[max_members];
                    string *member_names = new string[max_members];
                    int member_count = 0;

                    string t, n;
                    while (iss >> t >> n && member_count < max_members)
                    {
                        member_types[member_count] = t;
                        member_names[member_count] = n;
                        member_count++;
                    }

                    cout << tag << ",{";
                    for (int j = 0; j < member_count; j++)
                    {
                        cout << "(" << member_types[j] << "," << member_names[j] << ")";
                        if (j != member_count - 1)
                            cout << ",";
                    }
                    cout << "}";

                    delete[] member_types;
                    delete[] member_names;
                }

                else
                {
                    cout << type;
                }

                cout << "> ";
                current = current->get_next();
            }

            cout << endl;
        }
    }

    ScopeTable *get_parent_scope() const
    {
        return parent_scope;
    }
    int get_unique_table_number() const
    {
        return unique_table_number;
    }
    void set_parent_scope(ScopeTable *parent_scope)
    {
        this->parent_scope = parent_scope;
    }
    int get_number_of_buckets() const
    {
        return number_of_buckets;
    }
};

#endif