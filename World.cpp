#include "headers.h"

using OrganismConstruc = unique_ptr<Organism>(*)(vecc, vecc, double, int, Genome, unique_ptr<IBrain>);
static unordered_map<int, OrganismConstruc> organism_constructors = {
    {1, [](vecc p, vecc v, double e, Genome g, unique_ptr<IBrain> b) -> unique_ptr<Organism> {
        return make_unique<Sheep>(p, v, e, 1, g, move(b));
    }},
    {2, [](vecc p, vecc v, double e, Genome g, unique_ptr<IBrain> b) -> unique_ptr<Organism> {
        return make_unique<Wolf>(p, v, e, 2, g, move(b));
    }},
    {3, [](vecc p, vecc v, double e, Genome g, unique_ptr<IBrain> b) -> unique_ptr<Organism> {
        return make_unique<Bear>(p, v, e, 3, g, move(b));
    }}
};

class World {
private:
    int width, height, org_num;
    vector<unique_ptr<Organism>> orgs;
    vector<Plant> plants;
    vecc rand_pos() {return {get_rand((double)width), get_rand((double)height)};}
public:
    World(int _width = 1000, int _height = 800, int _org_num = 10) : width(_width), height(_height), org_num(_org_num) {
        orgs.reserve(org_num);
        for(int i = 0; i < org_num; ++i) {
            vecc p(rand_pos());
            double vx = get_rand(-1, 1), vy = get_rand(-1, 1);
            vecc v(vecc(vx, vy).normalized());
            double e = 50;
            int t = get_rand_int(1, 3);
            Genome g = {1, 1, 1, 0.5};
            auto b = make_unique<RuleBasedBrain>();
            orgs.push_back(organism_constructors[t](p, v, e, g, move(b)));
        }
        plants.resize(init_num[plant_type]);
        for(auto &plant : plants) {
            plant.pos = rand_pos();
            plant.size = init_sz[plant_type];
            plant.ener = init_ener[plant_type];
            plant.is_eaten = false;
        }
    }
    void step() {
        // for all org:orgs, org->tick() (moves)
        // check eating
        // clean deads, reproduce, spawn plants
    }
    int get_width() {return width;}
    int get_height() {return height;}
    vector<RenderEntity> get_render_entities() {
        vector<RenderEntity> entities;
        for(auto &org : orgs)
            entities.push_back({org->pos.x, org->pos.y, org->gen.size, org->type});
        for(auto &plant : plants)
            entities.push_back({plant.pos.x, plant.pos.y, plant.size, plant_type});
        return entities;
    }
    WorldContext to_ctx() {return {orgs, plants, width, height};}
};

/*
int main() {
    for(int i = 1; i <= 10; ++i) {
        for(int j = 0; j < 3; ++j) cout << get_rand(0, i) << ' ';
        cout << '\n';
    }
}
*/

// pybind11
PYBIND11_MODULE(World, m) {
    pybind11::class_<RenderEntity>(m, "RenderEntity")
        .def_readonly("x", &RenderEntity::x)
        .def_readonly("y", &RenderEntity::y)
        .def_readonly("radius", &RenderEntity::size)
        .def_readonly("type", &RenderEntity::type);

    pybind11::class_<World>(m, "World")
        .def(pybind11::init<>())
        .def("step", &World::step)
        .def("get_width", &World::get_width)
        .def("get_height", &World::get_height)
        .def("get_render_entities", &World::get_render_entities);
}