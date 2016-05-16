#include <cstdio>
#include "Helpers.h"
//#include "hashset.h"
#include <map>

int main(int argc, char* argv[]){
	//if run without file args, run interpreter
	std::map<string, float> variables;
	if(argc  == 1){
		string codeline;
	LOOP:
		for(cout<<">>",getline(cin,codeline); codeline != "q"; getline(cin,codeline)){ 
		    execute(codeline, &variables);
		    cout<<">>";
		}
		
		string ans;
		do{
		cout<<"Are you sure you want to quit?(y/n)";
		getline(cin,ans);
		
		// I hate gotos but Fastest way to do it ^^.
		if(ans == "n") goto LOOP;
		}while(ans != "y" && ans != "n");
		
	}
	//else read from file.
}


//Instance variables

