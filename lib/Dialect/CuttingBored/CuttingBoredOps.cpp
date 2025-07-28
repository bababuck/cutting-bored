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

// Remove a column if solid color
struct InsertRowMatch : public OpRewritePattern<BuildOp> {
  InsertRowMatch(mlir::MLIRContext *context)
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


    int RowToRemove = -1;
    for (int Row = 0; Row < Rows; ++Row) {
      if ((Squares[Row] == 0) ||
          (Squares[Row] == ((1 << Columns) - 1)))
          RowToRemove = Row;
    }
    if (RowToRemove == -1) return failure();

    std::vector<int> NewBoardSquareValues(Squares);
    std::vector<int> NewRowSquareValues(1, Squares[RowToRemove]);
    NewBoardSquareValues.erase(NewBoardSquareValues.begin() + RowToRemove);

    llvm::ArrayRef<int> NewBoardSquares(NewBoardSquareValues);
    llvm::ArrayRef<int> NewRowSquares(NewRowSquareValues);

    BoardType NewBoardType = BoardType::get(rewriter.getContext(), Rows - 1, Columns, NewBoardSquares);
    BoardType NewRowType = BoardType::get(rewriter.getContext(), 1, Columns, NewRowSquares);
    Operation *NewBuild = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.build"), {}, {NewBoardType});
    Operation *NewRow = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.column"), {}, {NewRowType});
    Operation *NewInsertRow = rewriter.create(op.getLoc(), rewriter.getStringAttr("cuttingbored.insert_row"), {NewBuild->getResult(0), NewRow->getResult(0)}, {InputBoardType});
    rewriter.replaceOp(op, NewInsertRow);
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

  int getSquare(int Row, int Column, bool RowWise = true) {
    if (!RowWise)
      std::swap(Row, Column);
    return (Squares[Row] >> Column) & 1;
  }

  void setSquare(int Row, int Column, int Color) {
    uint64_t ClearMask = ~(1 << Column);
    uint64_t ColorMask = Color << Column;
    Squares[Row] = (Squares[Row] & ClearMask) | ColorMask;
  }

  void rotateRowOrColumn(int Dim, bool RowWise) {
    if (RowWise)
      rotateRow(Dim);
    else
      rotateColumn(Dim);
  }

  void rotateColumn(int Column) {
    for (int Row = 0; Row < (Rows / 2); ++Row) {
      int LowColor = getSquare(Row, Column);
      int HighColor = getSquare(Rows - Row - 1, Column);
      setSquare(Row, Column, HighColor);
      setSquare(Rows - Row - 1, Column, LowColor);
    }
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

static Operation* BuildRotateSeq(Board CurrBoard, const uint64_t RotateMask, PatternRewriter &Rewriter, const Location &Loc, bool RowWise) {
  int Dimension = RowWise ? CurrBoard.Rows : CurrBoard.Columns;
  for (int Dim = 1; Dim < Dimension; ++Dim) {
    if ((RotateMask >> Dim) & 1) {
      CurrBoard.rotateRowOrColumn(Dim, RowWise);
    }
  }

  Operation *PrevOp = nullptr;
  for (int Dim = Dimension - 1; Dim > 0; --Dim) {
    if ((RotateMask >> Dim) & 1) {
      if (!PrevOp) {
        BoardType NewBoardType = BoardType::get(Rewriter.getContext(), CurrBoard.Rows, CurrBoard.Columns, llvm::ArrayRef<int>(CurrBoard.Squares));
        PrevOp = Rewriter.create(Loc, Rewriter.getStringAttr("cuttingbored.build"), {}, {NewBoardType});
      }
      CurrBoard.rotateRowOrColumn(Dim, RowWise);
      BoardType NewBoardType = BoardType::get(Rewriter.getContext(), CurrBoard.Rows, CurrBoard.Columns, llvm::ArrayRef<int>(CurrBoard.Squares));
      auto OpName = RowWise ? "cuttingbored.rotate_row" : "cuttingbored.rotate_column";
      PrevOp = Rewriter.create(Loc, Rewriter.getStringAttr(OpName), {PrevOp->getResult(0)}, {NewBoardType});
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
    if (Columns <= 1 || Rows <= 1)
      return failure();

    Board CurrBoard(Columns, Rows, Squares);
    auto FindBestRotation = [](Board &CurrBoard, bool RowWise) -> std::pair<uint64_t, uint64_t> {
      Board RotatedBoard(CurrBoard);

      int RotateDimension = RowWise ? CurrBoard.Rows : CurrBoard.Columns;
      int OtherDimension = RowWise ? CurrBoard.Columns : CurrBoard.Rows;
      for (int Dim = 0; Dim < RotateDimension; ++Dim) {
        RotatedBoard.rotateRowOrColumn(Dim, RowWise);
      }

      std::unordered_map<uint64_t, int> RotateMaskCounts;
      uint64_t MaxMask;
      uint64_t MaxCount = 0;
      for (int i = 0; i < OtherDimension; ++i) {
        // Assume the first row will not be rotated
        // That is we will try to match it's color
        int Color = CurrBoard.getSquare(0, i, RowWise);

        // Find all possible rotate/don't rotate combinations to find which
        // if any will allow for removal of this column. That is, which rotations
        // will lead to the column to be all of one color.
        bool PossibleToRemove = true;
        std::vector<uint64_t> CurrRotateMasks(1, 0);
        for (int j = 1; j < RotateDimension; ++j) {
          bool GoodAsIs = CurrBoard.getSquare(j, i, RowWise) == Color;
          bool GoodReversed = RotatedBoard.getSquare(j, i, RowWise) == Color;
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
      return {MaxCount, MaxMask};
    };
    auto [MaxRowCount, MaxRowMask] = FindBestRotation(CurrBoard, /*RowWise*/true);
    auto [MaxColumnCount, MaxColumnMask] = FindBestRotation(CurrBoard, /*RowWise*/false);
    bool RowWise = MaxRowCount >= MaxColumnCount;
    auto MaxCount = RowWise ? MaxRowCount : MaxColumnCount;
    auto MaxMask = RowWise ? MaxRowMask : MaxColumnMask;
    if (MaxCount == 0) return failure();

    Operation *RotateSeq = BuildRotateSeq(CurrBoard, MaxMask, rewriter, op.getLoc(), RowWise);
    rewriter.replaceOp(op, RotateSeq);
    return success();
  }
};


void BuildOp::getCanonicalizationPatterns(::mlir::RewritePatternSet &results,
                                        ::mlir::MLIRContext *context) {
  results.add<InsertColumnMatch>(context);
  results.add<InsertRowMatch>(context);
  results.add<RotateRowMatch>(context);
}

}  // namespace cuttingbored
}  // namespace mlir
