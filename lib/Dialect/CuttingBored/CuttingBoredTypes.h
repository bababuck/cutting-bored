#ifndef LIB_TYPES_CUTTINGBORED_CUTTINGBOREDTYPES_H_
#define LIB_TYPES_CUTTINGBORED_CUTTINGBOREDTYPES_H_

// Required because the .h.inc file refers to MLIR classes and does not itself
// have any includes.
#include "mlir/include/mlir/IR/DialectImplementation.h"

#define GET_TYPEDEF_CLASSES
#include "lib/Dialect/CuttingBored/CuttingBoredTypes.h.inc"

#endif  // LIB_TYPES_CUTTINGBORED_CUTTINGBOREDTYPES_H_
