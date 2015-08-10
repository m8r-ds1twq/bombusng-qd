  1. Скачиваете архив http://bombusng-qd.googlecode.com/files/BombusQD-NG_53.zip
  1. Распаковываете его
  1. Устанавливаете файл BombusQD-NG.cab (рекомендую устанавливать в память кпк)
  1. Качаете архив с ревизией [r57](https://code.google.com/p/bombusng-qd/source/detail?r=57): http://bombusng-qd.googlecode.com/files/Bombusqd-ng.zip
  1. Распаковываете архив в папку \Program Files\fuze\BombusQD-NG\ с заменой файлов

Также можно назначить Bombus на софт-клавишу "Тудея".
  1. В реестре по адресу HKCU\Software\Microsoft\Today\ создаете ключ Keys (если еще не создан)
  1. В ключе Keys создаете новый ключ, 112 (если хотите назначить на левую софт-клавишу) или 113 (для правой софт-клавиши)
  1. В созданном ключе создаете 2 строковых значения:
    * default - надпись на клавише
    * open - путь к программе: \Program Files\fuze\BombusQD-NG\BombusQD-NG.exe
  1. Делаете софт-ресет устройства