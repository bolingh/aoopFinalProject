#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <cstdlib>

struct RenderEntity {
    float x, y;
    float radius;
    int type; // 0: 食物(綠), 1: 草食(藍), 2: 肉食(紅)
};

// 內部模擬用的生物資料結構
struct SimpleAgent {
    float x, y;
    float vx, vy;
    float radius;
    int type;
};

class SimulationWorld {
private:
    std::vector<SimpleAgent> agents;
    int width = 1000;
    int height = 800;

public:
    SimulationWorld() {
        // 先隨機塞 15 隻生物進去測試移動與反彈
        for (int i = 0; i < 15; ++i) {
            agents.push_back({
                static_cast<float>(rand() % (width - 100) + 50),
                static_cast<float>(rand() % (height - 100) + 50),
                static_cast<float>((rand() % 7) - 3), // vx
                static_cast<float>((rand() % 7) - 3), // vy
                static_cast<float>(rand() % 10 + 8),  // radius
                rand() % 3                           // type
            });
        }
    }

    void step() {
        // C++ 負責算物理與邊界碰撞
        for (auto& a : agents) {
            a.x += a.vx;
            a.y += a.vy;

            // 撞牆反彈
            if (a.x - a.radius < 0 || a.x + a.radius > width)  a.vx *= -1;
            if (a.y - a.radius < 0 || a.y + a.radius > height) a.vy *= -1;
        }
    }

    std::vector<RenderEntity> get_render_entities() const {
        std::vector<RenderEntity> renders;
        renders.reserve(agents.size());
        for (const auto& a : agents) {
            renders.push_back({a.x, a.y, a.radius, a.type});
        }
        return renders;
    }
};

// pybind11 綁定
PYBIND11_MODULE(initialExample, m) {
    pybind11::class_<RenderEntity>(m, "RenderEntity")
        .def_readonly("x", &RenderEntity::x)
        .def_readonly("y", &RenderEntity::y)
        .def_readonly("radius", &RenderEntity::radius)
        .def_readonly("type", &RenderEntity::type);

    pybind11::class_<SimulationWorld>(m, "SimulationWorld")
        .def(pybind11::init<>())
        .def("step", &SimulationWorld::step)
        .def("get_render_entities", &SimulationWorld::get_render_entities);
}