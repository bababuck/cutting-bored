// RUN: cutting-bored-opt %s

module {
  func.func @main(%arg0: !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
    return %arg0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }

  func.func @build(%arg0: !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) -> () {
    cuttingbored.build %arg0 : (!cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>)
    func.return
  }

  func.func @insert_column(%arg0: !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) -> (!cuttingbored.board<5 x 4 : [0, 1, 2, 3, 4]>, !cuttingbored.board<5 x 1 : [0, 0, 0, 0, 0]>) {
    %0, %1 = cuttingbored.insert_column %arg0 : (!cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) -> !cuttingbored.board<5 x 4 : [0, 1, 2, 3, 4]>, !cuttingbored.board<5 x 1 : [0, 0, 0, 0, 0]>
    return %0, %1 : !cuttingbored.board<5 x 4 : [0, 1, 2, 3, 4]>, !cuttingbored.board<5 x 1 : [0, 0, 0, 0, 0]>
  }
}