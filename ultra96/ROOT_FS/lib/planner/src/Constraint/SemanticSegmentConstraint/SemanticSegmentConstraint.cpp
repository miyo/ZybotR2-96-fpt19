/**
 *  MIT License
 *
 *  Copyright (c) 2019 Yuya Kudo
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <Constraint/SemanticSegmentConstraint/SemanticSegmentConstraint.h>

namespace planner {
    SemanticSegmentConstraint::SemanticSegmentConstraint(const EuclideanSpace& space) :
        base::ConstraintBase(space) {
    }

    SemanticSegmentConstraint::SemanticSegmentConstraint(const EuclideanSpace&              space,
                                                         const std::vector<ConstraintType>& constraint,
                                                         const std::vector<uint32_t>&       each_dim_size) :
        base::ConstraintBase(space) {
        set(constraint, each_dim_size);
    }

    SemanticSegmentConstraint::~SemanticSegmentConstraint() {
    }

    void SemanticSegmentConstraint::set(const std::vector<ConstraintType>& constraint,
                                        const std::vector<uint32_t>&       each_dim_size) {
        constraint_    = constraint;
        each_dim_size_ = each_dim_size;
    }

    const std::vector<ConstraintType>& SemanticSegmentConstraint::getConstraintRef() const {
        return constraint_;
    }

    const std::vector<uint32_t>& SemanticSegmentConstraint::getEachDimSizeRef() const {
        return each_dim_size_;
    }

    bool SemanticSegmentConstraint::checkCollision(const State&  src,
                                                   const State&  dst,
                                                   const double& size) const {
        if(src.getDim() != dst.getDim() || getDim() != src.getDim()) {
            throw std::invalid_argument("[" + std::string(__PRETTY_FUNCTION__) + "] " +
                                        "State dimension is invalid");
        }

        // TODO: calc appropriate sampling period
        const auto vec = dst - src;
        for(auto ratio_i = 0.15; ratio_i < 1.0; ratio_i += 0.15) {
            auto center = src + (vec * ratio_i);
            if(checkConstraintType(center) == ConstraintType::NOENTRY) {
                return false;
            }

            if(size != 0) {
                // Up down left right
                for(size_t i = 0; i < center.vals.size() * 2; i++) {
                    auto target = center;
                    target.vals[i / 2] += (i % 2 == 0) ? size : -size;
                    if(checkConstraintType(target) == ConstraintType::NOENTRY) {
                        return false;
                    }
                }
                // Diagonal direction
                const auto diag_val = std::sin(4 / M_PI) * size;
                for(int i = 0; i < (1 << center.vals.size()); i++) {
                    auto target = center;
                    for(size_t j = 0; j < center.vals.size(); j++) {
                        target.vals[j] += (i & (1 << j)) ? diag_val : -diag_val;
                    }
                    if(checkConstraintType(target) == ConstraintType::NOENTRY) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    ConstraintType SemanticSegmentConstraint::checkConstraintType(const State& state) const {
        if(getDim() != state.getDim()) {
            throw std::invalid_argument("[" + std::string(__PRETTY_FUNCTION__) + "] " +
                                        "State dimension is invalid");
        }

        // get index on single dimension array which correspond with state
        uint32_t index = 0;
        for(size_t i = 0; i < state.getDim(); i++) {
            auto bound = space.getBound(i + 1);

            // return NOENTRY Type if the state is out of range
            if(state.vals[i] < bound.low || bound.high < state.vals[i]) {
                return ConstraintType::NOENTRY;
            }

            uint32_t product = 1;
            for(size_t j = 0; j < i; j++) {
                product *= each_dim_size_[j];
            }
            index += std::floor((state.vals[i] - bound.low) * each_dim_size_[i] / bound.getRange()) * product;
        }

        return constraint_[index];
    }
}

