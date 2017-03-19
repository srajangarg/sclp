#ifndef CFG_HH
#define CFG_HH

#include <unordered_map>
#include "icode.hh"

struct CFGnode
{	
	CFGnode(string s)
	{
		name = s;
	}

	string name;
	vector<Icode_Stmt *> icode_list;

	vector<int> children;
	// vector<int> parents;
	// add in kill/gen sets
};

struct CFG
{
	unordered_map<string, int> name_to_index;
	vector<CFGnode> nodes;

	void construct_from_icode(list<Icode_Stmt *> &ic_list)
	{
		int curr = 0, exec = 0;
		int niche;
		bool aftergoto = false;

		nodes.push_back(CFGnode(to_string(exec)));
		name_to_index[to_string(exec)] = 0;

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
				break;

			case label:

				name = static_cast<LabS*>(ic)->get_offset();
				curr = getCFGNode(name);
				aftergoto = false;
				break;

			default:

				if (aftergoto)
					continue;

				nodes[curr].icode_list.push_back(ic);
				// do kill gen updation
			}
		}
	}

	int getCFGNode(string name)
	{
		if (name_to_index.find(name) == name_to_index.end())
		{
			name_to_index[name] = nodes.size();
			nodes.push_back(CFGnode(name));
		}
		return name_to_index[name];
	}

	void print()
	{	
		std::ostream out(std::cout.rdbuf());
		int i = 0;
		for (auto &node : nodes)
		{
			cout<<i++<<" ----- node "<<node.name<<endl;
			for (auto &ic : node.icode_list)
				ic->print_icode(out);
			cout<<endl;

			cout<<"children : ";
			for (auto &i : node.children)
				cout<<i<<" ";
			cout<<endl;
			cout<<endl;
		}
	}
};

#endif