
![preview](/Progress_notes/preview0710.png)

# nba2k-shotchart-project

obtaining game data from nba2k11 to achieve further visualizations.

as Dr. Drew has always said, Everything is a number!

See results in the following links

[Plot for random shootout](https://yo1995.github.io/nba2k-shotchart-project/temp-plot.html)

[Plot for MyPlayer MJ](https://yo1995.github.io/nba2k-shotchart-project/jordan-plot.html)

binary file in *release* section might be outdated sometimes. you could always retrieve latest version in TrainerRelease folder, or compile it yourself.

## Description

This project aims to provide several interesting add-on features for NBA2K11 game.
NBA2K11 is a basketball video game which was published in late 2010 and ingeniously provided the first ever next-gen Michael Jordan game mode.

The tool specifically designed for the 1.0.1.1 version of the game, and all the memory addresses are obtained with cheatengine corresponding with this specific version. Hence before use please check your game executable “nba2k11.exe” to ensure the compatibility.

![result](/Progress_notes/nba2k11-2018-04-23-19-38-03-56.png)

The tool has multiple features, including:

-	F5 RecordMode: All – for practice > freestyle and my player > play now > shoot around.
-	F5 RecordMode: MJ mode – for Michael Jordan > play now > next game.
-	F6 Record: On – to record the data from game or practice. Related with F5 status
-	F7 GodMode: On - makes all the shots on court score – even if the shot is made in backcourt!
-	F8 – to toggle the elements shown in game.
-	Anticipated accuracy – shows the predicted percentage of made shot, so that you could have better knowledge about your shooting choice and release tempo.
-	Anticipated basket type – shows the type of contact of the ball and the rim. Could be air ball/touch the rim/clean shot/hit the board, etc.
-	Score or not – shows if the shot will score or not.
-	In-game shot chart – displays the position of your previous shot in the game on a half court diagram.
-	Game data save to local disk – when F6 is enabled, all the in game data including GameTrack shot spots, all the basic stats in game and game date will be save to a .csv file under your NBA2K11 Saves folder. If the Saves folder is not found, then the .csv file will be saved at your Desktop folder.
-	pause when minimized. pop up a toast in notification center to notify the user.

## Versions

### 20180706

- added first version. works fine with mine.

### 20180726

- changed to findpattern method. hardcoded "C:\\WINDOWS\\system32\\d3d9.dll" and "\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86" into my dll. if any problem occurs, PLEASE ISSUE IT!

- added notification center support.

- more to see in commit log and release log. many thing improved!

### 20190202

解决了全明星赛的寻址问题。由于这学期时间紧张，打游戏的进度基本和赛季进程相当，直到现在才发现这个问题。具体参见issue #3

主要原因是全明星赛新秀赛每队只有10人，而常规比赛固定12人不变。这就造成了球员数据结构体和球员名单结构体的序号错位。

解决方法直接读取球员数据结构体中指向名单的指针，从而确保球员对应的球队和姓名信息时准确的。

在解决这个问题的过程中还发现当前的球队判别机制有问题——无法识别全明星赛和其他混合球员的比赛。当前会将全明星名单中的控卫所在球队当做对阵双方。

## Usage

0. make sure the DirectX libraries are properly configured in your system.

1. put the d3d9.dll and court.png under the directory of "nba2k11.exe" executable; also make sure directory "C:\\temp\\" exists so that d3d hook log file can output

2. run the game, the dashboard will pop up about 10 seconds after the game running. do not click the message box if in full screen mode, or the game might crash!

3. use previously mentioned F5 F6 F7 and F8 keys to toggle the features.

4. (advanced) run the Python script to obtain a visualized shot chart with plot.ly. configure at your own wish!

5. (advanced) i have included a *REALLY NASTY* REDitor II turk script to reduce the roster age a little bit. you could understand when you read it ;-). REDitor II(ver 4.8) required. or you could just use [the tweaked ROSTER file](/Resources/Rosteryoung.ROS) included!

## More to add

- ~~obtain player ID and link it with shot data. ~~

- ~~更改延迟启动逻辑~~ 更：修复了hook方式，不再存在延迟启动的问题。

- 从配置文件读取数据存储目录 - ~~读取注册表存入存档目录~~

- 改进显示顺序

- 改进显示位置

![preview](/Progress_notes/diff.png)

![diff](/Progress_notes/2k-plan.png)

根据不完全对比，游戏中绘制投篮点的场地背景图比例与正规场地不同。半场图比正常的短，而全场图比正常的长（通过截图来看似乎游戏自己画的图都不准🤣）。因此绘制点会存在漂移问题。但对于实际投篮点影响不大。因此游戏内实时投篮点绘制的功能可能会保持不完善的状态了……

- ~~更新Python绘图代码~~

- 整合钩子和修改器代码，汇总为一个项目

- ~~暂停时halt减少资源占用~~

- ~~修复全屏alt tab跳出时崩溃的问题（可能与lost device有关）https://www.unknowncheats.me/forum/c-and-c-/256699-alt-tab-support-directx.html~~ 更：通过reset修复

- ~~alt tab 切出全屏模式~~ 更：在切出前判断是否为全屏模式，从而正确弹出通知

- 改进头文件结构，清理无用变量函数

- 改进代码风格，整理各种杂乱的写法

- 增加真实/比赛日期记录

- 研究为什么弹出通知中心时会在Windows菜单中新建快捷方式 =^=

- 当前会将全明星名单中的控卫所在球队当做对阵双方

- 当前启用回放模式时会造成卡顿

## Keywords

tool | D3D Hook | Trainer | NBA2K | nba2k11 | mod | visualization | NBA2K 投篮热区

## Refs

I've exported the links that I have referred to. please check below.

[Bookmarks of all the ref links](https://yo1995.github.io/nba2k-shotchart-project/bookmarks_2018_7_10.html)

## Credits

- [minicsv.h](https://github.com/shaovoon/minicsv)

for providing me a really handy tool to export shot data for further visualization.

- [WinToast](https://github.com/mohabouje/WinToast)

for notification message.

- [hackbase](https://github.com/adstep/HackBaseFix2)

for giving me significant insights on DirectX hooking.

- [hack-template](https://github.com/LegendL3n/Hack-Template)

for giving me a clear structure to design my own tool.

- [DX11Overlay](https://github.com/stevemk14ebr/DX11Overlay)

for providing me first thoughts about the idea of D3D hooks.

- [detourxs](https://github.com/DominicTobias/detourxs)

for implementation of the hook.

- [Script Hook V](http://www.dev-c.com/gtav/scripthookv/)

for thoughts on how to deal with keyboard message and menu design.

- [Extreme Injector v3](https://github.com/master131/ExtremeInjector)

for injecting my primary in-dev dlls to test the features.

- [AhrixSpie](https://github.com/Famous02/AhrixSpie)

for ref to reset hooking. really well structured code for d3d9 hooking & hacking.

---

## 具体实现介绍

[博文](https://yo1995.github.io/coding/nba2k11-shotchart-project)

Also inspired by https://github.com/datadesk/kobe-every-shot-ever

### DirectX Hook

第一部分内容是如何在全屏游戏中绘制自定义图形。

- 32位
- D3D9钩子已完成
- D3D10与D3D11钩子大部分完成。

想在全屏模式下的游戏中绘制自己的图形，叠加透明窗口、切换置顶模式等常规方法行不通。

查阅资料后发现，通常的做法便是对游戏的DirectX相关函数挂钩，将所需的绘制代码插入运行的程序中，再卸掉钩子即可。

什么是挂钩？钩子函数是Windows消息处理机制的一部分。简言之，程序一步步执行，而我们想办法把我们的一个步骤加到众多步骤当中，并与前后衔接得当，便能神不知鬼不觉地实现我们的功能。

#### Vtable、Detour

说到底，程序就是数字。我们要做的就是把一串数找对地方放进去。自然，挂钩的选择也有很多。挂在开头，挂在中间，挂在结尾。最常见的想法有几种：直接把原来的函数改成我们所需要的；在原来的函数中间拴根小绳拉到我们的代码片段上；把原函数整个重写并加入我们的功能等等。

最通用也最容易实现的方法便是Present/EndScene Hook了。顾名思义，要么是在源程序开始绘制前绘制，要么是在绘制完成后绘制。写自己的函数，让原函数无路可走。创建一个device然后得到其Vtable，从而找到EndScene所在位置。detour到我么的函数上，Voila！

对于DX9而言，主要两步：

```c
pD3D9->CreateDevice
pD3D9EndScene = DetourFunc
```

具体参考程序。

### D3D Draw Functions

![茶壶](/Progress_notes/mesh_teapot.png)

调用DX原生的绘制API，主要有画线画字画图片。当然如果愿意，还可以一键绘制茶壶（D3DXCreateTeapot）

主要是为了制作类似于dashboard形式的显示功能，因此只需要简单的形状和文字即可。

D3DXCreateLine
D3DXCreateSprite
D3DXCreateFont
D3DXCreateTextureFromFile

#### In-game shot-chart

本来想实现实时的投篮点图显示，但后来考虑到那样需要把大量投篮点数据维护在内存中，对我来说没有太大必要。而且每次绘制屏幕如果需要遍历投篮点数组，可能会~~肯定不会~~影响到绘制的性能。这个功能因此烂尾了，目前只是显示上一次投篮点位置与信息。

### Memcpy

第二部分内容是如何读取/写入内存以获得/调整数据。

从最开始的金山游侠、八门神器到现在的cheatengine，手动查找内存地址并修改数据并不困难。从最基础的基址修改，到指针修改、多级指针、汇编注入等等，CE都能实现。具体可以参考[cheatengine练习](https://blog.csdn.net/xfgryujk/article/details/50067271)这篇博文。

对于本项目而言，与内存有关的操作主要有几类：
- 实时读取内存更新数据的；
- 按键触发读取内存更新数据的；
- 按键触发读写内存改变汇编代码的。

为了尽可能避免读写锁的使用，我想办法借助1字节变量的特性模仿了锁的逻辑，但代价便是使用了大量全局变量来实现。

#### assembly

![bytedance](/Progress_notes/assembly/bytedance.gif)

最开始，为了理清关键函数的逻辑，我阅读了大量的汇编代码。最终大致了解了NBA2K11里投篮判定的基本思路：

1. 记录键盘投篮键按下的时间
2. 记录键盘投篮键抬起的时间，同时记录当前篮球所处的位置坐标，并更新所有与投篮时间与位置有关的数据
3. 同时，给出当前投篮命中概率预测，并判定球与篮筐接触的类型以及是否命中。也就是说，球出手的一霎那是否命中、如何命中都已经决定了。这倒是和真实世界中一致
4. 当球与篮筐接触发生后，更新对应球员的出场数据表格

在这套逻辑中，有几个比较困难的问题。

第一是如何判定扣篮。在游戏中，扣篮时坐标不会发生变化，仍旧保持着上一次投篮的坐标点。如果游戏中存在出手类型判定的变量，那么这个问题一定迎刃而解。但或许是我没有找全所有有用的地址，因而只能从已知的地址入手来处理这些数据。我对扣篮的判定是：如果Z键释放且出手时间戳更新，但位置坐标没有更新的话，则为一次扣篮。后来在此基础上进一步完善了对于防守时抢断也需按投篮键，以及假动作虚晃时按投篮键的判断。

然而，直到最后，我发现之前的想法存在漏洞。其原因便在于第二个问题：游戏中判定投篮、记录投篮坐标的函数是不分主客队、球员ID的。也就是说，假如我不知道球员对应的ID，那么当别人出手时我恰巧按下Z键，则会把这次投篮记到我的头上。这对我来说是不可接受的。因此，暂时只好把投篮记录模式分为两种，一种是训练场单人投篮的记录，另一种是比赛中基于球员ID的记录。

第三个问题是如何确定出手球员。由于游戏自带的GameTrack存储的数据是与出手球员相关的，因此肯定有储存球员ID的字段。但是没有精力去看汇编代码来找到对应的地址了。如果以后有机会再添加吧！

#### cheatengine

寻找地址是一项费时费力的工作。如果像无头苍蝇一样四处乱撞，肯定没法得到太多有效数据。我没有太多经验，只有两个笨办法：第一是多思考——如果这个数据结构由我来设计，我会如何存储这些数据、存储哪些数据？第二是多找找——一般有关的数据地址相距不远，上下翻找一下没准能得到惊喜。

以球场坐标系为例。最开始我设想的是以中圈为原点，篮筐方向为y轴的坐标系，反复投篮来逐渐模糊搜索坐标。通过这种方法确实找到了篮球离筐距离和x坐标，但发现x坐标是左正右负的。上下翻找地址后发现了y坐标，试了几次后发现原来坐标系是以广播视角建立的：篮筐连线为x轴，中场线为y轴，单位是厘米。

![opcode](/Progress_notes/assembly/opcode.png)

另一个便是投篮成功率。发现锁定投篮命中率为1后仍旧不能保证必中。读了如上的代码后发现了判定命中的标准，干脆把不中改为必中，从而实现了投篮命中的功能。

180719 关于主客场地址差别的问题

![jordan name](/Progress_notes/jordan.png)

最开始写程序的时候完全疏忽了这一点，一直相当然地认为对应乔丹模式的球员数据结构体应该只有一个地址。结果打完第三场比赛发现，原来根本不是自己想象的那样啊😂

重新搜索内存一番后发现，原来球员数据的结构是按照比赛时主客队分别12名球员进行初始化的。也就是说，以主队PG开始，每名球员有长度为0x43C的内存空间存储其得分、篮板、助攻等数据。起始地址为一固定的基址0x05C2E14F。于是，按照这个结构重新设计了代码。

接下来又遇到一个问题：如何确定乔丹的地址呢？一般而言，乔丹是球队首发，打的位置几乎总是SG，但当球队内首发SF受伤时有时教练会安排乔丹替补；也不排除去某些球队乔丹客串首发SF的可能性。总之，不能一概而论。因此，找到一种绝对稳妥的确定乔丹对应数据的方法是非常有必要的。

首先我猜测，结构体里一定有对应球员ID的数值。找找吧，翻来翻去貌似确实有两个值跟球员有关，但对从80年代到90年代的不同乔丹值并未保持一致。接着猜测，总该有个指针指向球员姓名，翻来找去终于找到了，是在每个球员基址+0x11的位置的一个指针，指向另一个以0x05E4A538为基址，以B58为偏移量的球员信息结构体。巧合的是，这个结构体与球员数据结构体顺序上一一对应，也免去了为了得到姓名需要双重指针的麻烦。当然也在意料之中：倘若自己设计，也会把球员信息和球员数据写死一一对应的，这样方便调用。

于是问题变成了如何读取字符型指针并进行比较。其中查找了wchar_t、MessageBoxW、wcsncmp等用法，以及关于更新地址读取内存的逻辑优化，看着满屏幕的全局变量有点后悔……

最后实现的是通过遍历24名球员信息结构体找到姓名匹配Michael Jordan的球员，将他的得分地址写入全局变量PTS_ADDR即可。

#### toggle logics

由于缺少对源程序的理解，只能自己想办法判定投篮情况。程序里有不少逻辑不够完善，仍有提升空间。

### Keyboard

第三部分内容是读取键盘按键。修改器切换功能，读写内存都少不了键盘控制。查找资料了解到主要有键盘钩子和获取键位状态两种方法。

#### KeyboardHookLL

把底层键盘钩子挂到消息处理循环中，从而获取键位。这个方法虽然可以用，但总会引起防护软件的警觉，而且与我的修改器线程的循环逻辑不够匹配，最终放弃了。

#### GetAsyncKeyState

循环读取键盘动作。

### Data serialization

C++的文件操作不如Python或者MATLAB那么方便。为了偷懒，使用了[minicsv.h](https://github.com/shaovoon/minicsv)来实现数据写入硬盘。其实就是把fstream封装一下，但自己写感觉又是一个项目了，索性用现成的了。

### Injection

最开始用exe加透明窗的时候，还不知道注入是什么意思。后来把程序编译为dll，才知道需要把dll注入到母程序中，才能创建子线程实现我所需的功能。使用了[Extreme Injector v3](https://github.com/master131/ExtremeInjector)来实现dll注入。

#### d3d9.dll hijacking

![hijack](/Progress_notes/assembly/201806271407.png)

以前用过一些修改器，对目录下的dsound.dll/d3d9.dll/dinput8.dll有所耳闻。像fxaa就使用了d3d9.dll，而GTA历代修改器都经常会使用dinput8.dll，但过去不知道它们是如何实现的。查阅资料得知，借助一种称为“dll劫持”的方法可以实现自动注入代码的需求。其原理是Windows在调用dll时如未指明绝对路径，遵循先搜索当前目录再搜索系统目录的规则，因而可以把dll放在程序所在目录下，即可先调用我们的dll。之后在此dll中再调用原程序需要的函数即可。

首先通过procmon或是exescope找到源程序启动时都调了哪些库，然后找到其中包含方法或函数最少的库来对应实现即可。一般来讲，对于比较老的游戏，d3d9.dll和dinput8.dll是比较稳妥的选择。在NBA2K11中，d3d9.dll只调用了Direct3DCreate9一个函数，因此只需按照原定义实现此函数即可。需要注意的是，启动时该函数被调用了很多次，因此不能立刻注入我们的代码，否则无法成功挂上钩子。目前采用的方法是延迟启动，未来有待改进。

### Misc

几乎在我完成项目时才发现一个非常有用的[教程](https://blog.csdn.net/xfgryujk/article/category/5956585)。感叹要是早些看到的话也能少走点弯路，不过在走弯路过程中也收获颇丰。

## 180803更新

### 日期记录

每次打完比赛后需要手动记录对阵极为不优雅，于是走上了寻找对阵和日期的道路。

最先想到的方法当然还是搜索字符串。幸运的是，通过搜索timberwolves这个长关键字，我成功定位了几个读取该字符串的函数。然而，这些函数似乎和游戏进程关系不大。

其次想到的方法是映射球队ID。[得到了](./Progress_notes/teamIDs.txt)nba2k11的球队ID表，甚至成功找到了快速游戏和练习模式等不依赖于其他赛前状态的模式中，对阵ID的地址。然而，在MP和王朝模式中，由于引入了日历这一概念，我发现这些地址只在读取游戏存档后第一次保持更新，之后便不再改变；同时，在季后赛模式中读取的数值并不对应球队ID，只得作罢。

然后想到能否另辟蹊径，通过比赛日期找到比赛对阵ID？结果发现，球员dashboard中NBA Today和Schedule选项卡确实是依赖时间的，每次载入都会读取（游戏中）当前日期。然而，比赛本身似乎更依赖场次数。或许存档文件中存在日期与比赛ID的映射？

感到失落之际，想到球员本身应该是依赖球队ID的，这样才能成功将主客队各自12名出场球员成功从名单中找出。于是在球员结构体附近转来转去，终于还是找到了球队名称字段。Voila！写入数据文件。

## 191220 Hardlink notes

I've move the files to another volume, thus the original hardlink mechanism does not work anymore. I need to switch to shell copy solution instead.

