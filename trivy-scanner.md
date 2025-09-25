# Trivy 配置错误扫描指南

本文档介绍了如何使用 Trivy 进行配置错误扫描（misconfiguration scanning），帮助您识别基础设施即代码（IaC）和容器化平台中的配置问题。

## 概述

Trivy 的配置错误扫描功能可以检测流行基础设施即代码（IaC）工具和容器化平台中的配置问题，帮助您识别潜在的安全风险和最佳实践违规。

## 支持的配置类型

Trivy 支持扫描以下类型的配置文件：

### 1. 容器化平台
- **Docker** - Dockerfile 配置检查
- **Kubernetes** - YAML 清单文件检查
- **Helm** - Helm Chart 配置检查

### 2. 基础设施即代码（IaC）
- **Terraform** - `.tf` 文件检查
- **CloudFormation** - AWS CloudFormation 模板
- **Azure ARM** - Azure Resource Manager 模板

## 快速开始

### 基本扫描命令

```bash
# 扫描当前目录的所有配置文件
trivy config .

# 扫描特定文件
trivy config deployment.yaml

# 扫描特定目录
trivy config /path/to/kubernetes/manifests/
```

### 扫描特定类型

```bash
# 只扫描 Kubernetes 配置
trivy config --scanners config deployment.yaml

# 扫描 Terraform 文件
trivy config main.tf

# 扫描 Dockerfile
trivy config Dockerfile
```

## 扫描结果示例

根据文档，扫描结果会显示：

1. **配置类型识别** - 自动识别文件类型（如 `kubernetes`、`terraform`、`dockerfile` 等）
2. **测试统计** - 显示总测试数和成功/失败数量
3. **问题详情** - 包括严重级别、问题描述和修复建议

### 示例输出格式：

```
deployment.yaml (kubernetes)
============================
Tests: 28 (SUCCESSES: 15, FAILURES: 13)
Failures: 13 (MEDIUM: 4, HIGH: 1, CRITICAL: 0)

HIGH: Deployment 'hello-kubernetes' should not set 'spec.template.spec.containers[].securityContext.privileged' to true
═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
Privileged containers can allow almost completely unrestricted host access.

See https://avd.aquasec.com/misconfig/ksv006
```

### 多文件扫描结果示例：

```
Dockerfile (dockerfile)
=======================
Tests: 23 (SUCCESSES: 22, FAILURES: 1)
Failures: 1 (HIGH: 1, CRITICAL: 0)

deployment.yaml (kubernetes)
============================
Tests: 28 (SUCCESSES: 15, FAILURES: 13)
Failures: 13 (MEDIUM: 4, HIGH: 1, CRITICAL: 0)

main.tf (terraform)
===================
Tests: 23 (SUCCESSES: 14, FAILURES: 9)
Failures: 9 (HIGH: 6, CRITICAL: 1)

bucket.yaml (cloudformation)
============================
Tests: 9 (SUCCESSES: 3, FAILURES: 6)
Failures: 6 (UNKNOWN: 0, LOW: 0, MEDIUM: 2, HIGH: 4, CRITICAL: 0)

mysql-8.8.26.tar:templates/primary/statefulset.yaml (helm)
==========================================================
Tests: 20 (SUCCESSES: 18, FAILURES: 2)
Failures: 2 (MEDIUM: 2, HIGH: 0, CRITICAL: 0)
```

## 配置选项

### 输出格式

```bash
# JSON 格式输出
trivy config . --format json --output results.json

# 表格格式（默认）
trivy config .

# SARIF 格式
trivy config . --format sarif --output results.sarif
```

### 严重级别过滤

```bash
# 只显示高危和严重问题
trivy config . --severity HIGH,CRITICAL

# 显示所有级别
trivy config . --severity UNKNOWN,LOW,MEDIUM,HIGH,CRITICAL
```

### 排除特定检查

```bash
# 排除特定检查 ID
trivy config . --skip-checks KSV001,KSV002

# 排除特定严重级别
trivy config . --skip-severity LOW,MEDIUM
```

## 网络连接要求

Trivy 需要连接到互联网以下载检查规则包。如果您在离线环境中运行，请参考 [高级网络场景文档](https://trivy.dev/latest/docs/advanced/network/) 了解如何配置离线使用。

## 实际应用场景

### 1. CI/CD 集成

```bash
# 在 CI 管道中扫描配置
trivy config . --exit-code 1 --severity HIGH,CRITICAL
```

### 2. 开发环境检查

```bash
# 开发时快速检查
trivy config deployment.yaml
```

### 3. 批量扫描

```bash
# 扫描整个项目目录
trivy config /path/to/project --format json --output config-scan.json
```

## 常见检查类型

### Kubernetes 安全检查
- 容器特权模式检查
- 安全上下文配置
- 网络策略配置
- 资源限制设置

### Terraform 安全检查
- 资源访问控制
- 网络安全组配置
- 存储加密设置
- IAM 权限配置

### Docker 安全检查
- 基础镜像选择
- 用户权限设置
- 端口暴露检查
- 安全最佳实践

## 完整示例命令

### 基本扫描

```bash
# 扫描当前项目
trivy config .

# 扫描指定项目
trivy config /path/to/your/project
```

### 高级扫描

```bash
# 扫描当前项目，只显示高危问题，输出 JSON 格式
trivy config . --severity HIGH,CRITICAL --format json --output config-results.json

# 扫描特定项目路径，排除特定检查
trivy config /path/to/your/project --skip-checks KSV001,KSV002

# 扫描所有类型，输出到文件
trivy config /path/to/your/project --format json --output full-config-scan.json
```

### 特定文件扫描

```bash
# 扫描 Kubernetes 部署文件
trivy config deployment.yaml

# 扫描 Terraform 文件
trivy config main.tf

# 扫描 Dockerfile
trivy config Dockerfile

# 扫描 Helm Chart
trivy config mysql-8.8.26.tar
```

## 最佳实践

### 1. 扫描前准备

1. **在项目根目录执行扫描**
   ```bash
   cd /path/to/your/project
   trivy config .
   ```

2. **使用正确的文件名和扩展名**
   - 确保配置文件名称正确
   - 检查文件是否存在

3. **检查文件权限**
   - 确保有读取权限
   - 避免文件被锁定

4. **验证文件格式**
   - 检查 YAML/JSON 格式
   - 确认文件编码正确

### 2. 扫描配置

1. **使用适当的扫描选项**
   ```bash
   trivy config . --severity HIGH,CRITICAL
   ```

2. **选择合适的输出格式**
   ```bash
   # 开发环境：表格格式
   trivy config .
   
   # CI/CD：JSON 格式
   trivy config . --format json --output results.json
   ```

3. **排除不必要的检查**
   ```bash
   trivy config . --skip-checks KSV001,KSV002
   ```

### 3. 结果处理

1. **保存扫描结果**
   ```bash
   trivy config . --format json --output config-scan.json
   ```

2. **集成到 CI/CD**
   ```bash
   # 在 CI 中设置退出码
   trivy config . --exit-code 1 --severity HIGH,CRITICAL
   ```

## 故障排查

### 1. 检查文件状态

```bash
# 检查文件是否存在
ls -la /path/to/project/deployment.yaml

# 检查文件权限
stat /path/to/project/deployment.yaml

# 检查文件内容
head -10 /path/to/project/deployment.yaml
```

### 2. 查看 Trivy 日志

```bash
# 启用详细日志
trivy config . --debug

# 查看错误信息
trivy config . 2>&1 | tee trivy-config.log
```

### 3. 验证环境

```bash
# 检查 Trivy 版本
trivy --version

# 检查 Trivy 配置
trivy config
```

## 常见错误和解决方案

### 1. 文件名错误

```bash
# ❌ 错误示例
trivy config deployment1.yaml        # 应该是 deployment.yaml
trivy config main1.tf               # 应该是 main.tf

# ✅ 正确示例
trivy config deployment.yaml
trivy config main.tf
```

### 2. 路径错误

```bash
# ❌ 错误：路径不存在
trivy config /nonexistent/path

# ✅ 正确：使用存在的路径
trivy config /existing/path/to/project
```

### 3. 权限错误

```bash
# 检查文件权限
ls -la /path/to/project/deployment.yaml

# 修改权限（如果需要）
chmod 644 /path/to/project/deployment.yaml
```

## 总结

使用 Trivy 进行配置错误扫描时：

1. **必须使用正确的文件名和扩展名**
2. **确保文件路径正确**
3. **检查文件权限**
4. **验证文件格式**
5. **使用适当的扫描选项**
6. **遵循最佳实践**
7. **及时处理扫描结果**

通过使用 Trivy 的配置错误扫描功能，您可以有效识别和修复基础设施配置中的安全问题，提高整体安全态势。

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

# Trivy 许可证扫描指南

Trivy 的许可证扫描功能可以帮助您识别和分类项目中使用的开源许可证，确保合规性并避免法律风险。

## 许可证扫描概述

许可证扫描可以检测：
- 操作系统包中的许可证
- 语言特定包中的许可证（如 npm、pip、gem 等）
- 容器镜像中的许可证
- 文件系统中的许可证文件

## 快速开始

### 标准扫描

```bash
# 扫描容器镜像的许可证
trivy image --scanners license grafana/grafana:latest

# 扫描文件系统
trivy fs --scanners license /path/to/project

# 扫描代码仓库
trivy repo --scanners license /path/to/project
```

### 完整扫描

```bash
# 启用完整许可证扫描（包括松散文件）
trivy fs --scanners license --license-full /path/to/project

# 扫描 SPDX 许可证列表数据
trivy fs --scanners license --license-full <path/to/spdx/list/data>
```

## 扫描结果示例

### 标准扫描结果

```
OS Packages (license)
=====================
Total: 2 (HIGH: 2, CRITICAL: 0)

┌───────────────────┬─────────┬────────────────┬──────────┐
│      Package      │ License │ Classification │ Severity │
├───────────────────┼─────────┼────────────────┼──────────┤
│ alpine-baselayout │ GPL-2.0 │ Restricted     │ HIGH     │
├───────────────────┤         │                │          │
│ ssl_client        │         │                │          │
└───────────────────┴─────────┴────────────────┴──────────┘
```

### 完整扫描结果

```
Loose File License(s) (license)
===============================
Total: 282 (UNKNOWN: 81, LOW: 97, MEDIUM: 24, HIGH: 58, CRITICAL: 22)

┌─────────────────┬──────────┬──────────────┬──────────────────────────────────────────────────────────────┐
│ Classification  │ Severity │ License      │                        File Location                         │
├─────────────────┼──────────┼──────────────┼──────────────────────────────────────────────────────────────┤
│ Forbidden       │ CRITICAL │ AGPL-3.0     │ /usr/share/grafana/LICENSE                                   │
├─────────────────┼──────────┼──────────────┼──────────────────────────────────────────────────────────────┤
│ Non Standard    │ UNKNOWN  │ BSD-0-Clause │ /usr/share/grafana/public/build/5069.d6aae9dd11d49c741a80.j- │
│                 │          │              │ s.LICENSE.txt                                                │
│                 │          │              ├──────────────────────────────────────────────────────────────┤
│                 │          │              │ /usr/share/grafana/public/build/6444.d6aae9dd11d49c741a80.j- │
│                 │          │              │ s.LICENSE.txt                                                │
└─────────────────┴──────────┴──────────────┴──────────────────────────────────────────────────────────────┘
```

## 许可证分类

Trivy 将许可证分为以下几个类别：

### 1. Forbidden（禁止）- CRITICAL
- AGPL-3.0
- 其他具有严格限制的许可证

### 2. Restricted（限制）- HIGH
- GPL-2.0, GPL-3.0
- LGPL-2.0, LGPL-2.1, LGPL-3.0
- 其他具有传染性的许可证

### 3. Reciprocal（互惠）- MEDIUM
- MPL-2.0
- EPL-1.0, EPL-2.0
- CDDL-1.0, CDDL-1.1

### 4. Notice（通知）- LOW
- Apache-2.0
- MIT
- BSD-2-Clause, BSD-3-Clause

### 5. Unencumbered（无负担）- UNKNOWN
- CC0-1.0
- Unlicense
- 0BSD

### 6. Permissive（宽松）- UNKNOWN
- 最宽松的开源许可证

## 配置选项

### 忽略特定许可证

```bash
# 忽略特定许可证
trivy image --scanners license --ignored-licenses MPL-2.0,MIT --severity HIGH grafana/grafana:latest

# 只显示高危和严重问题
trivy image --scanners license --severity HIGH,CRITICAL grafana/grafana:latest
```

### 配置分类器置信度

```bash
# 调整置信度级别（0.0 到 1.0，默认 0.9）
trivy fs --scanners license --license-full --license-confidence-level 0.8 /path/to/project
```

置信度级别说明：
- **0.9（默认）**：高精度，检测 258 个许可证
- **0.8**：中等精度，检测 282 个许可证
- **更低值**：检测更多许可证，但可能包含误报

### 自定义许可证分类

#### 1. 生成默认配置

```bash
# 生成默认配置文件
trivy image --generate-default-config
```

#### 2. 编辑配置文件

```yaml
# trivy.yaml
license:
  forbidden:
  - AGPL-3.0

  restricted:
  - AGPL-1.0
  - GPL-2.0
  - GPL-3.0
  - LGPL-2.0
  - LGPL-2.1
  - LGPL-3.0

  reciprocal:
  - MPL-2.0
  - EPL-1.0
  - EPL-2.0

  notice:
  - Apache-2.0
  - MIT
  - BSD-2-Clause
  - BSD-3-Clause

  unencumbered:
  - CC0-1.0
  - Unlicense
  - 0BSD

  permissive: []
```

#### 3. 文本许可证配置

对于无法识别的文本许可证，可以使用 `text://` 前缀：

```yaml
license:
  forbidden:
    - "text://Text of Apache Software Foundation License"
    - "text://.* Apache Software .*"  # 使用正则表达式
```

### 启用特定包类型

```bash
# 只扫描操作系统包
trivy image --pkg-types os --scanners license grafana/grafana:latest

# 只扫描语言特定包
trivy image --pkg-types library --scanners license grafana/grafana:latest

# 扫描所有包类型（默认）
trivy image --scanners license grafana/grafana:latest
```

## 实际应用场景

### 1. CI/CD 集成

```bash
# 在 CI 管道中检查许可证合规性
trivy image --scanners license --exit-code 1 --severity HIGH,CRITICAL myapp:latest
```

### 2. 项目许可证审计

```bash
# 扫描整个项目
trivy repo --scanners license --license-full /path/to/project --format json --output license-report.json
```

### 3. 容器镜像许可证检查

```bash
# 检查生产环境镜像
trivy image --scanners license --severity HIGH,CRITICAL production-app:v1.0.0
```

### 4. 依赖许可证监控

```bash
# 定期检查依赖许可证变化
trivy fs --scanners license --format json --output license-snapshot.json /path/to/project
```

## 完整示例命令

### 基本许可证扫描

```bash
# 扫描当前项目
trivy repo --scanners license .

# 扫描指定项目
trivy repo --scanners license /path/to/your/project
```

### 高级许可证扫描

```bash
# 完整扫描，自定义置信度，输出 JSON
trivy fs --scanners license --license-full --license-confidence-level 0.8 --format json --output license-results.json /path/to/project

# 忽略特定许可证，只显示高危问题
trivy image --scanners license --ignored-licenses MIT,Apache-2.0 --severity HIGH,CRITICAL myapp:latest

# 只扫描操作系统包
trivy image --pkg-types os --scanners license --severity HIGH,CRITICAL myapp:latest
```

### 特定文件扫描

```bash
# 扫描特定目录
trivy fs --scanners license /path/to/project/src/

# 扫描特定文件
trivy fs --scanners license /path/to/project/package.json
```

## 最佳实践

### 1. 扫描前准备

1. **了解项目依赖**
   ```bash
   # 查看项目依赖结构
   find /path/to/project -name "package.json" -o -name "requirements.txt" -o -name "pom.xml"
   ```

2. **设置合适的置信度**
   ```bash
   # 开发环境：使用默认置信度
   trivy repo --scanners license .
   
   # 生产环境：使用较低置信度进行完整检查
   trivy repo --scanners license --license-full --license-confidence-level 0.8 .
   ```

### 2. 许可证合规策略

1. **建立许可证白名单**
   ```yaml
   # 只允许特定许可证
   license:
     allowed:
       - MIT
       - Apache-2.0
       - BSD-2-Clause
       - BSD-3-Clause
   ```

2. **设置许可证黑名单**
   ```yaml
   # 禁止使用特定许可证
   license:
     forbidden:
       - AGPL-3.0
       - GPL-3.0
   ```

### 3. 结果处理

1. **保存扫描结果**
   ```bash
   trivy repo --scanners license --format json --output license-audit.json .
   ```

2. **集成到 CI/CD**
   ```bash
   # 在 CI 中设置退出码
   trivy image --scanners license --exit-code 1 --severity HIGH,CRITICAL myapp:latest
   ```

## 故障排查

### 1. 许可证识别问题

```bash
# 启用详细日志
trivy repo --scanners license --debug .

# 检查许可证文件
find /path/to/project -name "*LICENSE*" -o -name "*license*"
```

### 2. 置信度调整

```bash
# 如果检测到的许可证太少，降低置信度
trivy repo --scanners license --license-confidence-level 0.7 .

# 如果误报太多，提高置信度
trivy repo --scanners license --license-confidence-level 0.95 .
```

### 3. 自定义分类问题

```bash
# 验证配置文件
trivy config

# 测试自定义分类
trivy repo --scanners license --config trivy.yaml .
```

## 常见错误和解决方案

### 1. 许可证文件未找到

```bash
# 检查许可证文件是否存在
ls -la /path/to/project/*LICENSE*

# 使用完整扫描模式
trivy fs --scanners license --license-full /path/to/project
```

### 2. 许可证分类错误

```bash
# 生成并自定义配置文件
trivy image --generate-default-config
vim trivy.yaml

# 使用自定义配置
trivy repo --scanners license --config trivy.yaml .
```

### 3. 扫描结果过多

```bash
# 过滤严重级别
trivy repo --scanners license --severity HIGH,CRITICAL .

# 忽略特定许可证
trivy repo --scanners license --ignored-licenses MIT,Apache-2.0 .
```

## 总结

使用 Trivy 进行许可证扫描时：

1. **选择合适的扫描模式**（标准 vs 完整）
2. **配置适当的置信度级别**
3. **建立许可证合规策略**
4. **定期进行许可证审计**
5. **集成到 CI/CD 流程**
6. **及时处理许可证问题**

通过使用 Trivy 的许可证扫描功能，您可以有效管理开源许可证合规性，避免法律风险，确保项目的长期可持续发展。

---

# Trivy 默认扫描器配置说明

## Trivy fs 默认扫描器配置

**`trivy fs` 默认情况下只会开启以下两个扫描器：**

1. **Vulnerability Scanner（漏洞扫描）** - `vuln`
2. **Secret Scanner（密钥扫描）** - `secret`

**默认情况下不会开启：**
- ❌ **Misconfiguration Scanner（配置错误扫描）** - `misconfig`
- ❌ **License Scanner（许可证扫描）** - `license`

## 代码证据

从 Trivy 源码 `pkg/flag/scan_flags.go` 中可以看到默认配置：

```go
ScannersFlag = Flag[[]string]{
    Name:       "scanners",
    ConfigName: "scan.scanners",
    Default: xstrings.ToStringSlice(types.Scanners{
        types.VulnerabilityScanner,  // 默认开启
        types.SecretScanner,         // 默认开启
    }),
    Values: xstrings.ToStringSlice(types.Scanners{
        types.VulnerabilityScanner,
        types.MisconfigScanner,      // 可选，但默认不开启
        types.SecretScanner,
        types.LicenseScanner,        // 可选，但默认不开启
    }),
}
```

## 如何启用所有扫描器

如果您想要启用 misconfiguration 和 license 扫描，需要显式指定：

### 启用所有扫描器

```bash
# 启用所有扫描器
trivy fs --scanners vuln,secret,misconfig,license /path/to/project

# 或者使用简写形式
trivy fs --scanners vuln,secret,misconfig,license .
```

### 启用特定扫描器

```bash
# 只启用配置错误扫描
trivy fs --scanners misconfig /path/to/project

# 只启用许可证扫描
trivy fs --scanners license /path/to/project

# 启用配置错误和许可证扫描
trivy fs --scanners misconfig,license /path/to/project
```

### 组合使用示例

```bash
# 漏洞 + 配置错误扫描
trivy fs --scanners vuln,misconfig /path/to/project

# 漏洞 + 许可证扫描
trivy fs --scanners vuln,license /path/to/project

# 密钥 + 配置错误扫描
trivy fs --scanners secret,misconfig /path/to/project
```

## 其他命令的默认扫描器

### trivy image 命令

```bash
# 默认启用所有扫描器
trivy image myapp:latest

# 等同于
trivy image --scanners vuln,secret,misconfig,license myapp:latest
```

### trivy repo 命令

```bash
# 默认只启用漏洞和密钥扫描
trivy repo /path/to/project

# 需要显式启用其他扫描器
trivy repo --scanners vuln,secret,misconfig,license /path/to/project
```

## 配置文件方式

您也可以通过配置文件来设置默认扫描器：

```yaml
# trivy.yaml
scan:
  scanners:
    - vuln
    - secret
    - misconfig
    - license
```

然后使用：

```bash
# 使用配置文件
trivy fs --config trivy.yaml /path/to/project
```

## 最佳实践建议

### 1. 开发环境

```bash
# 快速扫描，只检查漏洞和密钥
trivy fs /path/to/project
```

### 2. 预发布环境

```bash
# 完整扫描，包括配置错误
trivy fs --scanners vuln,secret,misconfig /path/to/project
```

### 3. 生产环境

```bash
# 全面扫描，包括许可证合规性
trivy fs --scanners vuln,secret,misconfig,license /path/to/project
```

### 4. CI/CD 集成

```bash
# 在 CI 中设置退出码，确保发现问题时构建失败
trivy fs --scanners vuln,secret,misconfig --exit-code 1 --severity HIGH,CRITICAL /path/to/project
```

## 扫描器说明

| 扫描器 | 标识符 | 用途 | 默认启用 |
|--------|--------|------|----------|
| Vulnerability Scanner | `vuln` | 检测软件包漏洞 | ✅ |
| Secret Scanner | `secret` | 检测硬编码密钥 | ✅ |
| Misconfiguration Scanner | `misconfig` | 检测配置错误 | ❌ |
| License Scanner | `license` | 检测许可证合规性 | ❌ |

## 注意事项

1. **性能考虑**：启用更多扫描器会增加扫描时间
2. **网络要求**：某些扫描器需要网络连接下载规则
3. **存储要求**：完整扫描可能需要更多磁盘空间
4. **权限要求**：某些扫描可能需要特定文件权限

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

# 离线环境下启用四个扫描器

在离线环境（air-gapped environment）中启用 Trivy 的所有四个扫描器需要预先准备和配置相关数据库和规则文件。以下是完整的实现步骤。

## 离线环境扫描器需求

### 四个扫描器及其依赖

| 扫描器 | 标识符 | 依赖资源 | 离线要求 |
|--------|--------|----------|----------|
| Vulnerability Scanner | `vuln` | Trivy DB | 需要预下载漏洞数据库 |
| Secret Scanner | `secret` | 内置规则 | 无需外部依赖 |
| Misconfiguration Scanner | `misconfig` | Trivy Checks Bundle | 需要预下载检查规则包 |
| License Scanner | `license` | 内置规则 | 无需外部依赖 |

## 方法一：使用内置检查（推荐）

### 优势
- **无需网络连接**：配置错误扫描使用内置检查规则
- **简单配置**：只需要下载漏洞数据库
- **快速部署**：适合大多数离线场景

### 实施步骤

#### 1. 在有网络的环境中准备数据库

```bash
# 创建缓存目录
mkdir -p /tmp/trivy-cache

# 下载漏洞数据库
trivy image --cache-dir /tmp/trivy-cache --download-db-only

# 下载 Java 数据库（如果需要扫描 Java 项目）
trivy image --cache-dir /tmp/trivy-cache --download-java-db-only
```

#### 2. 传输到离线环境

```bash
# 将缓存目录复制到离线环境
scp -r /tmp/trivy-cache user@offline-server:/home/user/.cache/trivy

# 或者使用其他传输方式
rsync -av /tmp/trivy-cache/ user@offline-server:/home/user/.cache/trivy/
```

#### 3. 在离线环境中启用所有扫描器

```bash
# 启用所有四个扫描器
trivy fs --scanners vuln,secret,misconfig,license /path/to/project

# 使用离线模式（避免尝试连接 Maven Central）
trivy fs --scanners vuln,secret,misconfig,license --offline-scan /path/to/project
```

## 方法二：自托管数据库（完整离线）

### 优势
- **完全离线**：所有资源都在内部网络
- **最新规则**：可以使用最新的检查规则包
- **企业级**：适合大型企业环境

### 实施步骤

#### 1. 准备容器镜像

在有网络的环境中：

```bash
# 使用 ORAS 下载数据库镜像
oras pull ghcr.io/aquasecurity/trivy-db:2
oras pull ghcr.io/aquasecurity/trivy-java-db:1
oras pull ghcr.io/aquasecurity/trivy-checks:latest

# 或者使用 Trivy 下载
trivy image --cache-dir . --download-db-only
trivy image --cache-dir . --download-java-db-only
```

#### 2. 设置内部容器仓库

```bash
# 推送到内部仓库
docker tag ghcr.io/aquasecurity/trivy-db:2 internal-registry.local/trivy-db:2
docker tag ghcr.io/aquasecurity/trivy-java-db:1 internal-registry.local/trivy-java-db:1
docker tag ghcr.io/aquasecurity/trivy-checks:latest internal-registry.local/trivy-checks:latest

docker push internal-registry.local/trivy-db:2
docker push internal-registry.local/trivy-java-db:1
docker push internal-registry.local/trivy-checks:latest
```

#### 3. 配置 Trivy 使用内部仓库

创建配置文件 `trivy-offline.yaml`：

```yaml
# trivy-offline.yaml
db:
  repository:
    - internal-registry.local/trivy-db:2
  java-repository:
    - internal-registry.local/trivy-java-db:1
  skip-update: true
  java-skip-update: true

misconfig:
  checks-bundle-repository:
    - internal-registry.local/trivy-checks:latest
  skip-checks-update: true
```

#### 4. 在离线环境中使用

```bash
# 使用配置文件启用所有扫描器
trivy fs --config trivy-offline.yaml --scanners vuln,secret,misconfig,license /path/to/project
```

## 方法三：手动缓存填充

### 实施步骤

#### 1. 下载数据库文件

```bash
# 使用 ORAS 下载
oras pull ghcr.io/aquasecurity/trivy-db:2
tar -xzf db.tar.gz

# 下载 Java 数据库
oras pull ghcr.io/aquasecurity/trivy-java-db:1
tar -xzf javadb.tar.gz

# 下载检查规则包
oras pull ghcr.io/aquasecurity/trivy-checks:latest
```

#### 2. 填充缓存目录

```bash
# 设置缓存目录
TRIVY_CACHE_DIR=/home/user/.cache/trivy

# 创建目录结构
mkdir -p ${TRIVY_CACHE_DIR}/db
mkdir -p ${TRIVY_CACHE_DIR}/javadb
mkdir -p ${TRIVY_CACHE_DIR}/checks

# 复制数据库文件
cp trivy.db metadata.json ${TRIVY_CACHE_DIR}/db/
cp trivy-java.db ${TRIVY_CACHE_DIR}/javadb/
cp -r checks/* ${TRIVY_CACHE_DIR}/checks/
```

#### 3. 启用所有扫描器

```bash
# 启用所有四个扫描器
trivy fs --scanners vuln,secret,misconfig,license /path/to/project
```

## 离线环境配置示例

### 完整配置文件

```yaml
# trivy-offline-full.yaml
# 全局配置
debug: false
quiet: false
timeout: 5m0s

# 缓存配置
cache:
  dir: "/home/user/.cache/trivy"
  backend: "fs"

# 数据库配置
db:
  repository:
    - internal-registry.local/trivy-db:2
  java-repository:
    - internal-registry.local/trivy-java-db:1
  skip-update: true
  java-skip-update: true

# 扫描配置
scan:
  scanners:
    - vuln
    - secret
    - misconfig
    - license
  offline-scan: true

# 配置错误扫描配置
misconfig:
  checks-bundle-repository:
    - internal-registry.local/trivy-checks:latest
  skip-checks-update: true

# 许可证扫描配置
license:
  forbidden:
    - AGPL-3.0
  restricted:
    - GPL-2.0
    - GPL-3.0
```

### 使用配置文件

```bash
# 使用完整配置启用所有扫描器
trivy fs --config trivy-offline-full.yaml /path/to/project

# 输出到文件
trivy fs --config trivy-offline-full.yaml --format json --output offline-scan-results.json /path/to/project
```

## 离线环境最佳实践

### 1. 数据库更新策略

```bash
# 定期更新脚本（在有网络的环境中运行）
#!/bin/bash
# update-trivy-db.sh

CACHE_DIR="/tmp/trivy-cache"
OFFLINE_SERVER="user@offline-server"

# 下载最新数据库
trivy image --cache-dir $CACHE_DIR --download-db-only
trivy image --cache-dir $CACHE_DIR --download-java-db-only

# 传输到离线环境
rsync -av $CACHE_DIR/ $OFFLINE_SERVER:/home/user/.cache/trivy/

echo "数据库更新完成"
```

### 2. 批量扫描脚本

```bash
#!/bin/bash
# offline-scan.sh

PROJECT_DIR="/path/to/project"
CONFIG_FILE="trivy-offline-full.yaml"
OUTPUT_DIR="/path/to/results"

# 创建输出目录
mkdir -p $OUTPUT_DIR

# 执行扫描
trivy fs --config $CONFIG_FILE \
  --scanners vuln,secret,misconfig,license \
  --format json \
  --output $OUTPUT_DIR/scan-results.json \
  $PROJECT_DIR

echo "扫描完成，结果保存在 $OUTPUT_DIR/scan-results.json"
```

### 3. CI/CD 集成

```yaml
# .gitlab-ci.yml 示例
stages:
  - security-scan

offline-security-scan:
  stage: security-scan
  script:
    - trivy fs --config trivy-offline-full.yaml --scanners vuln,secret,misconfig,license --exit-code 1 --severity HIGH,CRITICAL .
  artifacts:
    reports:
      junit: trivy-results.xml
    paths:
      - trivy-results.json
  only:
    - main
    - develop
```

## 故障排查

### 1. 数据库文件检查

```bash
# 检查缓存目录结构
ls -la ~/.cache/trivy/
ls -la ~/.cache/trivy/db/
ls -la ~/.cache/trivy/javadb/
ls -la ~/.cache/trivy/checks/

# 验证数据库文件
file ~/.cache/trivy/db/trivy.db
file ~/.cache/trivy/javadb/trivy-java.db
```

### 2. 网络连接测试

```bash
# 测试是否真的离线
trivy fs --scanners vuln --offline-scan /path/to/project

# 检查错误日志
trivy fs --scanners vuln,secret,misconfig,license --debug /path/to/project
```

### 3. 配置文件验证

```bash
# 验证配置文件语法
trivy config

# 测试配置
trivy fs --config trivy-offline-full.yaml --scanners vuln /path/to/project
```

## 注意事项

1. **数据库版本兼容性**：确保数据库版本与 Trivy 版本兼容
2. **存储空间**：完整数据库需要约 1-2GB 存储空间
3. **更新频率**：建议每周更新一次数据库
4. **权限设置**：确保 Trivy 有读取缓存目录的权限
5. **网络隔离**：使用 `--offline-scan` 标志确保不会尝试网络连接

------------------------------------------------------------------------------------------------------------------------------------------------------------------------

# Trivy 离线环境规则库读取机制详解

## 核心原理

Trivy 的配置错误扫描（misconfig scanner）实际上有**三层规则来源**，按优先级排序：

1. **外部 OCI 镜像规则包**（最高优先级）
2. **本地缓存规则包**（中等优先级）  
3. **内置嵌入规则包**（最低优先级，作为后备）

## 规则包存储位置

```bash
# Trivy 缓存目录结构
~/.cache/trivy/
├── db/                    # 漏洞数据库
├── javadb/               # Java 数据库
└── policy/               # 配置错误检查规则包
    ├── content/          # 规则内容
    │   ├── .manifest     # 规则清单文件
    │   ├── kubernetes/   # Kubernetes 规则
    │   ├── docker/       # Docker 规则
    │   ├── terraform/    # Terraform 规则
    │   └── ...
    └── metadata.json     # 元数据文件
```

## 规则加载优先级

Trivy 按以下顺序加载规则：

```go
// pkg/iac/rego/load.go 中的 LoadPolicies 方法
func (s *Scanner) LoadPolicies(srcFS fs.FS) error {
    // 1. 首先加载内置嵌入规则
    if err := s.loadEmbedded(); err != nil {
        return err
    }
    
    // 2. 如果启用内置规则，则使用嵌入规则
    if s.includeEmbeddedPolicies {
        s.policies = lo.Assign(s.policies, s.embeddedChecks)
    }
    
    // 3. 从本地文件系统加载规则（来自 OCI 镜像）
    if len(s.policyDirs) > 0 {
        loaded, err := LoadPoliciesFromDirs(srcFS, s.policyDirs...)
        // 本地规则会覆盖嵌入规则
        maps.Copy(s.policies, loaded)
    }
}
```

## 离线环境下的工作方式

### 方法一：使用内置嵌入规则（最简单）

```bash
# 直接使用内置规则，无需任何外部依赖
trivy fs --scanners misconfig /path/to/project
```

**工作原理：**
- Trivy 二进制文件在编译时已经嵌入了规则包
- 这些规则来自 `github.com/aquasecurity/trivy-checks` 仓库
- 通过 `checks.EmbeddedPolicyFileSystem` 访问
- **完全离线，无需网络连接**

### 方法二：使用本地缓存的 OCI 规则包

```bash
# 1. 在有网络的环境中下载规则包
trivy image --cache-dir /tmp/trivy-cache --download-db-only

# 2. 传输到离线环境
scp -r /tmp/trivy-cache user@offline-server:/home/user/.cache/trivy

# 3. 在离线环境中使用
trivy fs --scanners misconfig /path/to/project
```

**工作原理：**
- Trivy 从 OCI 镜像下载规则包到本地缓存
- 规则包以 OPA Bundle 格式存储
- 包含 `.manifest` 文件描述规则结构
- 离线时直接从本地缓存读取

### 方法三：使用自托管 OCI 仓库

```yaml
# trivy-offline.yaml
misconfig:
  checks-bundle-repository:
    - internal-registry.local/trivy-checks:latest
  skip-checks-update: true
```

**工作原理：**
- 将 OCI 规则包推送到内部仓库
- Trivy 从内部仓库拉取规则包
- 通过 `oci.NewArtifact()` 处理 OCI 镜像
- 解压并缓存到本地文件系统

## 规则包格式详解

### OPA Bundle 结构

```bash
# 规则包内容示例
policy/content/
├── .manifest              # 清单文件
│   {
│     "revision": "1",
│     "roots": ["kubernetes", "docker", "terraform"]
│   }
├── kubernetes/            # Kubernetes 规则
│   ├── security.rego
│   ├── networking.rego
│   └── ...
├── docker/                # Docker 规则
│   ├── security.rego
│   └── ...
└── terraform/             # Terraform 规则
    ├── aws.rego
    ├── azure.rego
    └── ...
```

### 规则文件示例

```rego
# kubernetes/security.rego
package kubernetes.security

import rego.v1

deny[msg] {
    input.kind == "Deployment"
    not input.spec.template.spec.securityContext.runAsNonRoot
    msg := "Container should run as non-root user"
}
```

## 实际执行流程

```
Trivy 启动
    ↓
检查本地缓存
    ↓
┌─────────────────┬─────────────────┐
│   有缓存        │    无缓存       │
└─────────────────┴─────────────────┘
    ↓                    ↓
从缓存加载规则      检查网络连接
    ↓                    ↓
加载 .manifest 文件  ┌─────────────┬─────────────┐
    ↓               │  有网络     │  无网络     │
解析规则目录结构     └─────────────┴─────────────┘
    ↓                    ↓               ↓
编译 Rego 规则      从 OCI 仓库下载   使用内置嵌入规则
    ↓                    ↓               ↓
执行扫描           解压到缓存目录      从二进制文件加载
    ↓                    ↓               ↓
                   从缓存加载规则      编译 Rego 规则
                        ↓               ↓
                   加载 .manifest 文件  执行扫描
                        ↓
                   解析规则目录结构
                        ↓
                   编译 Rego 规则
                        ↓
                   执行扫描
```

## 关键代码位置

- **规则加载**：`pkg/iac/rego/load.go`
- **嵌入规则**：`pkg/iac/rego/embed.go`
- **OCI 处理**：`pkg/policy/policy.go`
- **缓存管理**：`pkg/policy/policy.go`

## 为什么离线环境可以工作

1. **内置规则作为后备**：即使没有外部规则包，Trivy 也能使用编译时嵌入的规则
2. **本地缓存机制**：规则包下载后存储在本地，后续扫描直接使用缓存
3. **OCI 标准支持**：支持从任何 OCI 兼容的仓库获取规则包
4. **灵活的配置**：可以通过配置文件指定规则来源

## 验证规则加载状态

### 检查缓存目录

```bash
# 查看规则包缓存
ls -la ~/.cache/trivy/policy/
ls -la ~/.cache/trivy/policy/content/

# 查看规则清单
cat ~/.cache/trivy/policy/content/.manifest

# 查看元数据
cat ~/.cache/trivy/policy/metadata.json
```

### 启用调试模式

```bash
# 查看规则加载过程
trivy fs --scanners misconfig --debug /path/to/project

# 输出示例：
# 2024-01-01T10:00:00.000Z DEBUG Loading check bundle repository=internal-registry.local/trivy-checks:latest
# 2024-01-01T10:00:01.000Z DEBUG Embedded checks are loaded count=1500
# 2024-01-01T10:00:02.000Z DEBUG Checks from disk are loaded count=1500
```

### 检查规则来源

```bash
# 查看 Trivy 配置
trivy config

# 检查规则包版本
trivy version --format json | jq '.ChecksBundle'
```

## 规则包更新机制

### 自动更新

```bash
# Trivy 默认每 24 小时检查一次更新
# 可以通过配置文件禁用自动更新
```

### 手动更新

```bash
# 强制更新规则包
trivy image --download-db-only --force

# 清理缓存后重新下载
trivy clean --checks-bundle
trivy image --download-db-only
```

### 离线更新策略

```bash
#!/bin/bash
# update-offline-rules.sh

# 在有网络的环境中
CACHE_DIR="/tmp/trivy-cache"
OFFLINE_SERVER="user@offline-server"

# 下载最新规则包
trivy image --cache-dir $CACHE_DIR --download-db-only

# 传输到离线环境
rsync -av $CACHE_DIR/policy/ $OFFLINE_SERVER:/home/user/.cache/trivy/policy/

echo "规则包更新完成"
```

## 故障排查

### 规则包加载失败

```bash
# 检查规则包完整性
ls -la ~/.cache/trivy/policy/content/.manifest
file ~/.cache/trivy/policy/content/.manifest

# 验证 JSON 格式
cat ~/.cache/trivy/policy/content/.manifest | jq .
```

### 内置规则回退

```bash
# 如果外部规则包损坏，Trivy 会自动回退到内置规则
# 查看日志确认回退情况
trivy fs --scanners misconfig --debug /path/to/project 2>&1 | grep -i "embedded\|fallback"
```

### 规则包版本问题

```bash
# 检查规则包版本兼容性
trivy version
cat ~/.cache/trivy/policy/metadata.json | jq '.DownloadedAt'

# 如果版本不兼容，清理缓存重新下载
trivy clean --checks-bundle
```

## 最佳实践

### 1. 规则包管理

```bash
# 定期备份规则包
tar -czf trivy-policy-backup-$(date +%Y%m%d).tar.gz ~/.cache/trivy/policy/

# 版本控制规则包
git add ~/.cache/trivy/policy/
git commit -m "Update Trivy policy bundle"
```

### 2. 性能优化

```bash
# 使用 SSD 存储缓存目录
export TRIVY_CACHE_DIR="/fast-ssd/trivy-cache"

# 预加载规则包
trivy image --download-db-only
```

### 3. 安全考虑

```bash
# 验证规则包签名（如果支持）
# 使用内部可信的规则包源
# 定期审计规则包内容
```

## 总结

Trivy 的离线环境规则库读取机制设计得非常灵活和健壮：

1. **多层后备机制**：确保在任何情况下都能进行配置错误扫描
2. **标准 OCI 格式**：支持从任何 OCI 兼容的仓库获取规则包
3. **本地缓存优化**：避免重复下载，提高扫描效率
4. **内置规则保障**：即使完全离线也能提供基本的配置检查功能

这种设计使得 Trivy 能够在各种网络环境下稳定工作，特别适合企业内网、隔离环境或需要严格网络控制的安全环境。

## 参考资源

- [Trivy 官方文档 - 配置错误扫描](https://trivy.dev/latest/docs/scanner/misconfiguration/)
- [Trivy 官方文档 - 许可证扫描](https://trivy.dev/latest/docs/scanner/license/)
- [Trivy 官方文档 - 文件系统扫描](https://trivy.dev/latest/docs/target/filesystem/)
- [Trivy 高级网络场景](https://trivy.dev/latest/docs/advanced/network/)
- [Trivy 自托管数据库](https://trivy.dev/latest/docs/advanced/self-hosting/)
- [Trivy 配置选项](https://trivy.dev/latest/docs/configuration/)
- [OPA Bundle 规范](https://www.openpolicyagent.org/docs/latest/management-bundles/)
- [Trivy Checks 仓库](https://github.com/aquasecurity/trivy-checks)
