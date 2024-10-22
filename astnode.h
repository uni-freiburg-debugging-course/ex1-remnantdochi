#pragma once
#include <string>
#include <iostream>

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual int evaluate() const = 0;
    virtual void print() const = 0; // for debugging
};

class OprNode : public ASTNode
{
    std::string op; // because tok.val is string
    ASTNode *left;
    ASTNode *right;

public:
    OprNode(const std::string &opr, ASTNode *l, ASTNode *r)
        : op(opr), left(l), right(r) {}

    int evaluate() const override
    {
        if (op == "-" && left == nullptr)
        {
            return -right->evaluate();
        }
        int lval = left->evaluate();
        int rval = right->evaluate();

        if (op == "+")
            return lval + rval;
        if (op == "-")
            return lval - rval;
        if (op == "*")
            return lval * rval;
        if (op == "/")
            return lval / rval;
        return 0;
    }

    void print() const override
    {
        std::cout << "(" << op << " ";
        left->print();
        std::cout << " ";
        right->print();
        std::cout << ")";
    }

    ~OprNode()
    {
        delete left;
        delete right;
    }
};

class NrNode : public ASTNode
{
    std::string val;

public:
    NrNode(const std::string &v) : val(v) {}
    int evaluate() const override { return std::stoi(val); }

    void print() const override
    {
        std::cout << val;
    }
};