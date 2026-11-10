#pragma once
struct Vector2 {
    double x, y;
    Vector2 normalized() {
        double len = sqrt(x * x + y * y);
        if(abs(len) <= 1e-8) return {0, 0};
        return {x / len, y / len};
    }
};

struct Perception {
    bool has_food, has_predator;
    Vector2 food_dir, predator_dir;
};

struct Action {
    Vector2 forward;
};