#include <iostream>
#include <string>
#include <vector>
#include <cstdio> // for exception formatting
#include <math.h> // for pow

using namespace std;

class ShuntingYardCalculator {

private:
    // vector containing the split mathematical expression
    vector<string> tokenList;

    // splits mathematical expression to propery vector of operators and numbers
    void initiateTokenList(string expression) {
        // iterator over the expression characters
        vector<char> expressionVector (expression.begin(), expression.end());
        for (int index = 0; index < expression.length(); index) {
            switch (expressionVector[index]) {
            case '+': tokenList.push_back("+"); index++; break;
            case '-': tokenList.push_back("-"); index++; break;
            case '(': tokenList.push_back("("); index++; break;
            case ')': tokenList.push_back(")"); index++; break;
            case '*': tokenList.push_back("*"); index++; break;
            case '/': tokenList.push_back("/"); index++; break;
            case '^': tokenList.push_back("^"); index++; break;
            case ' ': index++; break;
            default: {
                if (isdigit(expressionVector[index])) {
                    string number;
                    while (true) {
                        char current_char = expressionVector[index];
                        if (isdigit(current_char) || current_char == '.') {
                            number += current_char;
                            index++;
                            continue;
                        }
                        else {
                            tokenList.push_back(number);
                            break;
                        }
                
                    }
                }
                else {
                    cout << "Unknown symbol contained at " << 
                    index << ", will be ignored.\n";
                    index++;
                }
            }
            }

        }
    }

    int operatorPrecedence(string mathOperator) {
        if (mathOperator == "+") return 2;
        if (mathOperator == "-") return 2;
        if (mathOperator == "*") return 3;
        if (mathOperator == "/") return 3;
        if (mathOperator == "^") return 4;
        if (mathOperator == "(" || mathOperator == ")") return 1;
        throw std::invalid_argument("Operator has no precedence: operator doesn't exist.");
    }

    // calculates [num1 mathOperator num2] as in [2 + 4]
    float compute(string mathOperator, float num1, float num2) {
        if (mathOperator == "+") return num1 + num2;
        if (mathOperator == "-") return num1 - num2;
        if (mathOperator == "*") return num1 * num2;
        if (mathOperator == "/") return num1 / num2;
        if (mathOperator == "^") return pow(num1, num2);
        char buff[25];
        snprintf(buff, sizeof(buff), "Can't compute operator (%s)", mathOperator);
        throw std::invalid_argument(buff);
    }

    string operatorType(string mathOperator) {
        if (mathOperator == "+" ||
            mathOperator ==  "-" ||
            mathOperator ==  "*" ||
            mathOperator == "/" ||
            mathOperator == "^" ) 
        {
            return mathOperator;
        }
        return "";
    }

    /*
    Reorders and modifies the token list into 
    the output stack, so that the reverse polish 
    calculating format can be used.
    */
    vector<string> shuntingAlgorythm() {
        // Vector for containing operators from the stack.
        vector<string> operatorStack;
        /* 
        Vector containing numbers and operators 
        for reverse polish calculating format.
        */
        vector<string> outputQueue;

        // Loop over tokens and react appropriatly to each.
        for (int index = 0; index < tokenList.size(); index++) {
            string token = tokenList[index];

            // If number, add to output stack.
            if (isdigit(token[0])) {
                outputQueue.push_back(token);
                continue;
            }

            // If operator, insert when with precedence.
            if (operatorType(token) != "") {
                int operatorStackLastPrecedence = operatorStack.size() != 0 ? operatorPrecedence(operatorStack.back()) : 0;
                while (operatorPrecedence(token) < operatorStackLastPrecedence) {
                    outputQueue.push_back(operatorStack.back());
                    operatorStack.pop_back();
                    operatorStackLastPrecedence = operatorStack.size() != 0 ? operatorPrecedence(operatorStack.back()) : 0;
                }
                operatorStack.push_back(token);
                continue;
            }

            if (token == "(") {
                operatorStack.push_back(token);
                continue;
            }

            // Push all operators from brackets to output.
            if (token == ")") {
                string opStackBack = operatorStack.back();
                while (opStackBack != "(")
                {
                    outputQueue.push_back(operatorStack.back());
                    operatorStack.pop_back();
                    opStackBack = operatorStack.back();
                }
                operatorStack.pop_back();
                continue;
            }
        }
        // Moves remaining operators to output stack.
        while (operatorStack.size() != 0) {
            string back = operatorStack.back();
            outputQueue.push_back(back);
            operatorStack.pop_back();
        }
        return outputQueue;
    }

    // Calculates the result from vector of numbers and operators
    float reversePolishFormateCalculate(vector<string> expressionInReversePolish) {
        // stack holding numbers until they are operated on
        vector<float> resultStack;
        for (string token: expressionInReversePolish) {
            // push numbers to stack
            if (isdigit(token[0])) {
                resultStack.push_back(std::stof(token));
            }
            /* When an operator is encountered perform operation
            between numbers on top of the stack. */
            else {
                // save and delete 2 top numbers
                float num2 = resultStack.back();
                resultStack.pop_back();
                float num1 = resultStack.back();
                resultStack.pop_back();

                // perform operation and save new number to stack
                resultStack.push_back(compute(token, num1, num2));
            }
        }
        /* By now only one number should be left in 
        the stack. If not, we have an exception */
        if (resultStack.size() == 1) {
            return resultStack[0];
        }
        throw std::invalid_argument("Invalid input. Did you miss an operator?");
    }

public:
    float calculate(string expression) {
        initiateTokenList(expression);
        // Vector of numbers and operators ordered to fit reverse polish format.
        vector<string> reversePolishInput = shuntingAlgorythm();
        return reversePolishFormateCalculate(reversePolishInput);
    }
};

int main() {

    cout << "Calculator\n" << "Type in expression: ";
    string expression;
    getline(cin, expression);

    ShuntingYardCalculator calculator;
    float result = calculator.calculate(expression);
    cout << "Result: " << result << endl;
}
