# ![Bit Wave](src/resources/assets/logo-full.svg)

建立在 MPV, Qt 与 ~~Microsoft Fluent Design System~~ 之上的音视频播放器。

Fluent Design 已经一股果味了（悲），我要捍卫大直角设计！

## Developing

开发环境需求:
```
$ sudo pacman -Syu mpv qt5 ffmpeg cmake extra-cmake-modules
```

在其他发行版上安装对应的包也可以完成构建。

其中，Qt版本要求在5.15以上，否则可能缺失特性/无法构建成功。

古早Ubuntu或者UOS用户可以等待本项目Release后使用AppImage版本（逃

```
$ git clone https://github.com/Reverier-Xu/BitWave.git && cd BitWave

BitWave $ mkdir build && cd build

BitWave/build $ cmake .. && make

BitWave/build $ ./bin/BitWave
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

## 状态

- 支持播放绝大部分格式的各种音频文件。
- 完善的视频播放支持。
- 歌词。
- 可记忆的播放队列。
- 逆序播放。
- 支持获取媒体的metadata与封面图片，能够自动探测封面图片的主色调。
- 好看的界面。

## 一些截图

![image.png](https://i.loli.net/2021/08/30/9B5DjOXR8uHKAme.png)

![image.png](https://i.loli.net/2021/08/30/2t74vKisSfbJl9o.png)

![image.png](https://i.loli.net/2021/08/30/ELjMKmfZVQN5sUJ.png)

![image.png](https://i.loli.net/2021/08/30/Hun5ckty1lDRGZB.png)

![image.png](https://i.loli.net/2021/08/30/AfJnXCY9ztcUqrg.png)

## License

Mozilla Public License v2.0

精力有限，随缘提供技术支持，随缘修bug、移植，欢迎有新点子的开发者对本仓库提交issue、fork、修改、提交pr。
