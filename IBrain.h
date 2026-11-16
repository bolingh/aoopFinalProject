#pragma once
#include "DataStrucs.h"
#include <cstdlib>

class IBrain {
public:
    virtual ~IBrain() = default;
    virtual Action decide(const Perception& p) = 0;
};

class RuleBasedBrain : public IBrain {
public:
    Action decide(const Perception& p) override {
        Action act;
        if(p.has_predator) { // has predator
            act.forward = p.predator_dir * (-1.0);
            return act;
        }
        else if(p.has_food) { // has food
            act.forward = p.food_dir;
            return act;
        }
        // move freely
        act.forward = {0.2, 0.0};
        return act;
    }
};

class RandomBrain : public IBrain {
public:
    Action decide(const Perception& p) override {
        double rx = get_rand(), ry = get_rand();
        Vector2 random_dir{rx, ry};
        return Action{ random_dir.normalized() };
    }
};
