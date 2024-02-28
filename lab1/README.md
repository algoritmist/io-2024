# Лабораторная работа 1

**Название:** "Разработка драйверов символьных устройств"

**Цель работы:** Разработать драйвер символьного устройства, который будет при чтении файла выводить количество введенных символов. 

## Вариант:
1.1 При записи текста в файл символьного устройства должен осуществляться подсчет введенных символов. Последовательность полученных результатов (количество символов) с момента загрузки модуля ядра должна выводиться при чтении файла.

## Описание функциональности драйвера ##
Реализован буффер, в который при записи в файл осуществляется запись количества введенных символов. При чтении файла выводится буффер. Если буффер переполняется, тогда выводится оповещение и очищается буффер. 

## Инструкция по сборке ##
Сборка осуществляется на версии ядра 5.15 и комилятором gcc 9.X и linux-headers. 

```sh
make
make -C /lib/modules/5.15.0-91-generic/build M="/home/nfs/io-2024/lab1" modules
make[1]: Entering directory '/usr/src/linux-headers-5.15.0-91-generic'
  CC [M]  /home/nfs/io-2024/lab1/ch_drv.o
/home/nfs/io-2024/lab1/ch_drv.c: In function ‘my_read’:
/home/nfs/io-2024/lab1/ch_drv.c:38:3: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
   38 |   size_t real_len = min(len, sizeof(in_buf));
      |   ^~~~~~
/home/nfs/io-2024/lab1/ch_drv.c: In function ‘my_write’:
/home/nfs/io-2024/lab1/ch_drv.c:56:3: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
   56 |   int rem = BUF_SIZE - ptr;
      |   ^
/home/nfs/io-2024/lab1/ch_drv.c:64:3: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
   64 |   int count = snprintf(in_buf + ptr, rem, "%d ", (int) len);
      |   ^
  MODPOST /home/nfs/io-2024/lab1/Module.symvers
  CC [M]  /home/nfs/io-2024/lab1/ch_drv.mod.o
  LD [M]  /home/nfs/io-2024/lab1/ch_drv.ko
  BTF [M] /home/nfs/io-2024/lab1/ch_drv.ko
Skipping BTF generation for /home/nfs/io-2024/lab1/ch_drv.ko due to unavailability of vmlinux
make[1]: Leaving directory '/usr/src/linux-headers-5.15.0-91-generic'
```
Нам было лень использовать флаг -std=gnu11, поэтому тут есть warnings :)

## Инструкция пользователя ##
После установки драйвера откройте терминал и перейдите к корневой директории с помощью команды ```cd /```. Чтобы писать на устройство, используйте команду ```echo "Ваше сообщение" > /dev/varl```. Чтобы прочитать количество введенных символов, используйте команду ```cat /dev/varl```. Чтобы установить права на чтение/
запись/модификацию используйте команду ```sudo chmod 777 /dev/varl```, теперь команды выше будут выполняться.
## Примеры использования ##
```sh
sudo chmod 777 /dev/var1
echo "Hello, " > /dev/var1
echo "World!" > /dev/var1
cat /dev/var1
Output: "8 7"
```
