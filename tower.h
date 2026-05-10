#ifndef TOWER_H
#define TOWER_H

#include <QPointF>
#include <QPainter>
#include <vector>
#include <memory>

class Enemy;

enum class TowerType {
    Arrow,
    Cannon,
    Ice,
    Poison,
    Lightning
};

struct TowerStats {
    int cost;
    double damage;
    double range;
    double attackSpeed;
    double splashRadius;
    double slowFactor;
    double slowDuration;
    double poisonDps;
    double poisonDuration;
    int chainCount;
    QColor color;
};

class Tower {
public:
    Tower(TowerType type, int gridX, int gridY, double cellSize, double offsetX, double offsetY);

    void update(double dt, std::vector<std::unique_ptr<Enemy>>& enemies);
    bool canAttack() const { return m_cooldown <= 0; }

    TowerType type() const { return m_type; }
    int gridX() const { return m_gridX; }
    int gridY() const { return m_gridY; }
    QPointF centerPos() const { return m_center; }
    double rangePx() const { return m_stats.range * m_cellSize; }
    double cellSize() const { return m_cellSize; }
    int cost() const { return m_stats.cost; }
    TowerStats stats() const { return m_stats; }

    void draw(QPainter& p) const;

    static TowerStats statsForType(TowerType type);

    struct AttackResult {
        bool fired = false;
        QPointF targetPos;
        double damage;
        double splashRadius;
        double slowFactor;
        double slowDuration;
        double poisonDps = 0;
        double poisonDuration = 0;
        int chainCount = 0;
        QColor color;
    };

    AttackResult getAttack() { auto r = m_pendingAttack; m_pendingAttack = AttackResult(); return r; }

private:
    TowerType m_type;
    TowerStats m_stats;
    int m_gridX, m_gridY;
    QPointF m_center;
    double m_cellSize;
    double m_cooldown;

    AttackResult m_pendingAttack;

    Enemy* findTarget(std::vector<std::unique_ptr<Enemy>>& enemies) const;
    double distTo(const Enemy& e) const;
};

#endif // TOWER_H
