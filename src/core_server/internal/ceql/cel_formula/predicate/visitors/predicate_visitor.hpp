#pragma once

#include <stdexcept>

namespace InternalCORECEQL {

class AndFilter;
class AtomicFilter;
class OrFilter;
class AndPredicate;
class InPredicate;
class InequalityPredicate;
class LikePredicate;
class NotPredicate;
class OrPredicate;

class PredicateVisitor {
 public:
  virtual ~PredicateVisitor() = default;

  // clang-format off
  virtual void visit(AndFilter&)           {throw std::logic_error("visit AndFilter not implemented.");}
  virtual void visit(AtomicFilter&)        {throw std::logic_error("visit AtomicFilter not implemented.");}
  virtual void visit(OrFilter&)            {throw std::logic_error("visit OrFilter not implemented.");}
  virtual void visit(AndPredicate&)        {throw std::logic_error("visit AndPredicate not implemented.");}
  virtual void visit(InPredicate&)         {throw std::logic_error("visit InPredicate not implemented.");}
  virtual void visit(InequalityPredicate&) {throw std::logic_error("visit InequalityPredicate not implemented.");}
  virtual void visit(LikePredicate&)       {throw std::logic_error("visit LikePredicate not implemented.");}
  virtual void visit(NotPredicate&)        {throw std::logic_error("visit NotPredicate not implemented.");}
  virtual void visit(OrPredicate&)         {throw std::logic_error("visit OrPredicate not implemented.");}

  // clang-format on
};
}  // namespace InternalCORECEQL
