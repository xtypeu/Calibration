#pragma once

#include <opencv2/opencv.hpp>

#include <g2o/core/sparse_optimizer.h>
#include <g2o/solvers/cholmod/linear_solver_cholmod.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/solvers/eigen/linear_solver_eigen.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/solver.h>

#include "alias.h"

namespace slam {

class Optimizer
{
public:
    Optimizer();

    void adjust( std::list< StereoKeyFramePtr > &frames );

protected:
    static const int m_optimizationsCount = 10;

};

}
