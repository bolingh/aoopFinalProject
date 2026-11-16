#pragma once
const int plant_type = 0;
class Plant {
public:
    vecc pos;
    double size, ener;
    bool is_eaten;
};

struct Genome {
    double max_speed, fov, size, mutation_rate;
    Genome mutate() {
        double r = get_rand(-mutation_rate, mutation_rate);
        return {max_speed + r, 
                fov + r,
                size + r,
                mutation_rate + r};
    }
};

class Organism;

struct WorldContext {
    const vector<unique_ptr<Organism>> &orgs;
    const vector<Plant> &plants;
    int world_width, world_height;
};

class Organism {
public:
    virtual ~Organism() = default;
    vecc pos, velo;
    double ener;
    int type; // 1: sheep, 2: wolf, 3: bear
    Genome gen;
    unique_ptr<IBrain> brain;
    
    virtual void decay_energy() = 0;
    virtual Perception sense(const WorldContext &ctx) = 0;
    virtual void act(Action action) = 0;
    virtual bool can_reproduce() const {return ener > 150;};
    virtual unique_ptr<Organism> reproduce() = 0;
    void tick(const WorldContext &ctx) {
        // decay_energy() -> sense() -> decide() -> act() -> reproduce()
        decay_energy();
        Perception p = sense(ctx);
        Action action = brain->decide(p);
        act(action);
    }
};

class Sheep : public Organism {
public:
    Sheep() {}
};