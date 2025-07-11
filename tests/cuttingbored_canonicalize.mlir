// RUN: cutting-bored-opt --canonicalize %s | FileCheck %s

module {
// CHECK-LABEL:   func.func @canonicalize_build() -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
// CHECK:           %[[VAL_0:.*]] = cuttingbored.build : () -> <5 x 5 : [0, 1, 2, 3, 4]>
// CHECK:           return %[[VAL_0]] : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
// CHECK:         }
  func.func @canonicalize_build() -> (!cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
    func.return %0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }
}
