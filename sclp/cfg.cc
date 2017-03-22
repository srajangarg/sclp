#include <algorithm>
#include"common-classes.hh"
#include"cfg.hh"

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
		switch(ic->get_op().get_ic_format())
		{
			case i_r_op_o1:
			// case i_op_r_o1:                              // extra not used
			// case i_op_o1_r:                              // extra not used
				add_in_gen(ic->get_opd1());
				add_in_kill(ic->get_result());
				break;

			case i_r_o1_op_o2:
				add_in_kill(ic->get_result());

			case i_op_o1_o2_st:
				add_in_gen(ic->get_opd1());
				add_in_gen(ic->get_opd2());
		}
	}
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
			curr = getCFGNode(name);
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
    computeInOut();
}

void CFG::computeInOut()
{
    for (auto& node:nodes)
        node.computeGenKill();

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
	for (auto &node : nodes)
	{
		cout<<i++<<" ----- node "<<node.name<<endl;
		for (auto &ic : node.icode_list)
			ic->print_icode(out);

		cout<<"Kill   :   {";

		for(auto &opd : node.kill)
		{
			cout<<opd;
			cout<<", ";
		}
		cout<<"}\n";

		cout<<"Gen    :   {";
		for(auto &opd : node.gen)
		{
			cout<<opd;
			cout<<", ";
		}
		cout<<"}\n";

		cout<<"In    :   {";
		for(auto &opd : node.in)
		{
			cout<<opd;
			cout<<", ";
		}
		cout<<"}\n";

		cout<<"Out    :   {";
		for(auto &opd : node.out)
		{
			cout<<opd;
			cout<<", ";
		}
		cout<<"}\n";


		cout<<"children : ";
		for (auto &i : node.children)
			cout<<i<<" ";
		cout<<endl;
		cout<<endl;
	}
}

