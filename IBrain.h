#pragma once
#include "DataStrucs.h"
#include <cstdlib>

class IBrain {
public:
    virtual ~IBrain() = default;
    virtual Action decide(const Perception& input) = 0;
};
/*
class RuleBasedBrain : public IBrain {
public:
    Action decide(const Perception& input) override {
        Action act;
        if(input.has_predator) { // has predator
            act.forward = input.predator_dir * (-1.0);
            return act;
        }
        else if(input.has_food) { // has food
            act.forward = input.food_dir;
            return act;
        }
        // move freely
        act.forward = {0.2, 0.0};
        return act;
    }
};

class RandomBrain : public IBrain {
public:
    Action decide(const Perception& input) override {
        double rx = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        double ry = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        
        Vector2 random_dir{rx, ry};
        return Action{ random_dir.normalized() };
    }
};
*/