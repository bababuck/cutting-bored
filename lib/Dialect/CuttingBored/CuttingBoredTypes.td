#ifndef LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDTYPES_TD_
#define LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDTYPES_TD_

include "CuttingBoredDialect.td"
include "mlir/IR/AttrTypeBase.td"

// A base class for all types in this dialect
class CuttingBored_Type<string name, string typeMnemonic> : TypeDef<CuttingBored_Dialect, name> {
  let mnemonic = typeMnemonic;
}

def Board : CuttingBored_Type<"Board", "board"> {
  let summary = "An arbitrarily dimensioned cutting board";

  let description = [{
    A type for cutting board and sub-cutting boards.
  }];

  let assemblyFormat = "";
}

#endif  // LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDTYPES_TD_