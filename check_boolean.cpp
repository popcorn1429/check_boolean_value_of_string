/*
*  将一个由 0 1 and or ( ) 组成的字符串逻辑表达式，运算得出其逻辑结果
*  例如： "1 and (1 or (0 and 1 and 0)) and ((1 and 1) or 1) and 1"
*  计算后的结果是 1
*/

#include <string>
#include <stack>
#include <iostream>
using namespace std;
//operand1 和 operand2 可取 '0' '1'，oper 可取 'a' 'o'
char calc(char operand1, char operand2, char oper) {
    switch (oper)
    {
    case 'a': // &&
        return (operand1 == '1' && operand2 == '1') ? '1' : '0';
    case 'o': // ||
        return (operand1 == '0' && operand2 == '0') ? '0' : '1';
    default:
        return '0';
    break;
    }
}

//新的运算子operand是一个数字 '0' '1'
bool calc_with_digit(stack<char>& operands, stack<char>& operators, char operand) {
    if (operands.empty()) {
        if (!operators.empty()) {
            cout << "no digit before operator [index:" << operators.size()
                << ", oper:" << operators.top() << "], invalid expression!" << endl;
            return false;
        }
        else {
            //两个栈都空了
            operands.push(operand); //到当前值为止的结果保存在 运算子 栈里面
            return true;
        }
    }

    if (operands.top() == '(') {
        operands.push(operand);
    }
    else {
        if (operators.empty()) {
            cout << "no oper between two digit!" << endl;
            return false;
        }

        if ('a' == operators.top()) {
            char result = calc(operands.top(), operand, operators.top()); //一次逻辑运算
            operands.pop();
            operators.pop();
            operands.push(result);                                        //结果替换掉原来的栈顶数字
        }
        else if ('o' == operators.top()) {
            operands.push(operand); //存放进来暂不计算，因为 or的优先级低于 and
        }
    }
    return true;
}

//新的运算子是一个 ')'
bool calc_with_right_bracket(stack<char>& operands, stack<char>& operators) {
    if (operands.empty()) {
        cout << "no matched '(' before,invalid expression!" << endl;
        return false;
    }
    else if ('(' == operands.top()) {
        cout << "No digit after (,invalid expression!" << endl;  //连续的 "()" 也是不合法的，中间应该有数字
        return false;
    }
    else {
        char operand = operands.top();
        operands.pop(); //pop 数字
        if (operands.empty()) {
            cout << "no matched (,invalid expression!" << endl;
            return false;
        }
        
        if ('(' != operands.top()) {
            while ('(' != operands.top()) {
                operand = calc(operands.top(), operand, operators.top());
                operands.pop();
                operators.pop();
            }
            operands.pop(); //pop (
            calc_with_digit(operands, operators, operand);
        }
        else {
            operands.pop(); //pop '('
            return calc_with_digit(operands, operators, operand);
        }
    }
}

//operand 一定是 '0' '1' ')' 之一
bool calc(stack<char>& operands, stack<char>& operators, char operand) {
    if (operand == ')') {
        return calc_with_right_bracket(operands, operators);
    }
    else {
        return calc_with_digit(operands, operators, operand);
    }
}

char check(const string& src) {
    stack<char> operands;   //只保存三种元素 '1' '0' '('，栈顶向下不会有两个连续的数字
    stack<char> operators;  //只保存两种元素 'a' 'o'

    for (string::size_type i = 0; i < src.size();) {
        switch (src[i])
        {
        case ' ':
            ++i;
            break;
        case '1':
        case '0':
        case ')':
            //calc
            if (!calc(operands, operators, src[i]))
                return 'X';
            ++i;
            break;
        case '(':
            operands.push(src[i]);
            ++i;
            break;
        case 'a':
            //and
            operators.push(src[i]);
            i += 3;
            break;
        case 'o':
            //or
            operators.push(src[i]);
            i += 2;
            break;
        default:
            break;
        }
    }

    //走到这里，所有括号都已经消失了，而且and运算也应该没有了，只剩下一个结果，或者简单的or运算了
    if (!operands.empty()) {
        char operand = operands.top();
        operands.pop();
        while (!operators.empty()) {
            operand = calc(operands.top(), operand, operators.top());
            operands.pop();
            operators.pop();
        }

        if (operands.empty()) {
            operands.push(operand);
        }
        return operands.top();
    }
    else {
        return 'X';         //入参是空串或是不合法的表达式
    }
}

int main() {
    string src{"1 and (0 or (0 and 1 and 0)) or ((1 and 1) or 1) and 0"};
    cout << check(src) << endl;

    system("pause");
    return 0;
}