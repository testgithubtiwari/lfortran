#include <nlohmann/json.hpp>

#include <lfortran/ast_to_json.h>

using json = nlohmann::json;
using LFortran::AST::expr_t;
using LFortran::AST::Name_t;
using LFortran::AST::Num_t;
using LFortran::AST::BinOp_t;
using LFortran::AST::operatorType;
using LFortran::AST::BaseWalkVisitor;


namespace LFortran {

std::string op2str(const operatorType type)
{
    switch (type) {
        case (operatorType::Add) : return "Add";
        case (operatorType::Sub) : return "Sub";
        case (operatorType::Mul) : return "Mul";
        case (operatorType::Div) : return "Div";
        case (operatorType::Pow) : return "Pow";
    }
    throw std::runtime_error("Unknown type");
}


class JSONVisitor : public BaseWalkVisitor<JSONVisitor>
{
    json j;
public:
    void visit_BinOp(const BinOp_t &x) {
        this->visit_expr(*x.m_left);
        json left = j;
        this->visit_expr(*x.m_right);
        json right = j;
        j = {
            {"type", "BinOp"},
            {"op", op2str(x.m_op)},
            {"left", left },
            {"right", right }
        };
    }
    void visit_Name(const Name_t &x) {
        j = {
            {"type", "Name"},
            {"id", x.m_id }
        };
    }
    void visit_Num(const Num_t &x) {
        j = {
            {"type", "Num"},
            {"n", x.m_n }
        };
    }
    std::string get_str() {
        return j.dump(4);
    }
};

std::string ast_to_json(LFortran::AST::expr_t &ast) {
    JSONVisitor v;
    v.visit_expr(ast);
    return v.get_str();
}

}
