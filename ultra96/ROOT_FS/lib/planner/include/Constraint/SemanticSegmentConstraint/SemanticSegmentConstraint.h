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

#ifndef LIB_INCLUDE_CONSTRAINT_SEMANTICSEGMENTCONSTRAINT_SEMANTICSEGMENTCONSTRAINT_H_
#define LIB_INCLUDE_CONSTRAINT_SEMANTICSEGMENTCONSTRAINT_SEMANTICSEGMENTCONSTRAINT_H_

#include <iostream>
#include <vector>
#include <cstdint>

#include <State/State.h>
#include <Constraint/ConstraintBase.h>

namespace planner {

    /**
     *  Super class of planner::ConstraintBase
     *  This class express constraint as like an image or multidimensional array
     *  and nomalize automatically for space size when call checkConstraintType()
     */
    class SemanticSegmentConstraint : public base::ConstraintBase {
    public:

        /**
         *  Constructor(SemanticSegmentConstraint)
         *  @space: target space
         */
        explicit SemanticSegmentConstraint(const EuclideanSpace& space);

        /**
         *  Constructor(SemanticSegmentConstraint)
         *  @space:         target space
         *  @constraint:    multidimensional array express as one dimensional array
         *                  (e.g. '(x, y)' -> 'x + y * x_size' where 2 dimensions)
         *  @each_dim_size: each dimension size of constraint you set
         */
        SemanticSegmentConstraint(const EuclideanSpace&              space,
                                  const std::vector<ConstraintType>& constraint,
                                  const std::vector<uint32_t>&       each_dim_size);

        ~SemanticSegmentConstraint() override;

        void set(const std::vector<ConstraintType>& constraint,
                 const std::vector<uint32_t>&       each_dim_size);

        const std::vector<ConstraintType>& getConstraintRef() const;

        const std::vector<uint32_t>& getEachDimSizeRef() const;

        bool checkCollision(const State&  src,
                            const State&  dst,
                            const double& size = 0) const override;

        ConstraintType checkConstraintType(const State& state) const override;

    private:
        std::vector<ConstraintType> constraint_;
        std::vector<uint32_t>       each_dim_size_;
    };
}

#endif /* LIB_INCLUDE_CONSTRAINT_SEMANTICSEGMENTCONSTRAINT_SEMANTICSEGMENTCONSTRAINT_H_ */
