# TPG - Taller de Programación I (7542/9508)
# Worms2D

![worms-logo](https://cdn.discordapp.com/attachments/1162524969142599822/1176873747249111070/pngegg.png?ex=65707416&is=655dff16&hm=ab3c9c99ee749053d6545c462345340b3ade8931082e20abc46a4c63a65aeec8&)

Worms2D es el proyecto final para la materia Taller de programación I (Cátedra Veiga - Facultad de Ing. UBA) y consta de una re-imaginación del popular juego de estrategia por turnos Worms Armageddon.

Para el proyecto se emplearon una serie de bibliotecas las cuales se listan como dependencias necesarias para correr el programa a continuación.

## Dependencias:

- Box2D // Engine de Físicas para juegos 2D.
- SDL2pp // Wrapper de la biblioteca SDL para tareas de renderizado y reproducción de audio.
- QT // Framework de diseño utilizado para la UI del juego.
- Google Tests // Suite para test unitarios en C++.
- LibYAML // Librería para facilitar el parseo y lectura de archivos YAML.
- Spdlog // Libreria de logging

![worm-dependencias](https://cdn.discordapp.com/attachments/1162524969142599822/1176873875846463558/pngegg1.png?ex=65707435&is=655dff35&hm=e1bed770c3dea42b01800af9efa5ead113d4bc6d8d645dfb2497efc2278785c1&)

## Guía de instalación de las dependencias

- Box2D

```console
sudo apt install doxygen
git clone git@github.com:erincatto/box2d.git
cd box2d
mkdir build
cd build
cmake -DBOX2D_BUILD_DOCS=ON ..
make -j4
sudo make install
```


- SDL2pp

Para desarrollo del juego se utilizo la versión 0.18 de libsdl2pp. Para su correcto funcionamiento se necesita descargar las siguientes releases de bibliotecas del paquete SDL: 
  - libsdl2-image ( 2.6.3, https://github.com/libsdl-org/SDL_image/releases/tag/release-2.6.3)
  - libsdl2-mixer (2.6.3, https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.6.3)
  - libsdl2-ttf (2.20.2, https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2)

Acto seguido instalamos algunas dependencias que deberemos linkear para compilar el proyecto. 
```console
sudo apt-get install libjpeg-dev libpng-dev libfreetype-dev libopusfile-dev libflac-dev libxmp-dev libfluidsynth-dev libwavpack-dev cmake libmodplug-dev libsdl2-dev
```

Luego, con cada uno de los zip de la primera parte seguimos el siguiente proceso de compilado e instalado:
```console
cd carpeta_sdl_x
mkdir build
cd build
cmake ..
make -j4
sudo make install #importante el hacerlo como sudo
```

Una vez compiladas e instaladas las ultimas versiones de libsdl2 (mixer, ttf e image), compilamos e instalamos la version 0.18.1 de libsdl2pp
 - Link a la versión: https://github.com/libSDL2pp/libSDL2pp/releases/tag/0.18.1

```console
cd libSDL2pp-0.18.1
mkdir build
cd build
cmake ..
make -j4
sudo make install #importante el hacerlo como sudo
```

- QT

```console
sudo apt install qmake6 qt6-base-dev qt6-base-dev-tools qt6-multimedia-dev qt6-tools-dev qt6-tools-dev-tools qt6-wayland qt6-wayland-dev qt6-wayland-dev-tools libqt6multimedia6 libqt5multimedia5-plugins
```

- libYAML & GoogleTests
```console
sudo apt install libgtest-dev libyaml-cpp-dev libyaml-cpp0.7 -y
```

- Spdlog
```console
sudo apt install libspdlog-dev libspdlog1.10 -y
```

## Ejecucion de la aplicacion


```console
git clone git@github.com:JM-Pascual/TPG-Taller-Worms2D.git
cd TPG-Taller-Worms2D
mkdir build
cd build
cmake ..
make -j4
../target/worms2dsv 8080 #servidor
../target/worms2d 127.0.0.1 8080 #cliente
```

### Disclaimer

Se utilizo el [socket](https://github.com/eldipa/sockets-en-cpp), [queues y thread](https://github.com/eldipa/hands-on-threads/tree/master/libs) provistos por la catedra Veiga de Taller de Programacion I UBA. Bajo licencia GPL v2.
