#ifndef LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_H_
#define LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_H_

#include "lib/Dialect/CuttingBored/CuttingBoredDialect.h"
#include "lib/Dialect/CuttingBored/CuttingBoredTypes.h"
#include "mlir/include/mlir/IR/BuiltinOps.h"    // from @llvm-project
#include "mlir/include/mlir/IR/BuiltinTypes.h"  // from @llvm-project
#include "mlir/include/mlir/IR/Dialect.h"       // from @llvm-project

#define GET_OP_CLASSES
#include "lib/Dialect/CuttingBored/CuttingBored.h.inc"

#endif  // LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_H_
