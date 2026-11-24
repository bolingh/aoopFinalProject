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
        return {max_speed + get_rand(-mutation_rate, mutation_rate), 
                fov + get_rand(-mutation_rate, mutation_rate),
                size + get_rand(-mutation_rate, mutation_rate),
                mutation_rate + get_rand(-mutation_rate, mutation_rate)};
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
    Organism(vecc p, vecc v, double e, int t, Genome g, unique_ptr<IBrain> b)
        : pos(p), velo(v), ener(e), type(t), gen(g), brain(move(b)) {}
    virtual ~Organism() = default;
    vecc pos, velo;
    double ener;
    int type; // 1: sheep, 2: wolf, 3: bear
    Genome gen;
    unique_ptr<IBrain> brain;
    
    void decay_energy() {
        double cost = init_decay[type] * (0.5 + 0.5 * velo.len() / init_speed[type]);
        ener = max(0.0, ener - cost);
    }
    virtual Perception sense(const WorldContext &ctx) = 0;
    void act(Action action) {
        vecc des_dir = action.forward.normalized();
        if(des_dir.x != 0 || des_dir.y != 0) {
            vecc des_velo = des_dir * gen.max_speed;
            velo = velo * 0.8 + des_velo * 0.2; // to move smoother
            if(velo.len() > gen.max_speed) velo = velo.normalized() * gen.max_speed;
        }
        pos = pos + velo;
        double r = gen.size;
        if(pos.x - r < 0) { // left
            pos.x = r;
            velo.x = -velo.x;
        }
        else if(pos.x + r > init_world_width) { // right
            pos.x = init_world_width - r;
            velo.x = -velo.x;
        }
        if(pos.y - r < 0) { // ceil
            pos.y = r;
            velo.y = -velo.y;
        }
        else if(pos.y + r > init_world_height) { // floor
            pos.y = init_world_height - r;
            velo.y = -velo.y;
        }
    }
    virtual bool can_reproduce() const {return ener > 150;};
    virtual unique_ptr<Organism> reproduce() = 0;
    void tick(const WorldContext& ctx) {
        // decay_energy() -> sense() -> decide() -> act() -> reproduce()
        decay_energy();
        Perception p = sense(ctx);
        Action action = brain->decide(p);
        act(action);
    }
};

class Sheep : public Organism {
public:
    Sheep(vecc p, vecc v, double e, Genome g, unique_ptr<IBrain> b) 
        : Organism(p, v, e, 1, g, move(b)) {}
    Perception sense(const WorldContext &ctx) {
        Perception p;
        p.has_food = p.has_predator = false;
        p.food_dir = p.predator_dir = {0, 0};
        double fov_sq = gen.fov * gen.fov, min_food_dist_sq = fov_sq, min_predator_dist_sq = fov_sq;
        for(auto &plant : ctx.plants) {
            if(plant.is_eaten) continue;
            vecc diff = plant.pos - this->pos;
            double dist_sq = diff.len();
            if(dist_sq < min_food_dist_sq) {
                min_food_dist_sq = dist_sq;
                p.has_food = true;
                p.food_dir = diff.normalized();
            }
        }
        for(auto &other : ctx.orgs) {
            if(other->type > this->type) {
                vecc diff = other->pos - this->pos;
                double dist_sq = diff.len();
                if (dist_sq < min_predator_dist_sq) {
                    min_predator_dist_sq = dist_sq;
                    p.has_predator = true;
                    p.predator_dir = diff.normalized();
                }
            }
        }
        return p;
    }
    unique_ptr<Organism> reproduce() override { return nullptr; }
};

class Wolf : public Organism {
public:
    Wolf(vecc p, vecc v, double e, Genome g, unique_ptr<IBrain> b) 
        : Organism(p, v, e, 2, g, move(b)) {}
    Perception sense(const WorldContext&) override {
        Perception p;
        p.has_food = p.has_predator = false;
        p.food_dir = p.predator_dir = {0, 0};
        double fov_sq = gen.fov * gen.fov, min_food_dist_sq = fov_sq, min_predator_dist_sq = fov_sq;
        for(auto &other : ctx.orgs) {
            if(other->type < this->type) {
                vecc diff = other->pos - this->pos;
                double dist_sq = diff.len();
                if (dist_sq < min_food_dist_sq) {
                    min_food_dist_sq = dist_sq;
                    p.has_food = true;
                    p.food_dir = diff.normalized();
                }
            }
            else if(other->type > this->type) {
                vecc diff = other->pos - this->pos;
                double dist_sq = diff.len();
                if (dist_sq < min_predator_dist_sq) {
                    min_predator_dist_sq = dist_sq;
                    p.has_predator = true;
                    p.predator_dir = diff.normalized();
                }
            }
        }
        return p;
    }
    unique_ptr<Organism> reproduce() override { return nullptr; }
};

class Bear : public Organism {
public:
    Bear(vecc p, vecc v, double e, Genome g, unique_ptr<IBrain> b) 
        : Organism(p, v, e, 3, g, move(b)) {}
    Perception sense(const WorldContext&) override {
        Perception p;
        p.has_food = p.has_predator = false;
        p.food_dir = {0, 0};
        double fov_sq = gen.fov * gen.fov, min_food_dist_sq = fov_sq;
        for(auto &other : ctx.orgs) {
            if(other->type < this->type) { // other->type = 1, 2
                vecc diff = other->pos - this->pos;
                double dist_sq = diff.len();
                if (dist_sq < min_food_dist_sq) {
                    min_food_dist_sq = dist_sq;
                    p.has_food = true;
                    p.food_dir = diff.normalized();
                }
            }
        }
        return p;
    }
    unique_ptr<Organism> reproduce() override { return nullptr; }
};