#ifndef ENEMY_H
#define ENEMY_H

#include <QPointF>
#include <QPainter>
#include <vector>
#include <utility>

enum class EnemyType {
    Normal,
    Fast,
    Tank,
    Boss,
    Swarm
};

struct EnemyStats {
    double maxHp;
    double speed;
    int reward;
    int damage;
    QColor color;
    int radius;
};

class Enemy {
public:
    Enemy(EnemyType type, const std::vector<QPointF>& path);

    void update(double dt);
    void applySlow(double factor, double duration);
    void applyPoison(double dps, double duration);
    void takeDamage(double dmg);

    bool isDead() const { return m_hp <= 0; }
    bool reachedEnd() const { return m_reachedEnd; }
    bool isActive() const { return !m_reachedEnd && !isDead(); }

    void updatePath(const std::vector<QPointF>& newPath);

    QPointF pos() const { return m_pos; }
    double hp() const { return m_hp; }
    double maxHp() const { return m_stats.maxHp; }
    EnemyType type() const { return m_type; }
    int reward() const { return m_stats.reward; }
    int damage() const { return m_stats.damage; }
    double radius() const { return m_stats.radius; }
    QColor color() const { return m_stats.color; }
    double speed() const { return m_stats.speed * m_slowFactor; }

    void draw(QPainter& p) const;

    static EnemyStats statsForType(EnemyType type);

private:
    EnemyType m_type;
    EnemyStats m_stats;
    QPointF m_pos;
    std::vector<QPointF> m_path;
    int m_pathIndex;
    double m_hp;
    bool m_reachedEnd;

    double m_slowFactor;
    double m_slowTimer;
    double m_poisonDps;
    double m_poisonTimer;
};

#endif
