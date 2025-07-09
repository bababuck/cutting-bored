// RUN: cutting-bored-opt %s

module {
  func.func @main(%arg0: !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>) -> !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]> {
    return %arg0 : !cuttingbored.board<5 x 5 : [0, 1, 2, 3, 4]>
  }
}