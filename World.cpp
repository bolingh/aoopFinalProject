#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <bits/stdc++.h>
#include "dataStrucs.h"
using namespace std;
using vecc = Vector2;

struct RenderEntity {
    double x, y, size;
    int type;
};

struct Organism {
    vecc pos, velo;
    double size, ener;
    int type;
};

class World {
private:
    int width, height;
    vector<Organism*> orgs;
public:
    World() : width(1), height(1) {}
    World(int _width, int _height) : width(_width), height(_height) {}
    void step() {

    }
    vector<RenderEntity> get_render_entities() {
        vector<RenderEntity> ans;
        for(auto &org : orgs)
            ans.push_back({org->pos.x, org->pos.y, org->size, org->type});
        return ans;
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