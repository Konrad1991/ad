#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

template <typename T, typename L>
class VEC {

private:
  const L& data_;
  bool ismatrix_;
  int nrows_;
  int ncols_;

public:
  VEC(const L& data, bool ismatrix = false, int nrows = 0, int ncols = 0)
      : data_(data), ismatrix_(ismatrix), nrows_(nrows), ncols_(ncols) {}

  T operator[](const int i) const { return data_[i]; }

  int size() const { return data_.size(); }

  bool im() const { return ismatrix_; }

  int nr() const { return nrows_; }

  int nc() const { return ncols_; }

  L data() const {
    return data;
  }
};

template<typename T, typename L, typename R>
class VVEXP {

private:
  const L& r; 
  bool ismatrix;
  int nrow_;
  int ncol_;
  double exponent;

public:

  VVEXP(const L &a, bool r_ismatrix, int r_rows, int r_cols, double exponent_) : r(a), exponent(exponent_) {

    if( r_ismatrix == true) {
      ismatrix = r_ismatrix;
      nrow_ = r_rows;
      ncol_ = r_cols;
    }

  }

   T operator[](const int i) const {
     return pow(r[i% r.size()], exponent);
   }

   int size() const {
     return r.size();
   }

   bool im() const {
     return ismatrix;
   }

   int nc() const {
     return ncol_;
   }

   int nr() const {
     return nrow_;
   }

};

template<typename T, typename L>
inline VEC< T, VVEXP< T, L, int> > operator^(const VEC<T, L>& a, const int exponent) {

  bool ismatrix_ = false;
  int nrows_ = 0;
  int ncols_ = 0;

  if(a.im() == true) {
    ismatrix_ = true;
    nrows_ = a.nr();
    ncols_ = a.nc();
  }

  VEC<T, VVEXP<T, L, int> > ret (VVEXP<T, L, int>(a.data(), a.im(), a.nr(), a.nc(), exponent) );

  ret.ismatrix_ = ismatrix_;
  ret.ncols_ = ncols_;
  ret.nrows_ = nrows_;

  return ret;
}

int main() {
  // create a vector of doubles
  vector<double> data = {1, 2, 3, 4};

  // create a VEC object from the vector
  VEC<double, vector<double>> v(data);

  // raise v to the power of 2
  VEC<double, VVEXP<double, vector<double>, double>> result = v ^ 2;

  // print the result
  for (int i = 0; i < result.size(); i++) {
    cout << result[i] << " ";
  }
  cout << endl;

  return 0;
}
