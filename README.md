# Route-Finder

Консольное приложение для нахождения маршрутов между городами, использующее Yandex Api.


## Функциональность

Консольное приложение ищет список возможных маршрутов до пункта назначения и обратно любыми видами транспорта (например самолет + поезд) на определенную дату (передается аргументом командной строки). Найденые промежуточные результаты кэшируются в памяти и на файловой системе для уменьшения количества запросов на внешний сервис.

## Источник данных

В качестве источника информации используется [API Яндекса Расписаний](https://yandex.ru/dev/rasp/doc/ru/). Для разработчика оно ограничено 500 запросами в день. Вы передаете свой апи-ключ в качестве аргументов командной строки

## Реализация 

Работа с API предполагает выполнение [HTTP-запросов](https://en.wikipedia.org/wiki/HTTP). Для этого используется бибилиотека [С++ Requests](https://github.com/libcpr/cpr). В качестве библиотеки для работы с json-ответом - [nlohmann/json](https://github.com/nlohmann/json).

## Cборка

Cтандартная через CMake

## Пример работы программы
Usage: route-finder.exe [options]

Options:
  --date=<string>               Date of departure in YYYY-MM-DD format
  -a, --api-key=<string>                Sets api key
  -s, --source=<string>         Sets departure city
  -m, --max-transfer=<int>              Sets max amount of possible transfers
  -d, --destination=<string>            Sets arrival city
  -h, --help=<bool>             Displays all available commands

###### Пример: route-finder.exe --date=2025-04-30 -s saint-petersburg -d pekin -m 5
