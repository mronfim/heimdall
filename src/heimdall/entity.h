#pragma once

namespace Heimdall {

class World;
struct Entity {
    int id;

    friend bool operator<(const Entity &l, const Entity &r) { return l.id < r.id; }
};

}