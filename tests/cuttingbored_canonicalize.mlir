// RUN: cutting-bored-opt --canonicalize %s | FileCheck %s

module {
// CHECK-LABEL:   func.func @canonicalize_build() -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
// CHECK:           %[[VAL_0:.*]] = cuttingbored.build : () -> <3 x 0 : [0, 0, 0]>
// CHECK:           %[[VAL_1:.*]] = cuttingbored.column : () -> <3 x 1 : [0, 0, 0]>
// CHECK:           %[[VAL_2:.*]] = cuttingbored.insert_column %[[VAL_0]], %[[VAL_1]] : (<3 x 0 : [0, 0, 0]>, <3 x 1 : [0, 0, 0]>) -> <3 x 1 : [0, 0, 0]>
// CHECK:           %[[VAL_3:.*]] = cuttingbored.column : () -> <3 x 1 : [1, 1, 1]>
// CHECK:           %[[VAL_4:.*]] = cuttingbored.insert_column %[[VAL_2]], %[[VAL_3]] : (<3 x 1 : [0, 0, 0]>, <3 x 1 : [1, 1, 1]>) -> <3 x 2 : [1, 1, 1]>
// CHECK:           %[[VAL_5:.*]] = cuttingbored.rotate_row %[[VAL_4]] : (<3 x 2 : [1, 1, 1]>) -> <3 x 2 : [1, 2, 1]>
// CHECK:           %[[VAL_6:.*]] = cuttingbored.column : () -> <1 x 2 : [3]>
// CHECK:           %[[VAL_7:.*]] = cuttingbored.insert_row %[[VAL_5]], %[[VAL_6]] : (<3 x 2 : [1, 2, 1]>, <1 x 2 : [3]>) -> <4 x 2 : [1, 2, 3, 1]>
// CHECK:           %[[VAL_8:.*]] = cuttingbored.column : () -> <4 x 1 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_9:.*]] = cuttingbored.insert_column %[[VAL_7]], %[[VAL_8]] : (<4 x 2 : [1, 2, 3, 1]>, <4 x 1 : [0, 0, 0, 0]>) -> <4 x 3 : [1, 2, 3, 1]>
// CHECK:           %[[VAL_10:.*]] = cuttingbored.rotate_row %[[VAL_9]] : (<4 x 3 : [1, 2, 3, 1]>) -> <4 x 3 : [1, 2, 3, 4]>
// CHECK:           %[[VAL_11:.*]] = cuttingbored.column : () -> <1 x 3 : [0]>
// CHECK:           %[[VAL_12:.*]] = cuttingbored.insert_row %[[VAL_10]], %[[VAL_11]] : (<4 x 3 : [1, 2, 3, 4]>, <1 x 3 : [0]>) -> <5 x 3 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_13:.*]] = cuttingbored.column : () -> <5 x 1 : [0, 0, 0, 0, 0]>
// CHECK:           %[[VAL_14:.*]] = cuttingbored.insert_column %[[VAL_12]], %[[VAL_13]] : (<5 x 3 : [0, 1, 2, 3, 4]>, <5 x 1 : [0, 0, 0, 0, 0]>) -> <5 x 4 : [0, 1, 2, 3, 4]>
// CHECK:           %[[VAL_15:.*]] = cuttingbored.column : () -> <5 x 1 : [0, 0, 0, 0, 0]>
// CHECK:           %[[VAL_16:.*]] = cuttingbored.insert_column %[[VAL_14]], %[[VAL_15]] : (<5 x 4 : [0, 1, 2, 3, 4]>, <5 x 1 : [0, 0, 0, 0, 0]>) -> <5 x 5 : [0, 1, 2, 3, 4]>
// CHECK:           return %[[VAL_16]] : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
// CHECK:         }
  func.func @canonicalize_build() -> (!cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
    func.return %0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }

// CHECK-LABEL:   func.func @canonicalize_checkerboard() -> !cuttingbored.board<4 x 4 : [5, 10, 5, 10]> {
// CHECK:           %[[VAL_0:.*]] = cuttingbored.build : () -> <4 x 0 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_1:.*]] = cuttingbored.column : () -> <4 x 1 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_2:.*]] = cuttingbored.insert_column %[[VAL_0]], %[[VAL_1]] : (<4 x 0 : [0, 0, 0, 0]>, <4 x 1 : [0, 0, 0, 0]>) -> <4 x 1 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_3:.*]] = cuttingbored.column : () -> <4 x 1 : [1, 1, 1, 1]>
// CHECK:           %[[VAL_4:.*]] = cuttingbored.insert_column %[[VAL_2]], %[[VAL_3]] : (<4 x 1 : [0, 0, 0, 0]>, <4 x 1 : [1, 1, 1, 1]>) -> <4 x 2 : [1, 1, 1, 1]>
// CHECK:           %[[VAL_5:.*]] = cuttingbored.column : () -> <4 x 1 : [0, 0, 0, 0]>
// CHECK:           %[[VAL_6:.*]] = cuttingbored.insert_column %[[VAL_4]], %[[VAL_5]] : (<4 x 2 : [1, 1, 1, 1]>, <4 x 1 : [0, 0, 0, 0]>) -> <4 x 3 : [2, 2, 2, 2]>
// CHECK:           %[[VAL_7:.*]] = cuttingbored.column : () -> <4 x 1 : [1, 1, 1, 1]>
// CHECK:           %[[VAL_8:.*]] = cuttingbored.insert_column %[[VAL_6]], %[[VAL_7]] : (<4 x 3 : [2, 2, 2, 2]>, <4 x 1 : [1, 1, 1, 1]>) -> <4 x 4 : [5, 5, 5, 5]>
// CHECK:           %[[VAL_9:.*]] = cuttingbored.rotate_row %[[VAL_8]] : (<4 x 4 : [5, 5, 5, 5]>) -> <4 x 4 : [5, 5, 5, 10]>
// CHECK:           %[[VAL_10:.*]] = cuttingbored.rotate_row %[[VAL_9]] : (<4 x 4 : [5, 5, 5, 10]>) -> <4 x 4 : [5, 10, 5, 10]>
// CHECK:           return %[[VAL_10]] : !cuttingbored.board<4 x 4 : [5, 10, 5, 10]>
// CHECK:         }
  func.func @canonicalize_checkerboard() -> (!cuttingbored.board<4 x 4 : [5, 10, 5, 10]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<4 x 4 : [5, 10, 5, 10]>
    func.return %0 : !cuttingbored.board<4 x 4 : [5, 10, 5, 10]>
  }

// CHECK-LABEL:   func.func @canonicalize_checkerboard_rectangle() -> !cuttingbored.board<6 x 4 : [5, 10, 5, 10, 5, 10]> {
// CHECK:           %[[VAL_0:.*]] = cuttingbored.build : () -> <6 x 0 : [0, 0, 0, 0, 0, 0]>
// CHECK:           %[[VAL_1:.*]] = cuttingbored.column : () -> <6 x 1 : [0, 0, 0, 0, 0, 0]>
// CHECK:           %[[VAL_2:.*]] = cuttingbored.insert_column %[[VAL_0]], %[[VAL_1]] : (<6 x 0 : [0, 0, 0, 0, 0, 0]>, <6 x 1 : [0, 0, 0, 0, 0, 0]>) -> <6 x 1 : [0, 0, 0, 0, 0, 0]>
// CHECK:           %[[VAL_3:.*]] = cuttingbored.column : () -> <6 x 1 : [1, 1, 1, 1, 1, 1]>
// CHECK:           %[[VAL_4:.*]] = cuttingbored.insert_column %[[VAL_2]], %[[VAL_3]] : (<6 x 1 : [0, 0, 0, 0, 0, 0]>, <6 x 1 : [1, 1, 1, 1, 1, 1]>) -> <6 x 2 : [1, 1, 1, 1, 1, 1]>
// CHECK:           %[[VAL_5:.*]] = cuttingbored.column : () -> <6 x 1 : [0, 0, 0, 0, 0, 0]>
// CHECK:           %[[VAL_6:.*]] = cuttingbored.insert_column %[[VAL_4]], %[[VAL_5]] : (<6 x 2 : [1, 1, 1, 1, 1, 1]>, <6 x 1 : [0, 0, 0, 0, 0, 0]>) -> <6 x 3 : [2, 2, 2, 2, 2, 2]>
// CHECK:           %[[VAL_7:.*]] = cuttingbored.column : () -> <6 x 1 : [1, 1, 1, 1, 1, 1]>
// CHECK:           %[[VAL_8:.*]] = cuttingbored.insert_column %[[VAL_6]], %[[VAL_7]] : (<6 x 3 : [2, 2, 2, 2, 2, 2]>, <6 x 1 : [1, 1, 1, 1, 1, 1]>) -> <6 x 4 : [5, 5, 5, 5, 5, 5]>
// CHECK:           %[[VAL_9:.*]] = cuttingbored.rotate_row %[[VAL_8]] : (<6 x 4 : [5, 5, 5, 5, 5, 5]>) -> <6 x 4 : [5, 5, 5, 5, 5, 10]>
// CHECK:           %[[VAL_10:.*]] = cuttingbored.rotate_row %[[VAL_9]] : (<6 x 4 : [5, 5, 5, 5, 5, 10]>) -> <6 x 4 : [5, 5, 5, 10, 5, 10]>
// CHECK:           %[[VAL_11:.*]] = cuttingbored.rotate_row %[[VAL_10]] : (<6 x 4 : [5, 5, 5, 10, 5, 10]>) -> <6 x 4 : [5, 10, 5, 10, 5, 10]>
// CHECK:           return %[[VAL_11]] : !cuttingbored.board<6 x 4 : [5, 10, 5, 10, 5, 10]>
// CHECK:         }
  func.func @canonicalize_checkerboard_rectangle() -> (!cuttingbored.board<6 x 4 : [5, 10, 5, 10, 5, 10]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<6 x 4 : [5, 10, 5, 10, 5, 10]>
    func.return %0 : !cuttingbored.board<6 x 4 : [5, 10, 5, 10, 5, 10]>
  }
}
