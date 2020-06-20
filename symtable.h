/*symtable.h*/

// 
// << Everardo Gutierrez >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #03: symtable
//
// Symbol Table: a symbol table is a stack of scopes, typically used by a
// compiler to keep track of symbols in a program (functions, variables,
// types, etc.).  In most programming languages, you "enter scope" when you 
// see {, and "exit scope" when you see the corresponding }.  Example:
//
// int main()
// {              <-- enterScope()
//    int i;      <-- enter "i" into symbol table as type "int"
//    .
//    .
//    while (true)
//    {              <-- enterScope()
//       char i;     <-- enter "i" into symbol table as type "char"
//
// Notice there are two variables named "i", which is legal because
// they are in different scopes.
//

#pragma once

#include <iostream>
#include <deque>
#include <map>

using namespace std;

template<typename KeyT, typename SymbolT>
class symtable
{
public:
  //
  // A symbol table is a stack of scopes.  Every scope has a name, and 
  // we use a map to hold the symbols in that scope.  You can *add* to
  // this class, but you must use the Name and Symbols as given to store
  // the scope name, and scope symbols, respectively.
  //
  class Scope
  {
  public:
    string              Name;
    map<KeyT, SymbolT>  Symbols;
    // constructors:
    Scope()
    {
      // default empty string and an empty map:
    }

    Scope(string name)
    {
      this->Name = name;
      // empty map created by map's constructor:
    }
	
  };

private:
  //
  // TODO: implementation details
  //
	deque<Scope> d;
	int totalSymbols;
public:
  enum class ScopeOption
  {
    ALL, 
    CURRENT,
    GLOBAL
  };

  //
  // default constructor:
  //
  // Creates a new, empty symbol table.  No scope is open.
  //
  symtable()
  {
    //
    // TODO: note that member variables will have their default constructor 
    // called automatically, so there may be nothing to do here.
    //
	totalSymbols = 0;
  }

  //
  // size
  //
  // Returns total # of symbols in the symbol table.
  //
  // Complexity: O(1)
  //
  int size() const
  {
    //
    // TODO: DONE
    //
	return totalSymbols;
  }

  //
  // numscopes
  //
  // Returns the # of open scopes.
  //
  // Complexity: O(1)
  //
  int numscopes() const
  {
    //
    // TODO: DONE
    //    
    return d.size();
  }

  //
  // enterScope
  //
  // Enters a new, open scope in the symbol table, effectively "pushing" on
  // a new scope.  You must provide a name for the new scope, although
  // the name is currently used only for debugging purposes.
  //
  // NOTE: the first scope you enter is known as the GLOBAL scope, since this
  // is typically where GLOBAL symbols are stored.  
  //
  // Complexity: O(1)
  //
  void enterScope(string name)
  {
    //
    // TODO: DONE
    //
    Scope s1(name);
	d.push_back(s1); //Push to the back of the deque to be popped off later on when we exit 
  }

  //
  // exitScope
  //
  // Exits the current open scope, discarding all symbols in this scope.
  // This effectively "pops" the symbol table so that it returns to the 
  // previously open scope.  A runtime_error is thrown if no scope is 
  // currently open.  
  //
  // Complexity: O(1)
  //
  void exitScope()
  {
    //
    // TODO: Done
    //
    if(d.empty()) //throw arror, attempting to exit a scope when none exist in the deque 
		throw runtime_error("No Scopes are Open");
	totalSymbols = totalSymbols - (d.back()).Symbols.size();
    d.pop_back(); //pop off the inner most scope from the deque (back of the deque)
  }

  //
  // curScope
  //
  // Returns a copy of the current scope.  A runtime_error is thrown if
  // no scope is currently open.
  //
  // Complexity: O(N) where N is the # of symbols in the current scope
  //
  Scope curScope() const
  {
    //
    // TODO: DONE
    //
    if(d.empty()) //throw arror, attempting to return a scope when none exist in the deque
		throw runtime_error("No Scopes are Open");
	return d.back(); //return the inner most scope from the deque (back of the deque)
  }

  //
  // insert
  //
  // Inserts the (key, symbol) pair in the *current* scope.  If the key
  // already exists in the current scope, the associated symbol is replaced
  // by this new symbol.
  //
  // Complexity: O(lgN) where N is the # of symbols in current scope
  //
  void insert(KeyT key, SymbolT symbol)
  {
    //
    // TODO: DONE
    //
    if(d.empty()) //attempting to enter a pair but no scopes currently are open so return
		return;
    auto ptr = (d.back()).Symbols.find(key); //search in current scope to determine if key already exists or not
	if(ptr == (d.back()).Symbols.end()){
		(d.back()).Symbols.emplace(key,symbol); //key does not exist. Add new pair of (key, symbol) to map of current scope
		totalSymbols++;
	}else{ 
		ptr->second = symbol; //key already exist in the map. Update with new symbol value 
	}
	
  }

  //
  // lookup
  //
  // Searches the symbol table for the first (key, symbol) pair that 
  // matches the given key.  The search starts in the current scope, and 
  // proceeds "outward" to the GLOBAL scope.  If a matching (key, symbol)
  // pair is found, true is returned along with a copy of the symbol (via 
  // "symbol" reference parameter).  If not found, false is returned and
  // the "symbol" parameter is left unchanged.
  //
  // NOTE: the search can be controlled by the "option" parameter.  By 
  // default, the entire symbol table is searched as described above.
  // However, the search can also be limited to just the current scope, 
  // or just the GLOBAL scope, via the "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   string                   symbol;
  //   bool                     found;
  //   ...
  //   found = table.lookup("i", 
  //                        symbol, 
  //                        symtable<string,string>::ScopeOption::CURRENT);
  //
  // Complexity: O(SlgN) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  bool lookup(KeyT key, 
              SymbolT& symbol, 
              ScopeOption option = ScopeOption::ALL) const
  {
    //
    // TODO: DONE
    //
    if(d.empty()) //attempting to lookup a pair but no scopes currently are open so return
		return false;
	if (option == ScopeOption::ALL){
		for(auto i = d.rbegin(); i != d.rend(); ++i){ //loop through the entire deque starting with current scope
			auto ptr = i->Symbols.find(key); //lookup the key in the current scope's map
			if(ptr != i->Symbols.end()){ //determine if key exists in map or not
				symbol = ptr->second; //key was found and return its symbol and value of true 
				return true;
			}
		}
	}else if (option == ScopeOption::GLOBAL){
		auto ptr = (d.front()).Symbols.find(key); //Global scope is the outer most scope and resides at the front of the deque
		if(ptr != (d.front()).Symbols.end()){ //determine if key exists in map or not
			symbol = ptr->second; //key was found and return its symbol and value of true 
			return true;
		}
	}else{
		auto ptr = (d.back()).Symbols.find(key); //current scope is the inner most scope and resides at the back of the deque
		if(ptr != (d.back()).Symbols.end()){ //determine if key exists in map or not
			symbol = ptr->second; //key was found and return its symbol and value of true 
			return true;
		}
	}
    return false;
  }

  //
  // dump
  // 
  // Dumps the contents of the symbol table to the output stream, 
  // starting with the current scope and working "outward" to the GLOBAL
  // scope.  You can dump the entire symbol table (the default), or dump
  // just the current scope or global scope; this is controlled by the 
  // "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   ...
  //   table.dump(std::cout, symtable<string,string>::ScopeOption::GLOBAL);
  //
  // Complexity: O(S*N) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  void dump(ostream& output, ScopeOption option = ScopeOption::ALL) const
  {
    output << "**************************************************" << endl;

    if (option == ScopeOption::ALL)
      output << "*************** SYMBOL TABLE (ALL) ***************" << endl;
    else if (option == ScopeOption::CURRENT)
      output << "*************** SYMBOL TABLE (CUR) ***************" << endl;
    else // global:
      output << "*************** SYMBOL TABLE (GBL) ***************" << endl;

    output << "** # of scopes: " << this->numscopes() << endl;
    output << "** # of symbols: " << this->size() << endl;

    //DONE
    // output format per scope:
    //
    // ** scopename **
    // key: symbol
    // key: symbol
    // ...
    //
	if (option == ScopeOption::ALL){ 
		for(auto i = d.rbegin(); i != d.rend(); ++i){ //loop through the entire deque starting with current scope
			output << "** " << i->Name << " **" << endl;
			for(auto m: i->Symbols) //continue looping as long as a pair exists in the map of the current scope
				output << m.first << ": " << m.second << endl;
		}
	}else if (option == ScopeOption::GLOBAL){
		output << "** " << (d.front()).Name << " **" << endl;
		//Global scope is the outer most scope and resides at the front of the deque
			for(auto m: (d.front()).Symbols) //continue looping as long as a pair exists in the map of the global scope
				output << m.first << ": " << m.second << endl; 
	}else{
		output << "** " << (d.back()).Name << " **" << endl;
		//current scope is the inner most scope and resides at the back of the deque
			for(auto m: (d.back()).Symbols) //continue looping as long as a pair exists in the map of the current scope
				output << m.first << ": " << m.second << endl;
	}
    output << "**************************************************" << endl;
  }

};
