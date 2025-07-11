configure_lit_site_cfg(
    ${CMAKE_CURRENT_SOURCE_DIR}/lit.cmake.site.cfg.py.in
    ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg.py
    MAIN_CONFIG
    ${CMAKE_CURRENT_SOURCE_DIR}/lit.cmake.cfg.py
)

set (MLIR_CUTTINGBORED_TEST_DEPENDS
    FileCheck count not
    mlir-opt
    mlir-cpu-runner
    cutting-bored-opt
)

add_lit_testsuite(check-mlir-cuttingbored "Running the MLIR CuttingBored dialect regression tests"
    ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CUTTINGBORED_TEST_DEPENDS}
)
