#include "./AI.hpp"

#include <iostream>
#include <cmath>

extern ECS::Coordinator coordinator;

void ECS::Systems::AISystem::init(int w, int h, const std::function<ECS::Entity(float, float)> &lambda)
{
    _windowSize.x = w;
    _windowSize.y = h;
    
    _start = _clock.now();
    _lambda = lambda;
}

bool ECS::Systems::AISystem::entitiesOverlapped(const ECS::Entity &a, const ECS::Entity &b)
{
    auto &posA = coordinator.GetComponent<ECS::Components::Position>(a);
    auto &posB = coordinator.GetComponent<ECS::Components::Position>(b);
    auto &sizeA = coordinator.GetComponent<ECS::Components::ImageTexture>(a);
    auto &sizeB = coordinator.GetComponent<ECS::Components::ImageTexture>(b);

    return !(posA.pos.x > posB.pos.x+sizeB.w || posA.pos.x+sizeA.w < posB.pos.x || posA.pos.y > posB.pos.y+sizeB.h || posA.pos.y+sizeA.h < posB.pos.y);
}

std::optional<ECS::Components::Vector2F>ECS::Systems::AISystem::findClosestSheep(const ECS::Components::Position &posWolf)
{
    float closest = 0.0f;
    Entity closestEntity;
    bool any = false;

    for (const auto &x : Entities) {
        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(x);

        if (type.type == ECS::Components::CharacterEnumType::SHEEP) {
            const auto &posSheep = coordinator.GetComponent<ECS::Components::Position>(x);
            float tmpClosest = std::sqrt(std::abs(posWolf.pos.x - posSheep.pos.x) + std::abs(posWolf.pos.y - posSheep.pos.y));
            
            if (tmpClosest < closest || closest == 0.0f) {
                closestEntity = x;
                closest = tmpClosest;
                any = true;
            }
        }
    }

    if (any)
        return coordinator.GetComponent<ECS::Components::Position>(closestEntity).pos;
    else
        return {};
}

ECS::Components::Vector2F ECS::Systems::AISystem::findClosestDog()
{
    float closest = 0.0f;
    Entity dog;

    for (const auto &x : Entities) {
        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(x);

        if (type.type == ECS::Components::CharacterEnumType::DOG) {
            dog = x;
            break;
        }
    }

    return coordinator.GetComponent<ECS::Components::Position>(dog).pos;
}

bool ECS::Systems::AISystem::isDogClose(const ECS::Components::Position &posWolf)
{
    ECS::Components::Vector2F dogPos = findClosestDog();

    ECS::RectF r1{ posWolf.pos.x - 100, posWolf.pos.y - 100, 100, 300 };
    ECS::RectF r2{ dogPos.x - 100, dogPos.y - 100, 100, 300 };

    if (ECS::rectIntersects(r1, r2))
        return true;
    return false;
}

int ECS::Systems::AISystem::sheepNumber()
{
    auto sheepNumber = 0;
    for (const auto &x : Entities) {
        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(x);

        if (type.type == ECS::Components::CharacterEnumType::SHEEP) {
            sheepNumber += 1;
        }
    }

    return sheepNumber;
}

void ECS::Systems::AISystem::update()
{
    auto now = _clock.now();
    auto interval = now - _start;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() < 2.5f) return;

    _start = _clock.now();

    for (auto &entity : Entities) {
        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(entity);
        auto &info = coordinator.GetComponent<ECS::Components::CharacterInfo>(entity);
        const auto &aiType = coordinator.GetComponent<ECS::Components::AITypeComponent>(entity);
        auto &position = coordinator.GetComponent<ECS::Components::Position>(entity);
        auto &image = coordinator.GetComponent<ECS::Components::ImageTexture>(entity);
        auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(entity);

        if (aiType.type == ECS::Components::AIType::PASSIVE) {
            if (position.pos.x + direction.vec.x * direction.velocity < 0 || position.pos.x + direction.vec.x * direction.velocity > _windowSize.x - image.w) {
                direction.vec.x = -direction.vec.x;
            }

            if (position.pos.y + direction.vec.y * direction.velocity < 0 || position.pos.y + direction.vec.y * direction.velocity > _windowSize.y - image.h) {
                direction.vec.y = -direction.vec.y;
            }
            
            position.pos.x += direction.vec.x * direction.velocity;
            position.pos.y += direction.vec.y * direction.velocity;

            bool toContinue = false;

            for (auto &other : Entities) {
                if (other == entity) continue;

                if (entitiesOverlapped(entity, other)) {
                    const auto &otherType = coordinator.GetComponent<ECS::Components::CharacterType>(other);
                    auto &otherInfo = coordinator.GetComponent<ECS::Components::CharacterInfo>(other);

                    if (type.type == ECS::Components::CharacterEnumType::SHEEP && otherType.type == ECS::Components::CharacterEnumType::WOLF) {
                        coordinator.DestroyEntity(entity);
                        otherInfo.lastSheepEaten = _clock.now();
                        return;
                    } else if (type.type == ECS::Components::CharacterEnumType::SHEEP && otherType.type == ECS::Components::CharacterEnumType::SHEEP) {
                        auto &otherInfo = coordinator.GetComponent<ECS::Components::CharacterInfo>(other);

                        if (otherInfo.gender == info.gender) continue; // déso les homosexuels
                        auto timeNow = _clock.now();

                        if (std::chrono::duration_cast<std::chrono::seconds>(timeNow - info.lastBirthGiven).count() > 10.0f &&
                            std::chrono::duration_cast<std::chrono::seconds>(timeNow - otherInfo.lastBirthGiven).count() > 10.0f) {
                            info.lastBirthGiven = timeNow;
                            otherInfo.lastBirthGiven = timeNow;

                            auto newSheep = _lambda(position.pos.x, position.pos.y);
                        }
                    }
                }
            }

            if (toContinue) continue;

        } else if (aiType.type == ECS::Components::AIType::AGGRESIVE) {
            const auto dogClose = isDogClose(position);

            if (direction.locked == false && dogClose == true) {
                direction.vec.x *= -1;
                direction.vec.y *= -1;
                direction.locked = true;
                direction.lastLocked = _clock.now();
            }

            // if (dogClose)
            //     continue;

            std::optional<ECS::Components::Vector2F> closestSheep = findClosestSheep(position);

            if (direction.locked == false && closestSheep.has_value()) {
                if (closestSheep->x > position.pos.x)
                    direction.vec.x = 1;
                else if (closestSheep->x < position.pos.x)
                    direction.vec.x = -1;
                else
                    direction.vec.x = 0;

                if (closestSheep->y > position.pos.y)
                    direction.vec.y = 1;
                else if (closestSheep->y < position.pos.y)
                    direction.vec.y = -1;
                else
                    direction.vec.y = 0;
            }

            position.pos.x += direction.vec.x * direction.velocity;
            position.pos.y += direction.vec.y * direction.velocity;


            if (direction.locked == true && std::chrono::duration_cast<std::chrono::seconds>(_clock.now() - direction.lastLocked).count() >= direction.lockTime) {
                direction.lastLocked = _clock.now();
                direction.locked = false;
            }

            auto timeNow = _clock.now();

            if (std::chrono::duration_cast<std::chrono::seconds>(timeNow - info.lastSheepEaten).count() > 20.0f) {
                coordinator.DestroyEntity(entity);
                break;
            }

        }
    }
}