#ifndef SYMBOLTABLE_HEADER
#define SYMBOLTABLE_HEADER

#include <bits/stdc++.h>
#include "ScopeTable.hpp"

class SymbolTable
{

    ScopeTable *current_scope;
    int number_of_buckets;
    int scope_no;

public:
    SymbolTable(int number_of_buckets) : number_of_buckets(number_of_buckets)
    {
        this->scope_no = 1;
        current_scope = new ScopeTable(number_of_buckets, scope_no);
        cout << "\tScopeTable# " << scope_no << " created" << endl;
    }
    ~SymbolTable()
    {
        while (current_scope != nullptr)
        {
            ScopeTable *temp = current_scope;
            current_scope = current_scope->get_parent_scope();
            delete temp;
        }
    }

    void Enter_Scope()
    {
        scope_no++;
        ScopeTable *new_Scope_Table = new ScopeTable(number_of_buckets, scope_no);
        new_Scope_Table->set_parent_scope(current_scope);
        current_scope = new_Scope_Table;
        cout << "\tScopeTable# " << scope_no << " created" << endl;
    }

    void Exit_Scope()
    {
        if (current_scope->get_parent_scope() == nullptr)
        {
            cout << "\tScopeTable# 1 cannot be removed" << endl;
            return;
        }
        ScopeTable *temp = current_scope;
        current_scope = current_scope->get_parent_scope();
        cout << "\tScopeTable# " << temp->get_unique_table_number() << " removed" << endl;
        delete temp;
    }
    bool Quit()
    {
        ScopeTable *temp = current_scope;
        while (temp != nullptr)
        {
            ScopeTable *parent = temp->get_parent_scope();
            cout << "\tScopeTable# " << temp->get_unique_table_number() << " removed" << endl;
            delete temp;
            temp = parent;
        }
        current_scope = nullptr;
        return true;
    }

    bool insert(const string &name, const string &type)
    {
        return current_scope->Insert(name, type);
    }
    bool remove(const string &name)
    {
        return current_scope->Delete(name);
    }

    SymbolInfo *lookup(const string &name)
    {
        ScopeTable *temp = current_scope;

        while (temp != nullptr)
        {
            SymbolInfo *symbol = temp->LookUp(name);
            if (symbol != nullptr)
            {
                return symbol;
            }
            temp = temp->get_parent_scope();
        }
        cout << "\t" << "'" << name << "'" << " not found in any of the ScopeTables" << endl;
        return nullptr;
    }

    void Print_current_scope_table()
    {
        current_scope->Print(1);
    }
    void Print_all_scope_table()
    {
        int level = 1;
        ScopeTable *temp = current_scope;
        while (temp != nullptr)
        {
            temp->Print(level);
            temp = temp->get_parent_scope();
            level++;
        }
    }
};

#endif