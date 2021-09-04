#include <iostream>
#include <string>
#include <vector>

using namespace std;
enum NodeType
{
  MULT = '*',
  DIV = '/',
  ADD = '+',
  SUB = '-',
  INT = 'I',
  LPARENTHESIS = '(',
  RPARENTHESIS = ')',
  DONE = '0'
};
class Node
{
public:
  NodeType type;
  int value;
  Node *left;
  Node *right;
  Node(NodeType type)
  {
    this->type = type;
  }
  Node(NodeType type, int v)
  {
    this->type = type;
    this->value = v;
  }
};
class Tokenizer
{
public:
  string expression;
  int current;
  char currentChar;

  Tokenizer(string expression)
  {
    this->expression = expression;
    this->current = 0;
    currentChar = expression[0];
  }

  void getNextChar()
  {
    if (current < expression.size() - 1)
    {
      current++;
      currentChar = expression[current];
    }
    else
    {
      currentChar = '\0';
    }
  }

  Node *getNextToken()
  {
    Node *node;
    while (isspace(currentChar))
    {
      getNextChar();
    }

    if (isdigit(currentChar))
    {
      char token = currentChar;
      getNextChar();

      // check that digit is single digit
      if (isdigit(currentChar))
      {
        throw "Integer too large";
      }
      if (currentChar == '(')
      {
        throw "Invalid Input"; //ex: 3(2+4)
      }

      int value = token - '0';
      node = new Node(INT, value);
    }

    else if (currentChar == '+')
    {
      node = new Node(ADD);
      getNextChar();
    }

    else if (currentChar == '-')
    {
      node = new Node(SUB);
      getNextChar();
    }
    else if (currentChar == '*')
    {
      node = new Node(MULT);
      getNextChar();
    }
    else if (currentChar == '/')
    {
      node = new Node(DIV);
      getNextChar();
    }
    else if (currentChar == '(')
    {
      node = new Node(LPARENTHESIS);
      getNextChar();
    }
    else if (currentChar == ')')
    {
      node = new Node(RPARENTHESIS);
      getNextChar();
    }
    else if (currentChar == '\0')
    {
      node = new Node(DONE);
    }
    else
    {
      throw "Invalid operand";
    }

    return node;
  }
};

class Parser
{
public:
  Tokenizer *tokenizer;
  Node *currentNode;
  Node *root;

  Parser(Tokenizer *tokenizer)
  {
    this->tokenizer = tokenizer;
    currentNode = (*this->tokenizer).getNextToken();
    root = currentNode;
  }

  Node *buildSubTree(Node *left, Node *operation, Node *right)
  {
    operation->left = left;
    operation->right = right;
    return operation;
  }

  void move()
  {
    currentNode = (*tokenizer).getNextToken();
  }

  Node *readMultiplicationandDivision()
  {
    Node *node = readIntOrParenthesis();

    while (currentNode->type == MULT || currentNode->type == DIV)
    {
      Node *current = currentNode;
      move();
      node = buildSubTree(node, current, readIntOrParenthesis());
    }
    return node;
  }

  Node *readIntOrParenthesis()
  {
    Node *node = currentNode;
    if (node->type == INT)
    {
      move();
      return node;
    }

    else if (node->type == LPARENTHESIS)
    {
      move();
      node = readAdditionandSubtraction();
      move();
      return node;
    }
    else
    {
      throw "Invalid Input";
    }
  }

  Node *readAdditionandSubtraction()
  {
    Node *node = readMultiplicationandDivision();

    while (currentNode->type == ADD || currentNode->type == SUB)
    {
      Node *current = currentNode;
      move();
      node = buildSubTree(node, current, readMultiplicationandDivision());
    }
    return node;
  }
};

class Calculator
{
public:
  int result;
  Parser *parser;
  Calculator(Parser *parser)
  {
    this->parser = parser;
  }

  int visit(Node *node)
  {
    if (node->type == ADD)
    {
      int a = visit(node->left);
      int b = visit(node->right);
      result = a + b;
    }
    else if (node->type == SUB)
    {
      int a = visit(node->left);
      int b = visit(node->right);
      result = (visit(node->left) - visit(node->right));
    }
    else if (node->type == MULT)
    {
      int a = visit(node->left);
      int b = visit(node->right);
      result = (a * b);
    }
    else if (node->type == DIV)
    {
      result = (visit(node->left) / visit(node->right));
    }
    else if (node->type == INT)
    {
      result = node->value;
    }
    else if (node->type == DONE)
    {
      result = 0;
    }
    else
    {
      throw "Invalid Input ";
    }

    return result;
  }

  int traverse()
  {
    return visit(parser->readAdditionandSubtraction());
  }
};

int evaluateExpression(string expression)
{
  try
  {
    Tokenizer *token = new Tokenizer(expression);
    Parser *parser = new Parser(token);

    Calculator *calculator = new Calculator(parser);
    return (*calculator).traverse();
  }
  catch (char const *e)
  {
    throw "Invalid Input";
  }
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