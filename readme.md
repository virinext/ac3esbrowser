AC3ESBrowser
==========

AC3ESBrowser is a tool for analyzing Dolby Digital and Dolby Digital Plus bitstreams.


This application displays syntax elements of ac3 and eac3 elementary streams. Input file for AC3ESBrowser should be unmuxed elementary stream. If analysis of the ac3/eac3 audio in any type of container (e.g. in .ts or .mp4 files) is needed, preliminary demuxing is required. For example, you can use ffmpeg for this: ffmpeg -i your_file -vn -acodec copy audio.ac3. There are gui and cli applications in this product.




Gui
-----

![alt tag](https://cloud.githubusercontent.com/assets/10683398/6414901/39fe7254-bec5-11e4-84f8-b2dc5dba3ef5.png)


Building requirements:
-----

* boost (for testing only)

* qt



Building:
-----

cd ac3esbrowser

mkdir project

cd project

cmake ../

make 

cd ../utils/ac3_es_browser_gui/

qmake ac3_es_browser_gui.pro

make gitinfo

make

