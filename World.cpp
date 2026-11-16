#include "headers.h"

class World {
private:
    int width, height, org_num;
    vector<unique_ptr<Organism>> orgs;
    vector<Plant> plants;
public:
    World(int _width = 1000, int _height = 800, int _org_num = 10) : width(_width), height(_height), org_num(_org_num) {
        orgs.reserve(org_num);
        for(int i = 0; i < org_num; ++i) {
            vecc pos(get_rand((double)width), get_rand((double)height));
            double vx = get_rand(-1, 1), vy = get_rand(-1, 1);
            vecc velo(vecc(vx, vy).normalized());
            double ener = 50;
            int type = (int)get_rand(3) + 1;
            Genome gen = {1, 1, 1, 0.5};
            auto brain = make_unique<RuleBasedBrain>();
            orgs.push_back({pos, velo, ener, type, gen, brain});
        }
    }
    void step() {
        // for all org:orgs, org->tick() (moves)
        // check eating
        // clean deads, reproduce, spawn plants
    }
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
        .def_readonly("width", &World::width)
        .def_readonly("height", &World::height)
        .def("get_render_entities", &World::get_render_entities);
}