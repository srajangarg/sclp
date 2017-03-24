#include <algorithm>
#include"common-classes.hh"
#include"cfg.hh"


template<typename T>
void printSet(string name, set<T> s)
{
    cout<<name<<"    :   {";
    for(auto &ele : s)
    {
        cout<<ele;
        cout<<", ";
    }
    cout<<"}\n";
}

string CFGNode::get_opd_variable(Ics_Opd* opd)
{
    stringstream sstr;
    string s = "";
    if(dynamic_cast<Mem_Addr_Opd *> (opd) != NULL || dynamic_cast<Register_Addr_Opd *>(opd) != NULL)
    {
        opd->print_ics_opd(sstr);
        sstr>>s;
    }   
        // return opd->symbol_entry->get_variable_name();
    if(s == "zero")
        return "";
    return s;
}

void CFGNode::add_in_gen(Ics_Opd* opd)
{
    string name = get_opd_variable(opd);
    if(name != "")
    {
        if(kill.find(name) == kill.end())
            gen.insert(name);
    }
}

void CFGNode::add_in_kill(Ics_Opd* opd)
{
    string name = get_opd_variable(opd);
    if(name != "")
        kill.insert(name);
}

void CFGNode::computeGenKill()
{
    gen.clear();
    kill.clear();
    for (auto &ic : icode_list)
    {
        switch (ic->get_op().get_ic_format())
        {
            case i_op_o1_r:                              // extra not used
            case i_op_r_o1:                              // extra not used
            case i_r_op_o1:

                add_in_gen(ic->get_opd1());
                add_in_kill(ic->get_result());
                break;

            case i_r_r_op_o1:                           // extra not used
                add_in_gen(ic->get_opd1());
                add_in_gen(ic->get_result());
                add_in_kill(ic->get_result());
                break;

            case i_r_o1_op_o2:

                add_in_gen(ic->get_opd1());
                add_in_gen(ic->get_opd2());
                add_in_kill(ic->get_result());
                break;

            case i_op_o1_o2_st:

                add_in_gen(ic->get_opd1());
                add_in_gen(ic->get_opd2());
                break;
            
            default:
                break;
        }
    }
}

int CFGNode::removeDeadStmt()
{
    int count = 0;
    set<string> live_vars = out;

    auto rit = icode_list.rbegin();
    while (rit != icode_list.rend())
    {
        Icode_Stmt* ic = (*rit);
        string var;
        // cout<<"Checking ";
        // ic->print_icode(cout);
        bool eliminate = false;
        switch (ic->get_op().get_ic_format())
        {
            case i_op_o1_r:                              // extra not used
            case i_op_r_o1:                              // extra not used
            case i_r_op_o1:

                var = get_opd_variable(ic->get_result());
                if(live_vars.find(var) == live_vars.end())
                {
                    eliminate = true;
                }
                else
                {
                    live_vars.erase(get_opd_variable(ic->get_result()));
                    live_vars.insert(get_opd_variable(ic->get_opd1()));
                }
                break;

            case i_r_r_op_o1:                           // extra not used

                var = get_opd_variable(ic->get_result());
                if(live_vars.find(var) == live_vars.end())
                {
                    eliminate = true;
                }
                else
                {
                    live_vars.insert(get_opd_variable(ic->get_opd1()));
                    live_vars.insert(var);
                }
                break;

            case i_r_o1_op_o2:

                var = get_opd_variable(ic->get_result());
                if(live_vars.find(var) == live_vars.end())
                {
                    eliminate = true;
                }
                else
                {
                    live_vars.erase(get_opd_variable(ic->get_result()));
                    live_vars.insert(get_opd_variable(ic->get_opd1()));
                    live_vars.insert(get_opd_variable(ic->get_opd2()));
                }
                break;

            case i_op_o1_o2_st:

                live_vars.insert(get_opd_variable(ic->get_opd1()));
                live_vars.insert(get_opd_variable(ic->get_opd2()));
                break;

            default:
                break;
        }
        if(eliminate)
        {
            auto it = rit.base();
            icode_list.erase(--it);
            count++;
            // cout<<"Eliminating ";
        }
        else
        {
            rit++;
        }
    }
    return count;
}

void  CFG::construct_from_icode(list<Icode_Stmt *> &ic_list)
{
    int curr = 0, exec = 0;
    int niche;
    bool aftergoto = false;

    nodes.push_back(CFGNode(to_string(exec)));
    name_to_index[to_string(exec)] = 0;
    node_order.push_back(0);

    for (auto &ic : ic_list)
    {
        string name;
        int nextNode;
        switch(ic->get_op().get_op())
        {
            case j:

                nodes[curr].icode_list.push_back(ic);
                name = static_cast<ContS*>(ic)->get_Offset();
                nodes[curr].children.push_back(getCFGNode(name));
                aftergoto = true;
                break;

            case beq:
            case bne:

                nodes[curr].icode_list.push_back(ic);
                name = static_cast<ContS*>(ic)->get_Offset();
                nodes[curr].children.push_back(getCFGNode(name));
                niche = getCFGNode(to_string(++exec));
                nodes[curr].children.push_back(niche);

                curr = niche;
                node_order.push_back(curr);
                
                break;
            case label:

                name = static_cast<LabS*>(ic)->get_offset();
                nextNode = getCFGNode(name);
                if (!aftergoto)
                {
                    nodes[curr].children.push_back(nextNode);
                }
                curr = nextNode;
                nodes[curr].icode_list.push_back(ic);
                node_order.push_back(curr);
                aftergoto = false;
                break;

            default:

                if (aftergoto)
                    continue;

                nodes[curr].icode_list.push_back(ic);
        }
    }
}

void CFG::computeInOut(set<string> global_vars)
{
    for (auto& node:nodes)
    {
        node.computeGenKill();
        node.in.clear();
        node.out.clear();
        if(node.children.empty())
        {
            node.out.insert(global_vars.begin(), global_vars.end());
        }
    }

    bool changed;
    do
    {
        changed = false;
        vector<bool> visited(nodes.size(), false);
        for (int i = 0; i < nodes.size(); i++)
        {
            if(!visited[i])
            {
                calcIn(i, visited, changed);
            }
        }
    } while (changed);

}

void CFG::calcIn(int k, vector<bool>& visited, bool& changed)
{
    if(visited[k])
        return;

    visited[k] = true;
    int in_count = nodes[k].in.size();
    int out_count = nodes[k].out.size();
    for(auto & i : nodes[k].children)
    {
        calcIn(i, visited, changed);
        nodes[k].out.insert(nodes[i].in.begin(), nodes[i].in.end());
    }

    nodes[k].in.clear();
    set_difference(nodes[k].out.begin(), nodes[k].out.end(), 
                   nodes[k].kill.begin(), nodes[k].kill.end(),
                   inserter(nodes[k].in, nodes[k].in.end()));

    nodes[k].in.insert(nodes[k].gen.begin(), nodes[k].gen.end());

    if(in_count < nodes[k].in.size() || out_count < nodes[k].out.size())
    {
        changed = true;
    }
}


int CFG::getCFGNode(string name)
{
    if (name_to_index.find(name) == name_to_index.end())
    {
        name_to_index[name] = nodes.size();
        nodes.push_back(CFGNode(name));
    }
    return name_to_index[name];
}

int CFG::getNumNodes()
{
    return nodes.size();
}
void CFG::print()
{   
    std::ostream out(std::cout.rdbuf());
    int i = 0;
    for (auto &index : node_order)
    {
        cout<<i++<<" ----- node "<<nodes[index].name<<endl;
        for (auto &ic : nodes[index].icode_list)
            ic->print_icode(out);

        printSet("Gen ", nodes[index].gen);
        printSet("Kill", nodes[index].kill);
        printSet("In  ", nodes[index].in);
        printSet("Out ", nodes[index].out);
        
        cout<<"children : ";
        for (auto &i : nodes[index].children)
            cout<<i<<" ";
        cout<<endl;
        cout<<endl;
    }
}

void CFG::deadCodeElimination(set<string> global_vars)
{
    int count;

    do
    {
        count = 0;
        computeInOut(global_vars);
        for(auto& node : nodes)
        {
            count += node.removeDeadStmt();
        }
        // cout<<"Deleted Number of lines : "<<count<<endl;
    } while (count > 0);
}

list<Icode_Stmt *> CFG::getIcodeList()
{
    list<Icode_Stmt *> result;

    for (auto &index : node_order)
    {
        list<Icode_Stmt *> temp (nodes[index].icode_list);
        result.splice(result.end(), temp);
    }

    return result;
}