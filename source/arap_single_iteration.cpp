#include "../include/arap_single_iteration.h"
#include <igl/polar_svd3x3.h>
#include <igl/min_quad_with_fixed.h>

void arap_single_iteration(const igl::min_quad_with_fixed_data<double> & data,
							const Eigen::SparseMatrix<double> & K,
							Eigen::Matrix<double, -1, -1, Eigen::RowMajor> & bc,
							Eigen::Matrix<double, -1, -1, Eigen::RowMajor> & U)
{
	Eigen::MatrixXd C = K.transpose() * U;
  
	// global step
	Eigen::MatrixXd R(3*data.n, 3);
	for (int k=0; k<data.n; k++){
	  	Eigen::Matrix3d Rk;
	  	Eigen::Matrix3d Ck = C.block(k*3, 0, 3, 3);
	  	igl::polar_svd3x3(Ck, Rk);
	  	R.block(k*3, 0, 3, 3) = Rk;    
	}
  
	// from min_quad_with_fixed_solve code
	Eigen::VectorXd Beq;
	Eigen::MatrixXd B = K*R;
  
	igl::min_quad_with_fixed_solve(data, B, bc, Beq, U);
  }