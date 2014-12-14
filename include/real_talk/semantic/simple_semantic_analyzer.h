
#ifndef _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_
#define _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include "real_talk/parser/node_visitor.h"
#include "real_talk/semantic/semantic_analysis_result.h"

namespace real_talk {
namespace parser {

class ExprNode;
}

namespace semantic {

class DataType;
class DataTypeAdaptor;

class SemanticErrorException: public std::runtime_error {
 public:
  explicit SemanticErrorException(std::unique_ptr<SemanticError> error);
  std::unique_ptr<SemanticError> GetError();
 private:
  std::unique_ptr<SemanticError> error_;
};

class SimpleSemanticAnalyzer: public real_talk::parser::NodeVisitor {
 public:
  explicit SimpleSemanticAnalyzer(
      const real_talk::parser::ProgramNode &program);
  SemanticAnalysisResult Analyze();
  virtual void VisitAnd(const real_talk::parser::AndNode &node) override;
  virtual void VisitArrayAllocWithoutInit(
      const real_talk::parser::ArrayAllocWithoutInitNode &node) override;
  virtual void VisitArrayAllocWithInit(
      const real_talk::parser::ArrayAllocWithInitNode &node) override;
  virtual void VisitAssign(const real_talk::parser::AssignNode &node) override;
  virtual void VisitBool(const real_talk::parser::BoolNode &node) override;
  virtual void VisitBreak(const real_talk::parser::BreakNode &node) override;
  virtual void VisitCall(const real_talk::parser::CallNode &node) override;
  virtual void VisitChar(const real_talk::parser::CharNode &node) override;
  virtual void VisitContinue(
      const real_talk::parser::ContinueNode &node) override;
  virtual void VisitDiv(const real_talk::parser::DivNode &node) override;
  virtual void VisitDouble(const real_talk::parser::DoubleNode &node) override;
  virtual void VisitEqual(const real_talk::parser::EqualNode &node) override;
  virtual void VisitExprStmt(
      const real_talk::parser::ExprStmtNode &node) override;
  virtual void VisitFuncDef(
      const real_talk::parser::FuncDefNode &node) override;
  virtual void VisitGreater(
      const real_talk::parser::GreaterNode &node) override;
  virtual void VisitGreaterOrEqual(
      const real_talk::parser::GreaterOrEqualNode &node) override;
  virtual void VisitIfElseIfElse(
      const real_talk::parser::IfElseIfElseNode &node) override;
  virtual void VisitIfElseIf(
      const real_talk::parser::IfElseIfNode &node) override;
  virtual void VisitImport(const real_talk::parser::ImportNode &node) override;
  virtual void VisitInt(const real_talk::parser::IntNode &node) override;
  virtual void VisitLess(const real_talk::parser::LessNode &node) override;
  virtual void VisitLessOrEqual(
      const real_talk::parser::LessOrEqualNode &node) override;
  virtual void VisitLong(const real_talk::parser::LongNode &node) override;
  virtual void VisitMul(const real_talk::parser::MulNode &node) override;
  virtual void VisitNegative(
      const real_talk::parser::NegativeNode &node) override;
  virtual void VisitNotEqual(
      const real_talk::parser::NotEqualNode &node) override;
  virtual void VisitNot(const real_talk::parser::NotNode &node) override;
  virtual void VisitOr(const real_talk::parser::OrNode &node) override;
  virtual void VisitPreDec(const real_talk::parser::PreDecNode &node) override;
  virtual void VisitPreInc(const real_talk::parser::PreIncNode &node) override;
  virtual void VisitPreTestLoop(
      const real_talk::parser::PreTestLoopNode &node) override;
  virtual void VisitProgram(
      const real_talk::parser::ProgramNode &node) override;
  virtual void VisitString(const real_talk::parser::StringNode &node) override;
  virtual void VisitSubscript(
      const real_talk::parser::SubscriptNode &node) override;
  virtual void VisitSub(const real_talk::parser::SubNode &node) override;
  virtual void VisitSum(const real_talk::parser::SumNode &node) override;
  virtual void VisitVarDefWithoutInit(
      const real_talk::parser::VarDefWithoutInitNode &node) override;
  virtual void VisitVarDefWithInit(
      const real_talk::parser::VarDefWithInitNode &node) override;
  virtual void VisitVarLoad(
      const real_talk::parser::VarLoadNode &node) override;
  virtual void VisitIntDataType(
      const real_talk::parser::IntDataTypeNode &node) override;
  virtual void VisitLongDataType(
      const real_talk::parser::LongDataTypeNode &node) override;
  virtual void VisitDoubleDataType(
      const real_talk::parser::DoubleDataTypeNode &node) override;
  virtual void VisitCharDataType(
      const real_talk::parser::CharDataTypeNode &node) override;
  virtual void VisitStringDataType(
      const real_talk::parser::StringDataTypeNode &node) override;
  virtual void VisitBoolDataType(
      const real_talk::parser::BoolDataTypeNode &node) override;
  virtual void VisitVoidDataType(
      const real_talk::parser::VoidDataTypeNode &node) override;
  virtual void VisitArrayDataType(
      const real_talk::parser::ArrayDataTypeNode &node) override;
  virtual void VisitBoundedArrayDataType(
      const real_talk::parser::BoundedArrayDataTypeNode &node) override;
  virtual void VisitReturnValue(
      const real_talk::parser::ReturnValueNode &node) override;
  virtual void VisitReturn(
      const real_talk::parser::ReturnNode &node) override;

 private:
  typedef std::unordered_map<std::string,
                             std::unique_ptr<DataType> > IdDataTypes;

  struct Scope {
    IdDataTypes id_data_types;
  };

  std::unique_ptr<DataType> CreateDataType(
      const real_talk::parser::DataTypeNode &data_type_node);
  std::unique_ptr<DataType> GetDataTypeOfId(
      const real_talk::parser::VarLoadNode &var_node);

  const real_talk::parser::ProgramNode &program_;
  std::vector< std::unique_ptr<SemanticError> > errors_;
  SemanticAnalysisResult::DefAnalyzes def_analyzes_;
  SemanticAnalysisResult::ExprAnalyzes expr_analyzes_;
  SemanticAnalysisResult::LitAnalyzes lit_analyzes_;
  SemanticAnalysisResult::IdAnalyzes id_analyzes_;
  std::vector<Scope> scopes_stack_;
  std::unique_ptr<DataType> current_data_type_;
};
}
}
#endif
