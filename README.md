# Bit Wave

![](src/resources/assets/logo-full.svg)

建立在 MPV, Qt 与 Microsoft Fluent Design System 之上的媒体播放器。

## Developing

developing env requirements:
```
$ sudo pacman -Syu mpv qt5 kf5 ffmpeg cmake
```

```
$ git clone https://github.com/Reverier-Xu/BitWave.git && cd BitWave

BitWave $ mkdir build && cd build

BitWave/build $ cmake .. && make

BitWave/build $ ./bin/BitWave
```

## Status

- 支持播放绝大部分格式的各种音频文件。
- 支持播放网易云音乐专有格式(ncm)。
- 支持获取媒体的metadata与封面图片，能够自动探测封面图片的主色调。
- 好看的界面。
