
#include <boost/iterator/indirect_iterator.hpp>
#include <string>
#include <vector>
#include "real_talk/semantic/semantic_error.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::equal;
using std::ostream;
using boost::make_indirect_iterator;

namespace real_talk {
namespace semantic {

SemanticError::SemanticError(
    const string &message,
    uint32_t line_number,
    uint32_t column_number,
    vector< unique_ptr<SemanticHint> > hints)
    : message_(message),
      line_number_(line_number),
      column_number_(column_number),
      hints_(move(hints)) {
}

bool operator==(const SemanticError &lhs, const SemanticError &rhs) {
  return lhs.message_ == rhs.message_
      && lhs.line_number_ == rhs.line_number_
      && lhs.column_number_ == rhs.column_number_
      && lhs.hints_.size() == rhs.hints_.size()
      && equal(make_indirect_iterator(lhs.hints_.begin()),
               make_indirect_iterator(lhs.hints_.end()),
               make_indirect_iterator(rhs.hints_.begin()));
}

ostream &operator<<(ostream &stream, const SemanticError &error) {
  stream << "message=" << error.message_ << "; line="
         << error.line_number_ << "; column=" << error.column_number_
         << "; hints=\n";

  for (const unique_ptr<SemanticHint> &hint: error.hints_) {
    stream << *hint << "\n";
  }

  return stream;
}
}
}
