#include "lib/Dialect/CuttingBored/CuttingBoredDialect.h"

#include "lib/Dialect/CuttingBored/CuttingBoredTypes.h"
#include "mlir/include/mlir/IR/Builders.h"
#include "llvm/include/llvm/ADT/TypeSwitch.h"

#include "lib/Dialect/CuttingBored/CuttingBoredDialect.cpp.inc"
#define GET_TYPEDEF_CLASSES
#include "lib/Dialect/CuttingBored/CuttingBoredTypes.cpp.inc"

namespace mlir {
namespace cuttingbored {

void CuttingBoredDialect::initialize() {
  addTypes<
#define GET_TYPEDEF_LIST
#include "lib/Dialect/CuttingBored/CuttingBoredTypes.cpp.inc"
    >();
}

} // namespace cuttingbored
} // namespace mlir
