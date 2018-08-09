; **********************************************************************
; Основной системный диск в памяти
; **********************************************************************

; Это первая функция, которая вызывается для RAM-диска - распаковка 
; содержимого "хвоста" sys.bin в память по файлам.

fs_rootdisk_unpack:

    ret

; Структура каталогов
; ----------------------------------------------------------------------
strz "disk"             ; Перечисление дисков в системе
strz "sys"              ; Системный раздел
strz "sys/bin"          ; Системные приложения
strz "sys/config"       ; Системный конфигурации
strz "tmp"              ; Временная директория (RW)
strz "home"             ; Домашний раздел
strz "root"             ; Папка суперпользователя
strz "proc"             ; Процессы
