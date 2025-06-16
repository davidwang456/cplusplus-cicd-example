# C++项目打包和部署工具指南

本文档介绍C++项目常用的打包和部署工具，帮助开发者选择适合的工具进行项目分发。

## 1. Conan包管理器

Conan是专门为C++设计的包管理器，支持跨平台，可以管理依赖关系。

### 基本配置示例
```python
# conanfile.py
from conans import ConanFile, CMake

class CalculatorConan(ConanFile):
    name = "calculator"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
```

## 2. vcpkg

vcpkg是微软开发的C++包管理器，与Visual Studio集成良好，支持Windows、Linux、macOS。

### 使用方式
```bash
vcpkg install calculator
```

## 3. Docker容器化

Docker可以将应用打包成容器，确保环境一致性。

### 示例Dockerfile
```dockerfile
FROM gcc:latest
WORKDIR /app
COPY . .
RUN mkdir build && cd build && \
    cmake .. && make
CMD ["./build/calculator"]
```

## 4. RPM/DEB包

使用rpmbuild或dpkg-buildpackage，适合Linux系统分发。

### 简化版RPM打包步骤（已编译完成）

如果您已经完成了编译和安装，只需要进行RPM打包，可以按照以下步骤操作：

1. **安装RPM构建工具**
```bash
# CentOS/RHEL
sudo yum install rpm-build rpmdevtools

# Fedora
sudo dnf install rpm-build rpmdevtools
```

2. **创建RPM构建环境**
```bash
# 创建RPM构建目录
rpmdev-setuptree
```

3. **创建简化的SPEC文件**
```spec
# ~/rpmbuild/SPECS/calculator.spec
Name:           calculator
Version:        1.0
Release:        1%{?dist}
Summary:        A simple calculator application

License:        MIT
URL:            https://your-company.com/calculator

%description
A simple calculator application that demonstrates C++ packaging.

%files
/usr/bin/calculator

%changelog
* Wed Mar 13 2024 Your Name <your.email@example.com> - 1.0-1
- Initial release
```

4. **准备文件**
```bash
# 假设您的可执行文件在 /path/to/your/calculator
# 复制到RPM构建目录
sudo mkdir -p ~/rpmbuild/BUILDROOT/calculator-1.0-1.el8.x86_64/usr/bin
sudo cp /path/to/your/calculator ~/rpmbuild/BUILDROOT/calculator-1.0-1.el8.x86_64/usr/bin/
```

5. **构建RPM包**
```bash
cd ~/rpmbuild/SPECS
rpmbuild -bb calculator.spec
```

6. **检查生成的RPM包**
```bash
# 检查RPM包信息
rpm -qip ~/rpmbuild/RPMS/x86_64/calculator-1.0-1.el8.x86_64.rpm

# 检查RPM包内容
rpm -qlp ~/rpmbuild/RPMS/x86_64/calculator-1.0-1.el8.x86_64.rpm
```

7. **安装测试**
```bash
# 安装RPM包
sudo rpm -ivh ~/rpmbuild/RPMS/x86_64/calculator-1.0-1.el8.x86_64.rpm

# 测试
calculator

# 卸载
sudo rpm -e calculator
```

8. **自动化脚本**
```bash
#!/bin/bash
# simple-build-rpm.sh

# 清理旧的构建
rm -rf ~/rpmbuild/BUILDROOT/*

# 准备文件
sudo mkdir -p ~/rpmbuild/BUILDROOT/calculator-1.0-1.el8.x86_64/usr/bin
sudo cp /path/to/your/calculator ~/rpmbuild/BUILDROOT/calculator-1.0-1.el8.x86_64/usr/bin/

# 构建RPM
cd ~/rpmbuild/SPECS
rpmbuild -bb calculator.spec

# 检查结果
if [ $? -eq 0 ]; then
    echo "RPM build successful"
    ls -l ~/rpmbuild/RPMS/x86_64/
else
    echo "RPM build failed"
    exit 1
fi
```

注意事项：
1. 确保文件权限正确
2. 检查文件路径是否正确
3. 确保SPEC文件中的版本号与实际一致
4. 如果文件有依赖，需要在SPEC文件中添加Requires标签

## 5. NuGet包

主要用于Windows平台，与Visual Studio集成，适合.NET和C++混合项目。

## 6. Bazel构建系统

Google开发的构建工具，支持多语言。

### 示例BUILD文件
```python
cc_binary(
    name = "calculator",
    srcs = ["src/main.cpp", "src/calculator.cpp"],
    hdrs = ["include/calculator.h"],
)
```

## 7. Meson构建系统

现代化的构建系统，配置简单。

### 示例meson.build
```python
project('calculator', 'cpp')
executable('calculator',
    'src/main.cpp',
    'src/calculator.cpp',
    include_directories: include_directories('include')
)
```

## 8. GitHub Packages

可以发布各种格式的包，支持版本控制，与GitHub Actions集成。

## 9. CPack

CPack是CMake的打包工具，支持多种打包格式，使用简单，与CMake构建系统无缝集成。

### 基本用法

1. **在CMakeLists.txt中配置CPack**
```cmake
# 设置CPack基本配置
set(CPACK_PACKAGE_NAME "calculator")
set(CPACK_PACKAGE_VERSION "1.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple calculator")
set(CPACK_PACKAGE_VENDOR "Your Company")
set(CPACK_PACKAGE_CONTACT "your.email@example.com")

# 选择打包格式
set(CPACK_GENERATOR "RPM")  # 可选：RPM, DEB, TGZ, ZIP, NSIS等

# 设置RPM特定配置
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")

# 包含CPack模块
include(CPack)
```

2. **使用CPack命令打包**
```bash
# 在build目录下执行
cpack

# 指定打包格式
cpack -G RPM

# 查看支持的打包格式
cpack --help
```

### 支持的打包格式

1. **RPM包（Linux）**
```cmake
set(CPACK_GENERATOR "RPM")
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
```

2. **DEB包（Debian/Ubuntu）**
```cmake
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
```

3. **TGZ/ZIP包（跨平台）**
```cmake
set(CPACK_GENERATOR "TGZ")
# 或
set(CPACK_GENERATOR "ZIP")
```

4. **NSIS安装程序（Windows）**
```cmake
set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/icon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/icon.ico")
```

### 高级配置

1. **文件安装规则**
```cmake
# 安装可执行文件
install(TARGETS calculator
    RUNTIME DESTINATION bin
)

# 安装文档
install(FILES README.md LICENSE
    DESTINATION share/doc/calculator
)

# 安装配置文件
install(FILES config.ini
    DESTINATION etc/calculator
)
```

2. **依赖管理**
```cmake
# RPM依赖
set(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.17")

# DEB依赖
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.17)")
```

3. **多组件打包**
```cmake
# 定义组件
set(CPACK_COMPONENTS_ALL applications libraries headers)

# 设置组件显示名称
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Calculator Application")
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Calculator Libraries")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")

# 设置组件描述
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "Calculator executable")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Shared libraries")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION "C++ header files")
```

### 使用场景

1. **简单应用打包**
   - 单个可执行文件
   - 少量配置文件
   - 适合使用TGZ/ZIP格式

2. **系统包分发**
   - 需要系统集成
   - 有依赖关系
   - 适合使用RPM/DEB格式

3. **Windows应用分发**
   - 需要安装程序
   - 需要开始菜单集成
   - 适合使用NSIS格式

4. **跨平台库分发**
   - 包含头文件
   - 包含库文件
   - 适合使用多组件打包

### 最佳实践

1. **版本管理**
```cmake
# 使用项目版本
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

# 使用语义化版本号
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
```

2. **文件组织**
```cmake
# 遵循FHS标准
set(CPACK_PACKAGE_INSTALL_DIRECTORY "calculator")
set(CPACK_INSTALL_PREFIX "/usr")
```

3. **测试集成**
```cmake
# 在打包前运行测试
enable_testing()
add_test(NAME calculator_test COMMAND calculator_test)
```

4. **自动化脚本**
```bash
#!/bin/bash
# build-and-package.sh

# 构建项目
mkdir -p build
cd build
cmake ..
make

# 运行测试
ctest

# 打包
cpack
```

### 注意事项

1. **平台特定配置**
   - 检查目标平台支持
   - 设置正确的架构
   - 处理平台特定依赖

2. **权限管理**
   - 设置正确的文件权限
   - 处理特殊文件（如配置文件）

3. **调试技巧**
   - 使用`cpack --debug`查看详细信息
   - 检查生成的文件列表
   - 验证包内容

4. **常见问题**
   - 确保所有文件都被正确安装
   - 检查依赖关系
   - 验证包的可安装性

### CPack版本设置

1. **基本版本设置**
```bash
# 设置主版本号
cpack -D CPACK_PACKAGE_VERSION="1.0.0"

# 分别设置主版本号、次版本号、修订号
cpack -D CPACK_PACKAGE_VERSION_MAJOR="1" \
      -D CPACK_PACKAGE_VERSION_MINOR="0" \
      -D CPACK_PACKAGE_VERSION_PATCH="0"
```

2. **使用项目版本**
```bash
# 使用CMake项目版本
cpack -D CPACK_PACKAGE_VERSION=${PROJECT_VERSION}

# 在CMakeLists.txt中设置
project(Calculator VERSION 1.0.0)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
```

3. **不同格式的版本设置**
```bash
# RPM包版本
cpack -G RPM -D CPACK_RPM_PACKAGE_VERSION="1.0.0" \
            -D CPACK_RPM_PACKAGE_RELEASE="1"

# DEB包版本
cpack -G DEB -D CPACK_DEBIAN_PACKAGE_VERSION="1.0.0-1"

# NSIS安装程序版本
cpack -G NSIS -D CPACK_PACKAGE_VERSION="1.0.0" \
             -D CPACK_NSIS_DISPLAY_VERSION="1.0.0"
```

4. **版本号格式**
```bash
# 语义化版本号 (MAJOR.MINOR.PATCH)
cpack -D CPACK_PACKAGE_VERSION="1.0.0"

# 带发布号的版本
cpack -D CPACK_PACKAGE_VERSION="1.0.0" \
      -D CPACK_PACKAGE_RELEASE="1"

# 完整版本号
cpack -D CPACK_PACKAGE_VERSION="1.0.0-1"
```

5. **版本号验证**
```bash
# 使用正则表达式验证版本号格式
if(NOT CPACK_PACKAGE_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+$")
    message(FATAL_ERROR "Invalid version format: ${CPACK_PACKAGE_VERSION}")
endif()
```

6. **版本号最佳实践**
```bash
# 使用语义化版本号
cpack -D CPACK_PACKAGE_VERSION="1.0.0"

# 添加构建号
cpack -D CPACK_PACKAGE_VERSION="1.0.0" \
      -D CPACK_PACKAGE_BUILD_NUMBER="123"

# 添加预发布标识
cpack -D CPACK_PACKAGE_VERSION="1.0.0-beta.1"
```

7. **版本号组合使用**
```bash
# 完整版本信息
cpack -D CPACK_PACKAGE_NAME="calculator" \
      -D CPACK_PACKAGE_VERSION="1.0.0" \
      -D CPACK_PACKAGE_RELEASE="1" \
      -D CPACK_PACKAGE_BUILD_NUMBER="123" \
      -D CPACK_GENERATOR="RPM"
```

注意事项：
1. 版本号应该遵循语义化版本规范
2. 不同包格式可能有不同的版本号要求
3. 版本号应该与项目版本保持一致
4. 考虑使用构建号或发布号
5. 版本号中避免使用特殊字符

## 工具选择建议

根据不同的使用场景，推荐以下工具：

1. **跨平台项目**
   - 推荐使用Conan或vcpkg
   - 优点：依赖管理完善，跨平台支持好

2. **Linux系统**
   - 推荐使用RPM/DEB
   - 优点：系统集成度高，包管理方便

3. **容器化部署**
   - 推荐使用Docker
   - 优点：环境一致性，部署简单

4. **Windows平台**
   - 推荐使用NuGet
   - 优点：与Visual Studio集成好

5. **大型项目**
   - 推荐使用Bazel或Meson
   - 优点：构建效率高，配置灵活

## 最佳实践建议

1. **版本控制**
   - 使用语义化版本号
   - 保持版本号的一致性

2. **依赖管理**
   - 明确声明依赖版本
   - 使用锁文件固定依赖版本

3. **构建配置**
   - 提供清晰的构建说明
   - 支持不同的构建类型（Debug/Release）

4. **测试集成**
   - 在打包前运行测试
   - 提供测试覆盖率报告

5. **文档维护**
   - 提供详细的安装说明
   - 包含使用示例和API文档

## 注意事项

1. 选择合适的打包工具要考虑：
   - 目标平台
   - 项目规模
   - 团队熟悉度
   - 维护成本

2. 确保打包过程：
   - 可重复
   - 自动化
   - 可验证
   - 可追踪

3. 考虑安全性：
   - 签名验证
   - 依赖检查
   - 漏洞扫描 