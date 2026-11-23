#pragma once
struct Vector2 {
    double x, y;
    Vector2(double _x = 1, double _y = 0) : x(_x), y(_y) {}
    double len() const {
        return sqrt(x * x + y * y);
    }
    Vector2 normalized() const {
        double l = len();
        if(abs(l) <= 1e-8) return {0, 0};
        return {x / l, y / l};
    }
    Vector2 operator+(const Vector2 other) const {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 operator-(const Vector2 other) const {
        return Vector2(x - other.x, y - other.y);
    }
};

Vector2 operator*(double multi, Vector2 v) {
    return Vector2(multi * v.x, multi * v.y);
}

Vector2 operator*(Vector2 v, double multi) {
    return Vector2(multi * v.x, multi * v.y);
}

struct Perception {
    bool has_food, has_predator;
    Vector2 food_dir, predator_dir;
};

struct Action {
    Vector2 forward;
};

struct RenderEntity {
    double x, y, size;
    int type;
};

using vecc = Vector2;