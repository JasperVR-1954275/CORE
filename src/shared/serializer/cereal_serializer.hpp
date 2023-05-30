#pragma once
#include <cereal/archives/binary.hpp>

#include "shared/serializer/serializer.hpp"

// Headers for all cereal types
#include <cereal/types/array.hpp>
#include <cereal/types/atomic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/concepts/pair_associative_container.hpp>
#include <cereal/types/deque.hpp>
#include <cereal/types/forward_list.hpp>
#include <cereal/types/functional.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/queue.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/stack.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/valarray.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>

namespace InternalCORE {
template <typename StructName>
class CerealSerializer : Serializer<StructName> {
 public:
  static std::string serialize(const StructName& data) {
    std::stringstream ss;
    cereal::BinaryOutputArchive oarchive(ss);
    oarchive(data);
    return ss.str();
  }

  static StructName deserialize(const std::string& message) {
    std::stringstream ss(message);
    StructName data;
    cereal::BinaryInputArchive iarchive(ss);
    iarchive(data);
    return data;
  }
};
}  // namespace InternalCORE

/**
 * For cereal to work on abstract classes it is necessary to declar all
 * relationships between abstract classes and their concrete classes.
 */

// clang-format off
#include "core_server/internal/ceql/value/all_value_headers.hpp"

CEREAL_REGISTER_TYPE(InternalCORECEQL::Attribute);
CEREAL_REGISTER_TYPE(InternalCORECEQL::BooleanLiteral);
CEREAL_REGISTER_TYPE(InternalCORECEQL::DoubleLiteral);
CEREAL_REGISTER_TYPE(InternalCORECEQL::IntegerLiteral);
CEREAL_REGISTER_TYPE(InternalCORECEQL::LongLiteral);
CEREAL_REGISTER_TYPE(InternalCORECEQL::StringLiteral);
CEREAL_REGISTER_TYPE(InternalCORECEQL::Undefined);
CEREAL_REGISTER_TYPE(InternalCORECEQL::Addition);
CEREAL_REGISTER_TYPE(InternalCORECEQL::Division);
CEREAL_REGISTER_TYPE(InternalCORECEQL::Modulo);
CEREAL_REGISTER_TYPE(InternalCORECEQL::Multiplication);
CEREAL_REGISTER_TYPE(InternalCORECEQL::Negation);
CEREAL_REGISTER_TYPE(InternalCORECEQL::Subtraction);

CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::Attribute);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::BooleanLiteral);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::DoubleLiteral);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::IntegerLiteral);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::LongLiteral);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::StringLiteral);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::Undefined);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::Addition);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::Division);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::Modulo);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::Multiplication);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::Negation);
CEREAL_REGISTER_POLYMORPHIC_RELATION(InternalCORECEQL::Value, InternalCORECEQL::Subtraction);
