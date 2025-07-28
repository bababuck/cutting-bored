// RUN: cutting-bored-opt %s

module {
  func.func @main(%arg0: !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
    return %arg0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }

  func.func @build() -> (!cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) {
    %0 = cuttingbored.build : () -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
    func.return %0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }

  func.func @insert_column(%arg0: !cuttingbored.board<5 x 4 : [0, 1, 2, 3, 4]>, %arg1: !cuttingbored.board<5 x 1 : [0, 0, 0, 0, 0]>) -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
    %0 = cuttingbored.insert_column %arg0, %arg1 : (!cuttingbored.board<5 x 4 : [0, 1, 2, 3, 4]>, !cuttingbored.board<5 x 1 : [0, 0, 0, 0, 0]>) -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
    return %0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }

  func.func @insert_row(%arg0: !cuttingbored.board<4 x 5 : [1, 2, 3, 4]>, %arg1: !cuttingbored.board<1 x 5 : [0, 0, 0, 0, 0]>) -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
    %0 = cuttingbored.insert_row %arg0, %arg1 : (!cuttingbored.board<4 x 5 : [1, 2, 3, 4]>, !cuttingbored.board<1 x 5 : [0, 0, 0, 0, 0]>) -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
    return %0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }

  func.func @column() -> (!cuttingbored.board<5 x 1 : [0, 1, 0, 1, 0]>) {
    %0 = cuttingbored.column : () -> !cuttingbored.board<5 x 1 : [0, 1, 0, 1, 0]>
    func.return %0 : !cuttingbored.board<5 x 1 : [0, 1, 0, 1, 0]>
  }

  func.func @rotate_row(%arg0: !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) -> !cuttingbored.board<5 x 5 : [0, 8, 2, 3, 4]> {
    %0 = cuttingbored.rotate_row %arg0 : (!cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) -> !cuttingbored.board<5 x 5 : [0, 8, 2, 3, 4]>
    return %0 : !cuttingbored.board<5 x 5 : [0, 8, 2, 3, 4]>
  }
}
