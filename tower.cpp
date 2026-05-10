#include "tower.h"
#include "enemy.h"
#include <QtMath>
#include <algorithm>

Tower::Tower(TowerType type, int gridX, int gridY, double cellSize, double offsetX, double offsetY)
    : m_type(type), m_stats(statsForType(type))
    , m_gridX(gridX), m_gridY(gridY), m_cellSize(cellSize), m_cooldown(0)
{
    m_center = QPointF(offsetX + gridX*cellSize + cellSize/2.0,
                       offsetY + gridY*cellSize + cellSize/2.0);
}

void Tower::update(double dt, std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (m_cooldown > 0) { m_cooldown -= dt; return; }
    Enemy* target = findTarget(enemies);
    if (!target) return;

    m_pendingAttack.fired = true;
    m_pendingAttack.targetPos = target->pos();
    m_pendingAttack.damage = m_stats.damage;
    m_pendingAttack.splashRadius = m_stats.splashRadius * m_cellSize;
    m_pendingAttack.slowFactor = m_stats.slowFactor;
    m_pendingAttack.slowDuration = m_stats.slowDuration;
    m_pendingAttack.poisonDps = m_stats.poisonDps;
    m_pendingAttack.poisonDuration = m_stats.poisonDuration;
    m_pendingAttack.chainCount = m_stats.chainCount;
    m_pendingAttack.color = m_stats.color;
    m_cooldown = m_stats.attackSpeed;
}

Enemy* Tower::findTarget(std::vector<std::unique_ptr<Enemy>>& enemies) const
{
    Enemy* best = nullptr;
    double bestDist = 1e9;
    double r2 = rangePx() * rangePx();
    for (auto& e : enemies) {
        if (!e->isActive()) continue;
        double d2 = distTo(*e);
        if (d2 <= r2 && d2 < bestDist) { bestDist = d2; best = e.get(); }
    }
    return best;
}

double Tower::distTo(const Enemy& e) const {
    QPointF d = e.pos() - m_center;
    return d.x()*d.x() + d.y()*d.y();
}

TowerStats Tower::statsForType(TowerType type)
{
    // cost, dmg, range, atkSpd, splashR, slowF, slowDur, poiDps, poiDur, chain, color
    switch (type) {
    case TowerType::Arrow:
        return { 40, 20.0, 3.0, 0.6, 0, 1.0, 0, 0, 0, 0, QColor(139, 195, 74) };
    case TowerType::Cannon:
        return { 80, 40.0, 2.5, 1.5, 0.8, 1.0, 0, 0, 0, 0, QColor(255, 152, 0) };
    case TowerType::Ice:
        return { 60, 12.0, 2.5, 1.0, 0, 0.5, 2.0, 0, 0, 0, QColor(100, 180, 255) };
    case TowerType::Poison:
        return { 65, 8.0, 2.8, 1.2, 0, 1.0, 0, 25.0, 3.0, 0, QColor(120, 200, 80) };
    case TowerType::Lightning:
        return { 90, 18.0, 3.5, 1.8, 0, 1.0, 0, 0, 0, 3, QColor(220, 200, 60) };
    }
    return { 40, 20.0, 3.0, 0.6, 0, 1.0, 0, 0, 0, 0, QColor(139,195,74) };
}

void Tower::draw(QPainter& p) const
{
    double r = m_cellSize * 0.42;
    double cx = m_center.x(), cy = m_center.y();
    p.setRenderHint(QPainter::Antialiasing, true);

    // Base shadow
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0,0,0,60));
    p.drawEllipse(QPointF(cx, cy + r*0.2), r*0.8, r*0.3);

    switch (m_type) {
    case TowerType::Arrow: {
        // Pentagon arrow tower
        p.setBrush(m_stats.color);
        p.setPen(QPen(QColor(100,140,50), 1.5));
        QPolygonF poly;
        for (int i = 0; i < 5; ++i) {
            double a = -3.14159/2 + 3.14159*2*i/5.0;
            poly << QPointF(cx + r*cos(a), cy + r*sin(a));
        }
        p.drawPolygon(poly);
        // Arrow tip
        p.setPen(QPen(Qt::white, 2));
        p.drawLine(QPointF(cx, cy - r*0.6), QPointF(cx, cy + r*0.3));
        p.drawLine(QPointF(cx - r*0.4, cy - r*0.1), QPointF(cx, cy + r*0.3));
        p.drawLine(QPointF(cx + r*0.4, cy - r*0.1), QPointF(cx, cy + r*0.3));
        break;
    }
    case TowerType::Cannon: {
        // Circular cannon base
        p.setBrush(m_stats.color);
        p.setPen(QPen(QColor(200,120,0), 2));
        p.drawEllipse(QPointF(cx, cy), r*0.9, r*0.9);
        // Barrel
        p.setBrush(QColor(180,100,0));
        p.setPen(QPen(Qt::black, 1.5));
        p.drawRect(QRectF(cx - r*0.2, cy - r, r*0.4, r*0.6));
        break;
    }
    case TowerType::Ice: {
        // Diamond/rhombus ice crystal
        p.setBrush(m_stats.color);
        p.setPen(QPen(QColor(60,140,220), 2));
        QPolygonF diamond;
        diamond << QPointF(cx, cy - r) << QPointF(cx + r, cy)
                << QPointF(cx, cy + r) << QPointF(cx - r, cy);
        p.drawPolygon(diamond);
        // Snowflake center
        p.setPen(QPen(Qt::white, 1.5));
        p.drawLine(QPointF(cx, cy - r*0.5), QPointF(cx, cy + r*0.5));
        p.drawLine(QPointF(cx - r*0.5, cy), QPointF(cx + r*0.5, cy));
        break;
    }
    case TowerType::Poison: {
        // Hexagonal poison flask
        p.setBrush(m_stats.color);
        p.setPen(QPen(QColor(80,160,50), 2));
        QPolygonF hex;
        for (int i = 0; i < 6; ++i) {
            double a = 3.14159*i/3.0;
            hex << QPointF(cx + r*0.85*cos(a), cy + r*0.85*sin(a));
        }
        p.drawPolygon(hex);
        // Skull icon
        p.setPen(QPen(Qt::white, 1.5));
        p.setBrush(Qt::white);
        p.drawEllipse(QPointF(cx, cy - r*0.1), r*0.25, r*0.25);
        p.drawEllipse(QPointF(cx - r*0.25, cy + r*0.2), r*0.12, r*0.12);
        p.drawEllipse(QPointF(cx + r*0.25, cy + r*0.2), r*0.12, r*0.12);
        break;
    }
    case TowerType::Lightning: {
        // Star/bolt shape
        p.setBrush(m_stats.color);
        p.setPen(QPen(QColor(180,160,30), 2));
        QPolygonF star;
        for (int i = 0; i < 10; ++i) {
            double a = -3.14159/2 + 3.14159*i/5.0;
            double rr = (i%2==0) ? r : r*0.5;
            star << QPointF(cx + rr*cos(a), cy + rr*sin(a));
        }
        p.drawPolygon(star);
        // Bolt
        p.setPen(QPen(Qt::white, 2));
        p.drawLine(QPointF(cx - r*0.2, cy - r*0.5), QPointF(cx + r*0.3, cy));
        p.drawLine(QPointF(cx + r*0.3, cy), QPointF(cx - r*0.1, cy + r*0.5));
        break;
    }
    }
}
