
#include <string>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/func_def_with_body_node.h"
#include "real_talk/parser/func_def_without_body_node.h"
#include "real_talk/parser/unary_expr_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/if_node.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/array_alloc_without_init_node.h"
#include "real_talk/parser/primitive_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/subscript_node.h"
#include "real_talk/semantic/semantic_problems.h"
#include "real_talk/semantic/data_type.h"
#include "real_talk/compiler/simple_msg_printer.h"

using std::ostream;
using std::string;
using std::unique_ptr;
using boost::filesystem::path;
using real_talk::lexer::TokenInfo;
using real_talk::parser::ProgramNode;
using real_talk::semantic::ArrayAllocWithTooManyDimensionsError;
using real_talk::semantic::ArrayTypeWithTooManyDimensionsError;
using real_talk::semantic::DoubleWithOutOfRangeValueError;
using real_talk::semantic::LongWithOutOfRangeValueError;
using real_talk::semantic::IntWithOutOfRangeValueError;
using real_talk::semantic::CharWithMultipleCharsError;
using real_talk::semantic::CharWithEmptyHexValueError;
using real_talk::semantic::CharWithOutOfRangeHexValueError;
using real_talk::semantic::StringWithOutOfRangeHexValueError;
using real_talk::semantic::StringWithEmptyHexValueError;
using real_talk::semantic::UnaryExprWithUnsupportedTypeError;
using real_talk::semantic::AssignWithRightValueAssigneeError;
using real_talk::semantic::UnaryExprWithRightValueOperandError;
using real_talk::semantic::IdWithoutDefError;
using real_talk::semantic::SubscriptWithUnsupportedIndexTypeError;
using real_talk::semantic::SubscriptWithUnsupportedOperandTypeError;
using real_talk::semantic::ArrayAllocWithIncompatibleValueTypeError;
using real_talk::semantic::ArrayAllocWithUnsupportedElementTypeError;
using real_talk::semantic::ArrayAllocWithUnsupportedSizeTypeError;
using real_talk::semantic::IfWithIncompatibleTypeError;
using real_talk::semantic::BreakNotWithinLoopError;
using real_talk::semantic::ContinueNotWithinLoopError;
using real_talk::semantic::PreTestLoopWithIncompatibleTypeError;
using real_talk::semantic::ImportIsNotFirstStmtError;
using real_talk::semantic::VarDefWithIncompatibleValueTypeError;
using real_talk::semantic::ReturnWithoutValueError;
using real_talk::semantic::ReturnNotWithinFuncError;
using real_talk::semantic::ReturnWithIncompatibleTypeError;
using real_talk::semantic::BinaryExprWithUnsupportedTypesError;
using real_talk::semantic::CallWithIncompatibleArgTypeError;
using real_talk::semantic::DefWithUnsupportedTypeError;
using real_talk::semantic::DuplicateDefError;
using real_talk::semantic::FuncDefWithoutBodyNotNativeError;
using real_talk::semantic::FuncDefWithBodyIsNativeError;
using real_talk::semantic::FuncDefWithinNonGlobalScopeError;
using real_talk::semantic::FuncDefWithoutReturnValueError;
using real_talk::semantic::CallWithUnsupportedTypeError;
using real_talk::semantic::CallWithInvalidArgsCountError;
using real_talk::semantic::SemanticProblem;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::DataType;

namespace real_talk {
namespace compiler {

SimpleMsgPrinter::SimpleMsgPrinter(ostream *stream): stream_(stream) {
  assert(stream);
}

void SimpleMsgPrinter::PrintSemanticProblems(
    const SemanticAnalysis::ProgramProblems &program_problems,
    const ProgramFilePaths &program_file_paths) const {
  for (const auto &program_problems_pair: program_problems) {
    const ProgramNode *program = program_problems_pair.first;
    const auto program_file_paths_it = program_file_paths.find(program);
    assert(program_file_paths_it != program_file_paths.cend());
    const path &file_path = program_file_paths_it->second;
    const SemanticAnalysis::Problems &problems = program_problems_pair.second;

    for (const unique_ptr<SemanticProblem> &problem: problems) {
      PrintSemanticProblem(*problem, file_path);
    }
  }
}

void SimpleMsgPrinter::PrintSemanticProblem(
    const SemanticProblem &problem, const path &file_path) const {
  current_file_path_ = &file_path;
  problem.Accept(this);
}

void SimpleMsgPrinter::PrintTokenError(
    const TokenInfo &token, const path &file_path, const string &error) const {
  PrintFileError(file_path, token.GetLine(), token.GetColumn())
      << error << " \"" << token.GetValue() << "\"\n";
}

void SimpleMsgPrinter::PrintUnexpectedTokenError(
    const TokenInfo &token, const path &file_path) const {
  PrintFileError(file_path, token.GetLine(), token.GetColumn())
      << "unexpected token \"" << token.GetValue() << "\"\n";
}

void SimpleMsgPrinter::PrintUnexpectedCharError(
    char c, uint32_t line, uint32_t column, const path &file_path) const {
  PrintFileError(file_path, line, column)
      << "unexpected character \"" << c << "\"\n";
}

void SimpleMsgPrinter::PrintError(const string &error) const {
  DoPrintError() << error << '\n';
}

void SimpleMsgPrinter::PrintHelp(const string &help) const {
  *stream_ << help << '\n';
}

void SimpleMsgPrinter::VisitArrayAllocWithTooManyDimensionsError(
    const ArrayAllocWithTooManyDimensionsError &error) const {
  const TokenInfo &token = error.GetAlloc().GetStartToken();
  PrintArrayWithTooManyDimensionsError(token, error.GetMaxCount());
}

void SimpleMsgPrinter::VisitArrayTypeWithTooManyDimensionsError(
    const ArrayTypeWithTooManyDimensionsError &error) const {
  const TokenInfo &token = error.GetArrayType().GetStartToken();
  PrintArrayWithTooManyDimensionsError(token, error.GetMaxCount());
}

void SimpleMsgPrinter::VisitDoubleWithOutOfRangeValueError(
    const DoubleWithOutOfRangeValueError &error) const {
  PrintOutOfRangeValueError(error.GetDouble().GetStartToken());
}

void SimpleMsgPrinter::VisitLongWithOutOfRangeValueError(
    const LongWithOutOfRangeValueError &error) const {
  PrintOutOfRangeValueError(error.GetLong().GetStartToken());
}

void SimpleMsgPrinter::VisitIntWithOutOfRangeValueError(
    const IntWithOutOfRangeValueError &error) const {
  PrintOutOfRangeValueError(error.GetInt().GetStartToken());
}

void SimpleMsgPrinter::VisitCharWithMultipleCharsError(
    const CharWithMultipleCharsError &error) const {
  const TokenInfo &token = error.GetChar().GetStartToken();
  PrintCurrentFileError(token) << "multiple characters\n";
}

void SimpleMsgPrinter::VisitCharWithEmptyHexValueError(
    const CharWithEmptyHexValueError &error) const {
  const TokenInfo &token = error.GetChar().GetStartToken();
  PrintCurrentFileError(token)
      << "empty hex value\n";
}

void SimpleMsgPrinter::VisitCharWithOutOfRangeHexValueError(
    const CharWithOutOfRangeHexValueError &error) const {
  PrintOutOfRangeValueError(error.GetChar().GetStartToken());
}

void SimpleMsgPrinter::VisitStringWithOutOfRangeHexValueError(
    const StringWithOutOfRangeHexValueError &error) const {
  const TokenInfo &token = error.GetString().GetStartToken();
  PrintCurrentFileError(token) << "out of range hex value\n";
}

void SimpleMsgPrinter::VisitStringWithEmptyHexValueError(
    const StringWithEmptyHexValueError &error) const {
  const TokenInfo &token = error.GetString().GetStartToken();
  PrintCurrentFileError(token) << "empty hex value\n";
}

void SimpleMsgPrinter::VisitAssignWithRightValueAssigneeError(
    const AssignWithRightValueAssigneeError &error) const {
  const TokenInfo &token = error.GetAssign().GetOpToken();
  PrintCurrentFileError(token) << "can't assign to rvalue\n";
}

void SimpleMsgPrinter::VisitUnaryExprWithRightValueOperandError(
    const UnaryExprWithRightValueOperandError &error) const {
  const TokenInfo &token = error.GetExpr().GetOperand()->GetStartToken();
  PrintCurrentFileError(token)
      << "rvalue is not supported by operator \""
      << error.GetExpr().GetOpToken().GetValue() << "\"\n";
}

void SimpleMsgPrinter::VisitIdWithoutDefError(
    const IdWithoutDefError &error) const {
  const TokenInfo &token = error.GetId().GetStartToken();
  PrintCurrentFileError(token) << "undefined identifier\n";
}

void SimpleMsgPrinter::VisitSubscriptWithUnsupportedIndexTypeError(
    const SubscriptWithUnsupportedIndexTypeError &error) const {
  const TokenInfo &token = error.GetSubscript().GetIndex()->GetStartToken();
  PrintCurrentFileError(token)
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported as index of operator \""
      << error.GetSubscript().GetOpStartToken().GetValue()
      << error.GetSubscript().GetOpEndToken().GetValue() << "\"\n";
}

void SimpleMsgPrinter::VisitSubscriptWithUnsupportedOperandTypeError(
    const SubscriptWithUnsupportedOperandTypeError &error) const {
  const TokenInfo &token = error.GetSubscript().GetOperand()->GetStartToken();
  PrintCurrentFileError(token)
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported as operand of operator \""
      << error.GetSubscript().GetOpStartToken().GetValue()
      << error.GetSubscript().GetOpEndToken().GetValue() << "\"\n";
}

void SimpleMsgPrinter::VisitArrayAllocWithIncompatibleValueTypeError(
    const ArrayAllocWithIncompatibleValueTypeError &error) const {
  const TokenInfo &token =
      error.GetAlloc().GetValues()[error.GetValueIndex()]->GetStartToken();
  PrintIncompatibleDataTypeError(
      token, error.GetDestDataType(), error.GetSrcDataType());
}

void SimpleMsgPrinter::VisitArrayAllocWithUnsupportedElementTypeError(
    const ArrayAllocWithUnsupportedElementTypeError &error) const {
  const TokenInfo &token = error.GetAlloc().GetDataType()->GetStartToken();
  PrintCurrentFileError(token)
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported as element of array\n";
}

void SimpleMsgPrinter::VisitArrayAllocWithUnsupportedSizeTypeError(
    const ArrayAllocWithUnsupportedSizeTypeError &error) const {
  const TokenInfo &token = error.GetSize().GetValue()->GetStartToken();
  PrintCurrentFileError(token)
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported as size of array\n";
}

void SimpleMsgPrinter::VisitIfWithIncompatibleTypeError(
    const IfWithIncompatibleTypeError &error) const {
  const TokenInfo &token = error.GetIf().GetCond()->GetStartToken();
  PrintIncompatibleDataTypeError(
      token, error.GetDestDataType(), error.GetSrcDataType());
}

void SimpleMsgPrinter::VisitBreakNotWithinLoopError(
    const BreakNotWithinLoopError &error) const {
  const TokenInfo &token = error.GetBreak().GetStartToken();
  PrintCurrentFileError(token)
      << '"' << token.GetValue() << "\" is not within loop\n";
}

void SimpleMsgPrinter::VisitContinueNotWithinLoopError(
    const ContinueNotWithinLoopError &error) const {
  const TokenInfo &token = error.GetContinue().GetStartToken();
  PrintCurrentFileError(token)
      << '"' << token.GetValue() << "\" is not within loop\n";
}

void SimpleMsgPrinter::VisitPreTestLoopWithIncompatibleTypeError(
    const PreTestLoopWithIncompatibleTypeError &error) const {
  const TokenInfo &token = error.GetLoop().GetCond()->GetStartToken();
  PrintIncompatibleDataTypeError(
      token, error.GetDestDataType(), error.GetSrcDataType());
}

void SimpleMsgPrinter::VisitImportIsNotFirstStmtError(
    const ImportIsNotFirstStmtError &error) const {
  const TokenInfo &token = error.GetImport().GetStartToken();
  PrintCurrentFileError(token)
      << '"' << token.GetValue() << "\" statements must be before any other\n";
}

void SimpleMsgPrinter::VisitVarDefWithIncompatibleValueTypeError(
    const VarDefWithIncompatibleValueTypeError &error) const {
  const TokenInfo &token = error.GetVarDef().GetValue()->GetStartToken();
  PrintIncompatibleDataTypeError(
      token, error.GetDestDataType(), error.GetSrcDataType());
}

void SimpleMsgPrinter::VisitReturnWithoutValueError(
    const ReturnWithoutValueError &error) const {
  const TokenInfo &token = error.GetReturn().GetStartToken();
  PrintCurrentFileError(token)
      << '"' << token.GetValue() << "\" is without value\n";
}

void SimpleMsgPrinter::VisitReturnNotWithinFuncError(
    const ReturnNotWithinFuncError &error) const {
  const TokenInfo &token = error.GetReturn().GetStartToken();
  PrintCurrentFileError(token)
      << '"' << token.GetValue() << "\" is not within function\n";
}

void SimpleMsgPrinter::VisitReturnWithIncompatibleTypeError(
    const ReturnWithIncompatibleTypeError &error) const {
  const TokenInfo &token = error.GetReturn().GetValue()->GetStartToken();
  PrintIncompatibleDataTypeError(
      token, error.GetDestDataType(), error.GetSrcDataType());
}

void SimpleMsgPrinter::VisitCallWithIncompatibleArgTypeError(
    const CallWithIncompatibleArgTypeError &error) const {
  const TokenInfo &token =
      error.GetCall().GetArgs()[error.GetArgIndex()]->GetStartToken();
  PrintIncompatibleDataTypeError(
      token, error.GetDestDataType(), error.GetSrcDataType());
}

void SimpleMsgPrinter::VisitBinaryExprWithUnsupportedTypesError(
    const BinaryExprWithUnsupportedTypesError &error) const {
  const TokenInfo &token = error.GetExpr().GetOpToken();
  PrintCurrentFileError(token)
      << '"' << error.GetLeftOperandDataType().GetName() << "\" and \""
      << error.GetRightOperandDataType().GetName()
      << "\" data types are not supported by operator \"" << token.GetValue()
      << "\"\n";
}

void SimpleMsgPrinter::VisitUnaryExprWithUnsupportedTypeError(
    const UnaryExprWithUnsupportedTypeError &error) const {
  const TokenInfo &token = error.GetExpr().GetOperand()->GetStartToken();
  PrintCurrentFileError(token)
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported by operator \""
      << error.GetExpr().GetOpToken().GetValue() << "\"\n";
}

void SimpleMsgPrinter::VisitDefWithUnsupportedTypeError(
    const DefWithUnsupportedTypeError &error) const {
  const TokenInfo &token = error.GetDef().GetDataType()->GetStartToken();
  PrintCurrentFileError(token)
      << "definition doesn't support \"" << error.GetDataType().GetName()
      << "\" data type\n";
}

void SimpleMsgPrinter::VisitDuplicateDefError(
    const DuplicateDefError &error) const {
  const TokenInfo &token = error.GetDef().GetNameToken();
  PrintCurrentFileError(token) << "duplicate definition\n";
}

void SimpleMsgPrinter::VisitFuncDefWithoutBodyNotNativeError(
    const FuncDefWithoutBodyNotNativeError &error) const {
  const TokenInfo &token = error.GetDef().GetNameToken();
  PrintCurrentFileError(token)
      << "non-native function definition without body\n";
}

void SimpleMsgPrinter::VisitFuncDefWithBodyIsNativeError(
    const FuncDefWithBodyIsNativeError &error) const {
  const TokenInfo &token = error.GetDef().GetNameToken();
  PrintCurrentFileError(token) << "native function definition with body\n";
}

void SimpleMsgPrinter::VisitFuncDefWithinNonGlobalScopeError(
    const FuncDefWithinNonGlobalScopeError &error) const {
  const TokenInfo &token = error.GetDef().GetNameToken();
  PrintCurrentFileError(token)
      << "function definition within non-global scope\n";
}

void SimpleMsgPrinter::VisitFuncDefWithoutReturnValueError(
    const FuncDefWithoutReturnValueError &error) const {
  const TokenInfo &token = error.GetDef().GetNameToken();
  PrintCurrentFileError(token)
      << "function doesn't always returns value\n";
}

void SimpleMsgPrinter::VisitCallWithUnsupportedTypeError(
    const CallWithUnsupportedTypeError &error) const {
  const TokenInfo &token = error.GetCall().GetOperand()->GetStartToken();
  PrintCurrentFileError(token)
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported by operator \""
      << error.GetCall().GetOpStartToken().GetValue()
      << error.GetCall().GetOpEndToken().GetValue() << "\"\n";
}

void SimpleMsgPrinter::VisitCallWithInvalidArgsCountError(
    const CallWithInvalidArgsCountError &error) const {
  const TokenInfo &token = error.GetCall().GetOpStartToken();
  PrintCurrentFileError(token)
      << "expected " << error.GetExpectedCount() << " argument(s), but got "
      << error.GetActualCount() << "\n";
}

void SimpleMsgPrinter::PrintOutOfRangeValueError(const TokenInfo &token) const {
  PrintCurrentFileError(token) << "out of range value\n";
}

void SimpleMsgPrinter::PrintIncompatibleDataTypeError(
    const TokenInfo &token,
    const DataType &dest_data_type,
    const DataType &src_data_type) const {
  PrintCurrentFileError(token)
      << "expected \"" << dest_data_type.GetName() << "\" data type, but got \""
      << src_data_type.GetName() << "\"\n";
}

void SimpleMsgPrinter::PrintArrayWithTooManyDimensionsError(
    const TokenInfo &token, size_t max_count) const {
  PrintCurrentFileError(token)
      << "array can't have more than " << max_count << " dimensions\n";
}

ostream &SimpleMsgPrinter::PrintCurrentFileError(const TokenInfo &token) const {
  return PrintFileError(
      *current_file_path_, token.GetLine(), token.GetColumn());
}

ostream &SimpleMsgPrinter::PrintFileError(
    const path &file_path, uint32_t line, uint32_t column) const {
  return DoPrintError()
      << file_path.string() << ':' << line << ':' << column << ": ";
}

ostream &SimpleMsgPrinter::DoPrintError() const {
  return *stream_ << "[Error] ";
}
}
}
