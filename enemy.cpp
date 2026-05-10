#include "enemy.h"
#include <QtMath>
#include <algorithm>

Enemy::Enemy(EnemyType type, const std::vector<QPointF>& path)
    : m_type(type), m_stats(statsForType(type))
    , m_pos(path.empty() ? QPointF(0,0) : path[0])
    , m_path(path), m_pathIndex(1)
    , m_hp(m_stats.maxHp), m_reachedEnd(false)
    , m_slowFactor(1.0), m_slowTimer(0.0)
    , m_poisonDps(0.0), m_poisonTimer(0.0)
{}

void Enemy::updatePath(const std::vector<QPointF>& newPath)
{
    m_path = newPath;
    m_pathIndex = 0;
    double bestDist = 1e9;
    for (size_t i = 0; i < newPath.size(); ++i) {
        QPointF d = newPath[i] - m_pos;
        double dist = d.x()*d.x() + d.y()*d.y();
        if (dist < bestDist) { bestDist = dist; m_pathIndex = static_cast<int>(i) + 1; }
    }
    if (m_pathIndex >= static_cast<int>(m_path.size())) m_reachedEnd = true;
}

void Enemy::update(double dt)
{
    if (m_reachedEnd || isDead()) return;

    // Slow
    if (m_slowTimer > 0) {
        m_slowTimer -= dt;
        if (m_slowTimer <= 0) { m_slowFactor = 1.0; m_slowTimer = 0; }
    }
    // Poison
    if (m_poisonTimer > 0) {
        m_hp -= m_poisonDps * dt;
        m_poisonTimer -= dt;
        if (m_poisonTimer <= 0) { m_poisonDps = 0; m_poisonTimer = 0; }
    }

    if (m_pathIndex >= static_cast<int>(m_path.size())) { m_reachedEnd = true; return; }

    QPointF target = m_path[m_pathIndex];
    QPointF dir = target - m_pos;
    double dist = std::sqrt(dir.x()*dir.x() + dir.y()*dir.y());
    double moveDist = speed() * dt;
    if (moveDist >= dist) { m_pos = target; ++m_pathIndex; }
    else { dir /= dist; m_pos += dir * moveDist; }
}

void Enemy::applySlow(double factor, double duration) {
    if (factor < m_slowFactor) { m_slowFactor = factor; m_slowTimer = duration; }
}
void Enemy::applyPoison(double dps, double duration) {
    m_poisonDps = dps; m_poisonTimer = duration;
}
void Enemy::takeDamage(double dmg) { m_hp -= dmg; if (m_hp < 0) m_hp = 0; }

EnemyStats Enemy::statsForType(EnemyType type)
{
    switch (type) {
    case EnemyType::Normal:  return { 50.0,  50.0,  5, 1, QColor(220, 60, 60),   10 };
    case EnemyType::Fast:    return { 30.0, 100.0,  8, 1, QColor(255, 165, 0),    8 };
    case EnemyType::Tank:    return { 150.0, 28.0, 15, 1, QColor(100, 50, 150),  14 };
    case EnemyType::Boss:    return { 500.0, 18.0, 50, 3, QColor(200, 50, 50),   18 };
    case EnemyType::Swarm:   return { 15.0, 120.0,  3, 1, QColor(255, 200, 50),   6 };
    }
    return { 50.0, 50.0, 5, 1, QColor(220, 60, 60), 10 };
}

void Enemy::draw(QPainter& p) const
{
    if (!isActive()) return;
    int r = m_stats.radius;
    double cx = m_pos.x(), cy = m_pos.y();

    // Health bar
    int barW = r * 3;
    int barH = 4;
    p.fillRect(QRectF(cx - barW/2.0, cy - r - 10, barW, barH), QColor(40,40,40));
    double hpR = m_hp / m_stats.maxHp;
    QColor hpC = hpR > 0.5 ? QColor(76,175,80) : hpR > 0.25 ? QColor(255,193,7) : QColor(244,67,54);
    p.fillRect(QRectF(cx - barW/2.0, cy - r - 10, barW * hpR, barH), hpC);

    // Body color (blue if slowed, green if poisoned)
    QColor body = m_stats.color;
    if (m_poisonTimer > 0) body = QColor(120, 200, 80);
    else if (m_slowFactor < 1.0) body = QColor(100, 180, 255);

    p.setPen(Qt::NoPen);

    // Draw different shapes per type
    if (m_type == EnemyType::Boss) {
        // Boss: large hexagon
        p.setBrush(body);
        QPolygonF hex;
        for (int i = 0; i < 6; ++i) {
            double a = 3.14159 * i / 3.0;
            hex << QPointF(cx + r * cos(a), cy + r * sin(a));
        }
        p.drawPolygon(hex);
        // Crown dots
        p.setBrush(QColor(255, 215, 0));
        p.drawEllipse(QPointF(cx, cy - r*0.7), r*0.2, r*0.2);
    } else if (m_type == EnemyType::Swarm) {
        // Swarm: tiny triangle
        p.setBrush(body);
        QPolygonF tri;
        tri << QPointF(cx, cy - r) << QPointF(cx + r, cy + r) << QPointF(cx - r, cy + r);
        p.drawPolygon(tri);
    } else {
        // Normal/Fast/Tank: rounded body with eyes
        p.setBrush(body);
        p.drawEllipse(QPointF(cx, cy), r, r);
        // Eyes
        p.setBrush(Qt::white);
        p.drawEllipse(QPointF(cx - r*0.3, cy - r*0.25), r*0.22, r*0.22);
        p.drawEllipse(QPointF(cx + r*0.3, cy - r*0.25), r*0.22, r*0.22);
        p.setBrush(Qt::black);
        p.drawEllipse(QPointF(cx - r*0.25, cy - r*0.27), r*0.11, r*0.11);
        p.drawEllipse(QPointF(cx + r*0.35, cy - r*0.27), r*0.11, r*0.11);
    }
}
