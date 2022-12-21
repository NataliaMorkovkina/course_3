#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class Tree
{
public:
    double numb = 0;
    char   oper = ' ';


    Tree* left = NULL;
    Tree* righ = NULL;

public:
    Tree(double _numb) : numb(_numb) {}
    Tree(char   _oper) : oper(_oper) {}
};

string whiteList = "+-*/01234567890().";
bool isCorrect(string& expr)
{
    int bracketsNumber = 0;
    for (char& elem : expr)
    {
        if (elem == '(')
            bracketsNumber++;
        else if (elem == ')')
        {
            bracketsNumber--;
            if (bracketsNumber < 0)
                return false;
        }

        bool flag = false;
        for (auto& wl : whiteList)
            if (wl == elem)
            {
                flag = true;
                break;
            }

        if (!flag)
            return false;
    }

    return !bracketsNumber;
}

void reverse(string& str)
{
    str = string(str.rbegin(), str.rend());
}

void removeOuterBrackets(string& expr)
{
    int leftOuterBracketsNumber = 0;
    for (int i = 0; i < expr.size(); i++)
    {
        if (expr[i] != '(')
            break;
        leftOuterBracketsNumber++;
    }

    int rightOuterBracketsNumber = 0;
    for (int i = expr.size() - 1; i >= 0; i--)
    {
        if (expr[i] != ')')
            break;
        rightOuterBracketsNumber++;
    }

    int maxOuterBracketsNumber = min(leftOuterBracketsNumber, rightOuterBracketsNumber);
    int outerBracketsNumber = 0;
    int bracketsNumber = 0;
    bool stopOutNumbers = false;
    for (int i = 0; i < expr.size() - maxOuterBracketsNumber; i++)
    {
        if (expr[i] == '(')
        {
            bracketsNumber++;
            if (!stopOutNumbers)
                outerBracketsNumber++;
        }
        else if (expr[i] == ')')
        {
            stopOutNumbers = true;
            bracketsNumber--;
            outerBracketsNumber = min(outerBracketsNumber, bracketsNumber);
        }
    }


    for (int i = 0; i < outerBracketsNumber; i++)
        expr.pop_back();
    reverse(expr);

    for (int i = 0; i < outerBracketsNumber; i++)
        expr.pop_back();
    reverse(expr);
}

bool isDouble(string& numb)
{
    bool haveDot = false;
    for (int i = (numb[0] == '+' || numb[0] == '-'); i < numb.size(); i++)
        if (('0' > numb[i] || numb[i] > '9'))
            if (numb[i] == '.')
                if (haveDot)
                    return false;
                else
                    haveDot = true;
            else
                return false;

    return numb[0] != '+' && numb[0] != '-';
}


Tree* infixToTree(string& expr)
{
    if (expr == "" || !isCorrect(expr))
        return new Tree(0.f);

    removeOuterBrackets(expr);

    if (expr == "" || !isCorrect(expr))
        return new Tree(0.f);

    if (isDouble(expr))
        return new Tree(stod(expr));

    int bracketsNumber = 0;
    for (int i = 0; i < expr.size(); i++)
    {
        if (expr[i] == '(')
            bracketsNumber++;

        else if (expr[i] == ')')
            bracketsNumber--;

        else if (expr[i] == '+' || expr[i] == '-')
            if (!bracketsNumber)
            {
                Tree* root = new Tree(expr[i]);

                string left = string(expr.begin(), expr.begin() + i);
                root->left = infixToTree(left);

                if (expr[i] == '-')
                    for (int j = i + 1; j < expr.size(); j++)
                        if (expr[j] == '(')
                            bracketsNumber++;
                        else if (expr[j] == ')')
                            bracketsNumber--;
                        else if (!bracketsNumber)
                            expr[j] = expr[j] == '+' ? '-' : expr[j] == '-' ? '+' : expr[j];

                string righ = string(expr.begin() + i + 1, expr.end());
                root->righ = infixToTree(righ);

                return root;
            }
    }

    for (int i = 0; i < expr.size(); i++)
    {
        if (expr[i] == '(')
            bracketsNumber++;

        else if (expr[i] == ')')
            bracketsNumber--;

        else if (expr[i] == '*' || expr[i] == '/')
            if (!bracketsNumber)
            {
                Tree* root = new Tree(expr[i]);

                string left = string(expr.begin(), expr.begin() + i);
                root->left = infixToTree(left);

                if (expr[i] == '/')
                    for (int j = i + 1; j < expr.size(); j++)
                        if (expr[j] == '(')
                            bracketsNumber++;
                        else if (expr[j] == ')')
                            bracketsNumber--;
                        else if (!bracketsNumber)
                            expr[j] = expr[j] == '*' ? '/' : expr[j] == '/' ? '*' : expr[j];

                string righ = string(expr.begin() + i + 1, expr.end());
                root->righ = infixToTree(righ);

                return root;
            }
    }

    return NULL;
}

Tree* suffixToTree(string& expr)
{
    if (expr == "")
        return NULL;

    if (isDouble(expr))
        return new Tree(stod(expr));

    Tree* root = new Tree(expr[0]);

    int operNum = 1;
    int numbNum = 0;
    int leftSpace = 2;
    for (int i = 2; i < expr.size(); i++)
    {
        if (expr[i] == ' ')
        {
            string str = string(expr.begin() + leftSpace, expr.begin() + i);
            if (isDouble(str))
            {
                numbNum++;
                if (operNum == numbNum)
                {
                    string left = string(expr.begin() + 2, expr.begin() + i);
                    root->left = suffixToTree(left);

                    string righ = string(expr.begin() + i + 1, expr.end());
                    root->righ = suffixToTree(righ);

                    return root;
                }
            }
            else
                operNum++;

            leftSpace = i + 1;
        }
    }

    return NULL;
}

Tree* postfixToTree(string& expr)
{
    if (expr == "")
        return NULL;

    if (isDouble(expr))
        return new Tree(stod(expr));

    Tree* root = new Tree(expr[expr.size() - 1]);

    int operNum = 1;
    int numbNum = 0;
    int righSpace = expr.size() - 2;
    for (int i = expr.size() - 3; i > -0; i--)
    {
        if (expr[i] == ' ')
        {
            string str = string(expr.begin() + i + 1, expr.begin() + righSpace);
            if (isDouble(str))
            {
                numbNum++;
                if (operNum == numbNum)
                {
                    string left = string(expr.begin(), expr.begin() + i);
                    root->left = postfixToTree(left);

                    string righ = string(expr.begin() + i + 1, expr.end() - 2);
                    root->righ = postfixToTree(righ);

                    return root;
                }
            }
            else
                operNum++;

            righSpace = i - 1;
        }
    }

    return NULL;
}


void printTree(vector<Tree*> t/*, vector<string> f*/)
{
    if (!t.size())
        return;
    vector<Tree*> next;
    // vector<string> fn;
    for (int i = 0; i < t.size(); i++)
    {
        if (t[i]->oper != ' ')
            cout /*<< f[i] */ << t[i]->oper << ' ';
        else
            cout /*<< f[i] */ << t[i]->numb << ' ';


        if (t[i]->left != NULL)
        {
            next.push_back(t[i]->left);
            // fn.push_back(to_string(i) + "l");
        }

        if (t[i]->righ != NULL)
        {
            next.push_back(t[i]->righ);
            // fn.push_back(to_string(i) + "r");
        }
    }
    cout << '\n';
    printTree(next/*, fn*/);
}


string treeToInfix(Tree* root)
{
    if (root == NULL)
        return "0";

    string res;

    if (root->oper != ' ')
    {
        if (root->left->oper == ' ' || root->left->oper == '*' || root->left->oper == '/')
            res += treeToInfix(root->left);
        else
        {
            res += "(";
            res += treeToInfix(root->left);
            res += ")";
        }

        res += root->oper;

        if (root->righ->oper == ' ' || root->left->oper == '*' || root->left->oper == '/')
            res += treeToInfix(root->righ);
        else
        {
            res += "(";
            res += treeToInfix(root->righ);
            res += ")";
        }

        return res;
    }

    if (root->numb >= 0)
        return to_string(root->numb);

    res += "(";
    res += to_string(root->numb);
    res += ")";
    return res;
}

string treeToSuffix(Tree* root)
{
    if (root == NULL)
        return "0";

    string res;

    if (root->oper != ' ')
    {
        res += root->oper;
        res += " ";
        res += treeToSuffix(root->left);
        res += treeToSuffix(root->righ);

        return res;
    }

    res += to_string(root->numb);
    res += " ";

    return res;
}

string treeToPostfix(Tree* root)
{
    if (root == NULL)
        return "0";

    string res;

    if (root->oper != ' ')
    {
        res += treeToPostfix(root->left);
        res += treeToPostfix(root->righ);
        res += root->oper;
        res += " ";

        return res;
    }

    res += to_string(root->numb);
    res += " ";

    return res;
}


double resultTree(Tree* root)
{
    if (root->oper == ' ')
        return root->numb;

    double left = resultTree(root->left);
    double righ = resultTree(root->righ);

    switch (root->oper)
    {
    case '+':
        return left + righ;

    case '-':
        return left - righ;

    case '*':
        return left * righ;

    case '/':
        return left / righ;
    }
    return 0;
}


int main()
{
    string s;
    cin >> s;

    Tree* root = infixToTree(s);

    printTree({ root });
    cout << "\n";

    string s1 = treeToInfix(root);
    string s2 = treeToSuffix(root); s2.pop_back();
    string s3 = treeToPostfix(root); s3.pop_back();

    cout << s1 << "\n\n";
    cout << s2 << "\n\n";
    cout << s3 << "\n\n";

    Tree* r1 = infixToTree(s1);
    Tree* r2 = suffixToTree(s2);
    Tree* r3 = postfixToTree(s3);

    printTree({ r1 });
    cout << "\n";
    printTree({ r2 });
    cout << "\n";
    printTree({ r3 });
    cout << "\n";

    double res1 = resultTree(r1);
    double res2 = resultTree(r2);
    double res3 = resultTree(r3);
    cout << "\n" << res1 << "\n" << res2 << "\n" << res3 << "\n";
}
