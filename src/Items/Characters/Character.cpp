//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include "Character.h"

Character::Character(QGraphicsItem *parent) : Item(parent, "") {}

bool Character::isLeftDown() const { return leftDown; }
void Character::setLeftDown(bool leftDown) { Character::leftDown = leftDown; }

bool Character::isRightDown() const { return rightDown; }
void Character::setRightDown(bool rightDown) { Character::rightDown = rightDown; }

bool Character::isPickDown() const { return pickDown; }
void Character::setPickDown(bool pickDown) { Character::pickDown = pickDown; }

const QPointF &Character::getVelocity() const { return velocity; }
void Character::setVelocity(const QPointF &velocity) { Character::velocity = velocity; }

void Character::setJumpDown(bool jumpDown) { Character::jumpDown = jumpDown; }
bool Character::isJumping() const { return jumping; }
void Character::setJumping(bool jumping) { Character::jumping = jumping; }
qreal Character::getVelocityY() const { return velocityY; }
void Character::setVelocityY(qreal vy) { velocityY = vy; }

void Character::processInput() {
    auto velocity = QPointF(0, 0);
    const auto moveSpeed = 0.3;
    if (isLeftDown()) {
        velocity.setX(velocity.x() - moveSpeed);
        setTransform(QTransform().scale(1, 1));
    }
    if (isRightDown()) {
        velocity.setX(velocity.x() + moveSpeed);
        setTransform(QTransform().scale(-1, 1));
    }
    setVelocity(velocity);

    // 跳跃按键检测
    if (jumpDown && !jumping) {
        jumping = true;
        velocityY = -2.9; // 跳跃初速度
    }

    if (!lastPickDown && pickDown) { // first time pickDown
        picking = true;
    } else {
        picking = false;
    }
    lastPickDown = pickDown;
}

void Character::processMovement(qint64 deltaTime, qreal floorY) {
    QPointF pos = this->pos();
    QPointF v = getVelocity();
    qreal vy = getVelocityY();
    const qreal gravity = 0.02;

    if (isJumping()) {
        vy += gravity * deltaTime;
        pos.setY(pos.y() + vy * deltaTime);

        // 到达地面
        if (pos.y() >= floorY) {
            pos.setY(floorY);
            vy = 0;
            setJumping(false);
        }
    } else {
        pos.setY(floorY);
        vy = 0;
    }

    // 水平移动
    pos.setX(pos.x() + v.x() * deltaTime);

    setPos(pos);
    setVelocityY(vy);
}

bool Character::isPicking() const { return picking; }

Armor *Character::pickupArmor(Armor *newArmor) {
    auto oldArmor = armor;
    if (oldArmor != nullptr) {
        oldArmor->unmount();
        oldArmor->setPos(newArmor->pos());
        oldArmor->setParentItem(parentItem());
    }
    newArmor->setParentItem(this);
    newArmor->mountToParent();
    armor = newArmor;
    return oldArmor;
}
