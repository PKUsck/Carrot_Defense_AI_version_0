# CarrotDefense 项目规范

## 必读文件

**所有 AI 辅助操作必须遵循 `CODING_STANDARDS.md` 中的规范。**

---

## 核心原则

1. **代码规范**：遵循 `CODING_STANDARDS.md` 中的文件组织、命名规范、格式化规则、注释规范
2. **AI 代码审查**：AI 生成的代码必须经过人工审查，不得直接复制粘贴
3. **Git 操作**：所有 Git 操作（commit 除外）必须经过用户确认

---

## Git 操作强制规则

### 禁止行为
- **严禁**未经用户授权执行 `git push`
- **严禁**配置自动推送（auto-push）
- **严禁**自动 force push
- **严禁**在用户未确认前执行 commit

### Commit 操作流程
1. AI 分析代码变更
2. AI 生成符合规范的 commit 消息草稿，展示给用户
3. 用户确认或修改后，AI 执行 `git commit`
4. **禁止**使用任何自动接受 commit 消息的 Hook

### Push 操作规范
- AI 不得在用户明确授权前推送任何分支
- AI 仅能操作个人分支，不得操作主干或他人分支

---

## AI 提示词模板

在请求 AI 生成代码时，建议包含以下约束：

```
请你以 CODING_STANDARDS.md 为规范，按照以下规则生成代码：
- 缩进：4 空格
- 命名：CamelCase/PascalCase，成员变量 m_ 前缀
- 括号：函数体括号另起一行
- 注释：复杂逻辑需注释说明
```

---

## 详细规范

完整的代码规范请参阅：[CODING_STANDARDS.md](CODING_STANDARDS.md)
