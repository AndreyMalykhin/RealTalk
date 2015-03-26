
#ifndef _REAL_TALK_PARSER_NODE_VISITOR_H_
#define _REAL_TALK_PARSER_NODE_VISITOR_H_

namespace real_talk {
namespace parser {

class ScopeNode;
class AndNode;
class ArrayAllocWithoutInitNode;
class ArrayAllocWithInitNode;
class AssignNode;
class BoolNode;
class BreakNode;
class CallNode;
class CharNode;
class ContinueNode;
class DivNode;
class DoubleNode;
class EqualNode;
class ExprStmtNode;
class FuncDefWithBodyNode;
class FuncDefWithoutBodyNode;
class GreaterNode;
class GreaterOrEqualNode;
class IfElseIfElseNode;
class IfElseIfNode;
class ImportNode;
class IntNode;
class LessNode;
class LessOrEqualNode;
class LongNode;
class MulNode;
class NegativeNode;
class NotEqualNode;
class NotNode;
class OrNode;
class PreDecNode;
class PreIncNode;
class PreTestLoopNode;
class ProgramNode;
class StringNode;
class SubscriptNode;
class SubNode;
class SumNode;
class VarDefWithoutInitNode;
class VarDefWithInitNode;
class IdNode;
class IntDataTypeNode;
class LongDataTypeNode;
class DoubleDataTypeNode;
class CharDataTypeNode;
class StringDataTypeNode;
class BoolDataTypeNode;
class VoidDataTypeNode;
class ArrayDataTypeNode;
class ReturnValueNode;
class ReturnWithoutValueNode;
class ArgDefNode;

class NodeVisitor {
 public:
  virtual ~NodeVisitor() {}
  virtual void VisitAnd(const AndNode &node) = 0;
  virtual void VisitArrayAllocWithoutInit(
      const ArrayAllocWithoutInitNode &node) = 0;
  virtual void VisitArrayAllocWithInit(const ArrayAllocWithInitNode &node) = 0;
  virtual void VisitAssign(const AssignNode &node) = 0;
  virtual void VisitBool(const BoolNode &node) = 0;
  virtual void VisitBreak(const BreakNode &node) = 0;
  virtual void VisitCall(const CallNode &node) = 0;
  virtual void VisitChar(const CharNode &node) = 0;
  virtual void VisitContinue(const ContinueNode &node) = 0;
  virtual void VisitDiv(const DivNode &node) = 0;
  virtual void VisitDouble(const DoubleNode &node) = 0;
  virtual void VisitEqual(const EqualNode &node) = 0;
  virtual void VisitExprStmt(const ExprStmtNode &node) = 0;
  virtual void VisitFuncDefWithBody(const FuncDefWithBodyNode &node) = 0;
  virtual void VisitFuncDefWithoutBody(const FuncDefWithoutBodyNode &node) = 0;
  virtual void VisitGreater(const GreaterNode &node) = 0;
  virtual void VisitGreaterOrEqual(const GreaterOrEqualNode &node) = 0;
  virtual void VisitIfElseIfElse(const IfElseIfElseNode &node) = 0;
  virtual void VisitIfElseIf(const IfElseIfNode &node) = 0;
  virtual void VisitImport(const ImportNode &node) = 0;
  virtual void VisitInt(const IntNode &node) = 0;
  virtual void VisitLess(const LessNode &node) = 0;
  virtual void VisitLessOrEqual(const LessOrEqualNode &node) = 0;
  virtual void VisitLong(const LongNode &node) = 0;
  virtual void VisitMul(const MulNode &node) = 0;
  virtual void VisitNegative(const NegativeNode &node) = 0;
  virtual void VisitNotEqual(const NotEqualNode &node) = 0;
  virtual void VisitNot(const NotNode &node) = 0;
  virtual void VisitOr(const OrNode &node) = 0;
  virtual void VisitPreDec(const PreDecNode &node) = 0;
  virtual void VisitPreInc(const PreIncNode &node) = 0;
  virtual void VisitPreTestLoop(const PreTestLoopNode &node) = 0;
  virtual void VisitProgram(const ProgramNode &node) = 0;
  virtual void VisitString(const StringNode &node) = 0;
  virtual void VisitSubscript(const SubscriptNode &node) = 0;
  virtual void VisitSub(const SubNode &node) = 0;
  virtual void VisitSum(const SumNode &node) = 0;
  virtual void VisitVarDefWithoutInit(const VarDefWithoutInitNode &node) = 0;
  virtual void VisitVarDefWithInit(const VarDefWithInitNode &node) = 0;
  virtual void VisitId(const IdNode &node) = 0;
  virtual void VisitIntDataType(const IntDataTypeNode &node) = 0;
  virtual void VisitLongDataType(const LongDataTypeNode &node) = 0;
  virtual void VisitCharDataType(const CharDataTypeNode &node) = 0;
  virtual void VisitStringDataType(const StringDataTypeNode &node) = 0;
  virtual void VisitDoubleDataType(const DoubleDataTypeNode &node) = 0;
  virtual void VisitBoolDataType(const BoolDataTypeNode &node) = 0;
  virtual void VisitVoidDataType(const VoidDataTypeNode &node) = 0;
  virtual void VisitArrayDataType(const ArrayDataTypeNode &node) = 0;
  virtual void VisitReturnValue(const ReturnValueNode &node) = 0;
  virtual void VisitReturnWithoutValue(const ReturnWithoutValueNode &node) = 0;
  virtual void VisitArgDef(const ArgDefNode &node) = 0;
  virtual void VisitScope(const ScopeNode &node) = 0;
};
}
}
#endif
