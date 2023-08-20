#pragma once

#include <Eigen/Dense>
#include <exception>

#include "duna_optimizer/covariance/covariance.h"
#include "duna_optimizer/loss_function/loss_function.h"
#include "duna_optimizer/model.h"
#include "duna_optimizer/types.h"

namespace duna_optimizer {
/* Base class for cost functions. */

template <class Scalar = double>
class CostFunctionBase {
 public:
  using Model = IBaseModel<Scalar>;
  using ModelPtr = typename Model::Ptr;
  using ModelConstPtr = typename Model::ConstPtr;
  using LossFunctionPtr = typename loss::ILossFunction<Scalar>::Ptr;
  using CovariancePtr = typename covariance::ICovariance<Scalar>::Ptr;

  CostFunctionBase() = default;

  CostFunctionBase(ModelPtr model, int num_residuals)
      : model_(model),
        num_residuals_(num_residuals)

  {
    loss_function_.reset(new loss::NoLoss<Scalar>());
    covariance_.reset(new covariance::IdentityCovariance<Scalar>(1));
  }

  CostFunctionBase(const CostFunctionBase &) = delete;
  CostFunctionBase &operator=(const CostFunctionBase &) = delete;
  virtual ~CostFunctionBase() = default;

  inline void setNumResiduals(int num_residuals) { num_residuals_ = num_residuals; }
  inline void setLossFunction(LossFunctionPtr loss_function) { loss_function_ = loss_function; }

  // Setup internal state of the model. Runs at the beggining of the
  // optimization loop.
  virtual void update(const Scalar *x) { model_->update(x); }

  /// @brief Computes  || ∑_i f_i(x) ||²
  /// @param x
  /// @return
  virtual Scalar computeCost(const Scalar *x) = 0;
  virtual Scalar linearize(const Scalar *x, Scalar *hessian, Scalar *b) = 0;

  // Initialize internal variables.
  virtual void init(const Scalar *x, Scalar *hessian, Scalar *b) = 0;

 protected:
  int num_residuals_;

  // Interfaces
  ModelPtr model_;
  LossFunctionPtr loss_function_;
  CovariancePtr covariance_;
};
}  // namespace duna_optimizer