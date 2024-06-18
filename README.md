# Image Converter
### Описание проекта
Конвертер изображений - выполняет преобразование изображение из одного формата в другой. Программа обрабатывает PPM, JPEG и BMP(BGR24) без сжатия.
### Технологии
- C++17
- STL
- LibJPEG
- CMake
### Сборка
- Скачать проект
- Скачать библиотеку [LibJPEG](https://www.ijg.org/). Перейти в папку с файлом configure и выполнить сборку статической библиотеки.
- Создать в основной директории папки и добавить файлы собранной библиотеки, следующей иерархией:
```
    libjpeg/
    ├── include/
    │   ├── jconfig.h
    │   ├── jerror.h
    │   ├── jmorecfg.h
    │   └── jpeglib.h
    └── lib/
        ├── Debug
        │   └── libjpeg.a
        └── Release
            └── libjpeg.a 
```
- Перейти ImgConverter и выполнить команды:
```
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=<Debug или Release> -G "MinGW Makefiles" -DLIBJPEG_DIR=<Абсалютный путь к libjpeg>
    cmake --build .
```
### Автор
@nentron