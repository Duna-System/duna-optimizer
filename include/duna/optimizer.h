#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <duna/cost_function.h>
#include <Eigen/Dense>

namespace duna
{
    template <class Scalar = double>
    class Optimizer
    {
    public:
        using CostFunctionType = CostFunctionBase<Scalar>;

        Optimizer()
        {
            m_maximum_iterations = 15;
        }
        Optimizer(const Optimizer &) = delete;
        Optimizer &operator=(const Optimizer &) = delete;
        virtual ~Optimizer() = default;

        inline void setMaximumIterations(int max_iterations)
        {
            m_maximum_iterations = max_iterations;
        }

        void setCost(CostFunctionType *cost) { m_cost = cost; }
        virtual OptimizationStatus step(Scalar *x0) = 0;
        virtual OptimizationStatus minimize(Scalar *x0) = 0;

    protected:
        virtual bool hasConverged() = 0;
        CostFunctionType *m_cost;
        int m_maximum_iterations;
    };
}

#endif