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

// Remove a column if solid color
struct RotateRowMatch : public OpRewritePattern<BuildOp> {
  RotateRowMatch(mlir::MLIRContext *context)
      : OpRewritePattern<BuildOp>(context, /*benefit=*/1) {}

  virtual LogicalResult matchAndRewrite(BuildOp op,
                                        PatternRewriter &rewriter) const override {
    Value InputBoard = op.getToBuild();

    assert(InputBoard.hasOneUse() &&
           "Build operation should only ever operate on board with single use");

    const auto InputBoardType = dyn_cast<BoardType>(InputBoard.getType());
    const auto Rows = InputBoardType.getRows();
    const auto Columns = InputBoardType.getColumns();
    const auto Squares = InputBoardType.getSquares();
    llvm::SmallVector<int> RotatedSquares(Squares.begin(), Squares.end());
    if (Columns == 0)
      return failure();

    auto RotateRow = [Columns](uint64_t RowData) -> uint64_t {
      uint64_t EndMask = 1 << (Columns - 1);
      uint64_t StartMask = 1;
      uint64_t Rotated = 0;
      for (int i = 0; i < Columns; ++i) {
        if (RowData & StartMask) {
          Rotated |= EndMask;
        }
        EndMask >>= 1;
        StartMask <<= 1;
      }
      return Rotated;
    };
    for (auto &Row : RotatedSquares) {
      Row = RotateRow(Row);
    }

    std::unordered_map<uint64_t, int> RotateHashCounts;
    uint64_t MaxHash;
    uint64_t MaxCount = 0;
    for (int i = 0; i < Columns; ++i) {
      // Assume the first row will not be rotated
      // That is we will try to match it's color
      auto GetSquare = [&Squares](const int Row, const int Column) -> int {
        return (Squares[Row] >> Column) & 1;
      };
      int Color = GetSquare(0, i);
      bool PossibleToRemove = true;
      std::vector<uint64_t> CurrRotateHashes(1, 0);
      for (int j = 1; j < Rows; ++j) {
        bool GoodAsIs = GetSquare(j, i) == Color;
        // Can't rotate if middle column
        bool GoodReversed = ((Columns - i - 1) != i) && (GetSquare(j, Columns - i - 1) == Color);
        if (GoodAsIs && GoodReversed) {
          std::vector<uint64_t> NewHashes = CurrRotateHashes;
          for (auto &Hash : NewHashes) {
            Hash |= (1 >> j);
          }
          CurrRotateHashes.insert(CurrRotateHashes.end(), NewHashes.begin(), NewHashes.end());
        } else if (GoodReversed) {
          for (auto &Hash : CurrRotateHashes) {
            Hash |= (1 >> j);
          }
        } else if (!GoodAsIs) {
          PossibleToRemove = false;
          break;
        }
      }
      if (PossibleToRemove) {
        for (auto &Hash : CurrRotateHashes) {
          ++(RotateHashCounts[Hash]);
          if (RotateHashCounts[Hash] > MaxCount) {
            MaxCount = RotateHashCounts[Hash];
            MaxHash = Hash;
          }
        }
      }
    }
    if (MaxCount == 0) return failure();
    std::vector<int> NewBoardSquareValues(Squares);
    Operation *RotateOp;
    /*
    class RotateSeqBuilder {
      int Rows;
      uint64_t MaxHash;
      PatternRewriter &Rewriter;
      
      };*/
    std::function<Operation*(int)> BuildRotateSeq = [&rewriter, &NewBoardSquareValues, &Rows, &Columns, &MaxHash, &RotatedSquares, &BuildRotateSeq, &op](int Row) -> Operation* {
      if (Row == Rows)
        return nullptr;
      if ((MaxHash >> Row) & 1) {
        NewBoardSquareValues[Row] = RotatedSquares[Row];
        llvm::ArrayRef<int> NewBoardSquares(NewBoardSquareValues);
        Operation *InputOp = BuildRotateSeq(Row++);
        BoardType NewBoardType = BoardType::get(rewriter.getContext(), Rows, Columns, NewBoardSquares);
        if (InputOp == nullptr) {
          return rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.build"), {}, {NewBoardType});
        } else {
          return rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.rotate_row"), {InputOp->getResult(0)}, {NewBoardType});
        }
      } else {
        return BuildRotateSeq(Row++);
      }
    };
    rewriter.replaceOp(op, BuildRotateSeq(1));

    return success();
  }
};


void BuildOp::getCanonicalizationPatterns(::mlir::RewritePatternSet &results,
                                        ::mlir::MLIRContext *context) {
  results.add<InsertColumnMatch>(context);
  results.add<RotateRowMatch>(context);
}

}  // namespace cuttingbored
}  // namespace mlir
