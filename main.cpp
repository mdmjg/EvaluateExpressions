#include <iostream>
#include <stack>
#include <map>
#include <vector>

using namespace std;

map<char, int> precedence = {{'-', 1}, {'+', 1}, {'*', 2}, {'/', 2}, {'(', 0}, {')', 0}};

int processOperatorsandValues(stack<int> &numbers, stack<char> &operators)
{
  if (numbers.size() < 2)
  {
    throw "Invalid operation";
  }

  char currentOperator = operators.top();
  operators.pop();

  int value2 = numbers.top();
  numbers.pop();

  int value1 = numbers.top();
  numbers.pop();

  if (currentOperator == '*')
  {
    return value1 * value2;
  }
  else if (currentOperator == '/')
  {
    return value1 / value2;
  }
  else if (currentOperator == '+')
  {
    return value1 + value2;
  }
  return value1 - value2;
}

int evaluateExpression(string expression)
{
  stack<int> numbers;
  stack<char> operators;
  int valueFlag = 0; // determines if our last seen char was a number or not

  for (int i = 0; i < expression.length(); i++)
  {
    char current = expression[i];
    if (current == ' ')
    {
      continue;
    }

    if (isdigit(current))
    {
      if (valueFlag == 1) // detects if literal is too large
      {
        throw "Literal too large";
      }
      valueFlag = 1;
      numbers.push(current - 48);
    }
    else if (current == ')')
    {
      valueFlag = 0;

      while (!operators.empty() && operators.top() != '(')
      {
        try
        {
          int result = processOperatorsandValues(numbers, operators);
          numbers.push(result);
        }
        catch (char const *e)
        {
          throw e;
        }
      }
      if (!operators.empty())
      {
        operators.pop();
      }
    }
    else if (current == '(')
    {
      valueFlag = 0;
      operators.push(current);
    }
    else if (precedence.find(current) != precedence.end())
    {
      valueFlag = 0;
      while (!operators.empty() && precedence[operators.top()] >= precedence[current])
      {
        try
        {
          int result = processOperatorsandValues(numbers, operators);
          numbers.push(result);
        }
        catch (char const *e)
        {
          throw e;
        }
      }
      operators.push(current);
    }

    else
    {
      throw "Invalid operation!";
    }
  }

  while (!operators.empty())
  {
    try
    {
      int result = processOperatorsandValues(numbers, operators);
      numbers.push(result);
    }
    catch (char const *e)
    {
      throw e;
    }
  }

  if (numbers.size() > 1)
  {
    throw "Invalid Operation";
  }
  return numbers.top();
}

void testEvaluation()
{
  vector<string> input = {"1 + 2", "-1 + 3", "-1", "3(2*4)", "(4 + 5 * (7 - 3)) - 2", "4+5+7/2", "22-3"};

  for (int i = 0; i < input.size(); i++)
  {
    cout << input[i] << " correctly produces ";
    try
    {
      cout << evaluateExpression(input[i]) << endl;
    }
    catch (char const *e)
    {
      cerr << e << endl;
    }
  }
}

int main()
{
  testEvaluation();
  string expression;

  cout << "Input expression: " << endl;
  while (getline(cin, expression))
  {
    try
    {
      cout << evaluateExpression(expression) << endl;
    }
    catch (char const *e)
    {
      cerr << e << endl;
    }
  }
}