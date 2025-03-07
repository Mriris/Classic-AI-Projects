# Classic-AI-Projects

## 项目简介
本仓库收录了本人在人工智能课程中完成的经典项目示例，主要包括：
- **启发式搜索算法**：基于 A* 算法实现的滑动拼图求解（8数码/15数码问题）。
- **动物专家系统**：利用规则库进行正向和逆向推理，依据动物特征匹配规则，进行动物分类的专家系统。

## 文件说明
- `启发式搜索算法.cpp`  
  实现了 A* 算法求解滑动拼图问题。代码中定义了状态结构、启发式函数（包括错误数字个数、错误数字距离、线性冲突和自创的纵横匹配）以及优先队列排序等。

- `动物专家系统.cpp`  
  实现了动物专家系统，支持正向推理和逆向推理。通过预设的特征和规则库，实现了简单的动物分类和推理匹配。

## 编译与运行

### 环境要求
- 编译器：支持 C/C++ 的编译器（如 GCC、Visual Studio 等）。
- 平台：Windows / Linux 均可。

### 编译方法
在命令行中可使用如下命令编译：
```bash
gcc 启发式搜索算法.cpp -o heuristic_search
gcc 动物专家系统.cpp -o animal_expert_system
```
也可使用对应的 IDE 打开源码文件后进行编译运行。

### 运行说明
- 运行 `heuristic_search` 后，根据提示输入码数、初始状态和选择的启发式函数，即可体验 A* 算法求解滑动拼图问题。
- 运行 `animal_expert_system` 后，根据提示输入动物特征数字（支持正向和逆向推理），系统会根据规则库输出匹配的动物特征或最终判断结果。

## 项目特点
- **经典算法与推理**：涵盖 A* 搜索算法及基于规则的专家系统推理，展示了两种常见的 AI 技术。

## 注意事项
- 部分代码可能依赖具体编译环境，请在运行前确保相关环境配置正确。
- 本项目代码仅作为学习和研究用途，不应用于商业场景。