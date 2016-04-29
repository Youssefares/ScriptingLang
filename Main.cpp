#include <cstdio>
#include "Helpers.h"
#include "hashset.h"

int main(int argc, char* argv[]){
	//if run without file args, run interpreter
	if(argc  == 1){
		string codeline;
	LOOP:
		for(cout<<">>",getline(cin,codeline); codeline != "q"; getline(cin,codeline)){ 
		    execute(codeline);
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