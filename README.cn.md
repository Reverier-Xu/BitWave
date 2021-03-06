# ![Bit Wave](src/resources/assets/logo-full.svg)

建立在 MPV, Qt 与 ~~Microsoft Fluent Design System~~ 之上的音视频播放器。

Fluent Design 已经一股果味了（悲），我要捍卫大直角设计！

## 开始

### Arch Linux

aur/bitwave-git

```
paru -S bitwave-git
```

### other platforms

WIP

## 状态

- 理论上 MPV 能放的这里都能放，不过部分格式偷懒没写（XD
- 视频播放。
- 歌词（网易云的）。
- 可记忆的播放队列。
- 逆序播放。
- 支持获取媒体的 metadata 与封面图片，能够自动探测封面图片的主色调。
- 好看的界面。
- MPRIS2 集成。
- 播放视频时禁止熄屏。

## 未完成的功能

- 在线视频 / 在线歌曲
- 播放列表
- 播放列表 / 媒体库管理
- 搜索
- 音频均衡器（？）
- Osu！播放器（？）

## 不打算支持的功能

评论、私信、MV、点赞、交友、圈子、K歌等等等一切与听歌看视频没关系的额外功能。（媒体播放器就要干媒体播放器的事情啊喂）

## 云服务

本项目支持了一些云服务，包括：

- 获取在线歌词（已实现，来源：网易云）
- 获取在线播放列表（待实现）
- 在线搜索（待实现）
- 弹幕/字幕（待实现）
- 播放在线媒体（待实现）

**对在线音乐/视频厂商的API支持仅作学习使用，请勿用于商业/非法途径！在使用到在线服务时，请遵循对应厂商的用户协议与各类规则！**

本项目不对这些API是否能够正常工作负责，如果这些API发生更改或者移动/删除，作者会酌情跟进或者直接移除本播放器内的相关功能。

## Developing

开发环境需求:
```
$ sudo pacman -Syu mpv qt5 ffmpeg cmake extra-cmake-modules
```

在其他发行版上安装对应的包也可以完成构建。

其中，Qt版本要求在5.15以上，否则可能缺失特性/无法构建成功。

古早Ubuntu或者UOS用户可以等待本项目Release后使用AppImage版本（逃

```
git clone https://github.com/Reverier-Xu/BitWave.git && cd BitWave

mkdir build && cd build

cmake .. && make

./bin/BitWave
```

## ~~Windows~~ 目前不提供Windows版本的构建支持，如有需要请自行移植

需要Visual Studio 2019， MSVC 2017，Qt 5.15.2+

将`libmpv`放置于`3rd/`下，同时保证能够使用pkgconfig与ffmpeg，相关lib请自行下载。

```
|- 3rd
| |- libmpv
| | |- mpv-1.dll
| | |- libmpv.dll.a
| | -- ...
| |
| -- QtAES ...
|
|- ...
```

如果你有想法的话，可以自行添加Windows构建支持，然后对本仓库发起一个Pull Request（发出白嫖的声音，逃）

## 一些截图

![image.png](https://i.loli.net/2021/08/30/9B5DjOXR8uHKAme.png)

![image.png](https://i.loli.net/2021/08/30/2t74vKisSfbJl9o.png)

![image.png](https://i.loli.net/2021/08/30/ELjMKmfZVQN5sUJ.png)

![image.png](https://i.loli.net/2021/08/30/Hun5ckty1lDRGZB.png)

![image.png](https://i.loli.net/2021/08/30/AfJnXCY9ztcUqrg.png)

## License

Mozilla Public License v2.0

精力有限，随缘提供技术支持，随缘修bug、移植，欢迎有新点子的开发者对本仓库提交issue、fork、修改、提交pr。
