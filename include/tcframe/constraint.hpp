#ifndef TCFRAME_CONSTRAINT_H
#define TCFRAME_CONSTRAINT_H

#include <functional>
#include <string>
#include <vector>

using std::function;
using std::string;
using std::vector;

namespace tcframe {

class Constraint {
public:
    Constraint(function<bool()> predicate, string description)
            : predicate(predicate), description(description) { }

    bool isSatisfied() {
        return predicate();
    }

    string getDescription() {
        return description;
    }

private:
    function<bool()> predicate;
    string description;
};

class Subtask {
public:
    Subtask(int id)
            : id(id) { }

    void addConstraint(Constraint* constraint) {
        constraints.push_back(constraint);
    }

    int getId() {
        return id;
    }

    vector<Constraint*> getConstraints() {
        return constraints;
    }

private:
    int id;
    vector<Constraint*> constraints;
};

class ConstraintsCollector {
public:
    ConstraintsCollector()
            : curSubtaskId(0) { }

    void newSubtask() {
        curSubtaskId++;
        subtasks.push_back(new Subtask(curSubtaskId));
    }

    void addConstraint(function<bool()> predicate, string description) {
        if (subtasks.empty()) {
            subtasks.push_back(new Subtask(-1));
        }

        subtasks.back()->addConstraint(new Constraint(predicate, description));
    }

    vector<Subtask*> collectSubtasks() {
        return subtasks;
    }

private:
    int curSubtaskId;
    vector<Subtask*> subtasks;
};

}

#endif
