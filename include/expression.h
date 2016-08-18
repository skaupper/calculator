#ifndef EXPRESSION_HAND_SIDE_H
#define EXPRESSION_HAND_SIDE_H

#include "calculator.h"

class DataStore {
private:
	// name, value
	std::map<std::string, double> variables;
	
	// name, <variable, actual funcion>
	std::map<std::string, std::pair<std::string, std::string>> functions;
public:
	bool hasVariable(std::string name) { return find(variables, name); }
	bool hasFunction(std::string name) { return find(functions, name); }
	
	double getVariable(std::string name) { return variables[name]; }
	std::pair<std::string, std::string> getFunction(std::string name) { return functions[name]; }
	
	void setVariable(std::string name, double value) { variables[name] = value; }
	void setFunction(std::string name, std::string variableName, std::string functionBody)
		{ functions[name] = std::make_pair<std::string, std::string>(variableName, functionBody); }
	
};

class Expression {
private:
	bool errorFlag;
	std::string error;
	
	std::string expression;
	bool parsed;
	bool processed;
	bool resolved;
	
	std::string stringValue;
	double doubleValue;
	
	
	virtual void _parse() = 0;
	virtual void _process() = 0;
	virtual double _resolveValue() = 0;

protected:
	static DataStore store;
	
public:
	Expression() : Expression("") {}
	Expression(std::string exp) : 
		errorFlag(false), 
		error(""), 
		expression(exp), 
		parsed(false), 
		processed(false), 
		resolved(false),
		stringValue(""),
		doubleValue(0.0){}

	void parse();
	void process();
	double resolveValue();
	
	DataStore& getStore() { return Expression::store; }
	

	std::string getExpression() { return expression; }
	void setExpression(std::string exp) { expression = exp; parsed = false; processed = false; resolved = false; }

	std::string getStringValue() { return stringValue; }
	void setStringValue(std::string v) { stringValue = v; parsed = true; processed = true; resolved = false; }
	
	double getDoubleValue() { return doubleValue; }
	void setDoubleValue(double v) { doubleValue = v; parsed = true; processed = true; resolved = true; }
	
	bool isParsed() { return parsed; }
	
	std::string getError() { return error; }
	bool hasError() { return errorFlag; }
	
	void resetError() { errorFlag = false; }
};


#endif