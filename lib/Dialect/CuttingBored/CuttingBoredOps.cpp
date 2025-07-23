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
    BoardType NewColumnType = BoardType::get(rewriter.getContext(), Rows, 1, NewColumnSquares);
    Operation *NewBuild = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.build"), {}, {NewBoardType});
    Operation *NewColumn = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.column"), {}, {NewColumnType});
    Operation *NewInsertColumn = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.insert_column"), {NewBuild->getResult(0), NewColumn->getResult(0)}, {InputBoardType});
    rewriter.replaceOp(op, NewInsertColumn);
    return success();
  }
};

class Board {
public:
  int Columns;
  int Rows;
  std::vector<int> Squares;
public:
  Board(const int Columns_, const int Rows_, const std::vector<int> &Squares_):Columns(Columns_), Rows(Rows_), Squares(Squares_){}
  Board(const Board &other):Columns(other.Columns), Rows(other.Rows), Squares(other.Squares) {}

  int getSquare(int Row, int Column) {
    return (Squares[Row] >> Column) & 1;
  }
  void rotateRow(int Row) {
    uint64_t EndMask = 1 << (Columns - 1);
    uint64_t StartMask = 1;
    uint64_t Rotated = 0;
    uint64_t RowData = Squares[Row];
    for (int i = 0; i < Columns; ++i) {
      if (RowData & StartMask) {
        Rotated |= EndMask;
      }
      EndMask >>= 1;
      StartMask <<= 1;
    }
    Squares[Row] = Rotated;
  }
};

static Operation* BuildRotateSeq(Board CurrBoard, const uint64_t RotateMask, PatternRewriter &Rewriter, const Location &Loc) {
  for (int Row = 1; Row < CurrBoard.Rows; ++Row) {
    if ((RotateMask >> Row) & 1) {
      CurrBoard.rotateRow(Row);
    }
  }

  Operation *PrevOp = nullptr;
  for (int Row = CurrBoard.Rows - 1; Row > 0; --Row) {
    if ((RotateMask >> Row) & 1) {
      if (!PrevOp) {
        BoardType NewBoardType = BoardType::get(Rewriter.getContext(), CurrBoard.Rows, CurrBoard.Columns, llvm::ArrayRef<int>(CurrBoard.Squares));
        PrevOp = Rewriter.create(Loc, Rewriter.getStringAttr("cuttingbored.build"), {}, {NewBoardType});
      }
      CurrBoard.rotateRow(Row);
      BoardType NewBoardType = BoardType::get(Rewriter.getContext(), CurrBoard.Rows, CurrBoard.Columns, llvm::ArrayRef<int>(CurrBoard.Squares));
      PrevOp = Rewriter.create(Loc, Rewriter.getStringAttr("cuttingbored.rotate_row"), {PrevOp->getResult(0)}, {NewBoardType});
    }
  }
  return PrevOp;
}

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

    Board CurrBoard(Columns, Rows, Squares);
    Board RotatedBoard(CurrBoard);

    for (int Row = 0; Row < Rows; ++Row) {
      RotatedBoard.rotateRow(Row);
    }

    std::unordered_map<uint64_t, int> RotateMaskCounts;
    uint64_t MaxMask;
    uint64_t MaxCount = 0;
    for (int i = 0; i < Columns; ++i) {
      // Assume the first row will not be rotated
      // That is we will try to match it's color
      int Color = CurrBoard.getSquare(0, i);

      // Find all possible rotate/don't rotate combinations to find which
      // if any will allow for removal of this column. That is, which rotations
      // will lead to the column to be all of one color.
      bool PossibleToRemove = true;
      std::vector<uint64_t> CurrRotateMasks(1, 0);
      for (int j = 1; j < Rows; ++j) {
        bool GoodAsIs = CurrBoard.getSquare(j, i) == Color;
        bool GoodReversed = RotatedBoard.getSquare(j, i) == Color;
        if (GoodAsIs && GoodReversed) {
          std::vector<uint64_t> NewMasks = CurrRotateMasks;
          for (auto &Mask : NewMasks) {
            Mask |= (1 << j);
          }
          CurrRotateMasks.insert(CurrRotateMasks.end(), NewMasks.begin(), NewMasks.end());
        } else if (GoodReversed) {
          for (auto &Mask : CurrRotateMasks) {
            Mask |= (1 << j);
          }
        } else if (!GoodAsIs) {
          PossibleToRemove = false;
          break;
        }
      }
      if (PossibleToRemove) {
        // If at least one valid rotation scheme for this column, add the bitmasks
        // to the overall bitmask collection, incrementing their counters
        for (auto &Mask : CurrRotateMasks) {
          ++(RotateMaskCounts[Mask]);
          if (RotateMaskCounts[Mask] > MaxCount) {
            MaxCount = RotateMaskCounts[Mask];
            MaxMask = Mask;
          }
        }
      }
    }
    if (MaxCount == 0) return failure();

    Operation *RotateSeq = BuildRotateSeq(CurrBoard, MaxMask, rewriter, op.getLoc());
    rewriter.replaceOp(op, RotateSeq);
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
