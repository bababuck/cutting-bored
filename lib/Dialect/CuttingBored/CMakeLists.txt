add_mlir_dialect(CuttingBored cuttingbored)

add_mlir_dialect_library(MLIRCuttingBored
    CuttingBoredDialect.cpp

    ADDITIONAL_HEADER_DIRS
    ${PROJECT_SOURCE_DIR}/lib/Dialect/CuttingBored

    LINK_LIBS PUBLIC
    )
