# ![Bit Wave](src/resources/assets/logo-full.svg)

A simple media player based on MPV and Qt5.

[**简体中文**](README.cn.md)

## Status

- Theoretically BitWave can playing all the formats that MPV supported (After all, it is based on the MPV)
- Video playback.
- Lyrics (from Netease Cloud Music).
- Memorable play queue.
- Reverse order.
- Support for getting media metadata and cover images, and automatically detecting the main color of the cover image.
- Good-looking interface.
- MPRIS2 integration.
- Disable screen off when playing video.

## Work In Progress

- Online musics / online videos
- playlists management
- media library management
- search
- audio equalizers (?)
- Osu! player (?)

## Cloud Services

This project uses some cloud services:

- getting online lyrics (ok, from Netease)
- getting online playlists (WIP)
- searching online media resources (WIP)
- getting online subtitles (WIP)
- playing online media (WIP)

**The API support for online music/video vendors is for learning purposes only, please do not use it for commercial/illegal purposes! When using online services, please follow the corresponding vendor's user agreement and rules! **

This project is not responsible for the proper functioning of these APIs. If these APIs are changed or moved/removed, the author will follow up or directly remove the relevant functions from this player at its discretion.

## Developing

Environment:
```
$ sudo pacman -Syu mpv qt5 ffmpeg cmake extra-cmake-modules
```

```
git clone https://github.com/Reverier-Xu/BitWave.git && cd BitWave

mkdir build && cd build

cmake .. && make

./bin/BitWave
```

## Some screenshots

![image.png](https://i.loli.net/2021/08/30/9B5DjOXR8uHKAme.png)

![image.png](https://i.loli.net/2021/08/30/2t74vKisSfbJl9o.png)

![image.png](https://i.loli.net/2021/08/30/ELjMKmfZVQN5sUJ.png)

![image.png](https://i.loli.net/2021/08/30/Hun5ckty1lDRGZB.png)

![image.png](https://i.loli.net/2021/08/30/AfJnXCY9ztcUqrg.png)

## License

Mozilla Public License v2.0
