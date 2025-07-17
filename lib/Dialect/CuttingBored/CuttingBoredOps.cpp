#include "lib/Dialect/CuttingBored/CuttingBoredOps.h"
#include "lib/Dialect/CuttingBored/CuttingBoredTypes.h"

#include "mlir/Dialect/CommonFolders.h"
#include "mlir/Dialect/Complex/IR/Complex.h"
#include "mlir/IR/PatternMatch.h"

namespace mlir {
namespace cuttingbored {

// Remove a column if solid color
struct InsertColumnMatch : public OpRewritePattern<BuildOp> {
  InsertColumnMatch(mlir::MLIRContext *context)
      : OpRewritePattern<BuildOp>(context, /*benefit=*/1) {}

  virtual LogicalResult matchAndRewrite(BuildOp op,
                                PatternRewriter &rewriter) const override {
    Value InputBoard = op.getToBuild();

    assert(InputBoard.hasOneUse() &&
           "Build operation should only ever operate on board with single use");

    auto InputBoardType = dyn_cast<BoardType>(InputBoard.getType());
    auto Rows = InputBoardType.getRows();
    auto Columns = InputBoardType.getColumns();
    auto Squares = InputBoardType.getSquares();
    if (Columns == 0)
      return failure();

    uint64_t RowAnd = -1;
    uint64_t RowOr = 0;
    for (auto Row : Squares) {
      RowAnd &= Row;
      RowOr |= Row;
    }
    int ColumnToRemove = -1;
    int Color;
    for (int i = 0; i < Columns; ++i) {
      uint64_t RowSelect = 1 << i;
      if (RowAnd & RowSelect) {
        ColumnToRemove = i;
        Color = 1;
        break;
      } else if (!(RowOr & RowSelect)) {
        ColumnToRemove = i;
        Color = 0;
        break;
      }
    }
    if (ColumnToRemove == -1) return failure();

    std::vector<int> NewBoardSquareValues(Rows);
    uint64_t LowMask = (1 << ColumnToRemove) - 1;
    uint64_t HighMask = -1 << (ColumnToRemove + 1);
    for (int i = 0; i < Rows; ++i) {
      NewBoardSquareValues[i] = (Squares[i] & LowMask) | ((Squares[i] & HighMask) >> 1);
    }
    llvm::ArrayRef<int> NewBoardSquares(NewBoardSquareValues);
    std::vector<int> NewColumnSquareValues(Rows, Color);
    llvm::ArrayRef<int> NewColumnSquares(NewColumnSquareValues);

    BoardType NewBoardType = BoardType::get(rewriter.getContext(), Rows, Columns - 1, NewBoardSquares);
    BoardType NewColumnType = BoardType::get(rewriter.getContext(), Rows, 1, NewBoardSquares);
    Operation *NewBuild = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.build"), {}, {NewBoardType});
    Operation *NewColumn = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.column"), {}, {NewColumnType});
    Operation *NewInsertColumn = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.insert_column"), {NewBuild->getResult(0), NewColumn->getResult(0)}, {InputBoardType});
    rewriter.replaceOp(op, NewInsertColumn);
    return success();
  }
};

void BuildOp::getCanonicalizationPatterns(::mlir::RewritePatternSet &results,
                                        ::mlir::MLIRContext *context) {
  results.add<InsertColumnMatch>(context);
}

}  // namespace cuttingbored
}  // namespace mlir
