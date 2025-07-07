#ifndef LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDDIALECT_TD_
#define LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDDIALECT_TD_

include "mlir/IR/DialectBase.td"

def CuttingBored_Dialect : Dialect {
  let name = "cuttingbored";
  let summary = "A dialect for making cutting boards";
  let description = [{
    The CuttingBored dialect defines types and operations for creating
    patterned cutting boards from strips of wood.
  }];

  let cppNamespace = "::mlir::cuttingbored";

  let useDefaultTypePrinterParser = 1;
}

#endif  // LIB_DIALECT_CUTTINGBORED_CUTTINGBOREDDIALECT_TD_