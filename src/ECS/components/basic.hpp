#pragma once

#include "../include/component.hpp"

namespace ECS {
    namespace Components {

        typedef enum : std::uint16_t {
            SHEEP = 0,
            WOLF = 1,
            DOG = 2,
            PLAYER = 4,
        } CharacterEnumType;

        typedef enum : std::uint16_t {
            MALE = 1,
            FEMALE = 2,
        } CharacterGender;

        typedef enum : std::uint16_t{
            AGGRESIVE = 1,
            PASSIVE = 2,
        } AIType;

        struct Vector2 {
            int x, y;
        };

        struct Position {
            Vector2 pos;
        };

        struct CharacterType {
            CharacterEnumType type;
        };

        struct CharacterInfo {
            CharacterGender gender;
            float lastSpeedBoost;
            float lastBirthGiven;
        };

        struct AITypeComponent {
            AIType type;
        };

        struct DirectionComponent {
            Vector2 vec;
            int velocity;
        };
        
    };
}