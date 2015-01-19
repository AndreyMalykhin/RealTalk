
#include <cassert>
#include "real_talk/parser/def_node.h"
#include "real_talk/semantic/id_analysis.h"

using std::unique_ptr;
using std::ostream;
using real_talk::parser::DefNode;

namespace real_talk {
namespace semantic {

IdAnalysis::IdAnalysis(
    unique_ptr<DataType> data_type, ValueType value_type, const DefNode* def)
    : base_analysis_(move(data_type), value_type), def_(def) {
  assert(def_);
}

const DataType &IdAnalysis::GetDataType() const {
  return base_analysis_.GetDataType();
}

ValueType IdAnalysis::GetValueType() const {
  return base_analysis_.GetValueType();
}

const DefNode *IdAnalysis::GetDef() const {
  return def_;
}

bool IdAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const IdAnalysis &rhs =
      static_cast<const IdAnalysis&>(analysis);
  return base_analysis_ == rhs.base_analysis_ && def_ == rhs.def_;
}

void IdAnalysis::Print(ostream &stream) const {
  stream << base_analysis_ << "; def=" << *def_;
}
}
}
