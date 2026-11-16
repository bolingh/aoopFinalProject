#pragma once
struct Genome {
    double max_speed, fov, size, mutation_rate;
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
    virtual Perception sense(WorldContext ctx) = 0;
    virtual void act(Action action) = 0;
    virtual bool can_reproduce() const {return ener > 150;};
    unique_ptr<Organism> reproduce() = 0;
    void tick(WorldContext ctx) {
        // decay_energy() -> sense() -> decide() -> act() -> reproduce()
        decay_energy();
        Perception p = sense(ctx);
        Action action = brain->decide(p);
        act(action);
    }
};