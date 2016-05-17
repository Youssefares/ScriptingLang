#include <string>
#include "Stack.h"
#include <cctype>
#include <iostream>
#include <map>




/*
 * evaluates "operand (operator) operand"
 */
double eval(char op,double v1, double v2){
    if(op == '+') return v1+v2;
    if(op == '-') return v1-v2;
    if(op == '*') return v1*v2;
    if(op == '/') return v1/v2;
	if (op == '%') return (int)v1%(int)v2;
	else return -1;
}

int precedence(char op) {
	if (op == '*' || op == '/' || op == '%') return 2;
	if (op == '-' || op == '+') return 1;
	return -1;
}
/*
 * compares operators' precedence.
 */
int prior(char op1, char op2) {
	return precedence(op1) >= precedence(op2);
}

//bool errorFlag = false;

typedef enum error {
    INVALID_EXPRESSION,
    INVALID_SYMBOL,
    MISSING_EQUALS,
    LEFT_TOO_MANY_VARS,
    NONE
} error;


error errorFlag = NONE;

/**
 *
 * Used to encapsulate manipulating error flags to be easier to debug.
 */
void raiseError(error error){
    errorFlag = error;
}

void clearErrors(){
    errorFlag = NONE;
}

/**
 * Prints an error message appropriate to the error had there been any.
 * @return whether an error has been handled or not.
 */
bool interpretErrors(){
    switch (errorFlag){
            case INVALID_EXPRESSION:
             cout<<"Invalid Expression";
            break;
        case INVALID_SYMBOL:
            cout<<"Invalid Symbol";
            break;
        case MISSING_EQUALS:
            cout<<"The command is missing the = character";
            break;
        case LEFT_TOO_MANY_VARS:
            cout<<"There can't be more than one variable in the left hand side";
            break;
        default:
            return false;
    }
    errorFlag = NONE;
    return true;
}


/*
 * implements Dijkstra's Two Stack Algorithm for Infix Evaluation.
 * utilizes Stack.h's Template Class Stack.
 * returns double value of expression.
 */


//MSA ^_^
double evaluate(string s){
	Stack<double> operands;
	Stack<char> operators;

	bool negative = false;
	//iterate on input string
	for(int i = 0; i < s.length(); i++){
		//if char is part of an operand
		if(isdigit(s[i]) || s[i] == '.'){
			//get full number string.
           string numberString = "";
           while(isdigit(s[i]) || s[i] == '.') numberString += s[i++];
		   i--;

		   //parse string into double of base 10 && push to operands' stack.
		   double number = stod(numberString);
		   if (negative) {
			   number *= -1;
			   negative = false;
		   }
		   operands.push(number);
		   continue;
        }

		//TODO
		//if(isalpha(s[i]) check if in symbol table{ getvalue and push on to operands stack}

		//skip spaces
		if (s[i] == ' ') continue;

		//push left bracket to operators' stack to know when to stop popping when a right bracket is met.
		if (s[i] == '(') {
			operators.push(s[i]);
			continue;
		}

		//keep evaluating pairs of operands and their operation, and pushing the result to the stack till left bracket is met.
		if (s[i] == ')') {
			if(operators.isEmpty()){
                raiseError(INVALID_EXPRESSION);
					return 0;
			}
			while (operators.peak() != '(') {
				if(operands.isEmpty()){
                    raiseError(INVALID_EXPRESSION);
					return 0;
				}
				double v2 = operands.pop();
				if(operands.isEmpty()){
                    raiseError(INVALID_EXPRESSION);
					return 0;
				}
				double v1 = operands.pop();
				if(operators.isEmpty()){
                    raiseError(INVALID_EXPRESSION);
					return 0;
				}
				char o = operators.pop();
				operands.push(eval(o, v1, v2));
			}
			// pop the left bracket.
			if(operators.isEmpty()){
                raiseError(INVALID_EXPRESSION);
					return 0;
			}
			operators.pop();
			continue;
		}

		//handle -ve values
		if (s[i] == '-') {
			//if the -ve sign is the first character
			if (i == 0) {
				negative = true;
			}
			//find the first character that is not a space before the -ve sign.
			else for (int j = i - 1; j >= 0; j--) {
				if (s[j] == ' ') continue;

				// if it's a digit or a variable, then it's a subtraction. Nothing to be done.
				if (isdigit(s[j])) break;
				//if(in symbol table) break;

				// else it's an operator of some kind, meaning this is a -ve sign and not subtraction.
				else {
					negative = true;
					break;
				}
			}
		}
		if (negative) continue;

		//if it isn't an operand or a right or left bracket. It's an operator. Loop condition makes sure precedence isn't overlooked.
		while (!operators.isEmpty() && prior(operators.peak(),s[i]) && operators.peak() != '(') {
			if(operands.isEmpty()){
                raiseError(INVALID_EXPRESSION);
					return 0;
			}
			double v2 = operands.pop();
			if(operands.isEmpty()){
                raiseError(INVALID_EXPRESSION);
					return 0;
			}
			double v1 = operands.pop();
			if(operators.isEmpty()){
                raiseError(INVALID_EXPRESSION);
					return 0;
			}
			char o = operators.pop();
			operands.push(eval(o, v1, v2));
		}
		operators.push(s[i]);
    }

	//after all is said is done. Make sure nothing is left on the operators' stack.
	while (!operators.isEmpty()) {
		if(operands.isEmpty()){
            raiseError(INVALID_EXPRESSION);
					return 0;
		}
		double v2 = operands.pop();
		if(operands.isEmpty()){
            raiseError(INVALID_EXPRESSION);
					return 0;
		}
		double v1 = operands.pop();
		if(operators.isEmpty()){
            raiseError(INVALID_EXPRESSION);
					return 0;
		}
		char o = operators.pop();
		operands.push(eval(o, v1, v2));
	}
	//only value left is the result of the evaluated expression.
	if(operands.isEmpty()){
        raiseError(INVALID_EXPRESSION);
			return 0;
	}
	double returnV =  operands.pop();
	if(operands.isEmpty() == false){
        raiseError(INVALID_EXPRESSION);
		return 0;
	}
	return returnV;
}


/**
*Hashing function used for the HashMap
*/
unsigned long hashFunction(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

bool isOperandOrDigit(char x){
    return isdigit(x) || x == '+' ||  x == '-' || x == '/' || x == '*' || x == ')' || x == '(' || x == '.' || x == '=';
}


bool isOperand(char x){
    return  x == '+' ||  x == '-' || x == '/' || x == '*' || x == ')' || x == '(' ;
}


/**
 * Acts like a preprocessor. Removes all the occurences of any variable and replaces
 * it with its value.
 */
void removeVarOccurences(string &code, std::map<string,float> *variables){
    int numVariablesOnLeft = 0;
    int equalIndex = code.find('=');
    for(int i = 0; i < code.length(); i++){
        char  current =  code[i];
        if(!isOperandOrDigit(current)){
            int j = i +1;
            if(isOperand(current) && j < equalIndex)
                raiseError(INVALID_EXPRESSION);
            while(!isOperandOrDigit(code[j]) && j < code.length())
                j++;
            
            string  key = code.substr(i,j - i);
            if(variables->count(key) && (equalIndex == -1 || j > equalIndex)){
            float value = (*variables)[key];
            code = code.substr(0,i) + std::to_string(value) + code.substr(j);
            } else if(j > equalIndex) {
                raiseError(INVALID_SYMBOL);
            } else {
                numVariablesOnLeft++;
                if(numVariablesOnLeft > 1)
                    raiseError(LEFT_TOO_MANY_VARS);
            }
            i = j;
        }
    }
}

void removeSpaces(string * str){
    int i = 0;
    while(i < str->length()){
        if( (*str)[i] == ' ')
            str->erase(i, 1);
        else
            i++;
    }
}





/*
 * Parsing function: Executes statement passed on as string argument.
 *
 * If the expression only contains operators/operands it evaluates it.
 * 
 *
 * Otherwise it evaluates the expression and assign it to the variable
 *
 */

void execute(string code, std::map<string,float> *variables){
    clearErrors();
    removeSpaces(&code);
	int equalIndex = code.find('=');
    if( equalIndex == -1){
        removeVarOccurences(code,variables);
        if(!interpretErrors()){
            double result = evaluate(code);
            if(!interpretErrors())
                cout<<result;
        }
	} else {
        removeVarOccurences(code , variables);
        if(!interpretErrors()){
            
		string key = code.substr(0,equalIndex);
		(*variables)[key] = evaluate(code.substr(equalIndex + 1));
        interpretErrors();
        }
	}
	cout<<"\n\n";
}






