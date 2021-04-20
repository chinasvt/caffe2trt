# Caffe2TRT

欢迎使用 **caffe2trt**，您可以通过 **caffe2trt** 很方便将 **caffemodel** 转换为 **trt engine**，本仓库的代码基本都是来自 **[tensorrt](https://github.com/NVIDIA/TensorRT.git)** 官方仓库。

# Support Layer

TensorRT 官方目前支持Layer，见[链接](https://docs.nvidia.com/deeplearning/tensorrt/support-matrix/index.html)

| Layer | FP32 | FP16 | INT8 |
| ----- | ---- | ---- | ---- |
| Slice |  yes  |  yes  |  yes  |
| InstanceNorm |  yes |  yes  |  yes  |


# Support Model

**Model Performance**

| Model          | Original Model | TRT FP32 | TRT FP16 | TRT INT8 |     |
| -------------- | -------------- | -------- | -------- | -------- | --- |
| ResNet50-IBN-a | 77.118/93.572  | 77.118/93.572 | 77.142/93.574 | 75.676/92.858 | top1/top5 |

**Model Latency**

模型推理时间均是BatchSize 32，且推理1000次的平均前向时间/平均前向时间+预处理, 即T/1000, 单位ms

| Model          | Input Size | Batch | Original Model | TRT FP32 | TRT FP16 | TRT INT8 |  Device   |
| -------------- | ---------- | ----- | -------------- | -------- | -------- | -------- | --- |
| ResNet50-IBN-a |  3x224x224 |   32  | 103.44/106.65  | 72.71/74.81 | 24.24/25.73 | 17.83/19.50 |  Tesla T4 |

# Build 
 **Dependencies**
 * [CUDA](https://developer.nvidia.com/cuda-toolkit)
    * Recommended versions:
    * cuda-11.x + cuDNN-8.1
    * cuda-10.2 + cuDNN-8.1
 * TensorRT>=5.1.5.0
 * glog, gflags
 * OpenCV>=3.4.2(load image)

**Compile**
  
``` shell
git clone https://github.com/chinasvt/caffe2trt.git
cd caffe2trt
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
```

**Usage**

``` shell
./caffe2trt  \
--model=${model} \     #设置caffemodel文件路径
--deploy=${deploy} \    #设置prototxt文件路径
--engine=${engine} \    #设置输出引擎路径
--input=${input} \     #设置输入节点名字
--output=${output} \    #设置输出节点名字
--batch=${batch} \     #设置batch大小
--dataType=${dataType} \ #设置转换精度，支持FP32、FP16、INT8
--workspace=$4 \      #分配转换时的显存大小
--device=$3          #设置GPU Id 
```

