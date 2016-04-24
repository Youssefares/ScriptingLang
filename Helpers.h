#include <string>
#include "Stack.h"
#include <cctype>

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

/*
 * implements Dijkstra's Two Stack Algorithm for Infix Evaluation.
 * utilizes Stack.h's Template Class Stack.
 * returns double value of expression.
 */
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
		//if(isalpha(s[i]) check if in symbol table{ eval and push entire variable name on to operands stack}
		
		//skip spaces
		if (s[i] == ' ') continue;
		
		//push left bracket to operators' stack to know when to stop popping when a right bracket is met.
		if (s[i] == '(') {
			operators.push(s[i]);
			continue;
		}

		//keep evaluating pairs of operands and their operation, and pushing the result to the stack till left bracket is met.
		if (s[i] == ')') {
			while (operators.peak() != '(') {
				double v2 = operands.pop();
				double v1 = operands.pop();
				char o = operators.pop();
				operands.push(eval(o, v1, v2));
			}
			// pop the left bracket.
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
			else for (int j = i - 1; i >= 0; i--) {
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
			double v2 = operands.pop();
			double v1 = operands.pop();
			char o = operators.pop();
			operands.push(eval(o, v1, v2));
		}
		operators.push(s[i]);
    }

	//after all is said is done. Make sure nothing is left on the operators' stack.
	while (!operators.isEmpty()) {
		double v2 = operands.pop();
		double v1 = operands.pop();
		char o = operators.pop();
		operands.push(eval(o, v1, v2));
	}
	//only value left is the result of the evaluated expression.
	return operands.pop();
}