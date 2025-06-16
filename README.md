# C++ 计算器项目

这是一个简单的C++计算器项目，展示了完整的C++开发流程，包括编译、测试、打包和发布流程。

## 项目结构

```
.
├── src/            # 源代码目录
├── include/        # 头文件目录
├── tests/          # 测试文件目录
├── build/          # 构建输出目录
├── CMakeLists.txt  # CMake构建配置
└── .gitlab-ci.yml  # GitLab CI/CD配置
```

## 主要文件说明

- `calculator.h`: 计算器类的头文件
- `calculator.cpp`: 计算器类的实现
- `main.cpp`: 主程序
- `calculator_test.cpp`: 单元测试
- `CMakeLists.txt`: CMake构建配置
- `.gitlab-ci.yml`: GitLab CI/CD配置

## 功能特性

- 基本的数学运算：加、减、乘、除
- 完整的单元测试
- 自动化构建和部署流程

## 本地构建和测试

1. 创建构建目录：
```bash
mkdir build
cd build
```

2. 配置和构建项目：
```bash
cmake ..
make
```

3. 运行测试：
```bash
./calculator_test
```

4. 运行主程序：
```bash
./calculator
```

## CI/CD流程

项目使用GitLab CI/CD进行自动化构建和部署，包含以下阶段：

1. **构建阶段**
   - 使用CMake和Make编译项目
   - 生成可执行文件

2. **测试阶段**
   - 运行单元测试
   - 验证功能正确性

3. **打包阶段**
   - 将可执行文件打包成tar.gz格式
   - 保存构建产物

4. **部署阶段**
   - 准备部署环境
   - 执行部署操作（需要根据实际需求配置）

## 技术栈

- C++17
- CMake 3.10+
- GCC
- GitLab CI/CD

## 开发环境要求

- C++编译器（支持C++17）
- CMake 3.10或更高版本
- Make工具
- GitLab Runner（用于CI/CD）

## 使用说明

1. 克隆仓库：
```bash
git clone <repository-url>
```

2. 按照上述"本地构建和测试"步骤进行构建和测试

3. 修改代码后，推送到GitLab仓库将自动触发CI/CD流程

## 注意事项

- 确保GitLab Runner已正确配置
- 部署阶段需要根据实际需求进行配置
- 所有代码修改都应该通过测试验证

## 贡献指南

1. Fork项目
2. 创建特性分支
3. 提交更改
4. 推送到分支
5. 创建Pull Request

## 许可证

[添加许可证信息] 