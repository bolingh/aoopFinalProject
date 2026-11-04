# 執行步驟
## 自動裝好 pygame 與 pybind11
```wsl
pip install -r requirements.txt
```
## 編譯出 .so
```wsl
g++ -O3 -Wall -shared -std=c++17 -fPIC $(python3 -m pybind11 --includes) ecosim.cpp -o ecosim$(python3-config --extension-suffix)
```
## 執行
```wsl
python3 main.py
```