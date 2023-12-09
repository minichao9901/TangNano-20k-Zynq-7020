# TangNano-20K与Zynq-7020通用扩展板
# 1)简介
* 这是一款基于TangNano-20k和微相科技Z7-Nano的扩展板，可以同时兼容这2款开发板，并且是一个通用的扩展板
* 通用性体现在：可以兼容市面上<=40PIN的几乎所有的外设模块，包括但不限于：数码管，ADC/DAC，正点原子/野火的LCD屏，各种fpc接口裸屏，RGB接口摄像头等
* 它是如何解决不同模块之间引脚/电源分配差异巨大的呢？其实核心有2点：1)引脚的灵活分配是FPGA自有属性；2)电源/地的分配，是利用了跳线帽。
* 我们的40PIN接口上，每个引脚都可以有3种模式：由FPGA驱动，或者接3V3，或者接GND。当该引脚是信号时，那么就由FPGA驱动；
  当该引脚是VCC/GND时，那么就用跳线帽接3V3/GND。这样既解决了引脚的可编程，又解决了电源的可编程(用跳线帽)
* 杜邦线是非常令人讨厌又不得不接受的。利用该扩展板，我们不需要一根杜邦线，成功驱动了几十个不同类型的大大小小模块，彻底解决了这个痛点问题。

## 以下是2款开发板
<img width="442" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/bca0b759-68d9-4115-9a3c-caf27e82e1f6">
<img width="442" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/665ea163-831a-48eb-8905-70fa578c399b">


# 2)原理图与PCB工程
## 原理图与PCB功能
请参考上传的文件

## 3D效果图
![屏幕截图 2023-12-09 075042](https://github.com/minichao9901/fpga_verilog/assets/61445559/1f8073f1-1d1c-4417-be6b-fdc8bbd8c801)

## 实物效果图
<img width="429" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/56d4f2ef-cba5-49f6-9812-319c335ab7b8">
<img width="429" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/fcb59e60-14ec-4df5-862e-23a2abb4eb60">


# 3)用法
## 举例1：TangNano 20K驱动st7796 3.5寸LCD屏, 8080 16bits接口
### st7796 40pin fpc接口介绍
![image](https://github.com/minichao9901/blog/assets/61445559/19202ac1-0847-4726-b752-050f4c2fd445)
![image](https://github.com/minichao9901/blog/assets/61445559/647731da-e7d4-4eb1-b200-60001e227159)

### 引脚分配
* 我们选择mcu 16bits接口模式，因为im[2:0]要设置为3'b010
* 用的是我自己做的FPGA扩展板。将扩展板的fpc接口列在excel表里，再将模组的excel表列在excel表里一一对应，然后用excel的公式自动生成引脚约束文件，这样免去了手工抄写的繁琐和易错。
* 电源/地，用跳线帽接好：如图所示4/5/25/36脚接GND，8/34/35接3V3   
![image](https://github.com/minichao9901/blog/assets/61445559/bee32fef-64b1-41cd-aba0-c8193fd36190)
<img width="510" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/cf773382-aa87-4e3a-95f6-d399763b9532">

### 效果
<img width="515" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/11b00732-4f7a-461a-8913-59cff6e4855b">
<img width="413" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/5feac762-c032-4418-add4-f2453007fd5d">


## 举例2：Z7-Nano驱动st7796 3.5寸LCD屏, spi 4wire接口
### 引脚分配
* 我们选择4wire spi接口模式，因为im[2:0]要设置为3'b111
* 用的是我自己做的FPGA扩展板。将扩展板的fpc接口列在excel表里，再将模组的excel表列在excel表里一一对应，然后用excel的公式自动生成引脚约束文件，这样免去了手工抄写的繁琐和易错。
* 电源/地，用跳线帽接好：如图所示4/5/25/36脚接GND，8/34/35接3V3
* 如果用TangNano 20k驱动，使用如下引脚约束

![image](https://github.com/minichao9901/blog/assets/61445559/4201008f-ab2a-4f40-870a-96cf965fb49e)
* 如果使用Z7-Nano驱动，使用如下引脚约束
  
<img width="741" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/a2db0af4-fc00-4451-b58b-6122a55aac0f">

###  效果图
<img width="400" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/04a967bc-0c01-4265-ab89-894bb24c9bb6">


## 举例3：TangNano 20K驱动正点原子4.3寸LCD屏, RGB接口
### 野火与原子的引脚排布如下（原子使用反向fpc排线，野火使用同向fpc排线）
![image](https://github.com/minichao9901/blog/assets/61445559/c58f0f91-7abe-4ca3-a6e8-0d889319ceb1)

### 效果图
<img width="400" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/b9b66bac-5e66-43f1-9aab-03fac11eee47">
<img width="300" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/3d7460be-000d-4c24-af30-0f5ebd4723ff">

## 举例4：TangNano 20K驱动数码管(淘宝买的)
这个数码管排针竟然是朝上的，太扯蛋了，搞的我们没有办法直接插到40PIN的扩展排母接口中，只能插灰排线。
<img width="209" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/679a431f-dd26-46db-9ddf-6b804a3dc5c1">
<img width="142" alt="image" src="https://github.com/minichao9901/fpga_verilog/assets/61445559/bcf39810-3f9d-4cf1-930f-bba7a79bba53">



