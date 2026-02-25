#pragma once
#include "memory.h"
#include "v_math.h"

#include "../Offsets/offsets.h"
#include <vector>

struct Entity {
    uintptr_t pawn;
    int health;
    int team;
    Vector3 origin;
    Vector3 head;
};

class ESP {
public:
    void Update();
    const std::vector<Entity>& GetEntities() const { return entities; }

private:
    std::vector<Entity> entities;
};

extern ESP* esp;
