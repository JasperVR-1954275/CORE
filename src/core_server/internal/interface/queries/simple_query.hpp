#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/cea/cea.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/interface/evaluators/single_evaluator.hpp"
#include "core_server/internal/interface/queries/generic_query.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE::Internal::Interface {
template <typename ResultHandlerT>
class SimpleQuery : public GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT> {
  friend GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT>;
  // Underlying evaluator for tuples
  std::unique_ptr<SingleEvaluator> evaluator;

 public:
  SimpleQuery(Internal::Catalog& catalog,
              RingTupleQueue::Queue& queue,
              std::string inproc_receiver_address,
              ResultHandlerT& result_handler)
      : GenericQuery<SimpleQuery<ResultHandlerT>, ResultHandlerT>(catalog,
                                                                  queue,
                                                                  inproc_receiver_address,
                                                                  result_handler) {}

 private:
  void create_query(Internal::CEQL::Query&& query) {
    Internal::CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(this->catalog);

    query = transformer(std::move(query));

    auto predicates = std::move(transformer.physical_predicates);

    auto tuple_evaluator = Internal::Evaluation::PredicateEvaluator(std::move(predicates));

    auto visitor = Internal::CEQL::FormulaToLogicalCEA(this->catalog);
    query.where.formula->accept_visitor(visitor);
    if (!query.select.is_star) {
      query.select.formula->accept_visitor(visitor);
    }

    Internal::CEA::DetCEA cea(Internal::CEA::CEA(std::move(visitor.current_cea)));

    this->time_window = query.within.time_window;

    evaluator = std::make_unique<SingleEvaluator>(std::move(cea),
                                                  std::move(tuple_evaluator),
                                                  this->time_of_expiration,
                                                  query.consume_by.policy,
                                                  query.limit,
                                                  this->time_window,
                                                  this->catalog,
                                                  this->queue);
  }

  std::optional<tECS::Enumerator> process_event(RingTupleQueue::Tuple tuple) {
    return evaluator->process_event(tuple);
  }
};
}  // namespace CORE::Internal::Interface
