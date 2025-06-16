cmake_minimum_required(VERSION 3.10)
project(Calculator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(calculator 
    src/main.cpp
    src/calculator.cpp
)

# 添加测试可执行文件
add_executable(calculator_test
    tests/calculator_test.cpp
    src/calculator.cpp
)

# 设置包含目录
target_include_directories(calculator PRIVATE include)
target_include_directories(calculator_test PRIVATE include)

# 安装规则
install(TARGETS calculator
    RUNTIME DESTINATION bin
)

# 设置CPack配置
set(CPACK_PACKAGE_NAME "calculator")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple calculator application")
set(CPACK_PACKAGE_VENDOR "Your Company")
set(CPACK_PACKAGE_CONTACT "your.email@example.com")

# 设置RPM包配置
set(CPACK_GENERATOR "RPM")
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
set(CPACK_RPM_PACKAGE_URL "https://your-company.com")
set(CPACK_RPM_PACKAGE_REQUIRES "glibc")

include(CPack) 