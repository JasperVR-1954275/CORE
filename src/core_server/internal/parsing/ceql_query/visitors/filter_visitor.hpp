#include <memory>

#include "core_server/internal/ceql/cel_formula/filters/filter_headers.hpp"
#include "core_server/internal/ceql/query/where.hpp"
#include "core_server/internal/parsing/ceql_query/autogenerated/CEQLQueryParserBaseVisitor.h"
#include "predicate_visitor.hpp"

namespace CORE {
namespace Internal {
namespace Parsing {
class FilterVisitor : public CEQLQueryParserBaseVisitor {
 private:
  // this filter is the corresponding parsed filter after the
  // visitation to the ctx is finished.
  std::unique_ptr<CEQL::Filter> filter;

  PredicateVisitor predicate_visitor;

 public:
  std::unique_ptr<CEQL::Filter> get_parsed_filter() {
    return std::move(filter);
  }

  virtual std::any
  visitAtomic_filter(CEQLQueryParser::Atomic_filterContext* ctx) override {
    predicate_visitor.visit(ctx->predicate());
    filter = std::make_unique<CEQL::AtomicFilter>(
      ctx->event_name()->getText(),
      predicate_visitor.get_parsed_predicate());
    return {};
  }

  virtual std::any
  visitAnd_filter(CEQLQueryParser::And_filterContext* ctx) override {
    visit(ctx->filter()[0]);
    auto left = std::move(filter);
    visit(ctx->filter()[1]);
    filter = std::make_unique<CEQL::AndFilter>(std::move(left),
                                               std::move(filter));
    return {};
  }

  virtual std::any
  visitOr_filter(CEQLQueryParser::Or_filterContext* ctx) override {
    visit(ctx->filter()[0]);
    auto left = std::move(filter);
    visit(ctx->filter()[1]);
    filter = std::make_unique<CEQL::OrFilter>(std::move(left),
                                              std::move(filter));
    return {};
  }
};
}  // namespace Parsing
}  // namespace Internal
}  // namespace CORE
