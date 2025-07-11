#include "lib/Dialect/CuttingBored/CuttingBoredOps.h"
#include "lib/Dialect/CuttingBored/CuttingBoredTypes.h"

#include "mlir/Dialect/CommonFolders.h"
#include "mlir/Dialect/Complex/IR/Complex.h"
#include "mlir/IR/PatternMatch.h"

namespace mlir {
namespace cuttingbored {

// Remove a column if solid color
struct InsertColumnMatch : public OpRewritePattern<Build> {
  InsertColumnMatch(mlir::MLIRContext *context)
      : OpRewritePattern<Build>(context, /*benefit=*/1) {}

  virtual LogicalResult matchAndRewrite(Build op,
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
    llvm::errs() << Rows << "HEHE\n";
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
    llvm::errs() << "HEHE\n";
    Operation *NewColumn = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.column"), {}, {NewColumnType});
    //Build NewBuild = rewriter.create<Build>(op.getLoc(), {NewBoardType});
    // ColumnOp NewColumn = rewriter.create<ColumnOp>(op.getLoc(), {NewColumnType});
    Operation *NewInsertColumn = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.insert_column"), {NewBuild->getResult(0), NewColumn->getResult(0)}, {InputBoardType});
    /*
    AddOp newAdd = rewriter.create<AddOp>(op.getLoc(), x, y);
    SubOp newSub = rewriter.create<SubOp>(op.getLoc(), x, y);
    MulOp newMul = rewriter.create<MulOp>(op.getLoc(), newAdd, newSub);
    */
    rewriter.replaceOp(op, NewInsertColumn);
    // We don't need to remove the original ops because MLIR already has
    // canonicalization patterns that remove unused ops.
    return success();
  }
};

void Build::getCanonicalizationPatterns(::mlir::RewritePatternSet &results,
                                        ::mlir::MLIRContext *context) {
  results.add<InsertColumnMatch>(context);
}

}  // namespace cuttingbored
}  // namespace mlir
