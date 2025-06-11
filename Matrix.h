//
// Created by arsen on 18.05.25.
//

#ifndef MATRIX_H
#define MATRIX_H
#define MATRIX_SQUARE_MATRIX_IMPLEMENTED
#include <stdexcept>

class MatrixOutOfRange : public std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("") {
  }
};
class MatrixIsDegenerateError : public std::runtime_error {
 public:
  MatrixIsDegenerateError() : std::runtime_error("") {
  }
};

template <class T, size_t R, size_t C>
class Matrix {
  static_assert(R > 0 && C > 0, " ");
 public:
  T data[R][C];
  static size_t RowsNumber()  {
    return R;
  }
  static size_t ColumnsNumber()  {
    return C;
  }
  T& operator()(size_t row, size_t col) {
    return data[row][col];
  }
  const T& operator()(size_t row, size_t col) const {
    return data[row][col];
  }
  T& At(size_t row, size_t col) {
    if (row >= R || col >= C) {
      throw MatrixOutOfRange();
    }
    return data[row][col];
  }
  const T& At(size_t row, size_t col) const {
    if (row >= R || col >= C) {
      throw MatrixOutOfRange();
    }
    return data[row][col];
  }
  Matrix operator+(const Matrix& other) const {
    if (other.RowsNumber() != R || other.ColumnsNumber() != C) {
      throw std::invalid_argument("");
    }
    Matrix result;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        result(i, j) = data[i][j] + other(i, j);
      }
    }
    return result;
  }
  Matrix& operator+=(const Matrix& other) {
    if (other.RowsNumber() != R || other.ColumnsNumber() != C) {
      throw std::invalid_argument("");
    }
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        data[i][j] += other(i, j);
      }
    }
    return *this;
  }
  Matrix operator-(const Matrix& other) const {
    if (other.RowsNumber() != R || other.ColumnsNumber() != C) {
      throw std::invalid_argument("");
    }
    Matrix result;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        result(i, j) = data[i][j] - other(i, j);
      }
    }
    return result;
  }
  Matrix& operator-=(const Matrix& other) {
    if (other.RowsNumber() != R || other.ColumnsNumber() != C) {
      throw std::invalid_argument("");
    }
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        data[i][j] -= other(i, j);
      }
    }
    return *this;
  }
  Matrix operator*(const T& scalar) const {
    Matrix result;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        result(i, j) = data[i][j] * scalar;
      }
    }
    return result;
  }
  template <size_t P>
  Matrix<T, R, P>& operator*=(const Matrix<T, C, P>& other) {
    *this = *this * other;
    return *reinterpret_cast<Matrix<T, R, P>*>(this);
  }
  Matrix& operator*=(const T& scalar) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        data[i][j] *= scalar;
      }
    }
    return *this;
  }
  Matrix operator/(const T& scalar) const {
    if (scalar == T{}) {
      throw std::invalid_argument("");
    }
    Matrix result;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        result(i, j) = data[i][j] / scalar;
      }
    }
    return result;
  }
  Matrix& operator/=(const T& scalar) {
    if (scalar == T{}) {
      throw std::invalid_argument("");
    }
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        data[i][j] /= scalar;
      }
    }
    return *this;
  }
  template <size_t P>
  Matrix<T, R, P> operator*(const Matrix<T, C, P>& other) const {
    Matrix<T, R, P> result{};
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < P; ++j) {
        for (size_t k = 0; k < C; ++k) {
          result(i, j) += data[i][k] * other(k, j);
        }
      }
    }
    return result;
  }
  bool operator==(const Matrix& other) const {
    if (other.RowsNumber() != R || other.ColumnsNumber() != C) {
      throw std::invalid_argument("");
    }
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        if (data[i][j] != other(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
  bool operator!=(const Matrix& other) const {
    return !(*this == other);
  }
  friend std::istream& operator>>(std::istream& in, Matrix& mat) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        in >> mat(i, j);
      }
    }
    return in;
  }
  friend std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        out << mat(i, j);
        if (j + 1 < C) {
          out << " ";
        }
      }
      out << "\n";
    }
    return out;
  }
  void Transpose() {
    static_assert(R == C, " ");
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = i + 1; j < C; ++j) {
        std::swap(data[i][j], data[j][i]);
      }
    }
  }
  T Trace() const {
    static_assert(R == C, " ");
    T trace = T{};
    for (size_t i = 0; i < R; ++i) {
      trace += data[i][i];
    }
    return trace;
  }
  T Determinant() const {
    static_assert(R == C, " ");
    if constexpr (R == 0) {
      return T{1};
    } else if (R == 1) {
      return data[0][0];
    } else if (R == 2) {
      return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    } else {
      T det = T{};
      for (size_t j = 0; j < C; ++j) {
        det += data[0][j] * Decomposition(0, j);
      }
      return det;
    }
  }
  Matrix GetInversed() const {
    static_assert(R == C, " ");
    T det = Determinant();
    if (det == T{}) {
      throw MatrixIsDegenerateError();
    }
    Matrix result;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        result(j, i) = Decomposition(i, j) / det;
      }
    }
    return result;
  }
  void Inverse() {
    static_assert(R == C, " ");
    *this = GetInversed();
  }
 private:
  T Decomposition(const size_t row, const size_t col) const {
    return ((row + col) % 2 == 0 ? 1 : -1) * Minor(row, col);
  }
  T Minor(const size_t row, const size_t col) const {
    if constexpr (R == 1 && C == 1) {
      return T{1};
    } else {
      Matrix<T, R - 1, C - 1> minor;
      size_t minor_i = 0;
      size_t minor_j = 0;
      for (size_t i = 0; i < R; ++i) {
        if (i == row) {
          continue;
        }
        minor_j = 0;
        for (size_t j = 0; j < C; ++j) {
          if (j == col) {
            continue;
          }
          minor(minor_i, minor_j) = data[i][j];
          ++minor_j;
        }
        ++minor_i;
      }
      return minor.Determinant();
    }
  }
};

template <class T, size_t R, size_t C>
Matrix<T, R, C> operator*(const T& scalar, const Matrix<T, R, C>& matrix) {
  return matrix * scalar;
}
template <class T, size_t R, size_t C>
Matrix<T, C, R> GetTransposed(const Matrix<T, R, C>& mat) {
  Matrix<T, C, R> result;
  for (size_t i = 0; i < R; ++i) {
    for (size_t j = 0; j < C; ++j) {
      result(j, i) = mat(i, j);
    }
  }
  return result;
}
template <class T, size_t R, size_t C>
void Transpose(Matrix<T, R, C>& matrix) {
  static_assert(R == C, " ");
  matrix.Transpose();
}
template <class T, size_t R, size_t C>
T Trace(const Matrix<T, R, C>& matrix) {
  static_assert(R == C, " ");
  return matrix.Trace();
}
template <class T, size_t R, size_t C>
T Determinant(const Matrix<T, R, C>& matrix) {
  static_assert(R == C, " ");
  return matrix.Determinant();
}
template <class T, size_t R, size_t C>
void Inverse(Matrix<T, R, C>& matrix) {
  static_assert(R == C, " ");
  matrix.Inverse();
}
template <class T, size_t R, size_t C>
Matrix<T, R, C> GetInversed(const Matrix<T, R, C>& matrix) {
  static_assert(R == C, " ");
  return matrix.GetInversed();
}
#endif //MATRIX_H
