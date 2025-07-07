// RUN: cutting-bored-opt %s

module {
  func.func @main(%arg0: !cuttingbored.board) -> !cuttingbored.board {
    return %arg0 : !cuttingbored.board
  }
}