#include "or_predicate.hpp"

#include "and_predicate.hpp"

namespace InternalCORECEQL {
std::unique_ptr<Predicate> OrPredicate::negate() const {
  std::vector<std::unique_ptr<Predicate>> negated;
  for (auto& predicate : predicates) {
    auto neg = predicate->negate();
    negated.push_back(std::move(neg));
  }
  return std::make_unique<AndPredicate>(std::move(negated));
}
}  // namespace InternalCORECEQL
