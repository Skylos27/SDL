#pragma once

#include "../include/component.hpp"

#include <chrono>
#include <map>

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
            FOLLOW = 4,
            PLAYER_CONTROLLED = 8,
        } AIType;

        struct Vector2 {
            int x, y;
        };

        struct Vector2F {
            float x, y;
        };

        struct Position {
            Vector2F pos;
        };

        struct CharacterType {
            CharacterEnumType type;
        };

        struct CharacterInfo {
            CharacterGender gender;
            std::chrono::high_resolution_clock::time_point lastSpeedBoost;
            std::chrono::high_resolution_clock::time_point lastBirthGiven;
            std::chrono::high_resolution_clock::time_point lastSheepEaten;
        };

        struct AITypeComponent {
            AIType type;
        };

        struct DirectionComponent {
            Vector2 vec;
            float baseVelocity;
            float velocity;
            bool locked;
            std::chrono::high_resolution_clock::time_point lastLocked;
            float lockTime;
        };

        struct DogBoundary {
            Vector2F boundary;
            Vector2F current;
        };
        
    };
}