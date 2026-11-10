#include "headers.h"
using namespace std;
using vecc = Vector2;

struct RenderEntity {
    double x, y, size;
    int type;
};

struct Genome {
    double max_speed, fov, size, mutation_rate;
};

class Organism {
public:
    virtual ~Organism() = default;
    vecc pos, velo;
    double ener;
    int type;
    Genome gen;
};

const int plant_type = 0;
struct Plant {
    vecc pos;
    double size, ener;
    bool is_eaten;
};

class World {
private:
    int width, height;
    vector<unique_ptr<Organism>> orgs;
    vector<Plant> plants;
public:
    World(int _width = 1, int _height = 1) : width(_width), height(_height) {}
    void step() {

    }
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
    vecc v;
    v.x = 2.3, v.y = 9;
    cout << v.x + v.y << '\n';
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
        .def("get_render_entities", &World::get_render_entities);
}