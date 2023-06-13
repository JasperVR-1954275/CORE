#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/ceql/cel_formula/predicate/visitors/ceql_predicate_to_cea_predicate.hpp"
#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"

using namespace RingTupleQueue;
using namespace InternalCORECEQL;
using namespace InternalCORECEA;

namespace COREQueryParsingTestsValueToMathExpr {

TEST_CASE("Compare with constant predicate computed correctly.",
          "[ValueToMathExpr]") {
  std::vector<AttributeInfo> attributes_info;
  attributes_info.emplace_back("String",
                               CORETypes::ValueTypes::STRING_VIEW);
  attributes_info.emplace_back("Integer1", CORETypes::ValueTypes::INT64);
  attributes_info.emplace_back("Integer2", CORETypes::ValueTypes::INT64);
  attributes_info.emplace_back("Double1", CORETypes::ValueTypes::DOUBLE);
  attributes_info.emplace_back("Double2", CORETypes::ValueTypes::DOUBLE);
  EventInfo event_info(0, "some_event_name", std::move(attributes_info));

  TupleSchemas schemas;
  Queue ring_tuple_queue(100, &schemas);
  auto id =
      schemas.add_schema({SupportedTypes::STRING_VIEW,
                          SupportedTypes::INT64, SupportedTypes::INT64,
                          SupportedTypes::DOUBLE, SupportedTypes::DOUBLE});
  std::span<uint64_t> data = ring_tuple_queue.start_tuple(id);
  char* chars = ring_tuple_queue.writer<std::string>(11);
  std::string hello_world = "hello world";
  memcpy(chars, &hello_world[0], 11);
  int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  *integer_ptr = -1;
  integer_ptr = ring_tuple_queue.writer<int64_t>();
  *integer_ptr = 1;
  double* double_ptr = ring_tuple_queue.writer<double>();
  *double_ptr = -1.0;
  double_ptr = ring_tuple_queue.writer<double>();
  *double_ptr = 1.0;

  Tuple tuple(data, &schemas);

  CEQLPredicateToCEAPredicate converter(event_info);

  SECTION("Compare with a constant") {
    std::unique_ptr<InternalCORECEQL::Predicate> predicate =
        std::make_unique<InequalityPredicate>(
            std::make_unique<InternalCORECEQL::Attribute>("Integer1"),
            InequalityPredicate::LogicalOperation::LESS,
            std::make_unique<IntegerLiteral>(5));
    predicate->accept_visitor(converter);
    REQUIRE((*converter.predicate)(tuple));
  }
}
}  // namespace COREQueryParsingTestsValueToMathExpr
