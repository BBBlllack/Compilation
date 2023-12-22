### 编译原理代码使用说明

#### 计212 申洪建 210236

### 本代码已经托管在github上
https://github.com/BBBlllack/Compilation/

### 接下来通过tree介绍各个文件

├── CMakeLists.txt
├── README.MD
├── cmake-build-debug
│   ├── Closure.txt   **实验二中自动求解的项目集闭包**
│   ├── LR_Table.txt  **实验二中自动求解的分析表**
│   ├── out.txt  **实验一种的词法分析结果**
│   ├── parse.txt  **实验二中ll分析法的解析结果**
│   └── sentence_0.txt. **实验二中自动求解的句子分析结果**
└── cppBase
    ├── exp1
    │   ├── code.txt  **实验一的测试输入**
    │   ├── functions.h  **头文件**
    │   └── main.cpp **实验一的源程序文件**
    ├── exp2
    │   ├── autolr1 **输入文法自动求解项目集的lr分析, 此算法使用需要搭配实验一，具体使用方式在此main.cpp中有详细介绍**
    │   │   ├── functions.h   **头文件**
    │   │   └── main.cpp   **头文件**
    │   ├── ll1 **ll1分析法**
    │   │   ├── functions.h   **头文件**
    │   │   └── main.cpp  **原文件**
    │   └── lr  **lr分析**
    │       ├── code.txt  **输入文件**
    │       ├── fun.h   **头文件**
    │       ├── functions.h   **头文件**
    │       └── main.cpp
    └── exp3  **语义分析**
        ├── MyLex.h
        ├── code  **语义分析输入**
        ├── functions.h
        ├── funs.h
        └── main.cpp  **原文件**
