# 🚀 C++ Collaborative Project Setup Guide

Welcome to the project!  
This guide helps every contributor set up their environment — whether you're using **Windows (VS Code)** or **WSL (Linux)** — and get productive with GitHub and C++.

---

## 🧰 1. Prerequisites

You’ll need:
- A **GitHub account**
- **Git** installed
- A **C++ compiler** (GCC, MinGW, or MSVC)
- **VS Code** (recommended)
- Internet connection 😄

---

## 🪟 2. Setup on Windows (VS Code)

### Install Required Tools

1. **Git**
   - Download: [https://git-scm.com/download/win](https://git-scm.com/download/win)
   - During install: choose **“Use Git from Windows Command Prompt.”**

2. **C++ Compiler**
   - Option 1: **MSYS2 (recommended)**  
     Download → [https://www.msys2.org/](https://www.msys2.org/)  
     Then open *MSYS2 MINGW64* terminal and run:
     ```bash
     pacman -S mingw-w64-x86_64-gcc
     ```
   - Option 2: **Visual Studio Build Tools**  
     Install via [Visual Studio Installer](https://visualstudio.microsoft.com/visual-cpp-build-tools/).

3. **VS Code**
   - Download: [https://code.visualstudio.com/](https://code.visualstudio.com/)
   - Extensions:
     - `ms-vscode.cpptools` (C/C++)
     - `ms-vscode.cmake-tools` (if project uses CMake)
     - `eamodio.gitlens` (GitLens, optional)

4. **Verify Installation**
   ```bash
   git --version
   g++ --version
