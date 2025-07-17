// RUN: cutting-bored-opt --canonicalize %s | FileCheck %s

module {
// CHECK-LABEL:   func.func @canonicalize_build() -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
// CHECK:           %[[VAL_0:.*]] = cuttingbored.build : () -> <5 x 3 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_1:.*]] = cuttingbored.column : () -> <5 x 1 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_2:.*]] = cuttingbored.insert_column %[[VAL_0]], %[[VAL_1]] : (<5 x 3 : [0, 1, 2, 3, 4]>, <5 x 1 : [0, 1, 2, 3, 4]>) -> <5 x 4 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_3:.*]] = cuttingbored.column : () -> <5 x 1 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_4:.*]] = cuttingbored.insert_column %[[VAL_2]], %[[VAL_3]] : (<5 x 4 : [0, 1, 2, 3, 4]>, <5 x 1 : [0, 1, 2, 3, 4]>) -> <5 x 5 : [0, 1, 2, 3, 4]>
// CHECK:           return %[[VAL_4]] : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
// CHECK:         }
  func.func @canonicalize_build() -> (!cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
    func.return %0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }

// CHECK-LABEL:   func.func @canonicalize_checkerboard() -> !cuttingbored.board<3 x 3 : [5, 2, 5]> {
// CHECK:           %[[VAL_0:.*]] = cuttingbored.build : () -> <3 x 3 : [5, 2, 5]>
// CHECK:           return %[[VAL_0]] : !cuttingbored.board<3 x 3 : [5, 2, 5]>
// CHECK:         }
  func.func @canonicalize_checkerboard() -> (!cuttingbored.board<3 x 3 : [5, 2, 5]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<3 x 3 : [5, 2, 5]>
    func.return %0 : !cuttingbored.board<3 x 3 : [5, 2, 5]>
  }
}
