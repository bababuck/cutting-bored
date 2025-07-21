// RUN: cutting-bored-opt --canonicalize %s | FileCheck %s

module {
// CHECK-LABEL:   func.func @canonicalize_build() -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
// CHECK:           %[[VAL_0:.*]] = cuttingbored.build : () -> <5 x 2 : [0, 2, 1, 3, 2]>
// CHECK:           %[[VAL_1:.*]] = cuttingbored.column : () -> <5 x 1 : [0, 2, 1, 3, 2]>
// CHECK:           %[[VAL_2:.*]] = cuttingbored.insert_column %[[VAL_0]], %[[VAL_1]] : (<5 x 2 : [0, 2, 1, 3, 2]>, <5 x 1 : [0, 2, 1, 3, 2]>) -> <5 x 3 : [0, 4, 2, 6, 4]>
// CHECK:           %[[VAL_3:.*]] = cuttingbored.rotate_row %[[VAL_2]] : (<5 x 3 : [0, 4, 2, 6, 4]>) -> <5 x 3 : [0, 4, 2, 3, 4]>
// CHECK:           %[[VAL_4:.*]] = cuttingbored.rotate_row %[[VAL_3]] : (<5 x 3 : [0, 4, 2, 3, 4]>) -> <5 x 3 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_5:.*]] = cuttingbored.column : () -> <5 x 1 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_6:.*]] = cuttingbored.insert_column %[[VAL_4]], %[[VAL_5]] : (<5 x 3 : [0, 1, 2, 3, 4]>, <5 x 1 : [0, 1, 2, 3, 4]>) -> <5 x 4 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_7:.*]] = cuttingbored.column : () -> <5 x 1 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_8:.*]] = cuttingbored.insert_column %[[VAL_6]], %[[VAL_7]] : (<5 x 4 : [0, 1, 2, 3, 4]>, <5 x 1 : [0, 1, 2, 3, 4]>) -> <5 x 5 : [0, 1, 2, 3, 4]>
// CHECK:           return %[[VAL_8]] : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
// CHECK:         }
  func.func @canonicalize_build() -> (!cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
    func.return %0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }

// CHECK-LABEL:   func.func @canonicalize_checkerboard() -> !cuttingbored.board<4 x 4 : [5, 10, 5, 10]> {
// CHECK:           %[[VAL_0:.*]] = cuttingbored.build : () -> <4 x 0 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_1:.*]] = cuttingbored.column : () -> <4 x 1 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_2:.*]] = cuttingbored.insert_column %[[VAL_0]], %[[VAL_1]] : (<4 x 0 : [0, 0, 0, 0]>, <4 x 1 : [0, 0, 0, 0]>) -> <4 x 1 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_3:.*]] = cuttingbored.column : () -> <4 x 1 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_4:.*]] = cuttingbored.insert_column %[[VAL_2]], %[[VAL_3]] : (<4 x 1 : [0, 0, 0, 0]>, <4 x 1 : [0, 0, 0, 0]>) -> <4 x 2 : [1, 1, 1, 1]>
// CHECK:           %[[VAL_5:.*]] = cuttingbored.column : () -> <4 x 1 : [1, 1, 1, 1]>
// CHECK:           %[[VAL_6:.*]] = cuttingbored.insert_column %[[VAL_4]], %[[VAL_5]] : (<4 x 2 : [1, 1, 1, 1]>, <4 x 1 : [1, 1, 1, 1]>) -> <4 x 3 : [2, 2, 2, 2]>
// CHECK:           %[[VAL_7:.*]] = cuttingbored.column : () -> <4 x 1 : [2, 2, 2, 2]>
// CHECK:           %[[VAL_8:.*]] = cuttingbored.insert_column %[[VAL_6]], %[[VAL_7]] : (<4 x 3 : [2, 2, 2, 2]>, <4 x 1 : [2, 2, 2, 2]>) -> <4 x 4 : [5, 5, 5, 5]>
// CHECK:           %[[VAL_9:.*]] = cuttingbored.rotate_row %[[VAL_8]] : (<4 x 4 : [5, 5, 5, 5]>) -> <4 x 4 : [5, 5, 5, 10]>
// CHECK:           %[[VAL_10:.*]] = cuttingbored.rotate_row %[[VAL_9]] : (<4 x 4 : [5, 5, 5, 10]>) -> <4 x 4 : [5, 10, 5, 10]>
// CHECK:           return %[[VAL_10]] : !cuttingbored.board<4 x 4 : [5, 10, 5, 10]>
// CHECK:         }
  func.func @canonicalize_checkerboard() -> (!cuttingbored.board<4 x 4 : [5, 10, 5, 10]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<4 x 4 : [5, 10, 5, 10]>
    func.return %0 : !cuttingbored.board<4 x 4 : [5, 10, 5, 10]>
  }
}
