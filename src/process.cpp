/*
 * process.cpp
 * Copyright (C) 2016 sebastian <sebastian@ARCH-LINUX>
 *
 * Distributed under terms of the MIT license.
 */

#include "calculator.h"

DataStore NumberExpression::store;

string NumberExpression::getValue()
{
	if(errorFlag) {
		return "";
	}
	
	if(value == "") {
		process();
	} 
	
	if(value == "") {
		if(errorFlag) {
			return "";
		}
		errorFlag = true;
		error = "process() must set the value!";
	}
	
	return value;
}

double NumberExpression::resolveValue(NumberExpression& exp)
{
	if(errorFlag) {
		return 0.0;
	}
	
	double value = 0.0;
	try {
		value = stod(exp.getValue());
	} catch (invalid_argument e) {
		if(errorFlag) {
			return 0.0;
		}
		if(find(store.variables, exp.getValue())) {
			value = store.variables[exp.getValue()];
		} else {
			error = "symbol not found: " + exp.getValue();
			errorFlag = true;
		}
	}
	
	return value;
}


string NumberExpression::doCalculation(NumberExpression& num, string func)
{
	if(errorFlag) {
		return "";
	}
	
	double n = resolveValue(num);
	double result = 0.0;
	
	if(find(functions, func)) {
		result = functions[func](n);
	} else if(find(store.functions, func)) {
		// TODO: process functions
		error = "processing of custom functions not implemented yet!";
		errorFlag = true;
	} else {
		error = "function not found: " + func;
		errorFlag = true;
	}
	
	return to_string(result);
}

string NumberExpression::doCalculation(NumberExpression& num1, NumberExpression& num2, char op) 
{
	if(errorFlag) {
		return "";
	}
	
	double n1 = resolveValue(num1);
	double n2 = resolveValue(num2);
	
	switch(op) {
		case '*': return to_string(n1 * n2);
		case '/': return to_string(n1 / n2);
		case '+': return to_string(n1 + n2);
		case '-': return to_string(n1 - n2);
		case '^': return to_string(pow(n1, n2));
	}
	
	error = "unknown operator: " + string(1, op);
	errorFlag = true;
	return "";
}


void NumberExpression::process()
{	
	if(!parsed) {
		parse();
	}
	
	if(errorFlag) {
		return;
	}
	
	if (value != "") {
		return;
	}

	for(NumberExpression& exp : numbers) {
		exp.process();
	}

	vector<NumberExpression> numbers = this->numbers;
	vector<string> operations = this->operations;
	
	// precedences:
	// 0: functions
	// 1: ^
	// 2: * /
	// 3: + -
	for(int precedence = 0; precedence < 4; precedence++) {
		if(precedence == 1) {
			// process pow calls (sqrt calls are functions)
			for(int i = operations.size() - 1; i >= 0; i--) {
				if(operations[i].size() == 1) {
					if(find(rightOps[precedence], operations[i][0])) {
#ifdef DEBUG
						cout << "do: " << numbers[i].getValue() << operations[i] << numbers[i + 1].getValue();
#endif
						NumberExpression exp;
						exp.setValue(doCalculation(numbers[i], numbers[i + 1], operations[i][0]));
						
						if(errorFlag) {
							return;
						}
						
						numbers[i] = exp;		
#ifdef DEBUG
						cout << "=" << numbers[i].getValue() << endl;
#endif
						numbers.erase(numbers.begin() + i + 1);
						operations.erase(operations.begin() + i);
					} 
				}
			}
		} else {
			for(int i = 0; i < operations.size(); i++) {
				if(precedence == 0) {
					// process function calls
					if(operations[i].size() > 1) {
						if(find(functions, operations[i])) {
#ifdef DEBUG
							cout << "do: " << operations[i] << "(" << numbers[i].getValue() << ")";
#endif
							NumberExpression exp;
							exp.setValue(doCalculation(numbers[i], operations[i]));
					
							if(errorFlag) {
								return;
							}
							
							numbers[i] = exp;
#ifdef DEBUG
							cout << "=" << numbers[i].getValue() << endl;
#endif
							operations.erase(operations.begin() + i);
							i--;
						}
					}
				} else if(precedence >= 2) {
					// process * and /, + and - calls
					if(operations[i].size() == 1) {
						if(find(leftOps[precedence], operations[i][0])) {
#ifdef DEBUG
							cout << "do: " << numbers[i].getValue() << operations[i] << numbers[i + 1].getValue();
#endif
							NumberExpression exp;
							exp.setValue(doCalculation(numbers[i], numbers[i + 1], operations[i][0]));
							
							if (errorFlag) {
								return;
							}
							
							numbers[i] = exp;
#ifdef DEBUG
							cout << "=" << numbers[i].getValue() << endl;
#endif
							numbers.erase(numbers.begin() + i + 1);
							operations.erase(operations.begin() + i);
							i--;
						}
					}
				}
			}
		}
	}

#ifdef DEBUG
	cout << endl;
#endif

	value = numbers[0].getValue();
}



