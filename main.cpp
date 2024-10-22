#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "token.h"
#include "astnode.h"

using namespace std;

vector<Token> lexing(const string &line);
ASTNode *parsing(vector<Token> &tok, size_t &idx);
void evaluation(ASTNode *Node, int &result);
int sol(string line);

vector<Token> lexing(const string &line)
{
    istringstream stream(line);
    vector<Token> tok;
    char ch;
    while (stream >> ch)
    {
        switch (ch)
        {
        case '(':
            tok.push_back({LPAR, "("});
            break;
        case ')':
            tok.push_back({RPAR, ")"});
            break;
        case '+':
        case '*':
        case '-':
        case '/': // TBD
            tok.push_back({OPR, string(1, ch)});
            break;

        default:
            if (isdigit(ch))
            {
                string num = string(1, ch);
                while (stream.peek() != EOF && isdigit(stream.peek()))
                {
                    num += stream.get();
                }
                tok.push_back({NUM, num});
            }
            else if (isalpha(ch))
            {
                string key = string(1, ch);
                while (stream.peek() != EOF && isalpha(stream.peek()))
                {
                    key += stream.get();
                }
                tok.push_back({KEY, key});
            }
            break;
        }
    }
    return tok;
}

ASTNode *parsing(vector<Token> &tok, size_t &idx)
{
    if (tok[idx].type == LPAR)
    {
        ++idx; //"("

        if (tok[idx].type == KEY && tok[idx].value == "simplify")
        {
            ++idx; //"SIMPLIFY"
            ASTNode *expr = parsing(tok, idx);
            ++idx; //")"

            return expr;
        }

        string opr = tok[idx].value;
        ++idx; //"OPR"

        if (tok[idx].type == NUM && tok[idx + 1].type == RPAR)
        {
            ASTNode *right = new NrNode(tok[idx].value);
            ++idx; //"NUM"
            ++idx; //")"
            return new OprNode(opr, nullptr, right);
        }

        ASTNode *left = parsing(tok, idx);
        ASTNode *right = parsing(tok, idx);
        ++idx; //")"

        return new OprNode(opr, left, right);
    }

    if (tok[idx].type == NUM)
    {
        ASTNode *number = new NrNode(tok[idx].value);
        ++idx; //"Num"

        return number;
    }

    return nullptr;
}

void evaluation(ASTNode *Node, int &result)
{
    if (Node != nullptr)
    {
        result = Node->evaluate();
        // cout << result << endl;
    }
    else
    {
        cout << "ERR: null ASTNode" << endl;
    }
}

int sol(string line)
{
    vector<Token> tok = lexing(line);
    size_t idx = 0;
    int result = 0;
    ASTNode *HeadNode = parsing(tok, idx);
    evaluation(HeadNode, result);
    delete HeadNode;

    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "USE: " << argv[0] << " <input.smt2> <output>" << endl;
        return 1;
    }

    ifstream smtfile(argv[1]);
    if (!smtfile.is_open())
    {
        cout << "ERR: unable to open input file: " << argv[1] << endl;
        return 1;
    }

    ofstream outfile(argv[2]);
    if (!outfile.is_open())
    {
        cout << "ERR: unable to open output file: " << argv[2] << endl;
        smtfile.close();
        return 1;
    }

    string line;
    while (getline(smtfile, line))
    {
        if (line.empty())
        {
            continue;
        }
        int result = sol(line);
        if (result < 0)
            outfile << "(- " << to_string(-result) + ")" << endl;
        else
            outfile << result << endl;
    }

    smtfile.close();
    outfile.close();

    return 0;
}