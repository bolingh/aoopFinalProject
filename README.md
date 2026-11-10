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