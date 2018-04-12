/*
*  ��һ���� 0 1 and or ( ) ��ɵ��ַ����߼����ʽ������ó����߼����
*  ���磺 "1 and (1 or (0 and 1 and 0)) and ((1 and 1) or 1) and 1"
*  �����Ľ���� 1
*/

#include <string>
#include <stack>
#include <iostream>
using namespace std;
//operand1 �� operand2 ��ȡ '0' '1'��oper ��ȡ 'a' 'o'
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

//�µ�������operand��һ������ '0' '1'
bool calc_with_digit(stack<char>& operands, stack<char>& operators, char operand) {
    if (operands.empty()) {
        if (!operators.empty()) {
            cout << "no digit before operator [index:" << operators.size()
                << ", oper:" << operators.top() << "], invalid expression!" << endl;
            return false;
        }
        else {
            //����ջ������
            operands.push(operand); //����ǰֵΪֹ�Ľ�������� ������ ջ����
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
            char result = calc(operands.top(), operand, operators.top()); //һ���߼�����
            operands.pop();
            operators.pop();
            operands.push(result);                                        //����滻��ԭ����ջ������
        }
        else if ('o' == operators.top()) {
            operands.push(operand); //��Ž����ݲ����㣬��Ϊ or�����ȼ����� and
        }
    }
    return true;
}

//�µ���������һ�� ')'
bool calc_with_right_bracket(stack<char>& operands, stack<char>& operators) {
    if (operands.empty()) {
        cout << "no matched '(' before,invalid expression!" << endl;
        return false;
    }
    else if ('(' == operands.top()) {
        cout << "No digit after (,invalid expression!" << endl;  //������ "()" Ҳ�ǲ��Ϸ��ģ��м�Ӧ��������
        return false;
    }
    else {
        char operand = operands.top();
        operands.pop(); //pop ����
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

//operand һ���� '0' '1' ')' ֮һ
bool calc(stack<char>& operands, stack<char>& operators, char operand) {
    if (operand == ')') {
        return calc_with_right_bracket(operands, operators);
    }
    else {
        return calc_with_digit(operands, operators, operand);
    }
}

char check(const string& src) {
    stack<char> operands;   //ֻ��������Ԫ�� '1' '0' '('��ջ�����²�������������������
    stack<char> operators;  //ֻ��������Ԫ�� 'a' 'o'

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

    //�ߵ�����������Ŷ��Ѿ���ʧ�ˣ�����and����ҲӦ��û���ˣ�ֻʣ��һ����������߼򵥵�or������
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
        return 'X';         //����ǿմ����ǲ��Ϸ��ı��ʽ
    }
}

int main() {
    string src{"1 and (0 or (0 and 1 and 0)) or ((1 and 1) or 1) and 0"};
    cout << check(src) << endl;

    system("pause");
    return 0;
}