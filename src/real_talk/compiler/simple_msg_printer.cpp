
#include <string>
#include "real_talk/lexer/token_info.h"
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
using boost::filesystem::path;
using real_talk::lexer::TokenInfo;
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
using real_talk::semantic::CallWithNonFuncError;
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
    const SemanticAnalysis::ProgramProblems &problems,
    const ProgramFilePaths &program_file_paths) const {
  assert(false);
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
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "multiple characters\n";
}

void SimpleMsgPrinter::VisitCharWithEmptyHexValueError(
    const CharWithEmptyHexValueError &error) const {
  const TokenInfo &token = error.GetChar().GetStartToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "empty hex value\n";
}

void SimpleMsgPrinter::VisitCharWithOutOfRangeHexValueError(
    const CharWithOutOfRangeHexValueError &error) const {
  PrintOutOfRangeValueError(error.GetChar().GetStartToken());
}

void SimpleMsgPrinter::VisitStringWithOutOfRangeHexValueError(
    const StringWithOutOfRangeHexValueError &error) const {
  const TokenInfo &token = error.GetString().GetStartToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "out of range hex value\n";
}

void SimpleMsgPrinter::VisitStringWithEmptyHexValueError(
    const StringWithEmptyHexValueError &error) const {
  const TokenInfo &token = error.GetString().GetStartToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "empty hex value\n";
}

void SimpleMsgPrinter::VisitAssignWithRightValueAssigneeError(
    const AssignWithRightValueAssigneeError &error) const {
  const TokenInfo &token = error.GetAssign().GetOpToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "can't assign to rvalue\n";
}

void SimpleMsgPrinter::VisitIdWithoutDefError(
    const IdWithoutDefError &error) const {
  const TokenInfo &token = error.GetId().GetStartToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "undefined identifier\n";
}

void SimpleMsgPrinter::VisitSubscriptWithUnsupportedIndexTypeError(
    const SubscriptWithUnsupportedIndexTypeError &error) const {
  const TokenInfo &token = error.GetSubscript().GetIndex()->GetStartToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported as index of operator \"[]\"\n";
}

void SimpleMsgPrinter::VisitSubscriptWithUnsupportedOperandTypeError(
    const SubscriptWithUnsupportedOperandTypeError &error) const {
  const TokenInfo &token = error.GetSubscript().GetOperand()->GetStartToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported as operand of operator \"[]\"\n";
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
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << '"' << error.GetDataType().GetName()
      << "\" data type is not supported as element of array\n";
}

void SimpleMsgPrinter::VisitArrayAllocWithUnsupportedSizeTypeError(
    const ArrayAllocWithUnsupportedSizeTypeError &error) const {
  const TokenInfo &token = error.GetSize().GetValue()->GetStartToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
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
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << '"' << token.GetValue() << "\" is not within a loop\n";
}

void SimpleMsgPrinter::VisitContinueNotWithinLoopError(
    const ContinueNotWithinLoopError &error) const {
  const TokenInfo &token = error.GetContinue().GetStartToken();
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << '"' << token.GetValue() << "\" is not within a loop\n";
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
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << '"' << token.GetValue() << "\" statements must be before any other\n";
}

void SimpleMsgPrinter::VisitVarDefWithIncompatibleValueTypeError(
    const VarDefWithIncompatibleValueTypeError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitReturnWithoutValueError(
    const ReturnWithoutValueError&) const {assert(false);}

void SimpleMsgPrinter::VisitReturnNotWithinFuncError(
    const ReturnNotWithinFuncError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitReturnWithIncompatibleTypeError(
    const ReturnWithIncompatibleTypeError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitBinaryExprWithUnsupportedTypesError(
    const BinaryExprWithUnsupportedTypesError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitCallWithIncompatibleArgTypeError(
    const CallWithIncompatibleArgTypeError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitDefWithUnsupportedTypeError(
    const DefWithUnsupportedTypeError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitDuplicateDefError(
    const DuplicateDefError&) const {assert(false);}

void SimpleMsgPrinter::VisitFuncDefWithoutBodyNotNativeError(
    const FuncDefWithoutBodyNotNativeError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitFuncDefWithBodyIsNativeError(
    const FuncDefWithBodyIsNativeError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitFuncDefWithinNonGlobalScopeError(
    const FuncDefWithinNonGlobalScopeError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitFuncDefWithoutReturnValueError(
    const FuncDefWithoutReturnValueError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitCallWithNonFuncError(
    const CallWithNonFuncError&) const {assert(false);}

void SimpleMsgPrinter::VisitCallWithInvalidArgsCountError(
    const CallWithInvalidArgsCountError&)
    const {assert(false);}

void SimpleMsgPrinter::VisitUnaryExprWithUnsupportedTypeError(
    const UnaryExprWithUnsupportedTypeError&)
    const {assert(false);}

void SimpleMsgPrinter::PrintOutOfRangeValueError(const TokenInfo &token) const {
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "out of range value\n";
}

void SimpleMsgPrinter::PrintIncompatibleDataTypeError(
    const TokenInfo &token,
    const DataType &dest_data_type,
    const DataType &src_data_type) const {
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "expected \"" << dest_data_type.GetName()
      << "\" data type, but got \"" << src_data_type.GetName() << "\"\n";
}

void SimpleMsgPrinter::PrintArrayWithTooManyDimensionsError(
    const TokenInfo &token, size_t max_count) const {
  PrintFileError(*current_file_path_, token.GetLine(), token.GetColumn())
      << "array can't have more than " << max_count << " dimensions\n";
}

ostream &SimpleMsgPrinter::PrintFileError(
    const path &file_path, uint32_t line, uint32_t column) const {
  return DoPrintError() << file_path.string() << ':' << line << ':'
                        << column << ": ";
}

ostream &SimpleMsgPrinter::DoPrintError() const {
  return *stream_ << "[Error] ";
}
}
}