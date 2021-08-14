# vl53l1x_cpp

Bu depoda ST Micro tarafından üretilen ve 400 cm algılama mesafesine sahip VL53L1X ToF mesafe sensörünün C++ ile kullanılışı açıklanmıştır. Referans alınan depolardan biri ROS diğeri de Python için geliştirilmiş olup bu depoların adresleri aşağıda verilmiştir. C kütüphanesinin oluşturulması için Python kodundan; örnek test programı için ise ROS kodundan faydalanılmıştır. 

Makefile komutu ile hem VL53L1X kütüphanesi hem de örnek program  derlenmmektedir. Derlenen kütüphane adı *libvl53l1_api.a* olup /lib klasörüne el ile kopyalanmaktadır. Bunun nedeni Makefile kaynak kodunun kütüphane dosyasını __/lib__ klasörü altında aramasıdır.

Kütüphane oluşturmak için `make` komut satırı kullanılmaktadır.

Örnek program olan __test__ üretmek içim `make test` komut satırı kullanılmaktadır. Bu komut satırı ile __test.cpp__ dosyası */lib/libvl53l1_api.a* kütüphanesi ile bağlanarak __test__ çalıştırılabilir kodu üretilmektedir.

### Referanslar:
* https://github.com/okalachev/vl53l1x_ros
* https://github.com/pimoroni/vl53l1x-python

