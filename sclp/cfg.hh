#ifndef CFG_HH
#define CFG_HH

#include <unordered_map>
#include <set>
#include "icode.hh"

struct CFGNode
{   
    CFGNode(string s)
    {
        name = s;
    }

    string name;
    vector<Icode_Stmt *> icode_list;

    vector<int> children;
    // vector<int> parents;
    set<string> kill, gen;
    set<string> in, out;
    string get_opd_variable(Ics_Opd* opd);
    void add_in_gen(Ics_Opd* opd);
    void add_in_kill(Ics_Opd* opd);
    void computeGenKill();
    void deadCodeElimination();
};

struct CFG
{
    unordered_map<string, int> name_to_index;
    vector<CFGNode> nodes;
    vector<int> node_order;

    void construct_from_icode(list<Icode_Stmt *> &ic_list);

    int getCFGNode(string name);
    int getNumNodes();
    void print();
    void computeInOut();
    void calcIn(int k, vector<bool>& visited, bool& changed);
    void deadCodeElimination();

};

#endif