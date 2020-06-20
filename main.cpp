#include <iostream>
#include "symtable.h"
using namespace std;

int main(){
	symtable<string, string>  table;
	string s;
	table.enterScope("L1");
	table.insert("1", "-1");
	table.insert("2", "-2");
	table.insert("3", "-3");
	table.insert("4", "-4");
	table.insert("5", "-5");
	table.insert("6", "-6");
	table.insert("7", "-7");
	table.enterScope("L2");
	table.insert("1", "2");
	table.insert("2", "4");
	table.insert("3", "6");
	table.enterScope("L3_v2");
	table.insert("1", "26");
	table.insert("2", "48");
	table.enterScope("L4");
	table.dump(cout);
	table.dump(cout,table.ScopeOption::CURRENT);
	table.dump(cout,table.ScopeOption::GLOBAL);
	table.lookup("1", s);
	cout << "What is s? " << s << endl;
	table.lookup("1", s, table.ScopeOption::CURRENT);
	cout << "What is s? " << s << endl;
	table.lookup("1", s, table.ScopeOption::GLOBAL);
	cout << "What is s? " << s << endl;
	
	return 0;
}