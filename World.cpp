#include "headers.h"

class World {
private:
    int width, height;
    vector<unique_ptr<Organism>> orgs;
    vector<Plant> plants;
    vecc rand_pos() {return {get_rand((double)width), get_rand((double)height)};}
    vecc rand_dir() {
        vecc dir(get_rand(-1.0, 1.0), get_rand(-1.0, 1.0));
        return dir.normalized();
    }
    WorldContext to_ctx() {return {orgs, plants, width, height};}
public:
    World() : width(init_world_width), height(init_world_height) {
        plants.resize(init_num[plant_type]);
        for(auto &plant : plants) {
            plant.pos = rand_pos();
            plant.size = init_sz[plant_type];
            plant.ener = init_ener[plant_type];
            plant.is_eaten = false;
        }
        for(int t = 1; t <= 3; ++t) {
            for(int i = 0; i < init_num[t]; ++i) {
                vecc p(rand_pos());
                Genome g = {init_speed[t], init_fov[t], init_sz[t], 0.1};
                auto b = make_unique<RuleBasedBrain>();
                orgs.push_back(create_organism(t, p, rand_dir() * init_speed[t], init_ener[t], g, move(b)));
            }
        }
    }
    void step() {
        // for all org:orgs, org->tick() (moves)
        // check eating
        // clean deads, reproduce, spawn plants
        WorldContext ctx = to_ctx();
        for(auto &org : orgs) org->tick(ctx);
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