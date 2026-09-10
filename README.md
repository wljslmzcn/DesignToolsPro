# DesignTools Pro

一个基于 Qt 6 C++ 的桌面设计工具应用，提供颜色选择器和 RGB 颜色表浏览功能。

## 📢 关注我们

- 🔥 **公众号**: 网络技术联盟站
- 🌐 **在线工具**: [https://tool.geek.cab](https://tool.geek.cab)

欢迎关注公众号获取更多技术干货，在线工具箱提供各类实用开发工具！

## 功能

- **🎨 颜色选择器** — 支持纯色、线性渐变、径向渐变模式，屏幕取色，生成 CSS 和 Qt 代码片段
- **🌈 RGB 颜色表** — 可按分类浏览和搜索 RGB 颜色
- **🌙/☀️ 主题切换** — 支持深色/浅色主题
- **💬 微信交流群** — 扫码加入微信交流群

## 环境要求

- Windows 10/11
- Qt 6.6.3 (MinGW)
- CMake 3.20+

## 构建

```bat
# 直接运行 build.bat
build.bat
```

或手动构建：

```bat
# 配置
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="D:\Qt\6.6.3\mingw_64" -B build .

# 编译
cmake --build build --target DesignToolsPro -j

# 输出
build\DesignToolsPro.exe
```

## 项目结构

```
DesignToolsPro/
├── src/
│   ├── main.cpp                 # 主窗口
│   └── pages/
│       ├── ColorPickerPage.h/cpp    # 颜色选择器页面
│       └── RgbColorTablePage.h/cpp  # RGB 颜色表页面
├── resources/
│   ├── resources.qrc            # Qt 资源文件
│   ├── app.rc                   # Windows 资源（图标）
│   └── app.ico                  # 应用图标
├── img/
│   └── wechat.jpg               # 微信交流群二维码
├── CMakeLists.txt               # CMake 构建配置
├── build.bat                    # 一键构建脚本
└── README.md
```

## 技术栈

- **框架**: Qt 6 Widgets
- **语言**: C++17
- **构建**: CMake + MinGW
- **平台**: Windows

## 截图

> 待添加

## 许可证

MIT License
