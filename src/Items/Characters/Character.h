//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_CHARACTER_H
#define QT_PROGRAMMING_2024_CHARACTER_H

#include "../Item.h"
#include "../Armors/Armor.h"
#include "../HeadEquipments/HeadEquipment.h"
#include "../LegEquipments/LegEquipment.h"

class Character : public Item {
public:
    explicit Character(QGraphicsItem *parent);

    [[nodiscard]] bool isLeftDown() const;
    void setLeftDown(bool leftDown);

    [[nodiscard]] bool isRightDown() const;
    void setRightDown(bool rightDown);

    [[nodiscard]] bool isPickDown() const;
    void setPickDown(bool pickDown);

    [[nodiscard]] const QPointF &getVelocity() const;
    [[nodiscard]] bool isPicking() const;
    void setVelocity(const QPointF &velocity);

    void processInput();
    void processMovement(qint64 deltaTime, qreal floorY);

    Armor* pickupArmor(Armor* newArmor);

    void setJumpDown(bool jumpDown);
    bool isJumping() const;
    void setJumping(bool jumping);
    qreal getVelocityY() const;
    void setVelocityY(qreal vy);

protected:
    HeadEquipment *headEquipment{};
    LegEquipment *legEquipment{};
    Armor *armor{};
    QPointF velocity{};
    bool jumpDown{};
    bool jumping{};
    qreal velocityY{0};
private:
    bool leftDown{}, rightDown{}, pickDown{};
    bool lastPickDown{};
    bool picking{};
};

#endif //QT_PROGRAMMING_2024_CHARACTER_H
