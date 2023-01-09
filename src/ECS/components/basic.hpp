#pragma once

#include "../include/component.hpp"

namespace ECS {
    namespace Components {

        typedef enum {
            SHEEP = 0,
            WOLF = 1,
            DOG = 2,
            PLAYER = 4,
        } CharacterEnumType;

        struct Vector2 {
            int x, y;
        };

        struct Position {
            Vector2 pos;
        };

        struct CharacterType {
            CharacterEnumType type;
        };
        
    };
}