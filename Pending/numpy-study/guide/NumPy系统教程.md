# NumPy系统教程

## 课程目录

**第一章：NumPy基础**
1. [第一讲：NumPy简介与ndarray对象](#第一讲numpy简介与ndarray对象)
2. [第二讲：创建数组的多种方法](#第二讲创建数组的多种方法)
3. [第三讲：数组的基本属性与数据类型](#第三讲数组的基本属性与数据类型)

**第二章：数组操作**

4. [第四讲：数组索引与切片](#第四讲数组索引与切片)
5. [第五讲：数组形状操作](#第五讲数组形状操作)
6. [第六讲：数组的组合与分割](#第六讲数组的组合与分割)

**第三章：数学运算**

7. [第七讲：基本数学运算](#第七讲基本数学运算)
8. [第八讲：统计函数](#第八讲统计函数)
9. [第九讲：线性代数基础](#第九讲线性代数基础)

**第四章：高级功能**

10. [第十讲：广播机制](#第十讲广播机制)
11. [第十一讲：花式索引与布尔索引](#第十一讲花式索引与布尔索引)
12. [第十二讲：结构化数组与文件IO](#第十二讲结构化数组与文件io)

---

# 第一讲：NumPy简介与ndarray对象

## 概念
**NumPy**（Numerical Python）是Python科学计算的核心库，提供了一个高性能的多维数组对象`ndarray`，以及用于操作这些数组的工具。

**ndarray**（N-dimensional array，N维数组）是NumPy的核心数据结构，它是一个同质（所有元素类型相同）、固定大小的多维容器，用于存储相同类型的元素。

## 原理
1. **内存连续**：ndarray中的数据在内存中是连续存储的，这使得访问速度更快
2. **类型明确**：创建数组时需要指定数据类型，避免了Python列表的类型检查开销
3. **向量化操作**：可以对整个数组执行操作，而不需要循环，底层由C/C++实现
4. **同质性**：所有元素必须是相同数据类型，这是性能优化的关键

## 例子
```python
import numpy as np

# 1. 从Python列表创建数组
list_data = [1, 2, 3, 4, 5]
arr = np.array(list_data)
print("一维数组:", arr)
print("数组类型:", type(arr))

# 2. 二维数组
list_2d = [[1, 2, 3], [4, 5, 6]]
arr_2d = np.array(list_2d)
print("\n二维数组:")
print(arr_2d)

# 3. 查看数组属性
print("\n数组属性:")
print("维度:", arr_2d.ndim)  # 2
print("形状:", arr_2d.shape)  # (2, 3)
print("元素总数:", arr_2d.size)  # 6
print("数据类型:", arr_2d.dtype)  # int64 (取决于系统)

# 4. 简单向量化运算
print("\n向量化运算:")
print("数组 + 2:", arr + 2)  # 每个元素加2
print("数组 * 3:", arr * 3)  # 每个元素乘3

# 5. 与Python列表对比
import time

# Python列表运算
py_list = list(range(1000000))
start = time.time()
result = [x + 2 for x in py_list]
py_time = time.time() - start

# NumPy数组运算
np_arr = np.array(py_list)
start = time.time()
result = np_arr + 2
np_time = time.time() - start

print(f"\n性能对比:")
print(f"Python列表时间: {py_time:.4f}秒")
print(f"NumPy数组时间: {np_time:.4f}秒")
print(f"NumPy速度提升: {py_time/np_time:.1f}倍")
```

## 总结
1. **NumPy核心**是`ndarray`对象，用于高效存储和处理多维同质数据
2. **关键特性**：
   - 内存连续存储
   - 固定数据类型
   - 向量化操作
   - 广播功能
3. **主要优势**：
   - 比Python列表快10-100倍
   - 代码更简洁（向量化代替循环）
   - 丰富的数学函数库
4. **适用场景**：数值计算、数据分析、机器学习、图像处理等需要高效数值运算的领域

**第一讲重点**：理解ndarray是NumPy的核心，掌握其基本创建方法和属性查看，体会向量化运算的高效性。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第二讲：创建数组的多种方法

## 概念
NumPy提供了多种创建数组的方法，从简单的列表转换到复杂的数值序列生成。了解这些方法能帮助你更高效地准备和处理数据。

## 原理
NumPy数组创建方法可以分为几类：
1. **从现有数据转换**：从Python数据结构（列表、元组等）创建
2. **内置数组创建函数**：使用NumPy内置函数生成特定模式的数组
3. **从文件或外部数据源读取**：从磁盘文件、数据库等加载数据
4. **特殊数组生成**：生成特定数学模式或随机数的数组

## 例子

```python
import numpy as np

# 1. 从Python数据结构创建
print("=== 1. 从Python数据结构创建 ===")

# 从列表创建
list_data = [1, 2, 3, 4, 5]
arr_from_list = np.array(list_data)
print("从列表创建:", arr_from_list)

# 从元组创建
tuple_data = (6, 7, 8, 9, 10)
arr_from_tuple = np.array(tuple_data)
print("从元组创建:", arr_from_tuple)

# 从嵌套列表创建二维数组
nested_list = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
arr_2d = np.array(nested_list)
print("二维数组:\n", arr_2d)

# 2. 使用内置函数创建特殊数组
print("\n=== 2. 使用内置函数创建特殊数组 ===")

# 创建全零数组
zeros_arr = np.zeros((3, 4))
print("3x4全零数组:\n", zeros_arr)

# 创建全1数组
ones_arr = np.ones((2, 3))
print("2x3全1数组:\n", ones_arr)

# 创建单位矩阵（方阵，主对角线为1，其余为0）
identity = np.eye(4)
print("4x4单位矩阵:\n", identity)

# 创建未初始化的数组（值随机，取决于内存状态）
empty_arr = np.empty((2, 2))
print("2x2未初始化数组（值随机）:\n", empty_arr)

# 创建填充指定值的数组
full_arr = np.full((2, 3), 7)  # 2x3数组，所有元素为7
print("2x3全7数组:\n", full_arr)

# 3. 使用序列生成函数
print("\n=== 3. 使用序列生成函数 ===")

# arange: 类似Python的range，但返回数组
range_arr = np.arange(0, 10, 2)  # 从0到10（不含），步长为2
print("arange(0, 10, 2):", range_arr)

# linspace: 生成等差数列，指定元素个数
lin_arr = np.linspace(0, 1, 5)  # 从0到1，生成5个等间距的数
print("linspace(0, 1, 5):", lin_arr)

# logspace: 生成等比数列
log_arr = np.logspace(0, 2, 5)  # 10^0到10^2，生成5个数
print("logspace(0, 2, 5):", log_arr)

# 4. 创建随机数组
print("\n=== 4. 创建随机数组 ===")

# 设置随机种子，使结果可重复
np.random.seed(42)

# 生成0-1之间的均匀分布随机数
rand_arr = np.random.rand(3, 3)
print("3x3均匀分布随机数组:\n", rand_arr)

# 生成标准正态分布随机数
randn_arr = np.random.randn(2, 2)
print("2x2标准正态分布随机数组:\n", randn_arr)

# 生成指定范围内的随机整数
randint_arr = np.random.randint(0, 100, (3, 4))  # 0-100的随机整数，形状3x4
print("0-100的3x4随机整数数组:\n", randint_arr)

# 5. 从现有数组创建
print("\n=== 5. 从现有数组创建 ===")

# 复制数组
original = np.array([[1, 2], [3, 4]])
copy_arr = np.copy(original)
print("原始数组:\n", original)
print("复制数组:\n", copy_arr)
print("它们是同一个对象吗？", original is copy_arr)
print("它们值相等吗？", np.array_equal(original, copy_arr))

# 创建类似数组（相同形状，不同值）
zeros_like = np.zeros_like(original)
print("与original相同形状的全零数组:\n", zeros_like)

ones_like = np.ones_like(original)
print("与original相同形状的全1数组:\n", ones_like)

full_like = np.full_like(original, 9)  # 填充值9
print("与original相同形状的全9数组:\n", full_like)

# 6. 高级创建方法
print("\n=== 6. 高级创建方法 ===")

# meshgrid: 生成坐标矩阵
x = np.array([0, 1, 2])
y = np.array([0, 1, 2, 3])
X, Y = np.meshgrid(x, y)
print("X坐标矩阵:\n", X)
print("Y坐标矩阵:\n", Y)

# 创建对角线数组
diag_arr = np.diag([1, 2, 3, 4])  # 对角线为给定值，其余为0
print("对角线数组:\n", diag_arr)

# 创建范德蒙德矩阵
vander_arr = np.vander([1, 2, 3, 4], 3)  # 生成范德蒙德矩阵
print("范德蒙德矩阵:\n", vander_arr)

# 7. 指定数据类型创建
print("\n=== 7. 指定数据类型创建 ===")

# 创建数组时指定数据类型
float_arr = np.array([1, 2, 3], dtype=np.float32)
print("float32数组:", float_arr, "数据类型:", float_arr.dtype)

int_arr = np.array([1.1, 2.2, 3.3], dtype=np.int32)  # 注意会截断小数
print("int32数组:", int_arr, "数据类型:", int_arr.dtype)

complex_arr = np.array([1, 2, 3], dtype=np.complex128)
print("复数数组:", complex_arr, "数据类型:", complex_arr.dtype)

bool_arr = np.array([0, 1, 0, 1], dtype=np.bool_)
print("布尔数组:", bool_arr, "数据类型:", bool_arr.dtype)

# 8. 综合示例
print("\n=== 8. 综合示例 ===")

# 创建一个3x3的魔方矩阵
magic_square = np.array([[8, 1, 6], [3, 5, 7], [4, 9, 2]])
print("3x3魔方矩阵:\n", magic_square)

# 创建一个正弦波
x = np.linspace(0, 2*np.pi, 20)  # 0到2π，20个点
y = np.sin(x)
print("正弦波x坐标:", x)
print("正弦波y坐标:", y)
```

## 总结
1. **从Python结构创建**：`np.array()` 是最基本的方法，可以从列表、元组等创建数组
2. **特殊数组函数**：
   - `np.zeros()` / `np.ones()`：创建全0/全1数组
   - `np.empty()`：创建未初始化数组（最快，但值不确定）
   - `np.full()`：创建填充指定值的数组
   - `np.eye()` / `np.identity()`：创建单位矩阵
3. **序列生成函数**：
   - `np.arange()`：类似range，生成等差数列
   - `np.linspace()`：生成指定个数的等差数列
   - `np.logspace()`：生成等比数列
4. **随机数组**：
   - `np.random.rand()`：均匀分布
   - `np.random.randn()`：标准正态分布
   - `np.random.randint()`：随机整数
5. **从现有数组创建**：
   - `np.copy()`：深度复制数组
   - `np.zeros_like()` / `np.ones_like()`：创建同形状的特殊数组
6. **数据类型控制**：创建数组时可用`dtype`参数指定数据类型，影响内存和精度
7. **选择建议**：
   - 已知具体值用`np.array()`
   - 需要占位符用`np.zeros()`或`np.ones()`
   - 需要数值序列用`np.arange()`或`np.linspace()`
   - 需要随机数用`np.random`模块
   - 性能优先用`np.empty()`（但要记得初始化）

**第二讲重点**：掌握NumPy提供的多种数组创建方法，根据不同场景选择最合适的方法，理解不同创建方式的特点和适用场景。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第三讲：数组的基本属性与数据类型

## 概念
NumPy数组（ndarray）具有一系列描述其形状、大小和数据类型的属性。理解这些属性是操作数组的基础。数据类型（dtype）是NumPy的核心特性之一，它决定了数组元素在内存中的存储方式，并影响计算效率和精度。

## 原理
1. **数组属性**：包括形状（shape）、维度（ndim）、元素总数（size）、数据类型（dtype）、每个元素字节大小（itemsize）等，这些属性共同定义了一个数组的结构。
2. **数据类型（dtype）**：NumPy提供了丰富的数据类型，从整数、浮点数到复数等，每种类型有特定的内存占用和数值范围。数据类型是固定的，创建数组后不能更改（除非复制数组并转换类型）。
3. **类型转换**：可以通过`astype()`方法转换数组的数据类型，但需要注意转换可能带来的精度损失或值域变化。

## 例子
```python
import numpy as np

# 1. 创建数组并查看其属性
print("=== 1. 数组属性 ===")
arr = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]])
print("数组:\n", arr)

# 基本属性
print("数组维度 (ndim):", arr.ndim)          # 2
print("数组形状 (shape):", arr.shape)        # (4, 3)
print("数组元素总数 (size):", arr.size)       # 12
print("数组数据类型 (dtype):", arr.dtype)    # int64（取决于系统）
print("每个元素字节大小 (itemsize):", arr.itemsize, "字节")  # 8（int64是8字节）
print("数组总字节数 (nbytes):", arr.nbytes, "字节")  # 96 = 12 * 8
print("数组的转置:\n", arr.T)  # 转置属性

# 2. 理解不同维度的数组
print("\n=== 2. 不同维度的数组 ===")
# 标量（0维数组）
scalar = np.array(5)
print("标量:", scalar)
print("标量的维度:", scalar.ndim)  # 0
print("标量的形状:", scalar.shape) # ()

# 一维数组
arr_1d = np.array([1, 2, 3])
print("\n一维数组:", arr_1d)
print("一维数组的维度:", arr_1d.ndim)  # 1
print("一维数组的形状:", arr_1d.shape) # (3,)

# 二维数组
arr_2d = np.array([[1, 2, 3], [4, 5, 6]])
print("\n二维数组:\n", arr_2d)
print("二维数组的维度:", arr_2d.ndim)  # 2
print("二维数组的形状:", arr_2d.shape) # (2, 3)

# 三维数组
arr_3d = np.array([[[1, 2], [3, 4]], [[5, 6], [7, 8]]])
print("\n三维数组:\n", arr_3d)
print("三维数组的维度:", arr_3d.ndim)  # 3
print("三维数组的形状:", arr_3d.shape) # (2, 2, 2)

# 3. 数据类型（dtype）详解
print("\n=== 3. 数据类型（dtype） ===")
# NumPy支持的数据类型
# 整数类型
int8_arr = np.array([1, 2, 3], dtype=np.int8)
print("int8数组:", int8_arr, "dtype:", int8_arr.dtype, "范围:", np.iinfo(np.int8))

int16_arr = np.array([1, 2, 3], dtype=np.int16)
print("int16数组:", int16_arr, "dtype:", int16_arr.dtype, "范围:", np.iinfo(np.int16))

int32_arr = np.array([1, 2, 3], dtype=np.int32)
print("int32数组:", int32_arr, "dtype:", int32_arr.dtype, "范围:", np.iinfo(np.int32))

int64_arr = np.array([1, 2, 3], dtype=np.int64)
print("int64数组:", int64_arr, "dtype:", int64_arr.dtype, "范围:", np.iinfo(np.int64))

# 无符号整数
uint8_arr = np.array([1, 2, 3], dtype=np.uint8)
print("\nuint8数组:", uint8_arr, "dtype:", uint8_arr.dtype, "范围:", np.iinfo(np.uint8))

# 浮点数类型
float16_arr = np.array([1.1, 2.2, 3.3], dtype=np.float16)
print("\nfloat16数组:", float16_arr, "dtype:", float16_arr.dtype, "精度:", np.finfo(np.float16))

float32_arr = np.array([1.1, 2.2, 3.3], dtype=np.float32)
print("float32数组:", float32_arr, "dtype:", float32_arr.dtype, "精度:", np.finfo(np.float32))

float64_arr = np.array([1.1, 2.2, 3.3], dtype=np.float64)
print("float64数组:", float64_arr, "dtype:", float64_arr.dtype, "精度:", np.finfo(np.float64))

# 复数类型
complex64_arr = np.array([1+2j, 3+4j], dtype=np.complex64)
print("\ncomplex64数组:", complex64_arr, "dtype:", complex64_arr.dtype)

complex128_arr = np.array([1+2j, 3+4j], dtype=np.complex128)
print("complex128数组:", complex128_arr, "dtype:", complex128_arr.dtype)

# 布尔类型
bool_arr = np.array([True, False, True], dtype=np.bool_)
print("\n布尔数组:", bool_arr, "dtype:", bool_arr.dtype)

# 字符串类型
str_arr = np.array(['hello', 'world'], dtype=np.str_)
print("\n字符串数组:", str_arr, "dtype:", str_arr.dtype)

# 4. 数据类型转换
print("\n=== 4. 数据类型转换 ===")
# 使用astype()方法转换数据类型
original_arr = np.array([1.7, 2.3, 3.8, 4.1])
print("原始数组 (float):", original_arr, "dtype:", original_arr.dtype)

# 转换为整数（截断小数部分）
int_arr = original_arr.astype(np.int32)
print("转换为int32:", int_arr, "dtype:", int_arr.dtype)

# 转换为不同精度的浮点数
float16_arr = original_arr.astype(np.float16)
print("转换为float16:", float16_arr, "dtype:", float16_arr.dtype)

# 注意：转换时可能会损失精度或溢出
big_float_arr = np.array([1.2e100, 2.3e200], dtype=np.float64)
print("\n大浮点数数组 (float64):", big_float_arr)
try:
    # 尝试转换为float16，会溢出
    small_float_arr = big_float_arr.astype(np.float16)
    print("转换为float16:", small_float_arr)
except Exception as e:
    print("转换出错:", e)

# 5. 数据类型对计算的影响
print("\n=== 5. 数据类型对计算的影响 ===")
# 整数溢出示例
int8_max = np.array([127], dtype=np.int8)  # int8最大值是127
print("int8最大值:", int8_max)
int8_max_plus_one = int8_max + 1
print("int8最大值+1:", int8_max_plus_one, "（溢出！）")
print("计算后数据类型:", int8_max_plus_one.dtype)

# 浮点数精度问题
float32_arr = np.array([0.1, 0.2, 0.3], dtype=np.float32)
float64_arr = np.array([0.1, 0.2, 0.3], dtype=np.float64)
print("\n0.1 + 0.2 + 0.3 在float32中:", np.sum(float32_arr))
print("0.1 + 0.2 + 0.3 在float64中:", np.sum(float64_arr))
print("精确值应该是: 0.6")

# 6. 检查和修改数组数据类型
print("\n=== 6. 检查和修改数组数据类型 ===")
arr = np.array([1, 2, 3])
print("原始数组:", arr, "dtype:", arr.dtype)

# 查看数据类型信息
print("数据类型名称:", arr.dtype.name)
print("数据类型字符代码:", arr.dtype.char)  # 'l' 表示long
print("数据类型是否整数:", np.issubdtype(arr.dtype, np.integer))
print("数据类型是否浮点数:", np.issubdtype(arr.dtype, np.floating))

# 7. 内存布局
print("\n=== 7. 内存布局 ===")
arr = np.array([[1, 2, 3], [4, 5, 6]], order='C')  # C风格，行优先
print("C风格数组（行优先）:")
print("数组:\n", arr)
print("是否C连续:", arr.flags['C_CONTIGUOUS'])

arr_f = np.array([[1, 2, 3], [4, 5, 6]], order='F')  # Fortran风格，列优先
print("\nF风格数组（列优先）:")
print("数组:\n", arr_f)
print("是否F连续:", arr.flags['F_CONTIGUOUS'])

# 8. 数组标志（flags）
print("\n=== 8. 数组标志（flags） ===")
arr = np.array([[1, 2, 3], [4, 5, 6]])
print("数组标志:")
print("C_CONTIGUOUS (C连续):", arr.flags['C_CONTIGUOUS'])
print("F_CONTIGUOUS (F连续):", arr.flags['F_CONTIGUOUS'])
print("OWNDATA (拥有数据):", arr.flags['OWNDATA'])
print("WRITEABLE (可写):", arr.flags['WRITEABLE'])
print("ALIGNED (对齐):", arr.flags['ALIGNED'])
print("WRITEBACKIFCOPY:", arr.flags['WRITEBACKIFCOPY'])
```

## 总结
1. **数组属性**：
   - `ndim`：数组维度
   - `shape`：数组形状，元组表示各维度大小
   - `size`：数组元素总数
   - `dtype`：数组元素数据类型
   - `itemsize`：每个元素的字节大小
   - `nbytes`：数组总字节数（= size * itemsize）
   - `T`：数组的转置

2. **数据类型（dtype）**：
   - **整数类型**：`int8`, `int16`, `int32`, `int64`, `uint8`, `uint16`等
   - **浮点类型**：`float16`, `float32`, `float64`
   - **复数类型**：`complex64`, `complex128`
   - **布尔类型**：`bool_`
   - **字符串类型**：`str_`, `bytes_`

3. **类型转换**：
   - 使用`astype()`方法转换数组数据类型
   - 转换时需注意精度损失和溢出问题
   - 高精度转低精度可能丢失信息

4. **重要原则**：
   - 选择合适的数据类型可以节省内存
   - 高精度计算需要更大数据类型
   - 数组创建后数据类型固定，修改需创建新数组
   - 了解数据类型范围，避免整数溢出

5. **内存布局**：
   - C风格（行优先）：`order='C'`
   - Fortran风格（列优先）：`order='F'`
   - 影响数组操作和计算效率

**第三讲重点**：掌握NumPy数组的基本属性，理解不同数据类型的特点和适用场景，学会根据需求选择合适的数据类型，并注意类型转换可能带来的问题。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第四讲：数组索引与切片

## 概念
数组索引是访问数组元素的方法，切片是获取数组子集的方式。NumPy提供了多种灵活的索引机制，包括基本索引、切片索引、整数数组索引和布尔索引。

## 原理
1. **基本索引**：通过整数位置访问单个元素，从0开始计数
2. **切片索引**：通过`start:stop:step`语法获取连续的子数组
3. **整数数组索引**：使用整数数组作为索引，可以获取任意位置的元素
4. **布尔索引**：使用布尔数组作为索引，选择满足条件的元素
5. **视图与副本**：理解索引返回的是视图（view）还是副本（copy）至关重要，视图共享数据，副本独立

## 例子
```python
import numpy as np

# 1. 一维数组的索引和切片
print("=== 1. 一维数组的索引和切片 ===")
arr_1d = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
print("原始数组:", arr_1d)

# 基本索引
print("arr_1d[0]:", arr_1d[0])      # 第一个元素
print("arr_1d[-1]:", arr_1d[-1])    # 最后一个元素
print("arr_1d[-2]:", arr_1d[-2])    # 倒数第二个元素

# 切片索引
print("arr_1d[2:5]:", arr_1d[2:5])          # 索引2到4（不含5）
print("arr_1d[:5]:", arr_1d[:5])            # 从开始到索引4
print("arr_1d[5:]:", arr_1d[5:])            # 从索引5到结束
print("arr_1d[::2]:", arr_1d[::2])          # 步长为2
print("arr_1d[1::2]:", arr_1d[1::2])        # 从索引1开始，步长为2
print("arr_1d[::-1]:", arr_1d[::-1])        # 反转数组
print("arr_1d[5:1:-1]:", arr_1d[5:1:-1])    # 从索引5到索引2（反向）

# 2. 二维数组的索引和切片
print("\n=== 2. 二维数组的索引和切片 ===")
arr_2d = np.array([[1, 2, 3, 4],
                   [5, 6, 7, 8],
                   [9, 10, 11, 12]])
print("二维数组:\n", arr_2d)

# 基本索引
print("arr_2d[0, 0]:", arr_2d[0, 0])        # 第0行第0列
print("arr_2d[1, 2]:", arr_2d[1, 2])        # 第1行第2列
print("arr_2d[-1, -1]:", arr_2d[-1, -1])    # 最后一行最后一列

# 行切片
print("arr_2d[0]:", arr_2d[0])              # 第0行
print("arr_2d[1]:", arr_2d[1])              # 第1行
print("arr_2d[-1]:", arr_2d[-1])            # 最后一行
print("arr_2d[:2]:\n", arr_2d[:2])          # 前两行
print("arr_2d[1:]:\n", arr_2d[1:])          # 第1行到最后

# 列切片
print("arr_2d[:, 0]:", arr_2d[:, 0])        # 第0列
print("arr_2d[:, 1]:", arr_2d[:, 1])        # 第1列
print("arr_2d[:, -1]:", arr_2d[:, -1])      # 最后一列
print("arr_2d[:, 1:3]:\n", arr_2d[:, 1:3])  # 第1列到第2列

# 行列同时切片
print("arr_2d[1:, 2:]:\n", arr_2d[1:, 2:])  # 第1行及之后，第2列及之后
print("arr_2d[:2, :2]:\n", arr_2d[:2, :2])  # 前两行，前两列
print("arr_2d[::2, ::2]:\n", arr_2d[::2, ::2])  # 每隔一行一列

# 3. 三维数组的索引和切片
print("\n=== 3. 三维数组的索引和切片 ===")
arr_3d = np.array([[[1, 2, 3],
                    [4, 5, 6]],
                   [[7, 8, 9],
                    [10, 11, 12]]])
print("三维数组形状:", arr_3d.shape)
print("三维数组:\n", arr_3d)

# 三维数组索引
print("arr_3d[0]:\n", arr_3d[0])            # 第一个二维切片
print("arr_3d[1]:\n", arr_3d[1])            # 第二个二维切片
print("arr_3d[0, 0]:", arr_3d[0, 0])        # 第一个二维切片的第0行
print("arr_3d[0, 0, 0]:", arr_3d[0, 0, 0])  # 第一个元素
print("arr_3d[:, 0, 0]:", arr_3d[:, 0, 0])  # 每个二维切片的第0行第0列

# 4. 整数数组索引（花式索引）
print("\n=== 4. 整数数组索引（花式索引） ===")
arr = np.array([10, 20, 30, 40, 50])
print("原始数组:", arr)

# 使用整数列表索引
indices = [0, 2, 4]
print("arr[[0, 2, 4]]:", arr[[0, 2, 4]])  # 获取第0、2、4个元素
print("arr[[4, 2, 0]]:", arr[[4, 2, 0]])  # 可以改变顺序
print("arr[[0, 0, 1, 1, 2]]:", arr[[0, 0, 1, 1, 2]])  # 可以重复索引

# 二维数组的整数数组索引
arr_2d = np.array([[1, 2, 3],
                   [4, 5, 6],
                   [7, 8, 9]])
print("\n二维数组:\n", arr_2d)

# 获取多个元素
print("arr_2d[[0, 2], [1, 2]]:", arr_2d[[0, 2], [1, 2]])  # (0,1)和(2,2)位置的元素
print("arr_2d[[0, 0, 2], [0, 2, 1]]:", arr_2d[[0, 0, 2], [0, 2, 1]])  # (0,0),(0,2),(2,1)

# 获取整行或整列
print("arr_2d[[0, 2], :]:\n", arr_2d[[0, 2], :])  # 第0行和第2行
print("arr_2d[:, [0, 2]]:\n", arr_2d[:, [0, 2]])  # 第0列和第2列

# 5. 布尔索引
print("\n=== 5. 布尔索引 ===")
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
print("原始数组:", arr)

# 创建布尔条件
mask = arr > 5
print("arr > 5 的布尔数组:", mask)
print("arr[arr > 5]:", arr[arr > 5])  # 大于5的元素
print("arr[arr % 2 == 0]:", arr[arr % 2 == 0])  # 偶数
print("arr[(arr > 3) & (arr < 8)]:", arr[(arr > 3) & (arr < 8)])  # 3到8之间的元素
print("arr[(arr < 3) | (arr > 8)]:", arr[(arr < 3) | (arr > 8)])  # 小于3或大于8的元素
print("arr[~(arr > 5)]:", arr[~(arr > 5)])  # 不大于5的元素（即小于等于5）

# 二维数组的布尔索引
arr_2d = np.array([[1, 2, 3],
                   [4, 5, 6],
                   [7, 8, 9]])
print("\n二维数组:\n", arr_2d)

# 条件选择
mask = arr_2d > 5
print("arr_2d > 5 的布尔数组:\n", mask)
print("arr_2d[arr_2d > 5]:", arr_2d[arr_2d > 5])  # 展平后的结果

# 使用布尔数组选择行
row_mask = np.array([True, False, True])
print("选择第0行和第2行:\n", arr_2d[row_mask, :])

# 6. 视图与副本
print("\n=== 6. 视图与副本 ===")
# 视图（view）：共享数据
arr = np.array([1, 2, 3, 4, 5])
view = arr[1:4]  # 切片创建视图
print("原始数组:", arr)
print("切片视图:", view)
view[0] = 100  # 修改视图会影响原始数组
print("修改视图后，原始数组:", arr)
print("修改视图后，视图:", view)

# 副本（copy）：独立数据
arr = np.array([1, 2, 3, 4, 5])
copy = arr[1:4].copy()  # 使用copy()方法创建副本
print("\n原始数组:", arr)
print("切片副本:", copy)
copy[0] = 200  # 修改副本不会影响原始数组
print("修改副本后，原始数组:", arr)
print("修改副本后，副本:", copy)

# 检查是视图还是副本
print("\n检查对象:")
print("view.base is arr:", view.base is arr)  # True，view是arr的视图
print("copy.base is arr:", copy.base is arr)  # False，copy不是arr的视图
print("view.flags.owndata:", view.flags.owndata)  # False，不拥有自己的数据
print("copy.flags.owndata:", copy.flags.owndata)  # True，拥有自己的数据

# 7. 索引赋值
print("\n=== 7. 索引赋值 ===")
arr = np.array([1, 2, 3, 4, 5])
print("原始数组:", arr)

# 基本索引赋值
arr[0] = 10
print("arr[0] = 10 后:", arr)

# 切片赋值
arr[1:4] = 20
print("arr[1:4] = 20 后:", arr)

# 多元素赋值
arr[[0, 2, 4]] = 0
print("arr[[0, 2, 4]] = 0 后:", arr)

# 布尔索引赋值
arr[arr > 0] = 100
print("arr[arr > 0] = 100 后:", arr)

# 8. 高级示例
print("\n=== 8. 高级示例 ===")
# 创建一个5x5的数组
arr = np.arange(25).reshape(5, 5)
print("5x5数组:\n", arr)

# 获取中心3x3区域
center = arr[1:4, 1:4]
print("中心3x3区域:\n", center)

# 获取四个角
corners = arr[[0, 0, 4, 4], [0, 4, 0, 4]]
print("四个角元素:", corners)

# 获取对角线元素
diagonal = arr.diagonal()
print("对角线元素:", diagonal)

# 获取反对角线
anti_diagonal = arr[[0, 1, 2, 3, 4], [4, 3, 2, 1, 0]]
print("反对角线元素:", anti_diagonal)

# 使用条件替换元素
arr_copy = arr.copy()
arr_copy[arr_copy < 10] = 0
print("将小于10的元素替换为0:\n", arr_copy)

# 9. 索引技巧
print("\n=== 9. 索引技巧 ===")
# 使用np.where
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
indices = np.where(arr > 5)
print("arr > 5 的索引:", indices)  # 返回元组
print("arr[arr > 5]:", arr[indices])

# 使用np.nonzero获取非零元素索引
arr = np.array([0, 1, 0, 2, 0, 3])
nonzero_indices = np.nonzero(arr)
print("非零元素索引:", nonzero_indices)
print("非零元素:", arr[nonzero_indices])

# 使用np.take
arr = np.array([10, 20, 30, 40, 50])
indices = [0, 3, 4]
print("np.take(arr, [0, 3, 4]):", np.take(arr, indices))

# 10. 性能比较
print("\n=== 10. 性能比较 ===")
import time

# 大数组的布尔索引与整数索引性能比较
big_arr = np.random.rand(1000000)

# 布尔索引
start = time.time()
result1 = big_arr[big_arr > 0.5]
bool_time = time.time() - start

# 先获取索引再索引
start = time.time()
indices = np.where(big_arr > 0.5)[0]
result2 = big_arr[indices]
where_time = time.time() - start

print(f"布尔索引时间: {bool_time:.6f}秒")
print(f"先where再索引时间: {where_time:.6f}秒")
print(f"结果是否相等: {np.array_equal(result1, result2)}")
```

## 总结
1. **基本索引**：通过整数访问单个元素，支持负索引
2. **切片索引**：通过`start:stop:step`获取连续子数组，支持省略和负步长
3. **整数数组索引**：使用整数数组获取任意位置的元素，可以重复、乱序
4. **布尔索引**：使用布尔数组选择满足条件的元素，非常强大
5. **视图与副本**：
   - 基本切片返回视图（共享数据）
   - 整数数组索引和布尔索引返回副本（独立数据）
   - 使用`copy()`方法显式创建副本
6. **索引赋值**：可以通过各种索引方式修改数组元素
7. **多维数组索引**：用逗号分隔各维度索引
8. **高级技巧**：
   - `np.where()`：获取满足条件的索引
   - `np.nonzero()`：获取非零元素索引
   - `np.take()`：沿轴获取元素
9. **性能注意**：
   - 布尔索引和整数数组索引可能产生副本，内存开销大
   - 基本切片产生视图，内存效率高
   - 对于大数组，注意索引操作的性能

**第四讲重点**：掌握NumPy的各种索引方法，理解视图和副本的区别，学会根据需求选择合适的索引方式，并注意不同索引方式的性能差异。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第五讲：数组形状操作

## 概念
数组形状操作是指改变数组的维度、大小和排列方式，而不改变数组数据本身。NumPy提供了多种函数来重塑数组、展平数组、调整维度等。

## 原理
1. **重塑（Reshape）**：改变数组的形状，但保持元素总数不变。重塑操作返回一个视图（如果可能）或副本。
2. **展平（Flatten）和拉直（Ravel）**：将多维数组转换为一维数组。`flatten`返回副本，`ravel`返回视图（如果可能）。
3. **转置（Transpose）**：交换数组的轴，对于二维数组就是矩阵转置。
4. **调整维度**：增加或减少数组的维度，如使用`newaxis`、`squeeze`等。
5. **改变顺序**：改变数组元素在内存中的排列顺序，如C顺序（行优先）和F顺序（列优先）。

## 例子
```python
import numpy as np

# 1. 重塑（reshape）
print("=== 1. 重塑（reshape） ===")
arr = np.arange(12)  # 0到11的一维数组
print("原始一维数组:", arr)
print("原始形状:", arr.shape)

# 重塑为3行4列的二维数组
arr_2d = arr.reshape(3, 4)
print("重塑为3x4数组:\n", arr_2d)
print("新形状:", arr_2d.shape)

# 重塑为2x3x2的三维数组
arr_3d = arr.reshape(2, 3, 2)
print("重塑为2x3x2数组:\n", arr_3d)
print("新形状:", arr_3d.shape)

# 使用-1自动计算维度
arr_auto = arr.reshape(3, -1)  # 自动计算列数
print("使用-1自动计算，重塑为3行:\n", arr_auto)
arr_auto2 = arr.reshape(-1, 4)  # 自动计算行数
print("使用-1自动计算，重塑为4列:\n", arr_auto2)

# 注意：重塑后的数组是原始数组的视图（如果可能）
arr[0] = 100
print("修改原始数组后，重塑数组（视图）:\n", arr_2d)  # 会看到变化

# 2. 展平（flatten）和拉直（ravel）
print("\n=== 2. 展平（flatten）和拉直（ravel） ===")
arr_2d = np.array([[1, 2, 3], [4, 5, 6]])
print("原始二维数组:\n", arr_2d)

# flatten：返回展平后的副本
arr_flatten = arr_2d.flatten()
print("flatten结果（副本）:", arr_flatten)
arr_flatten[0] = 100
print("修改flatten结果后，原始数组:\n", arr_2d)  # 不变

# ravel：返回展平后的视图（如果可能）
arr_ravel = arr_2d.ravel()
print("ravel结果（视图）:", arr_ravel)
arr_ravel[0] = 100  # 修改会影响原始数组
print("修改ravel结果后，原始数组:\n", arr_2d)  # 改变

# 3. 转置（transpose）
print("\n=== 3. 转置（transpose） ===")
arr = np.array([[1, 2, 3], [4, 5, 6]])
print("原始数组:\n", arr)
print("原始形状:", arr.shape)

# 使用T属性
arr_T = arr.T
print("转置数组（使用.T）:\n", arr_T)
print("转置后形状:", arr_T.shape)

# 使用transpose方法
arr_transpose = arr.transpose()
print("转置数组（使用transpose）:\n", arr_transpose)

# 高维数组转置：可以指定轴顺序
arr_3d = np.arange(24).reshape(2, 3, 4)
print("三维数组形状:", arr_3d.shape)
arr_transpose_3d = arr_3d.transpose(1, 0, 2)  # 将第0轴和第1轴交换
print("转置(1,0,2)后形状:", arr_transpose_3d.shape)

# 4. 调整维度
print("\n=== 4. 调整维度 ===")
# 增加维度：使用np.newaxis或reshape
arr = np.array([1, 2, 3])
print("原始一维数组:", arr, "形状:", arr.shape)

# 增加一个轴，变为行向量（1行3列）
arr_row = arr[np.newaxis, :]
print("增加行维度（np.newaxis, :）:", arr_row, "形状:", arr_row.shape)

# 增加一个轴，变为列向量（3行1列）
arr_col = arr[:, np.newaxis]
print("增加列维度（:, np.newaxis）:\n", arr_col, "形状:", arr_col.shape)

# 使用reshape增加维度
arr_reshape = arr.reshape(1, -1)
print("使用reshape增加行维度:", arr_reshape, "形状:", arr_reshape.shape)

# 减少维度：使用squeeze移除长度为1的维度
arr_2d = np.array([[1, 2, 3]])  # 1行3列
print("二维数组:", arr_2d, "形状:", arr_2d.shape)
arr_squeeze = np.squeeze(arr_2d)
print("squeeze后:", arr_squeeze, "形状:", arr_squeeze.shape)

# 5. 改变数组顺序（C顺序和F顺序）
print("\n=== 5. 改变数组顺序 ===")
# C顺序（行优先）和F顺序（列优先）
arr = np.arange(12).reshape(3, 4)
print("原始数组（C顺序）:\n", arr)

# 转换为F顺序
arr_f = np.asfortranarray(arr)
print("F顺序数组:\n", arr_f)
print("原始数组flags:\n", arr.flags)
print("F顺序数组flags:\n", arr_f.flags)

# 使用order参数重塑
arr_c = np.reshape(arr, (2, 6), order='C')  # 按行填充
print("C顺序重塑为2x6:\n", arr_c)
arr_f = np.reshape(arr, (2, 6), order='F')  # 按列填充
print("F顺序重塑为2x6:\n", arr_f)

# 6. 数组堆叠
print("\n=== 6. 数组堆叠 ===")
a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])

# 水平堆叠
hstack = np.hstack((a, b))
print("水平堆叠:\n", hstack)

# 垂直堆叠
vstack = np.vstack((a, b))
print("垂直堆叠:\n", vstack)

# 深度堆叠（沿第三轴）
dstack = np.dstack((a, b))
print("深度堆叠:\n", dstack, "形状:", dstack.shape)

# 7. 数组分割
print("\n=== 7. 数组分割 ===")
arr = np.arange(12).reshape(3, 4)
print("原始数组:\n", arr)

# 水平分割（按列分割）
hsplit = np.hsplit(arr, 2)  # 分成2部分
print("水平分割成2部分:")
for i, part in enumerate(hsplit):
    print(f"部分{i}:\n", part)

# 垂直分割（按行分割）
vsplit = np.vsplit(arr, 3)  # 分成3部分
print("垂直分割成3部分:")
for i, part in enumerate(vsplit):
    print(f"部分{i}:\n", part)

# 8. 数组翻转
print("\n=== 8. 数组翻转 ===")
arr = np.array([[1, 2, 3], [4, 5, 6]])
print("原始数组:\n", arr)

# 上下翻转
flipud = np.flipud(arr)
print("上下翻转:\n", flipud)

# 左右翻转
fliplr = np.fliplr(arr)
print("左右翻转:\n", fliplr)

# 沿指定轴翻转
flip = np.flip(arr, axis=0)  # 沿轴0翻转（上下）
print("沿轴0翻转:\n", flip)
flip = np.flip(arr, axis=1)  # 沿轴1翻转（左右）
print("沿轴1翻转:\n", flip)

# 9. 数组旋转
print("\n=== 9. 数组旋转 ===")
arr = np.array([[1, 2, 3], [4, 5, 6]])
print("原始数组:\n", arr)

# 逆时针旋转90度
rot90 = np.rot90(arr)
print("逆时针旋转90度:\n", rot90)

# 旋转180度
rot180 = np.rot90(arr, 2)
print("旋转180度:\n", rot180)

# 顺时针旋转90度
rot270 = np.rot90(arr, 3)  # 或逆时针旋转270度
print("顺时针旋转90度（逆时针270度）:\n", rot270)

# 10. 数组重排
print("\n=== 10. 数组重排 ===")
arr = np.arange(10)
print("原始数组:", arr)

# 随机重排
np.random.shuffle(arr)  # 原地修改
print("随机重排后:", arr)

# 返回重排索引
arr = np.arange(10)
indices = np.random.permutation(10)
print("随机排列索引:", indices)
print("使用索引重排:", arr[indices])

# 11. 综合示例
print("\n=== 11. 综合示例 ===")
# 创建一个4x4数组
arr = np.arange(16).reshape(4, 4)
print("原始4x4数组:\n", arr)

# 将数组转换为2x8
reshaped = arr.reshape(2, 8)
print("重塑为2x8:\n", reshaped)

# 转置
transposed = reshaped.T
print("转置为8x2:\n", transposed)

# 展平
flattened = transposed.flatten()
print("展平为一维:", flattened)

# 重新调整为4x4，但按列填充
new_arr = flattened.reshape(4, 4, order='F')
print("按列填充的4x4数组:\n", new_arr)
```

## 总结
1. **重塑（reshape）**：改变数组形状，元素总数不变，返回视图（如果可能）
2. **展平（flatten/ravel）**：将多维数组转换为一维，`flatten`返回副本，`ravel`返回视图
3. **转置（transpose/T）**：交换数组的轴，对于矩阵就是行列互换
4. **调整维度**：
   - 增加维度：`np.newaxis`、`reshape`
   - 减少维度：`squeeze`移除长度为1的维度
5. **数组顺序**：C顺序（行优先）和F顺序（列优先），影响重塑和展平的结果
6. **堆叠与分割**：
   - 堆叠：`hstack`、`vstack`、`dstack`
   - 分割：`hsplit`、`vsplit`、`dsplit`
7. **翻转与旋转**：`flipud`、`fliplr`、`flip`、`rot90`
8. **重排**：`shuffle`（原地）、`permutation`（返回重排索引）
9. **重要原则**：
   - 重塑操作要求元素总数不变
   - 注意视图和副本的区别，避免意外修改
   - 合理使用`-1`自动计算维度
   - 根据数据访问模式选择C顺序或F顺序

**第五讲重点**：掌握NumPy中数组形状操作的各种方法，理解重塑、转置、维度调整等操作的原理和区别，学会根据需求改变数组的形状和排列方式。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第六讲：数组的组合与分割

## 概念
数组组合是将多个数组合并成一个更大的数组，数组分割是将一个数组拆分成多个较小的数组。这些操作是数据处理和分析中常用的基本操作，用于重组和划分数据。

## 原理
1. **组合（Concatenation）**：沿指定轴将多个数组合并，要求除连接轴外其他维度必须匹配
2. **堆叠（Stacking）**：沿新轴将多个数组合并，增加数组维度
3. **分割（Splitting）**：将数组沿指定轴分割成多个子数组
4. **重复（Repeating）和分块（Tiling）**：通过重复数组元素或块来创建新数组
5. **插入（Insert）和删除（Delete）**：在指定位置插入或删除元素

## 例子
```python
import numpy as np

# 1. 连接（concatenate）
print("=== 1. 连接（concatenate） ===")
# 一维数组连接
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
c = np.array([7, 8, 9])

# 沿现有轴（默认轴0）连接
result = np.concatenate((a, b, c))
print("一维数组连接:", result)

# 二维数组连接
x = np.array([[1, 2], [3, 4]])
y = np.array([[5, 6], [7, 8]])
print("\n二维数组x:\n", x)
print("二维数组y:\n", y)

# 沿轴0连接（垂直连接，增加行数）
result_0 = np.concatenate((x, y), axis=0)
print("沿轴0连接（垂直）:\n", result_0)

# 沿轴1连接（水平连接，增加列数）
result_1 = np.concatenate((x, y), axis=1)
print("沿轴1连接（水平）:\n", result_1)

# 三维数组连接
z1 = np.arange(12).reshape(2, 3, 2)
z2 = np.arange(12, 24).reshape(2, 3, 2)
print("\n三维数组z1形状:", z1.shape)
print("三维数组z2形状:", z2.shape)

# 沿不同轴连接三维数组
result_3d_0 = np.concatenate((z1, z2), axis=0)
print("沿轴0连接后形状:", result_3d_0.shape)  # (4, 3, 2)

result_3d_1 = np.concatenate((z1, z2), axis=1)
print("沿轴1连接后形状:", result_3d_1.shape)  # (2, 6, 2)

result_3d_2 = np.concatenate((z1, z2), axis=2)
print("沿轴2连接后形状:", result_3d_2.shape)  # (2, 3, 4)

# 2. 堆叠（stack）
print("\n=== 2. 堆叠（stack） ===")
# 堆叠会增加新的维度
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
c = np.array([7, 8, 9])

# 沿新轴堆叠（默认轴0，增加第一个维度）
stack_0 = np.stack((a, b, c), axis=0)
print("沿轴0堆叠（默认）:\n", stack_0, "形状:", stack_0.shape)

# 沿新轴堆叠到第二个维度
stack_1 = np.stack((a, b, c), axis=1)
print("沿轴1堆叠:\n", stack_1, "形状:", stack_1.shape)

# 二维数组堆叠
x = np.array([[1, 2, 3], [4, 5, 6]])
y = np.array([[7, 8, 9], [10, 11, 12]])
print("\n二维数组x:\n", x)
print("二维数组y:\n", y)

# 沿不同轴堆叠二维数组
stack_2d_0 = np.stack((x, y), axis=0)
print("沿轴0堆叠:\n", stack_2d_0, "形状:", stack_2d_0.shape)

stack_2d_1 = np.stack((x, y), axis=1)
print("沿轴1堆叠:\n", stack_2d_1, "形状:", stack_2d_1.shape)

stack_2d_2 = np.stack((x, y), axis=2)
print("沿轴2堆叠:\n", stack_2d_2, "形状:", stack_2d_2.shape)

# 3. 水平堆叠（hstack）和垂直堆叠（vstack）
print("\n=== 3. 水平堆叠（hstack）和垂直堆叠（vstack） ===")
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])

# 水平堆叠（列方向）
h = np.hstack((a, b))
print("一维数组水平堆叠:", h, "形状:", h.shape)

# 垂直堆叠（行方向）
v = np.vstack((a, b))
print("一维数组垂直堆叠:\n", v, "形状:", v.shape)

# 二维数组的水平堆叠和垂直堆叠
x = np.array([[1, 2], [3, 4]])
y = np.array([[5, 6], [7, 8]])
print("\n二维数组x:\n", x)
print("二维数组y:\n", y)

h_2d = np.hstack((x, y))
print("二维数组水平堆叠:\n", h_2d, "形状:", h_2d.shape)

v_2d = np.vstack((x, y))
print("二维数组垂直堆叠:\n", v_2d, "形状:", v_2d.shape)

# 4. 深度堆叠（dstack）和列堆叠（column_stack）
print("\n=== 4. 深度堆叠（dstack）和列堆叠（column_stack） ===")
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])

# 深度堆叠（沿第三个轴）
d = np.dstack((a, b))
print("一维数组深度堆叠:\n", d, "形状:", d.shape)

# 列堆叠（将一维数组作为列堆叠成二维数组）
c = np.column_stack((a, b))
print("列堆叠:\n", c, "形状:", c.shape)

# 行堆叠（row_stack）等同于vstack
r = np.row_stack((a, b))
print("行堆叠:\n", r, "形状:", r.shape)

# 5. 数组分割
print("\n=== 5. 数组分割 ===")
arr = np.arange(12).reshape(3, 4)
print("原始数组:\n", arr, "形状:", arr.shape)

# 沿轴0分割（垂直分割，按行分割）
result_vsplit = np.vsplit(arr, 3)  # 分割成3个数组
print("垂直分割成3部分:")
for i, part in enumerate(result_vsplit):
    print(f"部分{i}:\n", part)

# 沿轴1分割（水平分割，按列分割）
result_hsplit = np.hsplit(arr, 2)  # 分割成2个数组
print("\n水平分割成2部分:")
for i, part in enumerate(result_hsplit):
    print(f"部分{i}:\n", part)

# 不均匀分割
arr = np.arange(10)
print("\n数组:", arr)
result_split = np.array_split(arr, 3)  # 可以不均匀分割
print("不均匀分割成3部分:")
for i, part in enumerate(result_split):
    print(f"部分{i}: {part}")

# 6. 使用split函数指定分割点
print("\n=== 6. 使用split函数指定分割点 ===")
arr = np.arange(10)
print("数组:", arr)

# 在指定位置分割
split_positions = [2, 5, 8]
result = np.split(arr, split_positions)
print(f"在位置{split_positions}分割:")
for i, part in enumerate(result):
    print(f"部分{i}: {part}")

# 沿指定轴分割二维数组
arr_2d = np.arange(16).reshape(4, 4)
print("\n二维数组:\n", arr_2d)

# 沿轴0在指定行分割
result_axis0 = np.split(arr_2d, [1, 3], axis=0)  # 在第1行和第3行处分割
print("沿轴0在第1行和第3行处分割:")
for i, part in enumerate(result_axis0):
    print(f"部分{i}:\n", part)

# 沿轴1在指定列分割
result_axis1 = np.split(arr_2d, [2], axis=1)  # 在第2列处分割
print("沿轴1在第2列处分割:")
for i, part in enumerate(result_axis1):
    print(f"部分{i}:\n", part)

# 7. 重复（repeat）和分块（tile）
print("\n=== 7. 重复（repeat）和分块（tile） ===")
arr = np.array([1, 2, 3])

# 重复每个元素
repeat_each = np.repeat(arr, 3)  # 每个元素重复3次
print("每个元素重复3次:", repeat_each)

# 重复整个数组
repeat_all = np.tile(arr, 3)  # 整个数组重复3次
print("整个数组重复3次:", repeat_all)

# 二维数组重复
arr_2d = np.array([[1, 2], [3, 4]])
print("\n二维数组:\n", arr_2d)

# 沿轴0重复（垂直方向）
repeat_axis0 = np.repeat(arr_2d, 2, axis=0)
print("沿轴0每个元素重复2次:\n", repeat_axis0)

# 沿轴1重复（水平方向）
repeat_axis1 = np.repeat(arr_2d, 2, axis=1)
print("沿轴1每个元素重复2次:\n", repeat_axis1)

# 分块（平铺）
tile_2d = np.tile(arr_2d, (2, 3))  # 垂直方向重复2次，水平方向重复3次
print("分块(2,3):\n", tile_2d, "形状:", tile_2d.shape)

# 8. 插入（insert）和删除（delete）
print("\n=== 8. 插入（insert）和删除（delete） ===")
arr = np.array([1, 2, 3, 4, 5])
print("原始数组:", arr)

# 在指定位置插入元素
inserted = np.insert(arr, 2, [10, 11])  # 在索引2处插入[10, 11]
print("在索引2处插入[10, 11]:", inserted)

# 在指定位置插入数组
inserted_2d = np.insert(arr, [1, 3], [20, 30])  # 在索引1和3处分别插入20和30
print("在索引1和3处插入[20, 30]:", inserted_2d)

# 二维数组插入
arr_2d = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
print("\n二维数组:\n", arr_2d)

# 沿轴0插入行
inserted_row = np.insert(arr_2d, 1, [10, 11, 12], axis=0)
print("在第1行处插入[10, 11, 12]:\n", inserted_row)

# 沿轴1插入列
inserted_col = np.insert(arr_2d, 1, [20, 30, 40], axis=1)
print("在第1列处插入[20, 30, 40]:\n", inserted_col)

# 删除元素
deleted = np.delete(arr, 2)  # 删除索引2处的元素
print(f"\n删除索引2处的元素:", deleted)

# 删除多个元素
deleted_multi = np.delete(arr, [1, 3])  # 删除索引1和3处的元素
print("删除索引1和3处的元素:", deleted_multi)

# 二维数组删除行
deleted_row = np.delete(arr_2d, 1, axis=0)  # 删除第1行
print("删除第1行:\n", deleted_row)

# 二维数组删除列
deleted_col = np.delete(arr_2d, 1, axis=1)  # 删除第1列
print("删除第1列:\n", deleted_col)

# 9. 追加（append）
print("\n=== 9. 追加（append） ===")
arr = np.array([1, 2, 3])
print("原始数组:", arr)

# 追加元素
appended = np.append(arr, [4, 5, 6])
print("追加[4, 5, 6]:", appended)

# 二维数组追加
arr_2d = np.array([[1, 2, 3], [4, 5, 6]])
print("\n二维数组:\n", arr_2d)

# 沿轴0追加行
appended_row = np.append(arr_2d, [[7, 8, 9]], axis=0)
print("追加行[[7, 8, 9]]:\n", appended_row)

# 沿轴1追加列
appended_col = np.append(arr_2d, [[10], [11]], axis=1)
print("追加列[[10], [11]]:\n", appended_col)

# 10. 连接与分割的性能考虑
print("\n=== 10. 连接与分割的性能考虑 ===")
# 对于大型数组，连接操作可能会涉及内存重新分配
# 分割操作通常不涉及数据复制，返回的是视图

# 创建大型数组
big_arr1 = np.ones((1000, 1000))
big_arr2 = np.ones((1000, 1000))

# 水平连接
import time
start = time.time()
result = np.hstack((big_arr1, big_arr2))
hstack_time = time.time() - start
print(f"水平连接1000x1000数组时间: {hstack_time:.4f}秒")

# 垂直连接
start = time.time()
result = np.vstack((big_arr1, big_arr2))
vstack_time = time.time() - start
print(f"垂直连接1000x1000数组时间: {vstack_time:.4f}秒")

# 检查分割返回的是视图还是副本
arr = np.arange(12).reshape(3, 4)
parts = np.hsplit(arr, 2)
print(f"\n分割后部分0是原始数组的视图: {parts[0].base is arr}")
print(f"分割后部分0拥有自己的数据: {parts[0].flags.owndata}")

# 11. 综合示例
print("\n=== 11. 综合示例 ===")
# 创建两个数组
a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])
print("数组a:\n", a)
print("数组b:\n", b)

# 多种组合方式
print("\n1. 水平堆叠:")
h = np.hstack((a, b))
print(h)

print("\n2. 垂直堆叠:")
v = np.vstack((a, b))
print(v)

print("\n3. 深度堆叠:")
d = np.dstack((a, b))
print(d, "形状:", d.shape)

print("\n4. 列堆叠:")
c = np.column_stack((a.ravel(), b.ravel()))  # 先展平
print(c)

# 组合后的分割
print("\n5. 对水平堆叠结果进行分割:")
parts = np.hsplit(h, 2)
for i, part in enumerate(parts):
    print(f"部分{i}:\n", part)

# 使用concatenate实现复杂组合
print("\n6. 使用concatenate实现复杂组合:")
result = np.concatenate((a, b, a), axis=0)  # 垂直连接三个数组
print("垂直连接a, b, a:\n", result)

# 将一维数组转换为矩阵
arr_1d = np.array([1, 2, 3, 4, 5, 6])
matrix = arr_1d.reshape(2, 3)
print("\n7. 一维数组重塑为矩阵:\n", matrix)

# 矩阵分块
blocks = np.split(matrix, 2, axis=0)
print("\n8. 矩阵按行分割:")
for i, block in enumerate(blocks):
    print(f"块{i}:\n", block)
```

## 总结
1. **连接（concatenate）**：沿现有轴连接数组，要求除连接轴外其他维度匹配
2. **堆叠（stack）**：沿新轴堆叠数组，会增加数组维度
3. **水平/垂直堆叠**：
   - `hstack`：水平堆叠（沿列方向）
   - `vstack`：垂直堆叠（沿行方向）
   - `dstack`：深度堆叠（沿第三轴）
4. **分割**：
   - `split`：在指定位置分割
   - `hsplit`：水平分割（按列）
   - `vsplit`：垂直分割（按行）
   - `array_split`：支持不均匀分割
5. **重复与分块**：
   - `repeat`：重复数组元素
   - `tile`：重复整个数组块
6. **插入与删除**：
   - `insert`：在指定位置插入元素
   - `delete`：删除指定位置的元素
   - `append`：在末尾追加元素
7. **性能考虑**：
   - 连接操作可能涉及内存重新分配
   - 分割操作通常返回视图，效率较高
   - 对于大型数组，注意内存使用
8. **视图与副本**：
   - 分割操作通常返回视图
   - 连接操作返回新数组（副本）
   - 插入和删除操作返回新数组

**第六讲重点**：掌握NumPy中数组组合与分割的各种方法，学会根据需求合并或拆分数组，理解不同操作的原理和适用场景，注意视图与副本的区别。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第七讲：基本数学运算

## 概念
NumPy提供了丰富的数学运算功能，包括算术运算、三角函数、指数对数运算、舍入运算等。这些运算支持向量化操作，可以直接对整个数组执行运算，而无需显式循环，从而大大提高计算效率。

## 原理
1. **向量化运算**：NumPy的数学运算是向量化的，意味着运算会应用到数组的每个元素上，底层由优化过的C代码实现，比Python循环快得多。
2. **广播机制**：当对不同形状的数组进行运算时，NumPy会自动应用广播规则，使它们具有兼容的形状。
3. **原地操作**：有些运算（如`+=`、`*=`）会直接修改原数组，而有些运算（如`np.add()`）会返回新数组。
4. **逐元素运算**：大多数NumPy数学函数都是逐元素（element-wise）的，对数组中的每个元素独立执行运算。

## 例子
```python
import numpy as np

# 1. 算术运算
print("=== 1. 算术运算 ===")
a = np.array([1, 2, 3, 4])
b = np.array([5, 6, 7, 8])

print("数组a:", a)
print("数组b:", b)

# 基本算术运算
print("a + b =", a + b)  # 加法
print("a - b =", a - b)  # 减法
print("a * b =", a * b)  # 乘法（逐元素相乘，不是矩阵乘法）
print("a / b =", a / b)  # 除法
print("a // b =", a // b)  # 整除
print("a % b =", a % b)   # 取模
print("a ** 2 =", a ** 2)  # 幂运算

# 使用NumPy函数
print("np.add(a, b) =", np.add(a, b))
print("np.subtract(a, b) =", np.subtract(a, b))
print("np.multiply(a, b) =", np.multiply(a, b))
print("np.divide(a, b) =", np.divide(a, b))
print("np.power(a, 2) =", np.power(a, 2))
print("np.mod(a, b) =", np.mod(a, b))

# 标量与数组运算
print("\n标量与数组运算:")
print("a + 10 =", a + 10)
print("a * 2 =", a * 2)
print("1 / a =", 1 / a)

# 2. 比较运算
print("\n=== 2. 比较运算 ===")
a = np.array([1, 2, 3, 4])
b = np.array([2, 2, 3, 5])

print("数组a:", a)
print("数组b:", b)

print("a == b:", a == b)  # 相等
print("a != b:", a != b)  # 不等
print("a < b:", a < b)    # 小于
print("a <= b:", a <= b)  # 小于等于
print("a > b:", a > b)    # 大于
print("a >= b:", a >= b)  # 大于等于

# 使用NumPy函数
print("np.equal(a, b) =", np.equal(a, b))
print("np.not_equal(a, b) =", np.not_equal(a, b))
print("np.less(a, b) =", np.less(a, b))
print("np.less_equal(a, b) =", np.less_equal(a, b))
print("np.greater(a, b) =", np.greater(a, b))
print("np.greater_equal(a, b) =", np.greater_equal(a, b))

# 3. 逻辑运算
print("\n=== 3. 逻辑运算 ===")
a = np.array([True, True, False, False])
b = np.array([True, False, True, False])

print("数组a:", a)
print("数组b:", b)

print("a & b =", a & b)  # 逻辑与
print("a | b =", a | b)  # 逻辑或
print("~a =", ~a)        # 逻辑非
print("a ^ b =", a ^ b)  # 逻辑异或

# 使用NumPy函数
print("np.logical_and(a, b) =", np.logical_and(a, b))
print("np.logical_or(a, b) =", np.logical_or(a, b))
print("np.logical_not(a) =", np.logical_not(a))
print("np.logical_xor(a, b) =", np.logical_xor(a, b))

# 4. 三角函数
print("\n=== 4. 三角函数 ===")
angles = np.array([0, np.pi/6, np.pi/4, np.pi/3, np.pi/2])
print("角度（弧度）:", angles)
print("角度（度）:", np.degrees(angles))

print("sin(角度) =", np.sin(angles))
print("cos(角度) =", np.cos(angles))
print("tan(角度) =", np.tan(angles))

# 反三角函数
values = np.array([0, 0.5, 1])
print("\n值:", values)
print("arcsin(值) =", np.arcsin(values), "弧度")
print("arccos(值) =", np.arccos(values), "弧度")
print("arctan(值) =", np.arctan(values), "弧度")

# 弧度与角度转换
rad = np.pi / 4
deg = 45
print(f"\n{rad} 弧度 = {np.degrees(rad)} 度")
print(f"{deg} 度 = {np.radians(deg)} 弧度")

# 5. 指数和对数函数
print("\n=== 5. 指数和对数函数 ===")
a = np.array([1, 2, 3, 10])

print("数组a:", a)
print("e^a =", np.exp(a))  # 指数函数 e^x
print("2^a =", np.exp2(a))  # 2^x
print("10^a =", np.power(10, a))  # 10^x

print("ln(a) =", np.log(a))  # 自然对数
print("log2(a) =", np.log2(a))  # 以2为底的对数
print("log10(a) =", np.log10(a))  # 以10为底的对数

# 特殊对数
print("log1p(a) =", np.log1p(a))  # ln(1+a)，对于小的a值更精确
print("expm1(a) =", np.expm1(a))  # e^a - 1，对于小的a值更精确

# 6. 舍入函数
print("\n=== 6. 舍入函数 ===")
a = np.array([1.234, 2.567, 3.891, -1.234, -2.567])

print("数组a:", a)
print("四舍五入到整数:", np.round(a))  # 四舍五入
print("四舍五入到1位小数:", np.round(a, 1))
print("四舍五入到-1位小数（十位）:", np.round(a, -1))

print("向下取整:", np.floor(a))  # 向下取整
print("向上取整:", np.ceil(a))   # 向上取整
print("截断取整:", np.trunc(a))  # 向0取整

# 7. 绝对值函数
print("\n=== 7. 绝对值函数 ===")
a = np.array([-3, -2, -1, 0, 1, 2, 3])
b = np.array([-3.14, 2.71, -1.41])

print("整数数组:", a)
print("绝对值:", np.abs(a))
print("绝对值（别名）:", np.absolute(a))

print("\n浮点数数组:", b)
print("绝对值:", np.abs(b))

# 复数的绝对值（模）
c = np.array([1+2j, 3+4j, 5+12j])
print("\n复数数组:", c)
print("模（绝对值）:", np.abs(c))

# 8. 符号函数
print("\n=== 8. 符号函数 ===")
a = np.array([-3.14, 0, 2.71, -0.0, 0.0])

print("数组a:", a)
print("符号函数:", np.sign(a))  # -1表示负数，0表示零，1表示正数
print("正负号:", np.signbit(a))  # True表示负数，False表示非负数

# 9. 最大值和最小值
print("\n=== 9. 最大值和最小值 ===")
a = np.array([1, 5, 3, 9, 2, 8])

print("数组a:", a)
print("最大值:", np.max(a), "或", a.max())
print("最小值:", np.min(a), "或", a.min())
print("最大值索引:", np.argmax(a))
print("最小值索引:", np.argmin(a))

# 逐元素比较
b = np.array([3, 6, 1, 10, 0, 5])
print("\n数组b:", b)
print("逐元素最大值:", np.maximum(a, b))
print("逐元素最小值:", np.minimum(a, b))

# 10. 杂项数学函数
print("\n=== 10. 杂项数学函数 ===")
a = np.array([1, 2, 3, 4, 5])

print("数组a:", a)
print("平方根:", np.sqrt(a))
print("平方:", np.square(a))
print("立方根:", np.cbrt(a))  # 立方根

# 特殊函数
x = np.array([0.1, 0.5, 1.0, 2.0])
print("\n数组x:", x)
print("倒数:", np.reciprocal(x))
print("符号*绝对值:", np.copysign([1, 1, 1, 1], [-1, 1, -1, 1]))

# 11. 数组统计运算
print("\n=== 11. 数组统计运算 ===")
a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])

print("二维数组:\n", a)
print("总和:", np.sum(a))
print("总和（沿轴0，列方向）:", np.sum(a, axis=0))
print("总和（沿轴1，行方向）:", np.sum(a, axis=1))

print("均值:", np.mean(a))
print("均值（沿轴0）:", np.mean(a, axis=0))
print("均值（沿轴1）:", np.mean(a, axis=1))

print("标准差:", np.std(a))
print("方差:", np.var(a))
print("乘积:", np.prod(a))
print("累积乘积:", np.cumprod(a))

# 12. 矩阵运算
print("\n=== 12. 矩阵运算 ===")
# 注意：* 是逐元素相乘，@ 或 dot 是矩阵乘法
a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])

print("矩阵a:\n", a)
print("矩阵b:\n", b)

print("逐元素相乘（a * b）:\n", a * b)
print("矩阵乘法（a @ b）:\n", a @ b)
print("矩阵乘法（np.dot(a, b)）:\n", np.dot(a, b))
print("矩阵乘法（np.matmul(a, b)）:\n", np.matmul(a, b))

# 矩阵转置
print("a的转置:\n", a.T)

# 13. 广播示例
print("\n=== 13. 广播示例 ===")
a = np.array([[1, 2, 3], [4, 5, 6]])
b = np.array([10, 20, 30])

print("二维数组a:\n", a)
print("一维数组b:", b)
print("a + b (广播):\n", a + b)  # b被广播到a的每一行

c = np.array([[10], [20]])
print("\n二维列向量c:\n", c)
print("a + c (广播):\n", a + c)  # c被广播到a的每一列

# 14. 原地操作
print("\n=== 14. 原地操作 ===")
a = np.array([1, 2, 3, 4, 5])
print("原始数组a:", a)

# 原地操作
a += 10
print("a += 10 后:", a)

a *= 2
print("a *= 2 后:", a)

# 非原地操作
b = a + 10
print("b = a + 10 后，a:", a, "b:", b)

# 15. 处理特殊值
print("\n=== 15. 处理特殊值 ===")
a = np.array([1, 2, np.nan, 4, np.inf, -np.inf, 7])
print("包含特殊值的数组:", a)

print("是否为有限数:", np.isfinite(a))
print("是否为无穷大:", np.isinf(a))
print("是否为NaN:", np.isnan(a))

# 忽略NaN进行计算
print("总和（忽略NaN）:", np.nansum(a))
print("均值（忽略NaN）:", np.nanmean(a))
print("最大值（忽略NaN）:", np.nanmax(a))

# 16. 综合示例
print("\n=== 16. 综合示例 ===")
# 计算二维高斯函数
x = np.linspace(-2, 2, 5)
y = np.linspace(-2, 2, 5)
X, Y = np.meshgrid(x, y)

print("X坐标:\n", X)
print("Y坐标:\n", Y)

# 高斯函数: f(x,y) = exp(-(x^2 + y^2))
Z = np.exp(-(X**2 + Y**2))
print("高斯函数值:\n", Z)

# 归一化
Z_normalized = Z / np.sum(Z)
print("归一化后的值:\n", Z_normalized)
print("归一化和:", np.sum(Z_normalized))

# 17. 性能对比
print("\n=== 17. 性能对比 ===")
import time

# 创建大型数组
large_arr = np.random.rand(1000000)

# NumPy向量化运算
start = time.time()
result_np = np.sin(large_arr) + np.cos(large_arr)
np_time = time.time() - start

# Python循环运算
start = time.time()
result_py = np.empty_like(large_arr)
for i in range(len(large_arr)):
    result_py[i] = np.sin(large_arr[i]) + np.cos(large_arr[i])
py_time = time.time() - start

print(f"NumPy向量化运算时间: {np_time:.6f}秒")
print(f"Python循环运算时间: {py_time:.6f}秒")
print(f"速度提升: {py_time/np_time:.1f}倍")
print(f"结果相等: {np.allclose(result_np, result_py)}")
```

## 总结
1. **向量化运算**：NumPy数学运算都是向量化的，比Python循环快得多
2. **算术运算**：`+`, `-`, `*`, `/`, `//`, `%`, `**` 等运算符，以及对应的NumPy函数
3. **比较运算**：`==`, `!=`, `<`, `<=`, `>`, `>=` 等运算符，返回布尔数组
4. **逻辑运算**：`&`, `|`, `~`, `^` 等位运算符，以及`logical_and`, `logical_or`等函数
5. **数学函数**：
   - 三角函数：`sin`, `cos`, `tan`, `arcsin`, `arccos`, `arctan`
   - 指数对数：`exp`, `exp2`, `log`, `log2`, `log10`, `log1p`, `expm1`
   - 舍入函数：`round`, `floor`, `ceil`, `trunc`
   - 其他函数：`sqrt`, `square`, `abs`, `sign`, `reciprocal`
6. **统计运算**：`sum`, `mean`, `std`, `var`, `min`, `max`, `argmin`, `argmax`
7. **矩阵运算**：`@` 或 `dot` 或 `matmul` 用于矩阵乘法，`*` 用于逐元素乘法
8. **广播机制**：自动处理不同形状数组的运算
9. **特殊值处理**：`isfinite`, `isinf`, `isnan`, `nansum`, `nanmean` 等
10. **性能优势**：向量化运算通常比循环快10-100倍甚至更多

**第七讲重点**：掌握NumPy的基本数学运算，理解向量化运算的优势，学会使用各种数学函数，注意区分逐元素运算和矩阵运算，了解广播机制的原理和应用。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第八讲：统计函数

## 概念
统计函数用于对数组中的数据进行统计分析，包括集中趋势（如均值、中位数）、离散程度（如方差、标准差）、相关性等。这些函数可以沿着指定的轴计算，也可以忽略NaN值。

## 原理
1. **轴（axis）参数**：大多数统计函数支持`axis`参数，用于指定计算方向。对于二维数组，`axis=0`表示沿着列（垂直方向）计算，`axis=1`表示沿着行（水平方向）计算。如果不指定轴，则计算整个数组的统计量。
2. **NaN安全版本**：许多统计函数有对应的`nan*`版本（如`nanmean`、`nanstd`），可以忽略NaN值进行计算。
3. **多维数组**：统计函数可以应用于任意维度的数组，通过指定轴来获取不同维度的统计量。

## 例子
```python
import numpy as np

# 1. 集中趋势度量
print("=== 1. 集中趋势度量 ===")
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
print("数组:", arr)

print("平均值:", np.mean(arr))
print("中位数:", np.median(arr))
print("加权平均值（权重相同）:", np.average(arr))  # 默认权重相同，等同于均值
print("加权平均值（自定义权重）:", np.average(arr, weights=[1, 1, 1, 1, 1, 2, 2, 2, 2]))

# 众数（可能需要SciPy，这里用bincount近似）
arr2 = np.array([1, 2, 2, 3, 3, 3, 4, 4, 5])
counts = np.bincount(arr2.astype(int))
mode_value = np.argmax(counts)
print("数组:", arr2)
print("众数:", mode_value)  # 出现次数最多的值

# 2. 离散程度度量
print("\n=== 2. 离散程度度量 ===")
print("数组:", arr)
print("方差:", np.var(arr))  # 方差
print("标准差:", np.std(arr))  # 标准差
print("最小值:", np.min(arr))
print("最大值:", np.max(arr))
print("极差:", np.ptp(arr))  # 极差（最大值-最小值）

# 百分位数
print("25%分位数（第一四分位数）:", np.percentile(arr, 25))
print("50%分位数（中位数）:", np.percentile(arr, 50))
print("75%分位数（第三四分位数）:", np.percentile(arr, 75))
print("90%分位数:", np.percentile(arr, 90))

# 四分位距
q75, q25 = np.percentile(arr, [75, 25])
print("四分位距:", q75 - q25)

# 3. 多维数组的统计
print("\n=== 3. 多维数组的统计 ===")
arr_2d = np.array([[1, 2, 3, 4],
                   [5, 6, 7, 8],
                   [9, 10, 11, 12]])
print("二维数组:\n", arr_2d)

# 沿轴0计算（列方向）
print("每列的平均值（axis=0）:", np.mean(arr_2d, axis=0))
print("每列的中位数（axis=0）:", np.median(arr_2d, axis=0))
print("每列的方差（axis=0）:", np.var(arr_2d, axis=0))
print("每列的标准差（axis=0）:", np.std(arr_2d, axis=0))

# 沿轴1计算（行方向）
print("每行的平均值（axis=1）:", np.mean(arr_2d, axis=1))
print("每行的中位数（axis=1）:", np.median(arr_2d, axis=1))
print("每行的方差（axis=1）:", np.var(arr_2d, axis=1))
print("每行的标准差（axis=1）:", np.std(arr_2d, axis=1))

# 整个数组的统计
print("整个数组的平均值:", np.mean(arr_2d))
print("整个数组的中位数:", np.median(arr_2d))
print("整个数组的方差:", np.var(arr_2d))
print("整个数组的标准差:", np.std(arr_2d))

# 4. 累积统计
print("\n=== 4. 累积统计 ===")
arr = np.array([1, 2, 3, 4, 5])
print("数组:", arr)
print("累积和:", np.cumsum(arr))  # 累积和
print("累积积:", np.cumprod(arr))  # 累积积
print("累积最小值:", np.minimum.accumulate(arr))  # 累积最小值
print("累积最大值:", np.maximum.accumulate(arr))  # 累积最大值

# 二维数组的累积和（沿轴0）
arr_2d = np.array([[1, 2, 3], [4, 5, 6]])
print("\n二维数组:\n", arr_2d)
print("沿轴0累积和:\n", np.cumsum(arr_2d, axis=0))
print("沿轴1累积和:\n", np.cumsum(arr_2d, axis=1))

# 5. 相关性
print("\n=== 5. 相关性 ===")
x = np.array([1, 2, 3, 4, 5])
y = np.array([2, 4, 6, 8, 10])
z = np.array([5, 4, 3, 2, 1])

print("x:", x)
print("y:", y)
print("z:", z)

# 相关系数矩阵
corr_matrix = np.corrcoef([x, y, z])
print("相关系数矩阵（x, y, z）:\n", corr_matrix)

# 协方差矩阵
cov_matrix = np.cov([x, y, z])
print("协方差矩阵（x, y, z）:\n", cov_matrix)

# 单个相关系数
print("x和y的相关系数:", np.corrcoef(x, y)[0, 1])

# 6. 直方图统计
print("\n=== 6. 直方图统计 ===")
data = np.random.randn(1000)  # 1000个标准正态分布随机数
print("数据（前10个）:", data[:10])

# 计算直方图
hist, bin_edges = np.histogram(data, bins=10)
print("直方图计数:", hist)
print("箱子边界:", bin_edges)

# 加权直方图
weights = np.ones_like(data) * 2  # 每个数据点权重为2
hist_weighted, _ = np.histogram(data, bins=10, weights=weights)
print("加权直方图计数（权重为2）:", hist_weighted)

# 二维直方图
x = np.random.randn(1000)
y = np.random.randn(1000)
hist_2d, xedges, yedges = np.histogram2d(x, y, bins=10)
print("二维直方图形状:", hist_2d.shape)

# 7. 顺序统计
print("\n=== 7. 顺序统计 ===")
arr = np.array([3, 1, 4, 1, 5, 9, 2, 6, 5, 3])
print("数组:", arr)
print("排序后数组:", np.sort(arr))
print("排序后索引:", np.argsort(arr))  # 返回排序后的索引
print("分位数（0.25, 0.5, 0.75）:", np.quantile(arr, [0.25, 0.5, 0.75]))  # 类似percentile
print("第3小的元素（0-index）:", np.partition(arr, 2)[2])  # 分区函数，第3小（索引2）

# 8. 处理NaN值的统计
print("\n=== 8. 处理NaN值的统计 ===")
arr_with_nan = np.array([1, 2, np.nan, 4, 5, np.nan, 7])
print("包含NaN的数组:", arr_with_nan)
print("忽略NaN的平均值:", np.nanmean(arr_with_nan))
print("忽略NaN的中位数:", np.nanmedian(arr_with_nan))
print("忽略NaN的方差:", np.nanvar(arr_with_nan))
print("忽略NaN的标准差:", np.nanstd(arr_with_nan))
print("忽略NaN的最小值:", np.nanmin(arr_with_nan))
print("忽略NaN的最大值:", np.nanmax(arr_with_nan))
print("忽略NaN的百分位数（25%）:", np.nanpercentile(arr_with_nan, 25))

# 9. 其他统计函数
print("\n=== 9. 其他统计函数 ===")
arr = np.array([1, 2, 3, 4, 5])
print("数组:", arr)
print("数组的迹（对角线元素和）:", np.trace(np.diag(arr)))  # 对于对角矩阵，迹等于对角线元素和
print("数组的矩（k=1,2,3）:", [np.mean((arr - np.mean(arr))**k) for k in [1, 2, 3]])

# 峰度和偏度（需要SciPy，这里手动计算）
from scipy import stats
print("偏度（使用scipy）:", stats.skew(arr))
print("峰度（使用scipy）:", stats.kurtosis(arr))

# 10. 掩码数组统计
print("\n=== 10. 掩码数组统计 ===")
# 掩码数组允许我们标记无效或不需要的值
arr = np.array([1, 2, 3, 4, 5])
mask = np.array([False, False, True, False, False])  # 标记第三个元素为掩码（忽略）
masked_arr = np.ma.array(arr, mask=mask)
print("原始数组:", arr)
print("掩码:", mask)
print("掩码数组平均值:", masked_arr.mean())
print("掩码数组方差:", masked_arr.var())

# 11. 加权统计
print("\n=== 11. 加权统计 ===")
arr = np.array([1, 2, 3, 4, 5])
weights = np.array([1, 1, 2, 2, 1])  # 权重
print("数组:", arr)
print("权重:", weights)
print("加权平均值:", np.average(arr, weights=weights))
print("加权方差（频率权重）:")

# 计算加权方差
average = np.average(arr, weights=weights)
variance = np.average((arr - average)**2, weights=weights)
print(variance)

# 12. 移动窗口统计
print("\n=== 12. 移动窗口统计 ===")
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
window_size = 3

# 简单移动平均
moving_average = np.convolve(arr, np.ones(window_size)/window_size, mode='valid')
print("数组:", arr)
print(f"移动平均（窗口大小={window_size}）:", moving_average)

# 13. 综合示例
print("\n=== 13. 综合示例 ===")
# 模拟学生成绩
np.random.seed(42)
scores = np.random.randint(60, 100, size=(5, 4))  # 5个学生，4门课程
print("学生成绩表（5行4列）:\n", scores)

# 计算每个学生的平均分
student_avg = np.mean(scores, axis=1)
print("每个学生的平均分:", student_avg)

# 计算每门课程的平均分
course_avg = np.mean(scores, axis=0)
print("每门课程的平均分:", course_avg)

# 计算每门课程的标准差
course_std = np.std(scores, axis=0)
print("每门课程的标准差:", course_std)

# 计算总平均分
total_avg = np.mean(scores)
print("总平均分:", total_avg)

# 找出最高分和对应的学生、课程
max_score = np.max(scores)
max_index = np.argmax(scores)  # 展平后的索引
max_row, max_col = np.unravel_index(max_index, scores.shape)
print(f"最高分: {max_score}, 学生{max_row}, 课程{max_col}")

# 计算成绩的分布
hist, bins = np.histogram(scores, bins=[60, 70, 80, 90, 100])
print("成绩分布（60-70,70-80,80-90,90-100）:", hist)
```

## 总结
1. **集中趋势**：
   - `mean()`：平均值
   - `median()`：中位数
   - `average()`：加权平均值
   - `percentile()`/`quantile()`：百分位数/分位数

2. **离散程度**：
   - `var()`：方差
   - `std()`：标准差
   - `min()`/`max()`：最小值/最大值
   - `ptp()`：极差
   - 四分位距：通过`percentile(75) - percentile(25)`计算

3. **累积统计**：
   - `cumsum()`：累积和
   - `cumprod()`：累积积
   - `minimum.accumulate()`：累积最小值
   - `maximum.accumulate()`：累积最大值

4. **相关性**：
   - `corrcoef()`：相关系数矩阵
   - `cov()`：协方差矩阵

5. **直方图**：
   - `histogram()`：一维直方图
   - `histogram2d()`：二维直方图

6. **顺序统计**：
   - `sort()`：排序
   - `argsort()`：排序索引
   - `partition()`：分区

7. **NaN安全版本**：`nanmean()`、`nanstd()`、`nanvar()`等，忽略NaN值

8. **轴参数**：大多数统计函数支持`axis`参数，用于指定计算方向

9. **加权统计**：`average()`支持权重参数

10. **应用场景**：数据分析、数据清洗、特征工程、数据可视化等

**第八讲重点**：掌握NumPy的统计函数，理解不同统计量的含义，学会沿指定轴计算统计量，能够处理包含NaN值的数据，并了解加权统计和累积统计。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第九讲：线性代数基础

## 概念
线性代数是科学计算的基础，NumPy提供了丰富的线性代数功能，包括矩阵运算、分解、求逆、解线性方程组等。这些功能主要包含在`numpy.linalg`模块中。

## 原理
1. **矩阵表示**：NumPy中，矩阵可以用二维数组表示，但需要注意数组的乘法是逐元素乘法，矩阵乘法需要使用专门的函数或运算符。
2. **线性代数运算**：包括矩阵乘法、求逆、行列式、特征值/特征向量、奇异值分解等。
3. **数值稳定性**：线性代数运算需要考虑数值稳定性，特别是对于病态矩阵。
4. **广播与线性代数**：某些线性代数运算支持广播，但需要满足特定的维度要求。

## 例子
```python
import numpy as np

# 1. 矩阵创建和基本属性
print("=== 1. 矩阵创建和基本属性 ===")
# 创建矩阵
A = np.array([[1, 2, 3],
              [4, 5, 6],
              [7, 8, 9]])
B = np.array([[9, 8, 7],
              [6, 5, 4],
              [3, 2, 1]])

print("矩阵A:\n", A)
print("矩阵B:\n", B)
print("A的形状:", A.shape)
print("A的转置:\n", A.T)
print("A的迹（对角线元素和）:", np.trace(A))

# 2. 矩阵乘法
print("\n=== 2. 矩阵乘法 ===")
# 注意：* 是逐元素乘法，@ 或 dot 是矩阵乘法
print("逐元素乘法（A * B）:\n", A * B)
print("矩阵乘法（A @ B）:\n", A @ B)
print("矩阵乘法（np.dot(A, B)）:\n", np.dot(A, B))
print("矩阵乘法（np.matmul(A, B)）:\n", np.matmul(A, B))

# 向量-矩阵乘法
v = np.array([1, 2, 3])
print("\n向量v:", v)
print("A @ v:", A @ v)  # 矩阵乘以向量
print("v @ A:", v @ A)  # 向量乘以矩阵

# 3. 矩阵求逆
print("\n=== 3. 矩阵求逆 ===")
# 创建一个可逆矩阵
A = np.array([[4, 7],
              [2, 6]])
print("矩阵A:\n", A)

# 求逆
A_inv = np.linalg.inv(A)
print("A的逆矩阵:\n", A_inv)

# 验证逆矩阵
I = A @ A_inv
print("A * A_inv（应接近单位矩阵）:\n", np.round(I, 10))

# 伪逆（广义逆），适用于非方阵或奇异矩阵
B = np.array([[1, 2, 3],
              [4, 5, 6]])
B_pinv = np.linalg.pinv(B)
print("\n矩阵B:\n", B)
print("B的伪逆:\n", B_pinv)
print("B * B_pinv * B（应接近B）:\n", np.round(B @ B_pinv @ B, 10))

# 4. 行列式
print("\n=== 4. 行列式 ===")
A = np.array([[1, 2],
              [3, 4]])
print("矩阵A:\n", A)
det_A = np.linalg.det(A)
print("A的行列式:", det_A)

# 行列式的性质
B = np.array([[2, 4],
              [6, 8]])
print("\n矩阵B（B = 2A）:\n", B)
det_B = np.linalg.det(B)
print("B的行列式:", det_B)
print(f"验证: det(B) = {det_B}, 2^2 * det(A) = {4 * det_A}")

# 奇异矩阵的行列式为0
C = np.array([[1, 2],
              [2, 4]])
print("\n奇异矩阵C:\n", C)
print("C的行列式:", np.linalg.det(C))

# 5. 解线性方程组
print("\n=== 5. 解线性方程组 ===")
# 解方程组 Ax = b
A = np.array([[3, 1],
              [1, 2]])
b = np.array([9, 8])
print(f"方程组: 3x + y = 9, x + 2y = 8")
print("系数矩阵A:\n", A)
print("常数向量b:", b)

# 求解
x = np.linalg.solve(A, b)
print("解x:", x)
print("验证: A @ x =", A @ x, "应接近b")

# 使用逆矩阵求解
x_inv = np.linalg.inv(A) @ b
print("使用逆矩阵求解:", x_inv)

# 最小二乘解（对于超定方程组）
A_over = np.array([[1, 1],
                   [1, 2],
                   [1, 3]])
b_over = np.array([1, 2, 2])
print(f"\n超定方程组: x + y = 1, x + 2y = 2, x + 3y = 2")
x_lstsq, residuals, rank, s = np.linalg.lstsq(A_over, b_over, rcond=None)
print("最小二乘解:", x_lstsq)
print("残差平方和:", residuals)

# 6. 特征值和特征向量
print("\n=== 6. 特征值和特征向量 ===")
A = np.array([[4, 2],
              [1, 3]])
print("矩阵A:\n", A)

# 计算特征值和特征向量
eigenvalues, eigenvectors = np.linalg.eig(A)
print("特征值:", eigenvalues)
print("特征向量（每列是一个特征向量）:\n", eigenvectors)

# 验证：A * v = λ * v
for i in range(len(eigenvalues)):
    v = eigenvectors[:, i]
    λ = eigenvalues[i]
    print(f"验证特征向量{i}: A@v = {A@v}, λ*v = {λ*v}")
    print(f"是否接近: {np.allclose(A@v, λ*v)}")

# 特征值分解：A = V * diag(λ) * V^(-1)
V = eigenvectors
Λ = np.diag(eigenvalues)
V_inv = np.linalg.inv(V)
A_reconstructed = V @ Λ @ V_inv
print("\n通过特征值分解重构A:\n", np.round(A_reconstructed, 10))
print("重构是否准确:", np.allclose(A, A_reconstructed))

# 7. 奇异值分解（SVD）
print("\n=== 7. 奇异值分解（SVD） ===")
A = np.array([[1, 2, 3],
              [4, 5, 6],
              [7, 8, 9],
              [10, 11, 12]])
print("矩阵A（4x3）:\n", A)

# 进行SVD分解
U, S, Vt = np.linalg.svd(A, full_matrices=False)
print("U矩阵（4x3）:\n", U)
print("奇异值S:", S)
print("V转置矩阵Vt（3x3）:\n", Vt)

# 重构原始矩阵
Σ = np.diag(S)
A_reconstructed = U @ Σ @ Vt
print("\n通过SVD重构A:\n", np.round(A_reconstructed, 10))
print("重构是否准确:", np.allclose(A, A_reconstructed))

# 8. 矩阵分解
print("\n=== 8. 矩阵分解 ===")
# Cholesky分解（适用于对称正定矩阵）
A = np.array([[4, 2, 2],
              [2, 5, 3],
              [2, 3, 6]])
print("对称正定矩阵A:\n", A)

L = np.linalg.cholesky(A)
print("Cholesky分解下三角矩阵L:\n", L)
print("L * L.T:\n", L @ L.T)
print("是否等于A:", np.allclose(A, L @ L.T))

# QR分解
A = np.array([[1, 2, 3],
              [4, 5, 6],
              [7, 8, 9]])
Q, R = np.linalg.qr(A)
print("\n矩阵A:\n", A)
print("QR分解:")
print("Q矩阵（正交）:\n", np.round(Q, 10))
print("R矩阵（上三角）:\n", np.round(R, 10))
print("Q * R:\n", Q @ R)
print("是否等于A:", np.allclose(A, Q @ R))

# 9. 矩阵的秩
print("\n=== 9. 矩阵的秩 ===")
# 满秩矩阵
A = np.array([[1, 2, 3],
              [4, 5, 6],
              [7, 8, 9]])
rank_A = np.linalg.matrix_rank(A)
print("矩阵A:\n", A)
print("A的秩:", rank_A)

# 秩亏矩阵
B = np.array([[1, 2, 3],
              [2, 4, 6],  # 第二行是第一行的2倍
              [3, 6, 9]]) # 第三行是第一行的3倍
rank_B = np.linalg.matrix_rank(B)
print("\n矩阵B（秩亏）:\n", B)
print("B的秩:", rank_B)

# 10. 矩阵范数
print("\n=== 10. 矩阵范数 ===")
A = np.array([[1, 2],
              [3, 4]])
print("矩阵A:\n", A)

# 范数
print("Frobenius范数（默认）:", np.linalg.norm(A))
print("Frobenius范数（显式指定）:", np.linalg.norm(A, 'fro'))
print("L1范数（列和的最大值）:", np.linalg.norm(A, 1))
print("L∞范数（行和的最大值）:", np.linalg.norm(A, np.inf))
print("L2范数（最大奇异值）:", np.linalg.norm(A, 2))

# 向量范数
v = np.array([1, 2, 3])
print("\n向量v:", v)
print("L1范数:", np.linalg.norm(v, 1))
print("L2范数（默认）:", np.linalg.norm(v))
print("L∞范数:", np.linalg.norm(v, np.inf))

# 11. 矩阵指数和对数
print("\n=== 11. 矩阵指数和对数 ===")
# 从scipy导入矩阵指数和对数
try:
    from scipy.linalg import expm, logm
    
    A = np.array([[0, -1],
                  [1, 0]])
    print("矩阵A:\n", A)
    
    # 矩阵指数
    A_exp = expm(A)
    print("矩阵指数exp(A):\n", A_exp)
    
    # 矩阵对数
    A_log = logm(A_exp)
    print("矩阵对数log(exp(A)):\n", np.round(A_log, 10))
    print("是否等于A:", np.allclose(A, A_log))
    
except ImportError:
    print("需要scipy库来计算矩阵指数和对数")

# 12. 线性代数应用示例
print("\n=== 12. 线性代数应用示例 ===")
# 主成分分析（PCA）的简化实现
np.random.seed(42)
# 生成一些二维数据
X = np.random.randn(100, 2) @ np.array([[2, 1], [1, 2]]) + np.array([3, 5])

# 数据中心化
X_centered = X - np.mean(X, axis=0)

# 计算协方差矩阵
cov_matrix = np.cov(X_centered.T)

# 计算特征值和特征向量
eigenvalues, eigenvectors = np.linalg.eig(cov_matrix)

# 按特征值大小排序
idx = eigenvalues.argsort()[::-1]
eigenvalues = eigenvalues[idx]
eigenvectors = eigenvectors[:, idx]

print("数据形状:", X.shape)
print("协方差矩阵:\n", cov_matrix)
print("特征值（主成分方差）:", eigenvalues)
print("特征向量（主成分方向）:\n", eigenvectors)

# 13. 特殊矩阵
print("\n=== 13. 特殊矩阵 ===")
# 单位矩阵
I = np.eye(3)
print("3x3单位矩阵:\n", I)

# 对角矩阵
diag = np.diag([1, 2, 3])
print("对角矩阵:\n", diag)

# 范德蒙德矩阵
vander = np.vander([1, 2, 3, 4], 3)
print("范德蒙德矩阵:\n", vander)

# 托普利茨矩阵
toeplitz = np.zeros((4, 4))
for i in range(4):
    for j in range(4):
        toeplitz[i, j] = i - j
print("托普利茨矩阵:\n", toeplitz)

# 14. 线性代数性能比较
print("\n=== 14. 线性代数性能比较 ===")
import time

# 创建大矩阵
n = 500
A_large = np.random.randn(n, n)
B_large = np.random.randn(n, n)

# 矩阵乘法性能
start = time.time()
C1 = A_large @ B_large
time_dot = time.time() - start
print(f"{n}x{n}矩阵乘法用时: {time_dot:.3f}秒")

# 求逆性能
start = time.time()
A_inv = np.linalg.inv(A_large)
time_inv = time.time() - start
print(f"{n}x{n}矩阵求逆用时: {time_inv:.3f}秒")

# 解线性方程组性能
b = np.random.randn(n)
start = time.time()
x = np.linalg.solve(A_large, b)
time_solve = time.time() - start
print(f"{n}x{n}线性方程组求解用时: {time_solve:.3f}秒")

# 15. 数值稳定性
print("\n=== 15. 数值稳定性 ===")
# 病态矩阵示例
A_ill = np.array([[1, 2],
                  [1, 2.0001]])
b_ill = np.array([3, 3.0001])

print("病态矩阵A:\n", A_ill)
print("条件数（cond）:", np.linalg.cond(A_ill))

# 解病态方程组
x_ill = np.linalg.solve(A_ill, b_ill)
print("解:", x_ill)
print("验证: A@x =", A_ill @ x_ill, "应接近", b_ill)

# 使用伪逆求解
x_pinv = np.linalg.pinv(A_ill) @ b_ill
print("使用伪逆求解:", x_pinv)
```

## 总结
1. **矩阵乘法**：使用`@`、`dot()`或`matmul()`，注意与逐元素乘法`*`的区别
2. **矩阵求逆**：`inv()`用于可逆方阵，`pinv()`用于计算伪逆（广义逆）
3. **行列式**：`det()`计算行列式，可用于判断矩阵是否可逆
4. **解线性方程组**：`solve()`求解Ax=b，`lstsq()`求解最小二乘问题
5. **特征值和特征向量**：`eig()`计算特征值和特征向量
6. **奇异值分解**：`svd()`进行SVD分解
7. **矩阵分解**：
   - Cholesky分解：`cholesky()`，用于对称正定矩阵
   - QR分解：`qr()`，将矩阵分解为正交矩阵和上三角矩阵
8. **矩阵秩**：`matrix_rank()`计算矩阵的秩
9. **范数**：`norm()`计算矩阵或向量的范数
10. **矩阵指数和对数**：需要SciPy库的`expm()`和`logm()`
11. **数值稳定性**：注意病态矩阵的条件数，使用`cond()`评估
12. **特殊矩阵**：单位矩阵`eye()`、对角矩阵`diag()`、范德蒙德矩阵`vander()`
13. **性能考虑**：对于大矩阵，直接求解可能比求逆再乘法更高效
14. **应用场景**：机器学习、图像处理、信号处理、优化问题等

**第九讲重点**：掌握NumPy的线性代数功能，理解矩阵运算的基本概念，能够解决线性方程组、进行矩阵分解和特征值计算，注意数值稳定性和性能优化。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第十讲：广播机制

## 概念
广播（Broadcasting）是NumPy中处理不同形状数组间算术运算的方式。它允许NumPy在执行元素级运算时自动扩展较小的数组，使其与较大数组的形状兼容，而无需实际复制数据。

## 原理
1. **广播规则**：当两个数组形状不同时，NumPy会按照一定规则扩展较小的数组：
   - 规则1：如果数组维度数不同，在较小数组的形状前面补1
   - 规则2：如果数组在某个维度上大小不同，且其中一个为1，则可以沿该维度扩展
   - 规则3：如果数组在某个维度上大小不同，且都不为1，则无法广播

2. **广播步骤**：
   - 对齐数组维度（从最右边开始）
   - 检查每个维度是否兼容
   - 扩展维度为1的数组

3. **内存效率**：广播不会实际复制数据，而是在运算时动态扩展，节省内存

## 例子
```python
import numpy as np

# 1. 标量与数组的广播
print("=== 1. 标量与数组的广播 ===")
arr = np.array([1, 2, 3, 4, 5])
scalar = 10

print("数组:", arr)
print("标量:", scalar)
print("数组 + 标量:", arr + scalar)  # 标量被广播到数组的每个元素
print("数组 * 标量:", arr * scalar)
print("数组 > 标量:", arr > scalar)

# 2. 一维数组与二维数组的广播
print("\n=== 2. 一维数组与二维数组的广播 ===")
arr_2d = np.array([[1, 2, 3],
                   [4, 5, 6],
                   [7, 8, 9]])
arr_1d = np.array([10, 20, 30])

print("二维数组:\n", arr_2d)
print("一维数组:", arr_1d)
print("二维数组 + 一维数组:\n", arr_2d + arr_1d)  # 一维数组被广播到每一行

# 广播过程分析：
# arr_2d.shape = (3, 3)
# arr_1d.shape = (3,)
# 步骤1：对齐维度，arr_1d.shape变为(1, 3)
# 步骤2：沿轴0扩展，arr_1d.shape变为(3, 3)
# 步骤3：执行运算

# 3. 列向量与二维数组的广播
print("\n=== 3. 列向量与二维数组的广播 ===")
arr_2d = np.array([[1, 2, 3],
                   [4, 5, 6],
                   [7, 8, 9]])
col_vector = np.array([[10], [20], [30]])  # 3x1列向量

print("二维数组:\n", arr_2d)
print("列向量:\n", col_vector)
print("二维数组 + 列向量:\n", arr_2d + col_vector)  # 列向量被广播到每一列

# 4. 不匹配形状的广播
print("\n=== 4. 不匹配形状的广播 ===")
A = np.array([[1, 2, 3],
              [4, 5, 6]])  # 2x3
B = np.array([10, 20])     # (2,)

print("A (2x3):\n", A)
print("B (2,):", B)
print("尝试 A + B:")

try:
    result = A + B
    print("成功:", result)
except ValueError as e:
    print("失败:", e)
    print("原因: B的形状(2,)对齐后为(1,2)，与A的第二维(3)不匹配")

# 修正：将B转换为列向量
B_col = B.reshape(-1, 1)  # 2x1
print("\nB转换为列向量(2x1):\n", B_col)
print("A + B_col:\n", A + B_col)  # 成功广播

# 5. 三维数组的广播
print("\n=== 5. 三维数组的广播 ===")
# 创建3D数组
arr_3d = np.arange(24).reshape(2, 3, 4)
print("三维数组形状:", arr_3d.shape)
print("三维数组:\n", arr_3d)

# 与2D数组广播
arr_2d = np.array([[10, 20, 30, 40]])  # 1x4
print("\n二维数组(1x4):\n", arr_2d)
print("三维数组 + 二维数组:\n", arr_3d + arr_2d)

# 广播过程：
# arr_3d.shape = (2, 3, 4)
# arr_2d.shape = (1, 4)
# 步骤1：对齐维度，arr_2d.shape变为(1, 1, 4)
# 步骤2：沿轴0扩展，arr_2d.shape变为(2, 1, 4)
# 步骤3：沿轴1扩展，arr_2d.shape变为(2, 3, 4)
# 步骤4：执行运算

# 6. 使用newaxis显式添加维度
print("\n=== 6. 使用newaxis显式添加维度 ===")
arr = np.array([1, 2, 3])
print("一维数组:", arr, "形状:", arr.shape)

# 添加行维度
row_vector = arr[np.newaxis, :]  # 或 arr.reshape(1, -1)
print("行向量:\n", row_vector, "形状:", row_vector.shape)

# 添加列维度
col_vector = arr[:, np.newaxis]  # 或 arr.reshape(-1, 1)
print("列向量:\n", col_vector, "形状:", col_vector.shape)

# 7. 广播的实际应用
print("\n=== 7. 广播的实际应用 ===")
# 标准化数据
data = np.array([[1, 2, 3],
                 [4, 5, 6],
                 [7, 8, 9]])
print("原始数据:\n", data)

# 计算每列的均值和标准差
col_means = np.mean(data, axis=0)
col_stds = np.std(data, axis=0)
print("每列均值:", col_means)
print("每列标准差:", col_stds)

# 标准化：z = (x - μ) / σ
normalized = (data - col_means) / col_stds
print("标准化后的数据:\n", normalized)
print("验证：每列均值接近0:", np.mean(normalized, axis=0))
print("验证：每列标准差接近1:", np.std(normalized, axis=0))

# 8. 外积（outer product）通过广播实现
print("\n=== 8. 外积（outer product）通过广播实现 ===")
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])

# 使用广播计算外积
a_col = a[:, np.newaxis]  # 3x1
b_row = b[np.newaxis, :]  # 1x3
outer_product = a_col * b_row
print("向量a:", a)
print("向量b:", b)
print("外积a ⊗ b:\n", outer_product)

# 使用np.outer对比
print("np.outer(a, b):\n", np.outer(a, b))
print("结果相等:", np.array_equal(outer_product, np.outer(a, b)))

# 9. 距离矩阵计算
print("\n=== 9. 距离矩阵计算 ===")
# 计算点之间的距离矩阵
points = np.array([[0, 0],
                   [1, 0],
                   [0, 1],
                   [1, 1]])
print("点集:\n", points)

# 计算所有点对之间的欧氏距离
# 使用广播：√((x_i - x_j)² + (y_i - y_j)²)
x = points[:, 0]  # 所有点的x坐标
y = points[:, 1]  # 所有点的y坐标

# 通过广播计算差值的平方
dx = x[:, np.newaxis] - x[np.newaxis, :]  # x_i - x_j
dy = y[:, np.newaxis] - y[np.newaxis, :]  # y_i - y_j
distances = np.sqrt(dx**2 + dy**2)
print("距离矩阵:\n", distances)

# 10. 广播与reduce操作的结合
print("\n=== 10. 广播与reduce操作的结合 ===")
arr = np.array([[1, 2, 3],
                [4, 5, 6],
                [7, 8, 9]])

# 计算每行的总和
row_sums = np.sum(arr, axis=1, keepdims=True)  # keepdims=True保持维度
print("数组:\n", arr)
print("每行总和（保持维度）:\n", row_sums)
print("行总和的形状:", row_sums.shape)

# 计算每行的比例
row_proportions = arr / row_sums
print("每行的比例:\n", row_proportions)
print("验证：每行比例和为1:", np.sum(row_proportions, axis=1))

# 11. 广播的局限性
print("\n=== 11. 广播的局限性 ===")
# 无法广播的情况
A = np.array([[1, 2, 3],
              [4, 5, 6]])  # 2x3
B = np.array([[1, 2],
              [3, 4]])     # 2x2

print("A (2x3):\n", A)
print("B (2x2):\n", B)
print("尝试 A + B:")

try:
    result = A + B
    print("成功:", result)
except ValueError as e:
    print("失败:", e)
    print("原因: 形状(2,3)和(2,2)不兼容")

# 12. 广播的性能
print("\n=== 12. 广播的性能 ===")
import time

# 大数组广播性能
large_arr = np.random.rand(1000, 1000)
small_arr = np.random.rand(1, 1000)

# 广播运算
start = time.time()
result = large_arr + small_arr
broadcast_time = time.time() - start
print(f"广播运算时间: {broadcast_time:.4f}秒")

# 手动扩展对比
start = time.time()
# 手动扩展小数组
small_expanded = np.tile(small_arr, (1000, 1))
result_manual = large_arr + small_expanded
tile_time = time.time() - start
print(f"手动扩展后运算时间: {tile_time:.4f}秒")
print(f"广播效率提升: {tile_time/broadcast_time:.1f}倍")
print(f"结果相等: {np.allclose(result, result_manual)}")

# 13. 复杂广播示例
print("\n=== 13. 复杂广播示例 ===")
# 计算多项式在多个点的值
# 多项式: f(x) = a0 + a1*x + a2*x² + a3*x³
coefficients = np.array([1, 2, 3, 4])  # a0, a1, a2, a3
x_values = np.array([0, 1, 2, 3, 4])

# 使用广播计算
# 创建幂次矩阵: 形状(5, 4)，每行是[x⁰, x¹, x², x³]
powers = np.arange(4)  # [0, 1, 2, 3]
x_powers = x_values[:, np.newaxis] ** powers[np.newaxis, :]
print("x值:", x_values)
print("幂次矩阵:\n", x_powers)

# 计算多项式值
polynomial_values = np.sum(coefficients * x_powers, axis=1)
print("多项式值:", polynomial_values)

# 验证
for i, x in enumerate(x_values):
    manual = 1 + 2*x + 3*x**2 + 4*x**3
    print(f"f({x}) = {polynomial_values[i]} (应为{manual})")

# 14. 广播形状调试
print("\n=== 14. 广播形状调试 ===")
def broadcast_shape(shape1, shape2):
    """模拟广播过程"""
    # 确保shape1是维度较多的
    if len(shape1) < len(shape2):
        shape1, shape2 = shape2, shape1
    
    # 在shape2前面补1
    pad_len = len(shape1) - len(shape2)
    shape2_padded = (1,) * pad_len + shape2
    
    result_shape = []
    for d1, d2 in zip(shape1, shape2_padded):
        if d1 == 1 or d2 == 1 or d1 == d2:
            result_shape.append(max(d1, d2))
        else:
            raise ValueError(f"形状不兼容: {d1} 和 {d2}")
    
    return tuple(result_shape)

# 测试广播形状计算
test_cases = [
    ((3, 4), (4,)),          # 应该得到 (3, 4)
    ((2, 1, 4), (3, 1)),     # 应该得到 (2, 3, 4)
    ((5, 3, 2), (3, 2)),     # 应该得到 (5, 3, 2)
    ((3, 4), (3,)),          # 应该失败
]

for shape1, shape2 in test_cases:
    print(f"\n形状1: {shape1}, 形状2: {shape2}")
    try:
        result = broadcast_shape(shape1, shape2)
        print(f"广播后形状: {result}")
    except ValueError as e:
        print(f"错误: {e}")

# 15. 实际应用：图像处理
print("\n=== 15. 实际应用：图像处理 ===")
# 模拟一个RGB图像 (高度, 宽度, 通道)
height, width, channels = 3, 4, 3
image = np.random.randint(0, 256, (height, width, channels), dtype=np.uint8)
print("模拟图像形状:", image.shape)
print("图像数据:\n", image)

# 调整亮度（广播标量）
brightness_adjust = 50
brightened = np.clip(image.astype(np.int16) + brightness_adjust, 0, 255).astype(np.uint8)
print(f"\n亮度调整{50}后:\n", brightened)

# 应用滤镜（广播3D数组到每个像素）
filter_weights = np.array([0.299, 0.587, 0.114])  # RGB转灰度的权重
grayscale = np.sum(image * filter_weights, axis=2).astype(np.uint8)
print(f"\n灰度图像:\n", grayscale)
print("灰度图像形状:", grayscale.shape)
```

## 总结
1. **广播规则**：
   - 形状从最右边开始对齐
   - 维度为1的数组可以扩展
   - 维度相同或其中一个为1时可以广播

2. **常见广播模式**：
   - 标量与数组：标量广播到数组每个元素
   - 一维数组与二维数组：沿缺失维度扩展
   - 列向量与二维数组：沿列方向扩展
   - 行向量与二维数组：沿行方向扩展

3. **显式控制维度**：
   - 使用`np.newaxis`或`reshape`添加维度
   - 使用`keepdims=True`保持reduce操作的维度

4. **广播的优势**：
   - 代码简洁，避免显式循环
   - 内存高效，不实际复制数据
   - 计算高效，底层优化

5. **广播的局限性**：
   - 不兼容的形状无法广播
   - 需要理解广播规则，避免意外结果
   - 某些复杂广播可能难以理解

6. **实用技巧**：
   - 使用`reshape`或`np.newaxis`控制数组形状
   - 使用`keepdims`保持维度以便进一步广播
   - 调试时使用`broadcast_to`显式查看广播结果

7. **常见应用**：
   - 数据标准化/归一化
   - 距离计算
   - 多项式求值
   - 图像处理
   - 特征工程

**第十讲重点**：掌握NumPy的广播机制，理解广播规则，学会利用广播简化数组运算，注意广播的局限性，能够调试和优化广播操作。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第十一讲：花式索引与布尔索引

## 概念
花式索引（Fancy indexing）和布尔索引（Boolean indexing）是NumPy中两种强大的高级索引技术，它们允许我们使用整数数组或布尔数组来索引数组，从而实现复杂的数据选择和操作。

## 原理
1. **花式索引**：使用整数数组作为索引，可以选择任意顺序、任意数量的元素，甚至重复选择
2. **布尔索引**：使用布尔数组作为索引，选择满足条件的元素
3. **返回副本**：与基本切片不同，花式索引和布尔索引通常返回数据的副本而非视图
4. **多维索引**：支持多维数组的复杂索引操作
5. **广播结合**：可以与广播机制结合使用

## 例子
```python
import numpy as np

# 1. 一维数组的花式索引
print("=== 1. 一维数组的花式索引 ===")
arr = np.array([10, 20, 30, 40, 50, 60, 70, 80, 90, 100])
print("原始数组:", arr)

# 使用整数列表索引
indices = [2, 5, 7]
print("使用索引列表[2, 5, 7]:", arr[indices])

# 索引可以重复
indices_repeat = [0, 0, 1, 1, 2, 2, 3, 3]
print("重复索引[0,0,1,1,2,2,3,3]:", arr[indices_repeat])

# 索引可以改变顺序
indices_reorder = [9, 8, 7, 6, 5, 4, 3, 2, 1, 0]
print("反转索引:", arr[indices_reorder])

# 使用NumPy数组作为索引
indices_np = np.array([1, 3, 5, 7, 9])
print("使用NumPy数组索引:", arr[indices_np])

# 2. 二维数组的花式索引
print("\n=== 2. 二维数组的花式索引 ===")
arr_2d = np.arange(20).reshape(4, 5)
print("二维数组:\n", arr_2d)

# 选择多行
row_indices = [0, 2, 3]
print("选择第0,2,3行:\n", arr_2d[row_indices])

# 选择多列
col_indices = [1, 3, 4]
print("选择第1,3,4列:\n", arr_2d[:, col_indices])

# 选择特定元素
row_idx = np.array([0, 1, 2])
col_idx = np.array([1, 2, 3])
print("选择(0,1), (1,2), (2,3)元素:", arr_2d[row_idx, col_idx])

# 3. 花式索引的广播
print("\n=== 3. 花式索引的广播 ===")
# 行索引和列索引的广播
row_idx = np.array([[0], [1], [2]])  # 3x1
col_idx = np.array([0, 1, 2])        # 1x3
print("行索引形状:", row_idx.shape)
print("列索引形状:", col_idx.shape)
print("广播后选择:\n", arr_2d[row_idx, col_idx])

# 4. 花式索引赋值
print("\n=== 4. 花式索引赋值 ===")
arr = np.array([1, 2, 3, 4, 5])
indices = [0, 2, 4]
print("原始数组:", arr)
arr[indices] = 100
print("将索引[0,2,4]赋值为100:", arr)

# 二维数组赋值
arr_2d = np.zeros((3, 4))
rows = [0, 1, 2]
cols = [1, 2, 3]
arr_2d[rows, cols] = 1
print("二维数组赋值:\n", arr_2d)

# 5. 一维数组的布尔索引
print("\n=== 5. 一维数组的布尔索引 ===")
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
print("原始数组:", arr)

# 简单条件
mask = arr > 5
print("arr > 5 的布尔掩码:", mask)
print("arr[arr > 5]:", arr[arr > 5])

# 复合条件
mask_complex = (arr > 3) & (arr < 8)
print("3 < arr < 8:", arr[mask_complex])

mask_or = (arr < 3) | (arr > 8)
print("arr < 3 或 arr > 8:", arr[mask_or])

# 使用np.where
indices = np.where(arr > 5)
print("np.where(arr > 5) 索引:", indices)
print("通过索引获取:", arr[indices])

# 6. 二维数组的布尔索引
print("\n=== 6. 二维数组的布尔索引 ===")
arr_2d = np.array([[1, 2, 3],
                   [4, 5, 6],
                   [7, 8, 9]])
print("二维数组:\n", arr_2d)

# 对整个数组的条件
mask = arr_2d > 5
print("arr_2d > 5 的布尔掩码:\n", mask)
print("arr_2d[arr_2d > 5]:", arr_2d[arr_2d > 5])  # 返回一维数组

# 对行的条件
row_mask = np.array([True, False, True])
print("选择第0和第2行:\n", arr_2d[row_mask, :])

# 对列的条件
col_mask = np.array([True, False, True])
print("选择第0和第2列:\n", arr_2d[:, col_mask])

# 7. 使用np.where进行条件赋值
print("\n=== 7. 使用np.where进行条件赋值 ===")
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
print("原始数组:", arr)

# np.where(condition, x, y): condition为True时取x，否则取y
result = np.where(arr > 5, 100, 0)
print("大于5的置为100，否则置为0:", result)

# 更复杂的条件
result2 = np.where(arr % 2 == 0, arr**2, arr**3)
print("偶数平方，奇数立方:", result2)

# 8. 布尔索引赋值
print("\n=== 8. 布尔索引赋值 ===")
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
print("原始数组:", arr)

# 将大于5的元素置为0
arr[arr > 5] = 0
print("大于5的元素置为0:", arr)

# 恢复数组
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
arr[(arr > 3) & (arr < 8)] = 100
print("3到8之间的元素置为100:", arr)

# 9. 花式索引与布尔索引的组合
print("\n=== 9. 花式索引与布尔索引的组合 ===")
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

# 先布尔索引，再花式索引
even_numbers = arr[arr % 2 == 0]  # 布尔索引
print("偶数:", even_numbers)
print("第1和第3个偶数:", even_numbers[[0, 2]])  # 花式索引

# 或者一步完成
print("一步完成:", arr[arr % 2 == 0][[0, 2]])

# 10. 使用np.take和np.compress
print("\n=== 10. 使用np.take和np.compress ===")
arr = np.array([10, 20, 30, 40, 50])
indices = [0, 2, 4]

# np.take类似于花式索引
print("np.take(arr, [0,2,4]):", np.take(arr, indices))

# np.compress类似于布尔索引
mask = np.array([True, False, True, False, True])
print("np.compress(mask, arr):", np.compress(mask, arr))

# 11. 高级布尔索引技巧
print("\n=== 11. 高级布尔索引技巧 ===")
# 使用多个条件
arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
condition1 = arr > 3
condition2 = arr < 8
condition3 = arr % 2 == 0

# 组合条件
combined = condition1 & condition2 & condition3
print("3 < arr < 8 且为偶数:", arr[combined])

# 使用~取反
print("不满足combined条件的:", arr[~combined])

# 12. 结构化数据的布尔索引
print("\n=== 12. 结构化数据的布尔索引 ===")
# 创建结构化数组
names = np.array(['Alice', 'Bob', 'Charlie', 'David', 'Eve'])
ages = np.array([25, 30, 35, 40, 45])
scores = np.array([85, 92, 78, 88, 95])

# 基于年龄选择
young_mask = ages < 35
print("年龄小于35的人:", names[young_mask])
print("他们的分数:", scores[young_mask])

# 基于分数选择
high_score_mask = scores > 90
print("分数大于90的人:", names[high_score_mask])
print("他们的年龄:", ages[high_score_mask])

# 13. 性能比较
print("\n=== 13. 性能比较 ===")
import time

# 创建大型数组
large_arr = np.random.rand(1000000)

# 布尔索引性能
start = time.time()
result1 = large_arr[large_arr > 0.5]
bool_time = time.time() - start

# 先where再索引
start = time.time()
indices = np.where(large_arr > 0.5)
result2 = large_arr[indices]
where_time = time.time() - start

print(f"布尔索引时间: {bool_time:.6f}秒")
print(f"先where再索引时间: {where_time:.6f}秒")
print(f"结果是否相等: {np.array_equal(result1, result2)}")
print(f"哪种方法更快: {'布尔索引' if bool_time < where_time else 'where+索引'}")

# 14. 内存考虑
print("\n=== 14. 内存考虑 ===")
# 花式索引和布尔索引返回副本
arr = np.array([1, 2, 3, 4, 5])
indices = [0, 2, 4]
subset = arr[indices]  # 这是副本
subset[0] = 100
print("修改副本后，原始数组:", arr)  # 原始数组不变

# 与视图对比
view = arr[1:4]  # 这是视图
view[0] = 200
print("修改视图后，原始数组:", arr)  # 原始数组改变

# 15. 实际应用示例
print("\n=== 15. 实际应用示例 ===")
# 模拟学生数据
np.random.seed(42)
n_students = 20
students = {
    'id': np.arange(1001, 1001 + n_students),
    'name': np.array([f'Student{i}' for i in range(n_students)]),
    'score': np.random.randint(50, 101, n_students),
    'passed': np.random.choice([True, False], n_students, p=[0.7, 0.3])
}

# 找出分数高于90的学生
high_scorers = students['score'] > 90
print("分数高于90的学生ID:", students['id'][high_scorers])
print("分数高于90的学生姓名:", students['name'][high_scorers])
print("分数高于90的学生分数:", students['score'][high_scorers])

# 找出通过考试但分数低于60的学生
condition = (students['passed'] == True) & (students['score'] < 60)
if np.any(condition):
    print("\n通过但分数低于60的学生:")
    print("ID:", students['id'][condition])
    print("姓名:", students['name'][condition])
    print("分数:", students['score'][condition])
else:
    print("\n没有通过但分数低于60的学生")

# 计算平均分
mean_score = np.mean(students['score'])
print(f"\n所有学生平均分: {mean_score:.2f}")

# 找出高于平均分的学生
above_avg = students['score'] > mean_score
print(f"高于平均分({mean_score:.2f})的学生人数: {np.sum(above_avg)}")

# 16. 多维花式索引
print("\n=== 16. 多维花式索引 ===")
arr_2d = np.arange(12).reshape(3, 4)
print("二维数组:\n", arr_2d)

# 选择多个不连续区域
row_idx = np.array([0, 0, 2, 2])
col_idx = np.array([0, 3, 1, 2])
print("选择(0,0), (0,3), (2,1), (2,2):", arr_2d[row_idx, col_idx])

# 使用np.ix_创建网格索引
rows = np.array([0, 2])
cols = np.array([1, 3])
print("使用np.ix_选择:\n", arr_2d[np.ix_(rows, cols)])

# 17. 条件赋值的高级用法
print("\n=== 17. 条件赋值的高级用法 ===")
arr = np.array([[1, 2, 3],
                [4, 5, 6],
                [7, 8, 9]])

# 将大于5的元素替换为它们的负数
arr[arr > 5] = -arr[arr > 5]
print("大于5的元素替换为其负数:\n", arr)

# 18. 使用np.select进行多条件选择
print("\n=== 18. 使用np.select进行多条件选择 ===")
x = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

# 定义条件和对应的值
conditions = [
    x < 3,          # 条件1
    (x >= 3) & (x < 7),  # 条件2
    x >= 7          # 条件3
]

choices = [
    'small',        # 条件1满足时的值
    'medium',       # 条件2满足时的值
    'large'         # 条件3满足时的值
]

result = np.select(conditions, choices)
print("原始数组:", x)
print("分类结果:", result)
```

## 总结
1. **花式索引**：
   - 使用整数数组选择元素
   - 可以重复、乱序选择
   - 对于多维数组，可以同时指定行和列索引
   - 返回数据的副本

2. **布尔索引**：
   - 使用布尔数组选择满足条件的元素
   - 支持复合条件（使用`&`、`|`、`~`）
   - 可以使用比较运算符和逻辑函数创建布尔数组
   - 返回数据的副本

3. **重要函数**：
   - `np.where()`：根据条件返回元素或索引
   - `np.take()`：沿轴获取元素
   - `np.compress()`：使用布尔掩码选择元素
   - `np.select()`：多条件选择
   - `np.ix_()`：生成网格索引

4. **性能考虑**：
   - 布尔索引通常比先`np.where`再索引更快
   - 花式索引和布尔索引返回副本，有内存开销
   - 对于大数组，注意内存使用

5. **内存问题**：
   - 与基本切片返回视图不同，花式索引和布尔索引返回副本
   - 修改副本不会影响原始数组
   - 需要额外内存存储副本

6. **高级技巧**：
   - 可以组合使用花式索引和布尔索引
   - 可以使用`np.ix_()`创建网格索引
   - 可以使用`np.select()`进行多条件分类
   - 结构化数据的布尔索引

7. **常见应用**：
   - 数据清洗和过滤
   - 条件赋值
   - 数据抽样
   - 特征选择
   - 数据分类

**第十一讲重点**：掌握NumPy的花式索引和布尔索引，理解它们与基本切片的区别，学会使用这些高级索引技术进行复杂的数据选择和操作，注意性能优化和内存管理。

请理解吸收本讲内容后，告诉我"讲下一讲"继续学习。
# 第十二讲：结构化数组与文件IO

## 概念
**结构化数组**是NumPy中一种特殊的数组，它允许数组的每个元素具有多个字段，每个字段可以有不同的数据类型。这类似于C语言中的结构体或Pandas的DataFrame。

**文件IO**是指NumPy从磁盘文件读取数据或将数据写入磁盘文件的功能。NumPy支持多种文件格式，包括二进制格式和文本格式。

## 原理
1. **结构化数组原理**：
   - 通过定义dtype来指定每个字段的名称和数据类型
   - 内存布局是连续的，但允许混合数据类型
   - 可以像访问数组属性一样访问字段

2. **文件IO原理**：
   - 二进制格式（.npy, .npz）：高效、快速，保持数据类型和形状
   - 文本格式（.txt, .csv）：可读性好，但效率较低
   - 内存映射：大文件处理，不一次性加载到内存

## 例子
```python
import numpy as np
import os

# 1. 创建结构化数组
print("=== 1. 创建结构化数组 ===")

# 方法1：通过元组列表定义dtype
dt = np.dtype([('name', 'U10'), ('age', 'i4'), ('height', 'f4'), ('weight', 'f4')])
data = np.array([('Alice', 25, 1.65, 55.5),
                 ('Bob', 30, 1.80, 75.0),
                 ('Charlie', 35, 1.75, 70.0)], dtype=dt)
print("结构化数组:\n", data)
print("数据类型:", data.dtype)
print("字段名称:", data.dtype.names)

# 访问字段
print("\n姓名字段:", data['name'])
print("年龄字段:", data['age'])
print("身高字段:", data['height'])
print("体重字段:", data['weight'])

# 访问单个元素
print("\n第一个元素:", data[0])
print("第一个元素的姓名:", data[0]['name'])

# 方法2：通过字典定义dtype
dt2 = np.dtype({'names': ['id', 'score', 'grade'],
                'formats': ['i4', 'f4', 'U1']})
data2 = np.array([(1, 85.5, 'A'),
                  (2, 92.0, 'A'),
                  (3, 78.5, 'B')], dtype=dt2)
print("\n通过字典定义的结构化数组:\n", data2)

# 2. 结构化数组的操作
print("\n=== 2. 结构化数组的操作 ===")

# 索引和切片
print("前两个元素:\n", data[:2])
print("姓名以'A'开头的元素:\n", data[data['name'].astype(str).startswith('A')])

# 字段赋值
data['age'] = [26, 31, 36]
print("\n修改年龄后:\n", data)

# 添加新字段
new_dt = np.dtype(data.dtype.descr + [('bmi', 'f4')])
new_data = np.zeros(data.shape, dtype=new_dt)
for field in data.dtype.names:
    new_data[field] = data[field]
new_data['bmi'] = new_data['weight'] / (new_data['height'] ** 2)
print("\n添加BMI字段后:\n", new_data)

# 3. 嵌套结构化数组
print("\n=== 3. 嵌套结构化数组 ===")
nested_dt = np.dtype([('name', 'U10'),
                      ('address', [('street', 'U20'),
                                   ('city', 'U10'),
                                   ('zip', 'U10')]),
                      ('age', 'i4')])

nested_data = np.array([('Alice', ('123 Main St', 'Boston', '02115'), 25),
                        ('Bob', ('456 Elm St', 'NYC', '10001'), 30)],
                       dtype=nested_dt)
print("嵌套结构化数组:\n", nested_data)
print("地址字段:", nested_data['address'])
print("第一个人的城市:", nested_data[0]['address']['city'])

# 4. 记录数组（recarray）
print("\n=== 4. 记录数组（recarray） ===")
# recarray允许通过属性访问字段
rec_data = data.view(np.recarray)
print("记录数组:\n", rec_data)
print("通过属性访问姓名:", rec_data.name)
print("通过属性访问年龄:", rec_data.age)

# 5. 二进制文件IO（.npy格式）
print("\n=== 5. 二进制文件IO（.npy格式） ===")
# 创建一些数据
array1 = np.arange(12).reshape(3, 4)
array2 = np.array([1.0, 2.0, 3.0, 4.0, 5.0])

# 保存单个数组
np.save('array1.npy', array1)
print("保存 array1.npy 成功")

# 加载单个数组
loaded_array1 = np.load('array1.npy')
print("加载的数组:\n", loaded_array1)
print("与原始数组是否相等:", np.array_equal(array1, loaded_array1))

# 保存多个数组
np.savez('arrays.npz', arr1=array1, arr2=array2)
print("\n保存 arrays.npz 成功")

# 加载多个数组
loaded_arrays = np.load('arrays.npz')
print("npz文件中的键:", list(loaded_arrays.keys()))
print("arr1:\n", loaded_arrays['arr1'])
print("arr2:", loaded_arrays['arr2'])

# 压缩保存
np.savez_compressed('arrays_compressed.npz', arr1=array1, arr2=array2)
print("\n保存压缩文件 arrays_compressed.npz 成功")

# 6. 文本文件IO
print("\n=== 6. 文本文件IO ===")
# 创建一些数据
data_txt = np.array([[1.0, 2.0, 3.0],
                     [4.0, 5.0, 6.0],
                     [7.0, 8.0, 9.0]])

# 保存为文本文件
np.savetxt('data.txt', data_txt, delimiter=',', header='Col1,Col2,Col3', fmt='%.2f')
print("保存 data.txt 成功")

# 加载文本文件
loaded_data = np.loadtxt('data.txt', delimiter=',')
print("加载的文本数据:\n", loaded_data)

# 有缺失值的文本文件
with open('data_with_missing.txt', 'w') as f:
    f.write("1.0,2.0,3.0\n")
    f.write("4.0,,6.0\n")  # 缺失值
    f.write("7.0,8.0,9.0\n")

# 加载有缺失值的文件
loaded_missing = np.genfromtxt('data_with_missing.txt', delimiter=',', filling_values=0)
print("\n加载有缺失值的数据（用0填充）:\n", loaded_missing)

# 7. CSV文件处理
print("\n=== 7. CSV文件处理 ===")
# 创建结构化数据
csv_data = np.array([(1, 'Alice', 25, 1.65),
                     (2, 'Bob', 30, 1.80),
                     (3, 'Charlie', 35, 1.75)],
                    dtype=[('id', 'i4'), ('name', 'U10'), ('age', 'i4'), ('height', 'f4')])

# 保存为CSV
np.savetxt('people.csv', csv_data, delimiter=',',
           header='id,name,age,height',
           fmt=['%d', '%s', '%d', '%.2f'])
print("保存 people.csv 成功")

# 从CSV加载结构化数组
loaded_csv = np.genfromtxt('people.csv', delimiter=',', names=True, dtype=None, encoding='utf-8')
print("从CSV加载的结构化数组:\n", loaded_csv)
print("数据类型:", loaded_csv.dtype)

# 8. 内存映射文件
print("\n=== 8. 内存映射文件 ===")
# 创建一个大数组
large_array = np.random.randn(1000, 1000)
np.save('large_array.npy', large_array)

# 使用内存映射加载
mmap_array = np.load('large_array.npy', mmap_mode='r')
print("内存映射数组形状:", mmap_array.shape)
print("内存映射数组类型:", type(mmap_array))
print("访问第一行前5个元素:", mmap_array[0, :5])

# 9. 自定义二进制格式
print("\n=== 9. 自定义二进制格式 ===")
# 使用tofile和fromfile
array_bin = np.array([1.0, 2.0, 3.0, 4.0, 5.0], dtype='f4')
array_bin.tofile('data.bin')
print("保存 data.bin 成功")

# 从二进制文件加载
loaded_bin = np.fromfile('data.bin', dtype='f4')
print("从二进制文件加载:", loaded_bin)

# 10. 结构化数组的统计操作
print("\n=== 10. 结构化数组的统计操作 ===")
# 创建学生数据
students_dt = np.dtype([('name', 'U20'), ('score', 'f4'), ('grade', 'U1')])
students = np.array([('Alice', 85.5, 'A'),
                     ('Bob', 92.0, 'A'),
                     ('Charlie', 78.5, 'B'),
                     ('David', 65.0, 'C'),
                     ('Eve', 88.0, 'A')], dtype=students_dt)

print("学生数据:\n", students)
print("分数字段:", students['score'])
print("平均分:", np.mean(students['score']))
print("最高分:", np.max(students['score']))
print("最低分:", np.min(students['score']))
print("分数标准差:", np.std(students['score']))

# 按条件筛选
high_scorers = students[students['score'] > 80]
print("\n分数高于80的学生:\n", high_scorers)

# 11. 结构化数组的排序
print("\n=== 11. 结构化数组的排序 ===")
# 按分数排序
sorted_by_score = np.sort(students, order='score')
print("按分数升序排序:\n", sorted_by_score)

# 按分数降序排序
sorted_by_score_desc = np.sort(students, order='score')[::-1]
print("\n按分数降序排序:\n", sorted_by_score_desc)

# 多字段排序
students_with_id = np.array([(1, 'Alice', 85.5, 'A'),
                             (2, 'Bob', 92.0, 'A'),
                             (3, 'Charlie', 78.5, 'B'),
                             (4, 'David', 85.5, 'C'),
                             (5, 'Eve', 88.0, 'A')],
                            dtype=[('id', 'i4'), ('name', 'U20'), ('score', 'f4'), ('grade', 'U1')])

sorted_multi = np.sort(students_with_id, order=['score', 'id'])
print("\n按分数和ID排序:\n", sorted_multi)

# 12. 结构化数组的IO
print("\n=== 12. 结构化数组的IO ===")
# 保存结构化数组
np.save('students.npy', students)
print("保存 students.npy 成功")

# 加载结构化数组
loaded_students = np.load('students.npy')
print("加载的学生数据:\n", loaded_students)
print("数据类型:", loaded_students.dtype)

# 保存为文本
np.savetxt('students.csv', students, delimiter=',',
           header='name,score,grade',
           fmt=['%s', '%.1f', '%s'])
print("\n保存 students.csv 成功")

# 13. 高级结构化数组操作
print("\n=== 13. 高级结构化数组操作 ===")
# 创建两个结构化数组
dt1 = np.dtype([('id', 'i4'), ('value1', 'f4')])
dt2 = np.dtype([('id', 'i4'), ('value2', 'f4')])

arr1 = np.array([(1, 10.5), (2, 20.5), (3, 30.5)], dtype=dt1)
arr2 = np.array([(1, 100.5), (2, 200.5), (4, 400.5)], dtype=dt2)

print("数组1:\n", arr1)
print("数组2:\n", arr2)

# 使用np.lib.recfunctions进行合并
try:
    from numpy.lib import recfunctions as rfn
    
    # 连接
    concatenated = rfn.merge_arrays([arr1, arr2], flatten=True)
    print("\n合并数组:\n", concatenated)
    
    # 根据ID连接（类似数据库的JOIN）
    joined = rfn.join_by('id', arr1, arr2, jointype='inner')
    print("\n根据ID内连接:\n", joined)
    
except ImportError as e:
    print("recfunctions模块可能不可用:", e)

# 14. 性能比较
print("\n=== 14. 性能比较 ===")
import time

# 创建大型结构化数组
n = 1000000
big_struct = np.zeros(n, dtype=[('x', 'f8'), ('y', 'f8'), ('z', 'f8')])
big_struct['x'] = np.random.randn(n)
big_struct['y'] = np.random.randn(n)
big_struct['z'] = np.random.randn(n)

# 保存性能比较
start = time.time()
np.save('big_struct.npy', big_struct)
npy_time = time.time() - start
print(f"保存为.npy格式: {npy_time:.3f}秒")

start = time.time()
np.savetxt('big_struct.txt', big_struct, delimiter=',')
txt_time = time.time() - start
print(f"保存为.txt格式: {txt_time:.3f}秒")
print(f"二进制比文本快 {txt_time/npy_time:.1f}倍")

# 清理文件
os.remove('big_struct.npy')
os.remove('big_struct.txt')

# 15. 实际应用：处理表格数据
print("\n=== 15. 实际应用：处理表格数据 ===")
# 模拟销售数据
sales_dt = np.dtype([('date', 'U10'),
                     ('product', 'U20'),
                     ('quantity', 'i4'),
                     ('price', 'f4'),
                     ('region', 'U10')])

sales_data = np.array([('2023-01-01', 'Laptop', 5, 999.99, 'North'),
                       ('2023-01-01', 'Mouse', 20, 25.50, 'North'),
                       ('2023-01-02', 'Laptop', 3, 999.99, 'South'),
                       ('2023-01-02', 'Keyboard', 15, 45.00, 'South'),
                       ('2023-01-03', 'Monitor', 8, 299.99, 'East'),
                       ('2023-01-03', 'Laptop', 4, 999.99, 'West')],
                      dtype=sales_dt)

print("销售数据:\n", sales_data)

# 计算总销售额
sales_data['revenue'] = sales_data['quantity'] * sales_data['price']
print("\n添加收入字段后:\n", sales_data)

# 按地区统计
regions = np.unique(sales_data['region'])
for region in regions:
    region_mask = sales_data['region'] == region
    region_revenue = np.sum(sales_data[region_mask]['revenue'])
    print(f"{region}地区总收入: ${region_revenue:.2f}")

# 按产品统计
products = np.unique(sales_data['product'])
for product in products:
    product_mask = sales_data['product'] == product
    product_quantity = np.sum(sales_data[product_mask]['quantity'])
    product_revenue = np.sum(sales_data[product_mask]['revenue'])
    print(f"{product}: 销量={product_quantity}, 收入=${product_revenue:.2f}")

# 16. 清理临时文件
print("\n=== 16. 清理临时文件 ===")
files_to_remove = ['array1.npy', 'arrays.npz', 'arrays_compressed.npz',
                   'data.txt', 'data_with_missing.txt', 'people.csv',
                   'large_array.npy', 'data.bin', 'students.npy', 'students.csv']

for file in files_to_remove:
    if os.path.exists(file):
        os.remove(file)
        print(f"已删除: {file}")
```

## 总结
1. **结构化数组**：
   - 允许数组元素有多个字段，每个字段可以有不同的数据类型
   - 通过`dtype`定义结构
   - 可以使用字段名访问数据
   - 支持嵌套结构

2. **记录数组（recarray）**：
   - 结构化数组的变体
   - 可以通过属性访问字段

3. **文件IO**：
   - 二进制格式（.npy, .npz）：高效，保持数据类型
   - 文本格式（.txt, .csv）：可读性好
   - 内存映射：处理大文件
   - 自定义二进制格式：`tofile()`和`fromfile()`

4. **常用函数**：
   - 保存：`np.save()`, `np.savez()`, `np.savetxt()`
   - 加载：`np.load()`, `np.loadtxt()`, `np.genfromtxt()`
   - 结构化数组：`np.sort()`（支持`order`参数）

5. **性能考虑**：
   - 二进制格式比文本格式快得多
   - 对于大文件，使用内存映射
   - 结构化数组可以提高数据访问效率

6. **应用场景**：
   - 处理表格数据
   - 数据库操作
   - 科学数据存储
   - 配置文件

7. **注意事项**：
   - 结构化数组的字段名必须是有效的Python标识符
   - 文本格式会丢失数据类型信息
   - 二进制格式不可跨平台（但通常可以）

**第十二讲重点**：掌握结构化数组的创建和操作，理解不同文件格式的特点和适用场景，能够根据需求选择合适的文件IO方法，学会处理混合类型的数据。

**NumPy系统教程到此结束**。通过这12讲的学习，您应该已经掌握了NumPy的核心功能。实际应用中，可能需要结合具体问题选择合适的方法。建议通过实际项目来巩固所学知识。