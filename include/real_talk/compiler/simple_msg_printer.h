
#ifndef _REAL_TALK_COMPILER_SIMPLE_MSG_PRINTER_H_
#define _REAL_TALK_COMPILER_SIMPLE_MSG_PRINTER_H_

#include <string>
#include <iostream>
#include "real_talk/semantic/semantic_problem_visitor.h"
#include "real_talk/compiler/msg_printer.h"

namespace real_talk {
namespace semantic {

class DataType;
}

namespace compiler {

class SimpleMsgPrinter: public MsgPrinter,
                        private real_talk::semantic::SemanticProblemVisitor {
 public:
  explicit SimpleMsgPrinter(std::ostream *stream);
  virtual void PrintSemanticProblems(
      const real_talk::semantic::SemanticAnalysis::ProgramProblems &problems,
      const ProgramFilePaths &program_file_paths) const override;
  virtual void PrintSemanticProblem(
      const real_talk::semantic::SemanticProblem &problem,
      const boost::filesystem::path &file_path) const override;
  virtual void PrintTokenError(
      const real_talk::lexer::TokenInfo &token,
      const boost::filesystem::path &file_path,
      const std::string &error) const override;
  virtual void PrintUnexpectedTokenError(
      const real_talk::lexer::TokenInfo &token,
      const boost::filesystem::path &file_path) const override;
  virtual void PrintUnexpectedCharError(
      char c,
      uint32_t line,
      uint32_t column,
      const boost::filesystem::path &file_path) const override;
  virtual void PrintError(const std::string &error) const override;
  virtual void PrintHelp(const std::string &help) const override;

 private:
  virtual void VisitArrayAllocWithTooManyDimensionsError(
      const real_talk::semantic::ArrayAllocWithTooManyDimensionsError &error)
      const override;
  virtual void VisitArrayTypeWithTooManyDimensionsError(
      const real_talk::semantic::ArrayTypeWithTooManyDimensionsError &error)
      const override;
  virtual void VisitDoubleWithOutOfRangeValueError(
      const real_talk::semantic::DoubleWithOutOfRangeValueError &error)
      const override;
  virtual void VisitLongWithOutOfRangeValueError(
      const real_talk::semantic::LongWithOutOfRangeValueError &error)
      const override;
  virtual void VisitIntWithOutOfRangeValueError(
      const real_talk::semantic::IntWithOutOfRangeValueError &error)
      const override;
  virtual void VisitCharWithMultipleCharsError(
      const real_talk::semantic::CharWithMultipleCharsError &error)
      const override;
  virtual void VisitCharWithEmptyHexValueError(
      const real_talk::semantic::CharWithEmptyHexValueError &error)
      const override;
  virtual void VisitCharWithOutOfRangeHexValueError(
      const real_talk::semantic::CharWithOutOfRangeHexValueError &error)
      const override;
  virtual void VisitStringWithOutOfRangeHexValueError(
      const real_talk::semantic::StringWithOutOfRangeHexValueError &error)
      const override;
  virtual void VisitStringWithEmptyHexValueError(
      const real_talk::semantic::StringWithEmptyHexValueError &error)
      const override;
  virtual void VisitUnaryExprWithUnsupportedTypeError(
      const real_talk::semantic::UnaryExprWithUnsupportedTypeError &error)
      const override;
  virtual void VisitAssignWithRightValueAssigneeError(
      const real_talk::semantic::AssignWithRightValueAssigneeError &error)
      const override;
  virtual void VisitIdWithoutDefError(
      const real_talk::semantic::IdWithoutDefError &error) const override;
  virtual void VisitSubscriptWithUnsupportedIndexTypeError(
      const real_talk::semantic::SubscriptWithUnsupportedIndexTypeError &error)
      const override;
  virtual void VisitSubscriptWithUnsupportedOperandTypeError(
      const real_talk::semantic::SubscriptWithUnsupportedOperandTypeError &error)
      const override;
  virtual void VisitArrayAllocWithIncompatibleValueTypeError(
      const real_talk::semantic::ArrayAllocWithIncompatibleValueTypeError &error)
      const override;
  virtual void VisitArrayAllocWithUnsupportedElementTypeError(
      const real_talk::semantic::ArrayAllocWithUnsupportedElementTypeError &error)
      const override;
  virtual void VisitArrayAllocWithUnsupportedSizeTypeError(
      const real_talk::semantic::ArrayAllocWithUnsupportedSizeTypeError &error)
      const override;
  virtual void VisitIfWithIncompatibleTypeError(
      const real_talk::semantic::IfWithIncompatibleTypeError &error)
      const override;
  virtual void VisitBreakNotWithinLoopError(
      const real_talk::semantic::BreakNotWithinLoopError &error) const override;
  virtual void VisitContinueNotWithinLoopError(
      const real_talk::semantic::ContinueNotWithinLoopError &error)
      const override;
  virtual void VisitPreTestLoopWithIncompatibleTypeError(
      const real_talk::semantic::PreTestLoopWithIncompatibleTypeError &error)
      const override;
  virtual void VisitImportIsNotFirstStmtError(
      const real_talk::semantic::ImportIsNotFirstStmtError &error)
      const override;
  virtual void VisitVarDefWithIncompatibleValueTypeError(
      const real_talk::semantic::VarDefWithIncompatibleValueTypeError &error)
      const override;
  virtual void VisitReturnWithoutValueError(
      const real_talk::semantic::ReturnWithoutValueError &error) const override;
  virtual void VisitReturnNotWithinFuncError(
      const real_talk::semantic::ReturnNotWithinFuncError &error)
      const override;
  virtual void VisitReturnWithIncompatibleTypeError(
      const real_talk::semantic::ReturnWithIncompatibleTypeError &error)
      const override;
  virtual void VisitBinaryExprWithUnsupportedTypesError(
      const real_talk::semantic::BinaryExprWithUnsupportedTypesError &error)
      const override;
  virtual void VisitCallWithIncompatibleArgTypeError(
      const real_talk::semantic::CallWithIncompatibleArgTypeError &error)
      const override;
  virtual void VisitDefWithUnsupportedTypeError(
      const real_talk::semantic::DefWithUnsupportedTypeError &error)
      const override;
  virtual void VisitDuplicateDefError(
      const real_talk::semantic::DuplicateDefError &error) const override;
  virtual void VisitFuncDefWithoutBodyNotNativeError(
      const real_talk::semantic::FuncDefWithoutBodyNotNativeError &error)
      const override;
  virtual void VisitFuncDefWithBodyIsNativeError(
      const real_talk::semantic::FuncDefWithBodyIsNativeError &error)
      const override;
  virtual void VisitFuncDefWithinNonGlobalScopeError(
      const real_talk::semantic::FuncDefWithinNonGlobalScopeError &error)
      const override;
  virtual void VisitFuncDefWithoutReturnValueError(
      const real_talk::semantic::FuncDefWithoutReturnValueError &error)
      const override;
  virtual void VisitCallWithUnsupportedTypeError(
      const real_talk::semantic::CallWithUnsupportedTypeError &error) const override;
  virtual void VisitCallWithInvalidArgsCountError(
      const real_talk::semantic::CallWithInvalidArgsCountError &error)
      const override;
  void PrintOutOfRangeValueError(
      const real_talk::lexer::TokenInfo &token) const;
  void PrintIncompatibleDataTypeError(
      const real_talk::lexer::TokenInfo &token,
      const real_talk::semantic::DataType &dest_data_type,
      const real_talk::semantic::DataType &src_data_type) const;
  void PrintArrayWithTooManyDimensionsError(
      const real_talk::lexer::TokenInfo &token, size_t max_count) const;
  std::ostream &PrintCurrentFileError(
      const real_talk::lexer::TokenInfo &token) const;
  std::ostream &PrintFileError(const boost::filesystem::path &file_path,
                               uint32_t line,
                               uint32_t column) const;
  std::ostream &DoPrintError() const;

  std::ostream *stream_;
  mutable const boost::filesystem::path *current_file_path_;
};
}
}
#endif
