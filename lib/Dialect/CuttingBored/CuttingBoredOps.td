#ifndef LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_TD_
#define LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_TD_

include "mlir/IR/BuiltinAttributes.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

def CuttingBored_Build : Op<CuttingBored_Dialect, "build"> {
  let summary = "Create a cutting board, top level instruction.";
  let arguments = (ins);
  let results = (outs Board:$to_build);
  let assemblyFormat = "attr-dict `:` `(` `)` `->` type($to_build) ";
  let hasCanonicalizer = 1;
}

def CuttingBored_InsertColumn : Op<CuttingBored_Dialect, "insert_column"> {
  let summary = "Add a single colored column into the board.";
  let arguments = (ins Board:$partial, Board:$column);
  let results = (outs Board:$full);
  let assemblyFormat = "$partial  attr-dict `,` $column `:` `(` type($partial) `,` type($column) `)` `->` type($full)";
}

def CuttingBoard_ColumnOp : Op<CuttingBored_Dialect, "column"> {
  let summary = "Create single colored column";
  let arguments = (ins);
  let results = (outs Board:$column);
  let assemblyFormat = "attr-dict `:` `(` `)`  `->` type($column)";
}

#endif  // LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDOPS_TD_
