# CarrotDefense 代码规范

在请求 AI 生成代码时，建议包含以本文件为规范的提示词。

```
请你以CODING_STANDARDS.md为规范，……
```

## 1. 目的

本规范旨在确保多人协作时代码风格统一、可读性一致。无论使用何种 AI 辅助工具，每位开发者必须遵循本文档的规则。

---

## 2. 文件组织

### 2.1 头文件 (.h)
```
// 1. 防止重复包含
#ifndef MODULENAME_H
#define MODULENAME_H

// 2. 系统/标准库头文件
#include <QString>
#include <vector>

// 3. 第三方库头文件
#include <ThirdPartyLib.h>

// 4. 本项目头文件
#include "component.h"

// 5. 使用 using 声明（仅在命名空间过长时）
using DataVector = std::vector<CustomData>;

// 6. 类/结构体声明
class MyClass : public QObject {
    Q_OBJECT
public:
    explicit MyClass(QObject *parent = nullptr);
    ~MyClass();

    // 公有成员函数（按构造/公有方法/信号/槽/私有顺序）
    void publicMethod();
    int publicProperty() const { return m_property; }

signals:
    void valueChanged(int newValue);

public slots:
    void onValueChanged(int value);

private:
    int m_property = 0;
};

#endif // MODULENAME_H
```

### 2.2 源文件 (.cpp)
```
// 1. 头文件包含（与 .h 顺序对应）
#include "myclass.h"
#include <QDebug>

// 2. 静态常量/枚举定义（如有）
enum class Mode { Auto, Manual };

// 3. 类方法实现
MyClass::MyClass(QObject *parent)
    : QObject(parent)      // 成员初始化列表，每行一个成员
    , m_property(0)
{
}

MyClass::~MyClass() = default;  // 默认析构函数使用 = default

void MyClass::publicMethod()
{
    // 实现
}
```

---

## 3. 命名规范

### 3.1 通用规则
- **始终使用有意义的命名**，禁止使用 `tmp`, `temp2`, `data1` 等无意义名称
- **命名采用 CamelCase 或 PascalCase**，不得混用
- **禁止使用中文拼音命名**

### 3.2 具体命名

| 类型 | 规范 | 示例 |
|------|------|------|
| 类名 | PascalCase，以名词结尾 | `GameController`, `EnemyUnit` |
| 接口名 | PascalCase，以 Interface 结尾 | `IMovableInterface` |
| 成员变量 | m_camelCase，m_ 前缀 | `m_healthPoints`, `m_playerName` |
| 普通变量 | camelCase | `healthPoints`, `playerName` |
| 常量 | kPascalCase，k_ 前缀 | `kMaxHealth`, `kDefaultSpeed` |
| 函数名 | PascalCase，动词/动词+名词 | `GetHealth()`, `TakeDamage()` |
| 枚举值 | PascalCase | `Mode::Auto`, `Status::Running` |
| 文件名 | 与类名完全一致 | `gamecontroller.h` |

---

## 4. 格式化规则

### 4.1 缩进与空格
- **缩进：4 空格**（不使用 Tab）
- 运算符两侧各留 1 空格：`a + b`, `x = 5`
- 关键字与圆括号之间留 1 空格：`if (condition)`
- 函数名与圆括号之间**不留空格**：`GetValue()`
- 大括号**独占一行**：
```cpp
if (condition)
{
    doSomething();
}
```

### 4.2 行长度
- 单行不超过 **120 字符**
- 长表达式换行时，运算符放在行尾：
```cpp
int result = veryLongFunctionName(param1, param2)
           + anotherFunction(param3, param4);
```

### 4.3 空行使用
- 命名空间内各声明块之间留 1 空行
- 函数定义之间留 1 空行
- 逻辑相关代码块之间留 0 空行，分隔无关逻辑时留 1 空行

---

## 5. 注释规范

### 5.1 注释风格
- **使用 `//`**，不使用 `/* */`（除非多行注释）
- 行内注释与代码之间留 2 空格：`int x;  // 初始化为 0`

### 5.2 何时必须写注释
| 情况 | 要求 |
|------|------|
| 类/结构体 | 类前必须有注释说明用途 |
| 公有方法 | 方法前必须有简短说明 |
| 复杂逻辑 | 分支/循环前必须说明意图 |
| 魔法数字 | 必须用命名常量替代或注释说明 |

### 5.3 注释示例
```cpp
// 血量管理系统，负责处理伤害计算和死亡判定
class HealthSystem : public QObject {
    Q_OBJECT
public:
    // 获取当前血量
    int GetHealth() const { return m_health; }

    // 对目标造成伤害，处理护甲减免和死亡判定
    // @param damage 原始伤害值（未计算护甲）
    void TakeDamage(int damage);
};
```

---

## 6. AI 辅助开发规范

### 6.1 核心原则
**AI 生成的代码必须经过人工审查和调整**，不得直接复制粘贴未经阅读的代码。

### 6.2 AI 生成代码的检查清单
- [ ] 代码是否符合本规范的命名规则？
- [ ] 代码缩进是否为 4 空格？
- [ ] 大括号是否遵循本规范？
- [ ] 变量命名是否有意义（无 `tmp`、`data1` 等）？
- [ ] 魔法数字是否已定义为常量？
- [ ] 复杂逻辑是否有注释说明？
- [ ] 代码是否与现有风格一致？

## 7. Git 提交规范

### 7.1 提交信息格式
```
<类型>: <简短描述>

[可选的详细说明]
```

### 7.2 类型标签
| 类型 | 含义 |
|------|------|
| `feat` | 新功能 |
| `fix` | 修复 bug |
| `refactor` | 重构（不改变功能） |
| `style` | 代码格式调整 |
| `docs` | 文档更新 |
| `test` | 测试相关 |
| `chore` | 构建/工具变更 |

### 7.3 示例
```
feat: 添加敌人攻击系统

- 实现 Enemy::Attack() 方法
- 添加伤害计算公式
- 与 HealthSystem 集成

Closes #12
```

### 7.3.1 AI 生成 Commit 消息规范

AI 可以辅助生成 commit 消息，但必须遵循以下流程：

1. **AI 生成后必须展示给用户**，不得直接执行
2. **用户确认前**，AI 不得执行 `git commit` 命令
3. **用户可修改**：用户有权修改 AI 生成的 commit 消息后再确认
4. **禁止静默提交**：禁止配置任何「自动接受 AI 生成的 commit 消息」的 Hook

#### 推荐的 AI Commit 消息生成流程
```
1. AI 分析代码变更（git diff）
2. AI 生成符合 7.1-7.3 格式的 commit 消息草稿
3. AI 向用户展示草稿，等待确认
4. 用户确认/修改后，AI 执行 git commit
5. AI 不得在用户未确认的情况下自行提交
```

#### AI Commit 消息生成提示词模板
```
请分析以下代码变更，生成符合规范的 commit 消息：
- 格式：<类型>: <简短描述>
- 类型：feat/fix/refactor/style/docs/test/chore
- 要求：描述清晰，第一行不超过 72 字符
- 说明：列出主要变更点

[代码变更内容]
```

---

### 7.4 推送（Push）规范

#### 禁止行为
- **严禁**任何 AI 工具在未经人类明确授权的情况下执行 `git push`
- **严禁**为 AI 工具配置自动推送（auto-push）功能
- **严禁**使用 Webhook 或 CI/CD 流程自动推送代码到主干分支
- **严禁**配置 `git push --force` 的任何自动化脚本

#### 推送权限分级

| 分支 | 推送权限 | 说明 |
|------|----------|------|
| `main` | 仅 Review 后可推送 | 必须通过 PR，Review 批准后才能合并 |
| `develop` | 协作者可推送 | 仅限协作者，需遵守分支策略 |
| `feature/*` | 个人分支可推送 | 仅推送自己的分支，禁止强制推送 |

#### AI 辅助时的 Git 操作规范
1. **操作前审批**：AI 提出的任何 Git 操作（commit 除外）必须经过人工确认
2. **保留撤回权**：任何推送操作必须确保可以轻易回滚
3. **禁止链式推送**：禁止配置「commit 后自动 push」的 Hook
4. **明确边界**：AI 仅能操作个人分支，不得操作他人分支或主干

---

## 8. 分支管理规范

### 8.1 分支命名
```
feature/<功能描述>      # 新功能开发
fix/<问题描述>          # Bug 修复
refactor/<范围>         # 重构
docs/<更新内容>         # 文档更新
```

### 8.2 分支策略
- `main`：稳定分支，始终保持可发布状态
- `develop`：开发基准分支，汇总已完成功能
- 功能分支从 `develop` 创建，合并回 `develop`

---

## 9. 代码审查要点
