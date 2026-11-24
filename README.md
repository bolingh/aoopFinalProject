# 專案簡介
此專案為簡易生態系模擬。使用Ｃ++計算、模擬生態系，並透過pybind11將運算數據導出至 Python，再使用Pygame圖像化顯示，完成能自主運行的生態模擬系統。程式方面，結合了pybind11以及MOOS-IvP的控制邏輯，即中央核心僅負責環境物理與狀態同步，各個代理人則具備獨立的感知、決策與行動機制。此架構能高度展現OOP的模組化、可維護性與可擴充性。

# 執行步驟
## 安裝所需工具
```wsl
pip install -r requirements.txt
```
## 編譯出 .so
註：尚未完成，現在編譯會產生嚴重錯誤
```wsl
g++ -O3 -Wall -shared -std=c++17 -fPIC $(python3 -m pybind11 --includes) World.cpp -o World$(python3-config --extension-suffix)
```
## 執行
```wsl
python3 main.py
```