# dx_practice_pkg

dynamixel_workbenchを使用するための実験用プログラム

以下のコマンドでpull

```bash
git clone -b ros2 https://github.com/ROBOTIS-GIT/DynamixelSDK.git 
git clone -b ros2 https://github.com/ROBOTIS-GIT/dynamixel-workbench.git
```

/dev/ttyUSB** -> /dev/dxhub でアクセスできるようにrulesを更新

/etc/udev/rules.d/下にdxhub.rulesを配置して下記コマンドを実行し再起動

```bash
sudo adduser $USER dialout
sudo udevadm control --reload-rules
```

下記のコマンドで存在が確認できたらOK

```bash
$ ls -la /dev/dxhub
rwxrwxrwx 1 root root 7 Sep  5 21:05 /dev/dxhub -> ttyUSB0 # USB0へのシンボリックリンク
```

参考URL https://qiita.com/caad1229/items/309be550441515e185c0

