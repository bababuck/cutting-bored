#ifndef LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_TD_
#define LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_TD_

include "CuttingBoredDialect.td"
include "CuttingBoredTypes.td"
include "mlir/IR/BuiltinAttributes.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

def CuttingBored_InsertColumn : Op<CuttingBored_Dialect, "insert_column"> {
  let summary = "Add a single colored column into the board.";
  let arguments = (ins Board:$full);
  let results = (outs Board:$partial, Board:$column);
  let assemblyFormat = "$full attr-dict `:` `(` type($full) `)` `->` type($partial) `,` type($column)";
}

#endif  // LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_TD_
