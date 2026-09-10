# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

DesignTools Pro — a Qt 6 C++ desktop application providing design utilities: a color picker (solid/linear-gradient/radial-gradient modes with screen color picking) and an RGB color table browser. Windows-only, built with MinGW and CMake.

## Build Commands

```bash
# 1. 先 kill 正在运行的进程
taskkill /F /IM DesignToolsPro.exe 2>nul

# 2. 配置
"D:\Qt\Tools\CMake_64\bin\cmake.exe" -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="D:\Qt\6.6.3\mingw_64" -B build .

# 3. 编译
"D:\Qt\Tools\CMake_64\bin\cmake.exe" --build build --target DesignToolsPro -j

# 输出
build\DesignToolsPro.exe
```

## Architecture

- **MainWindow** (`src/main.cpp`): QMainWindow subclass containing a `QStackedWidget` with two pages. Navigation via toolbar buttons at the top. Dark theme applied globally via `qApp->setStyleSheet()`.
- **ColorPickerPage** (`src/pages/ColorPickerPage.h/.cpp`): Color picker with solid/linear/radial gradient modes. Screen color picking via timer-based screenshot capture. Generates CSS and Qt code snippets.
- **RgbColorTablePage** (`src/pages/RgbColorTablePage.h/.cpp`): Browsable RGB color table with category filtering and search.

## Key Patterns

- All UI is built programmatically (no `.ui` files) — Qt's `AUTOUIC` is enabled but unused.
- `Q_OBJECT` macro required in any class with signals/slots; main.cpp includes `main.moc` at the bottom.
- Chinese (UTF-16) string literals used for UI text: `QString::fromUtf16(u"...")`.
- C++17 standard, Qt 6 Widgets only.
- Resources compiled via `resources/resources.qrc`; Windows icon via `resources/app.rc`.
- Post-build step copies Qt DLLs to output directory on Windows.

## Adding New Pages

To add a new page to the application:
1. Create `src/pages/YourPage.h` and `src/pages/YourPage.cpp` inheriting `QWidget`.
2. Add both files to `SOURCES` and `HEADERS` in `CMakeLists.txt`.
3. In `main.cpp`: include the header, instantiate the page, add it to `m_stackedWidget`, add a nav button, and update `m_pageMap`.
