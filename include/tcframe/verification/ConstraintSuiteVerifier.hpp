#pragma once

#include <map>
#include <vector>
#include <string>
#include <utility>

#include "tcframe/constraint/Constraint.hpp"
#include "tcframe/constraint/ConstraintGroup.hpp"
#include "tcframe/constraint/ConstraintSuite.hpp"
#include "tcframe/verification/ConstraintSuiteVerificationResult.hpp"

using std::map;
using std::move;
using std::string;
using std::vector;

namespace tcframe { namespace experimental {

class ConstraintSuiteVerifier {
private:
    ConstraintSuite constraintSuite_;

public:
    virtual ~ConstraintSuiteVerifier() {}

    ConstraintSuiteVerifier(ConstraintSuite constraintSuite)
            : constraintSuite_(move(constraintSuite))
    {}

    ConstraintSuiteVerificationResult verify(const set<int>& constraintGroupIds) const {
        map<int, vector<string>> unsatisfiedConstraintDescriptionsByConstraintGroupId;
        set<int> satisfiedButNotAssignedConstraintGroupIds;

        for (const ConstraintGroup& constraintGroup : constraintSuite_.individualConstraints()) {
            vector<string> unsatisfiedConstraintDescriptions;
            for (const Constraint& constraint : constraintGroup.constraints()) {
                if (!constraint.predicate()()) {
                    unsatisfiedConstraintDescriptions.push_back(constraint.description());
                }
            }

            if (constraintGroupIds.count(constraintGroup.id())) {
                if (!unsatisfiedConstraintDescriptions.empty()) {
                    unsatisfiedConstraintDescriptionsByConstraintGroupId[constraintGroup.id()] = unsatisfiedConstraintDescriptions;
                }
            } else {
                if (unsatisfiedConstraintDescriptions.empty()) {
                    satisfiedButNotAssignedConstraintGroupIds.insert(constraintGroup.id());
                }
            }
        }
        return ConstraintSuiteVerificationResult(unsatisfiedConstraintDescriptionsByConstraintGroupId, satisfiedButNotAssignedConstraintGroupIds);
    }
};

}}