//
// Created by gerw on 8/20/24.
//

#include <QDebug>
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"

BattleScene::BattleScene(QObject *parent) : Scene(parent) {
    setSceneRect(0, 0, 1280, 720);
    map = new Battlefield();
    character = new Link();
    spareArmor = new FlamebreakerArmor();
    addItem(map);
    addItem(character);
    addItem(spareArmor);
    map->scaleToFitScene(this);
    character->setPos(map->getSpawnPos());
    spareArmor->unmount();
    spareArmor->setPos(sceneRect().left() + (sceneRect().right() - sceneRect().left()) * 0.75, map->getFloorHeight());
}

void BattleScene::processInput() {
    Scene::processInput();
    if (character != nullptr) {
        character->processInput();
    }
}

void BattleScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_A:
        if (character != nullptr) character->setLeftDown(true);
        break;
    case Qt::Key_D:
        if (character != nullptr) character->setRightDown(true);
        break;
    case Qt::Key_S:
        if (character != nullptr) character->setPickDown(true);
        break;
    case Qt::Key_W:
        if (character != nullptr) character->setJumpDown(true);
        break;
    default:
        Scene::keyPressEvent(event);
    }
}

void BattleScene::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_A:
        if (character != nullptr) character->setLeftDown(false);
        break;
    case Qt::Key_D:
        if (character != nullptr) character->setRightDown(false);
        break;
    case Qt::Key_S:
        if (character != nullptr) character->setPickDown(false);
        break;
    case Qt::Key_W:
        if (character != nullptr) character->setJumpDown(false);
        break;
    default:
        Scene::keyReleaseEvent(event);
    }
}

void BattleScene::update() {
    Scene::update();
}

void BattleScene::processMovement() {
    Scene::processMovement();
    if (character != nullptr) {
        character->processMovement(
            deltaTime,
            map->getFloorHeight()
            );
    }
}

void BattleScene::processPicking() {
    Scene::processPicking();
    if (character->isPicking()) {
        auto mountable = findNearestUnmountedMountable(character->pos(), 100.);
        if (mountable != nullptr) {
            spareArmor = dynamic_cast<Armor *>(pickupMountable(character, mountable));
        }
    }
}

Mountable *BattleScene::findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold) {
    Mountable *nearest = nullptr;
    qreal minDistance = distance_threshold;

    for (QGraphicsItem *item: items()) {
        if (auto mountable = dynamic_cast<Mountable *>(item)) {
            if (!mountable->isMounted()) {
                qreal distance = QLineF(pos, item->pos()).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = mountable;
                }
            }
        }
    }
    return nearest;
}

Mountable *BattleScene::pickupMountable(Character *character, Mountable *mountable) {
    if (auto armor = dynamic_cast<Armor *>(mountable)) {
        return character->pickupArmor(armor);
    }
    return nullptr;
}
